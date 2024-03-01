/* -*- c++ -*- */
/*
 * Copyright 2019-2022 The Regents of the University of California.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdexcept>

#include "compressing_usrp_source_impl.h"
#include "detail/registers.h"
#include "endian_converter.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace sparsdr {

namespace {

const std::uint32_t DEFAULT_FFT_SIZE = 2048;

/**
 * Returns the number of leading zeros in the binary representation of
 * a number
 */
std::uint32_t leading_zeros(std::uint32_t value)
{
    std::uint32_t zeros = 0;
    while ((value >> 31) == 0 && zeros < 32) {
        value <<= 1;
        zeros += 1;
    }
    return zeros;
}
} // namespace

namespace registers = gr::sparsdr::detail::registers;

compressing_usrp_source::sptr
compressing_usrp_source::make(const ::uhd::device_addr_t& device_addr)
{
    return gnuradio::get_initial_sptr(new compressing_usrp_source_impl(device_addr));
}

/*
 * The private constructor
 */
compressing_usrp_source_impl::compressing_usrp_source_impl(
    const ::uhd::device_addr_t& device_addr)
    : gr::hier_block2("compressing_usrp_source",
                      gr::io_signature::make(0, 0, 0),
                      gr::io_signature::make(1, 1, sizeof(std::uint32_t))),
      d_usrp(nullptr),
      d_fft_size(DEFAULT_FFT_SIZE)
{
    endian_converter::register_converter();

    // The sparsdr_sample format uses the custom
    // converter that was just registered
    d_usrp = gr::uhd::usrp_source::make(device_addr,
                                        ::uhd::stream_args_t("sparsdr_sample", "sc16"));
    // Connect the all-important output
    connect(d_usrp, 0, self(), 0);
}

/*
 * Our virtual destructor.
 */
compressing_usrp_source_impl::~compressing_usrp_source_impl() {}

// USRP settings

void compressing_usrp_source_impl::set_gain(double gain) { d_usrp->set_gain(gain); }
::uhd::tune_result_t
compressing_usrp_source_impl::set_center_freq(const ::uhd::tune_request_t& tune_request)
{
    return d_usrp->set_center_freq(tune_request);
}
void compressing_usrp_source_impl::set_antenna(const std::string& ant)
{
    d_usrp->set_antenna(ant);
}


// SparSDR-specific settings

void compressing_usrp_source_impl::set_compression_enabled(bool enabled)
{
    d_usrp->set_user_register(registers::ENABLE_COMPRESSION, enabled);
}

void compressing_usrp_source_impl::set_run_fft(bool enable)
{
    d_usrp->set_user_register(registers::RUN_FFT, enable);
}

void compressing_usrp_source_impl::set_send_average_samples(bool enable)
{
    d_usrp->set_user_register(registers::AVG_SEND, enable);
}

void compressing_usrp_source_impl::set_send_fft_samples(bool enable)
{
    d_usrp->set_user_register(registers::FFT_SEND, enable);
}

void compressing_usrp_source_impl::set_fft_size(std::uint32_t size)
{
    d_usrp->set_user_register(registers::FFT_SIZE, size);
    d_fft_size = size;
}

std::uint32_t compressing_usrp_source_impl::fft_size() const { return d_fft_size; }

void compressing_usrp_source_impl::set_shift_amount(std::uint8_t scaling)
{
    d_usrp->set_user_register(registers::SHIFT_AMOUNT, scaling);
}

void compressing_usrp_source_impl::set_bin_threshold(std::uint16_t index,
                                                     std::uint32_t threshold)
{
    // First write the threshold value, then write the bin number to apply
    // the change
    d_usrp->set_user_register(registers::THRESHOLD_VALUE, threshold);
    d_usrp->set_user_register(registers::THRESHOLD_BIN_NUMBER, index);
}

void compressing_usrp_source_impl::set_bin_window_value(std::uint16_t bin_index,
                                                        std::uint16_t value)
{
    d_usrp->set_user_register(registers::WINDOW_VAL,
                              (std::uint32_t(bin_index) << 16) | std::uint32_t(value));
}

void compressing_usrp_source_impl::set_bin_mask(std::uint16_t index)
{
    // Register format:
    // Bits 31:1 : index (31 bits)
    // Bit 0 : set mask (1) / clear mask (0)
    const std::uint32_t command = (index << 1) | 0x1;
    d_usrp->set_user_register(registers::MASK, command);
}
void compressing_usrp_source_impl::clear_bin_mask(std::uint16_t index)
{
    // Register format:
    // Bits 31:1 : index (31 bits)
    // Bit 0 : set mask (1) / clear mask (0)
    const std::uint32_t command = (index << 1) | 0x0;
    d_usrp->set_user_register(registers::MASK, command);
}

void compressing_usrp_source_impl::set_average_weight(float weight)
{
    if (weight < 0.0 || weight > 1.0) {
        throw std::out_of_range("weight must be in the range [0, 1]");
    }
    // Map to 0...255
    const uint8_t mapped = static_cast<uint8_t>(weight * 255.0);
    d_usrp->set_user_register(registers::AVG_WEIGHT, mapped);
}

void compressing_usrp_source_impl::set_average_interval(std::uint32_t interval)
{
    if (interval == 0) {
        throw std::out_of_range("interval must not be 0");
    }
    // Register format: ceiling of the base-2 logarithm of the interval
    const uint32_t ceiling_log_interval = 31 - leading_zeros(interval);
    d_usrp->set_user_register(registers::AVG_INTERVAL, ceiling_log_interval);
}

} /* namespace sparsdr */
} /* namespace gr */
