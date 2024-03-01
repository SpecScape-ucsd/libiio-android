/* -*- c++ -*- */
/*
 * Copyright 2021 The Regents of the University of California.
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

#include "combined_common.h"
#include "combined_pluto_receiver_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace sparsdr {

namespace {
constexpr float PLUTO_BANDWIDTH = 61.44e6;
constexpr unsigned int PLUTO_FFT_SIZE = 1024;
} // namespace

combined_pluto_receiver::sptr
combined_pluto_receiver::make(const std::string& uri,
                              std::size_t buffer_size,
                              unsigned int fft_size,
                              float center_frequency,
                              const std::vector<band_spec>& bands,
                              const std::string& reconstruct_path,
                              bool zero_gaps, int fd, const std::string& dirname)
{
    return gnuradio::get_initial_sptr(new combined_pluto_receiver_impl(uri,
                                                                       buffer_size,
                                                                       fft_size,
                                                                       center_frequency,
                                                                       bands,
                                                                       reconstruct_path,
                                                                       zero_gaps, fd, dirname));
}

/*
 * The private constructor
 */
combined_pluto_receiver_impl::combined_pluto_receiver_impl(
    const std::string& uri,
    std::size_t buffer_size,
    unsigned int fft_size,
    float center_frequency,
    const std::vector<band_spec>& bands,
    const std::string& reconstruct_path,
    bool zero_gaps, int fd, const std::string& dirname)
    : gr::hier_block2(
          "combined_pluto_receiver",
          gr::io_signature::make(0, 0, 1),
          gr::io_signature::make(bands.size(), bands.size(), sizeof(gr_complex))),
      d_pluto(nullptr),
      d_reconstruct(nullptr)
{
    // Convert the bands into bands relative to the center frequency
    std::vector<band_spec> relative_bands;
    relative_bands.reserve(bands.size());
    for (const band_spec& band : bands) {
        relative_bands.push_back(
            band_spec(band.frequency() - center_frequency, band.bins()));
    }

    d_pluto = compressing_pluto_source::make(uri, buffer_size, fd);

    const char* format_version_string;
    switch (d_pluto->format_version()) {
    case 1:
        format_version_string = "Pluto v1";
        break;
    case 2:
        format_version_string = "Pluto v2";
        break;
    default:
        throw std::runtime_error("Invalid format version, expected 1 or 2");
    }
    d_reconstruct = reconstruct::make(
        relative_bands, reconstruct_path, format_version_string, zero_gaps, fft_size, dirname);
    // Connect
    connect(d_pluto, 0, d_reconstruct, 0);
    for (std::size_t i = 0; i < bands.size(); i++) {
        connect(d_reconstruct, i, self(), i);
    }
}

/*
 * Our virtual destructor.
 */
combined_pluto_receiver_impl::~combined_pluto_receiver_impl() {}

void combined_pluto_receiver_impl::set_frequency(unsigned long long frequency)
{
    d_pluto->set_frequency(frequency);
}
void combined_pluto_receiver_impl::set_gain(double gain) { d_pluto->set_gain(gain); }
void combined_pluto_receiver_impl::set_gain_control_mode(const std::string& mode)
{
    d_pluto->set_gain_control_mode(mode);
}

void combined_pluto_receiver_impl::set_compression_enabled(bool enabled)
{
    d_pluto->set_compression_enabled(enabled);
}
void combined_pluto_receiver_impl::set_run_fft(bool enable)
{
    d_pluto->set_run_fft(enable);
}
void combined_pluto_receiver_impl::set_send_average_samples(bool enable)
{
    d_pluto->set_send_average_samples(enable);
}
void combined_pluto_receiver_impl::set_send_fft_samples(bool enable)
{
    d_pluto->set_send_fft_samples(enable);
}
void combined_pluto_receiver_impl::set_fft_size(std::uint32_t size)
{
    d_pluto->set_fft_size(size);
}
std::uint32_t combined_pluto_receiver_impl::fft_size() const
{
    return d_pluto->fft_size();
}
void combined_pluto_receiver_impl::set_shift_amount(std::uint8_t scaling)
{
    d_pluto->set_shift_amount(scaling);
}
void combined_pluto_receiver_impl::set_bin_threshold(std::uint16_t bin_index,
                                                     std::uint32_t threshold)
{
    d_pluto->set_bin_threshold(bin_index, threshold);
}
void combined_pluto_receiver_impl::set_bin_window_value(std::uint16_t bin_index,
                                                        std::uint16_t value)
{
    d_pluto->set_bin_window_value(bin_index, value);
}
void combined_pluto_receiver_impl::set_bin_mask(std::uint16_t bin_index)
{
    d_pluto->set_bin_mask(bin_index);
}
void combined_pluto_receiver_impl::clear_bin_mask(std::uint16_t bin_index)
{
    d_pluto->clear_bin_mask(bin_index);
}
void combined_pluto_receiver_impl::set_average_weight(float weight)
{
    d_pluto->set_average_weight(weight);
}
void combined_pluto_receiver_impl::set_average_interval(std::uint32_t interval)
{
    d_pluto->set_average_interval(interval);
}


} /* namespace sparsdr */
} /* namespace gr */
