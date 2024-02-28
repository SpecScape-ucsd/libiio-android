#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <libusb.h>

#include "iio.h"

#define usbinfo ""

struct iio_context_info {
    char *description;
    char *uri;
};

struct iio_scan_result {
    size_t size;
    struct iio_context_info **info;
};

int main(int argc, char **argv) {
    int ret;
    struct iio_scan_result scan_result = { 0, NULL };
    struct iio_context_info *info;

    libusb_context *context;
    libusb_device_handle *handle;
    struct libusb_device *device;
    struct libusb_device_descriptor device_descriptor;
    unsigned char buffer[256];
    int fd;
    assert((argc > 1) && (sscanf(argv[1], "%d", &fd) == 1));
    libusb_set_option(NULL, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
    assert(!libusb_init(&context));
    assert(!libusb_wrap_sys_device(context, (intptr_t) fd, &handle));
    device = libusb_get_device(handle);
    unsigned int intrfc = 0;

    ret = libusb_open(device, &handle);
    if (ret)
    {
        fprintf(stderr,"open failed with ret %d\n", ret);
        return ret;
    }

    if (!iio_usb_match_device(device, handle, &intrfc)) {
        info = iio_scan_result_add(scan_result);
        if (!info)
        {
            fprintf(stderr,"Get error information wrong\n");
            ret = -ENOMEM;
        }
        else
            ret = usb_fill_context_info(info, device, handle,
                                            intrfc);
    }

    libusb_close(handle);
    libusb_exit(context);
    return ret;
}