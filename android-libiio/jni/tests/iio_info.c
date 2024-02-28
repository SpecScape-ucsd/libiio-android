#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <iio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#include "iio_common.h"

#define MY_NAME "iio_info"

#ifdef _WIN32
#define snprintf sprintf_s
#endif

static const struct option options[] = {
    {0, 0, 0, 0},
};

static const char *options_descriptions[] = {
    ("[-x <xml_file>]\n"
        "\t\t\t\t[-u <uri>]"),
};

static int dev_is_buffer_capable(const struct iio_device *dev)
{
    unsigned int i;

    for (i = 0; i < iio_device_get_channels_count(dev); i++) {
        struct iio_channel *chn = iio_device_get_channel(dev, i);

        if (iio_channel_is_scan_element(chn))
            return true;
    }

    return false;
}

#define MY_OPTS ""

void getvidpid(char *usbarginput, int argc, char **argv, int *fd){
    int ret;
    char vid[5];
    char pid[5];

    fprintf(stderr,"argv1 is %s argv2 is %s\n", argv[1], argv[argc-1]);
    libusb_context *context;
    libusb_device_handle *handle;
    libusb_device *device;
    struct libusb_device_descriptor desc;
    unsigned char buffer[256];
    
    assert((argc > 1) && (sscanf(argv[argc-1], "%d", fd) == 1));
    libusb_set_option(NULL, LIBUSB_OPTION_NO_DEVICE_DISCOVERY);
    assert(!libusb_init(&context));
    assert(!libusb_wrap_sys_device(context, (intptr_t) *fd, &handle));
    fprintf(stderr,"fd is %d\n",*fd);
    device = libusb_get_device(handle);
    assert(!libusb_get_device_descriptor(device, &desc));
    // printf("Vendor ID: %04x\n", desc.idVendor);
    // printf("Product ID: %04x\n", desc.idProduct);
    sprintf(vid, "%04x", desc.idVendor);
    sprintf(pid, "%04x", desc.idProduct);

    // fprintf(stderr, "Before iio_usb_match_device\n");
    // if (!iio_usb_match_device(dev, hdl, &intrfc)) {
    //     info = iio_scan_result_add(scan_result);
    //     if (!info)
    //         ret = -ENOMEM;
    //     else
    //         ret = usb_fill_context_info(info, dev, hdl,
    //                         intrfc);
    // }
    // libusb_close(hdl);
    // itoa(desc.idVendor, vid, 16);
    // itoa(desc.idProduct, pid, 16);

    strcpy(usbarginput, "usb=");
    strncat(usbarginput, vid, 4);
    strncat(usbarginput, ":", 1);
    strncat(usbarginput, pid, 4);
    usbarginput[13] = '\0';
    // libusb_exit(context);
}

int main(int argc, char **argv) {
    int fd;
    char usbarginput[14];
    getvidpid(usbarginput, argc, argv, &fd);

    int newargc = 3;
    char *newargv[3];
    newargv[0] = malloc(strlen(MY_NAME)+1);
    memcpy(newargv[0],MY_NAME, strlen(MY_NAME));
    newargv[0][strlen(MY_NAME)] = '\0';
    newargv[1] = malloc(strlen("-S")+1);
    memcpy(newargv[1], "-S", strlen("-S"));
    newargv[1][strlen("-S")] = '\0';
    newargv[2] = malloc(4);
    memcpy(newargv[2], usbarginput, 3);
    newargv[2][3] = '\0';
    

    char **argw;
    struct iio_context *ctx;
    int c;
    unsigned int i, major, minor;
    char git_tag[8];
    int ret;
    struct option *opts;

    argw = dup_argv(MY_NAME, newargc, newargv);
    fprintf(stderr,"argw is %s\n", argw[2]);

    iio_library_get_version(&major, &minor, git_tag);
    printf("Library version: %u.%u (git tag: %s)\n", major, minor, git_tag);

    printf("Compiled with backends:");
    for (i = 0; i < iio_get_backends_count(); i++)
        printf(" %s", iio_get_backend(i));
    printf("\n");

    ctx = handle_common_opts(MY_NAME, newargc, argw, MY_OPTS, options, options_descriptions, &fd);
    opts = add_common_options(options);
    if (!opts) {
        fprintf(stderr, "Failed to add common options\n");
        return EXIT_FAILURE;
    }
    while ((c = getopt_long(newargc, argw, "+" COMMON_OPTIONS MY_OPTS "s", /* Flawfinder: ignore */
                    opts, NULL)) != -1) {
        switch (c) {
            /* All these are handled in the common */
        case 'h':
        case 'n':
        case 'x':
        case 'u':
        case 'T':
            break;
        case 'S':
        case 'a':
            if (!optarg && newargc > optind && argw[optind] != NULL
                    && argw[optind][0] != '-')
                optind++;
            break;
        case 's':
            autodetect_context(false, MY_NAME, NULL, &fd);
            return EXIT_SUCCESS;
        case '?':
            printf("Unknown argument '%c'\n", c);
            return EXIT_FAILURE;
        }
    }
    free(opts);

    if (optind != newargc) {
        fprintf(stderr, "Incorrect number of arguments.\n\n");
        usage(MY_NAME, options, options_descriptions);
        return EXIT_FAILURE;
    }

    if (!ctx)
        return EXIT_FAILURE;

    printf("IIO context created with %s backend.\n",
            iio_context_get_name(ctx));

    ret = iio_context_get_version(ctx, &major, &minor, git_tag);
    if (!ret)
        printf("Backend version: %u.%u (git tag: %s)\n",
                major, minor, git_tag);
    else {
        char err_str[1024];
        iio_strerror(-ret, err_str, sizeof(err_str));
        fprintf(stderr, "Unable to get backend version: %s\n", err_str);
    }

    printf("Backend description string: %s\n",
            iio_context_get_description(ctx));

    unsigned int nb_ctx_attrs = iio_context_get_attrs_count(ctx);
    if (nb_ctx_attrs > 0)
        printf("IIO context has %u attributes:\n", nb_ctx_attrs);

    for (i = 0; i < nb_ctx_attrs; i++) {
        const char *key, *value;

        ret = iio_context_get_attr(ctx, i, &key, &value);
        if (ret == 0)
            printf("\t%s: %s\n", key, value);
        else {
            char err_str[1024];
            iio_strerror(-ret, err_str, sizeof(err_str));
            fprintf(stderr, "\tUnable to read IIO context attributes: %s\n",
                    err_str);
        }
    }

    unsigned int nb_devices = iio_context_get_devices_count(ctx);
    printf("IIO context has %u devices:\n", nb_devices);
    char *buf = xmalloc(BUF_SIZE, MY_NAME);

    for (i = 0; i < nb_devices; i++) {
        const struct iio_device *dev = iio_context_get_device(ctx, i);
        const char *name = iio_device_get_name(dev);
        const char *label = iio_device_get_label(dev);
        printf("\t%s:", iio_device_get_id(dev));
        if (name)
            printf(" %s", name);
        if (label)
            printf(" (label: %s)", label);
        if (dev_is_buffer_capable(dev))
            printf(" (buffer capable)");
        printf("\n");

        unsigned int nb_channels = iio_device_get_channels_count(dev);
        printf("\t\t%u channels found:\n", nb_channels);

        unsigned int j;
        for (j = 0; j < nb_channels; j++) {
            struct iio_channel *ch = iio_device_get_channel(dev, j);
            const char *type_name;

            if (iio_channel_is_output(ch))
                type_name = "output";
            else
                type_name = "input";

            name = iio_channel_get_name(ch);
            printf("\t\t\t%s: %s (%s",
                    iio_channel_get_id(ch),
                    name ? name : "", type_name);

            if (iio_channel_get_type(ch) == IIO_CHAN_TYPE_UNKNOWN)
                printf(", WARN:iio_channel_get_type()=UNKNOWN");

            if (iio_channel_is_scan_element(ch)) {
                const struct iio_data_format *format =
                    iio_channel_get_data_format(ch);
                char sign = format->is_signed ? 's' : 'u';
                char repeat[12] = "";

                if (format->is_fully_defined)
                    sign += 'A' - 'a';

                if (format->repeat > 1)
                    snprintf(repeat, sizeof(repeat), "X%u",
                        format->repeat);

                printf(", index: %lu, format: %ce:%c%u/%u%s>>%u)\n",
                    iio_channel_get_index(ch),
                    format->is_be ? 'b' : 'l',
                    sign, format->bits,
                    format->length, repeat,
                    format->shift);
            } else {
                printf(")\n");
            }

            unsigned int nb_attrs = iio_channel_get_attrs_count(ch);
            if (!nb_attrs)
                continue;

            printf("\t\t\t%u channel-specific attributes found:\n",
                    nb_attrs);

            unsigned int k;
            for (k = 0; k < nb_attrs; k++) {
                const char *attr = iio_channel_get_attr(ch, k);
                ret = (int) iio_channel_attr_read(ch,
                        attr, buf, BUF_SIZE);

                printf("\t\t\t\tattr %2u: %s ", k, attr);

                if (ret > 0) {
                    printf("value: %s\n", buf);
                } else {
                    iio_strerror(-ret, buf, BUF_SIZE);
                    printf("ERROR: %s\n", buf);
                }
            }
        }

        unsigned int nb_attrs = iio_device_get_attrs_count(dev);
        if (nb_attrs) {
            printf("\t\t%u device-specific attributes found:\n",
                    nb_attrs);
            for (j = 0; j < nb_attrs; j++) {
                const char *attr = iio_device_get_attr(dev, j);
                ret = (int) iio_device_attr_read(dev,
                        attr, buf, BUF_SIZE);

                printf("\t\t\t\tattr %2u: %s ",
                        j, attr);

                if (ret > 0) {
                    printf("value: %s\n", buf);
                } else {
                    iio_strerror(-ret, buf, BUF_SIZE);
                    printf("ERROR: %s\n", buf);
                }
            }
        }

        nb_attrs = iio_device_get_buffer_attrs_count(dev);
        if (nb_attrs) {
            printf("\t\t%u buffer-specific attributes found:\n",
                    nb_attrs);
            for (j = 0; j < nb_attrs; j++) {
                const char *attr = iio_device_get_buffer_attr(dev, j);
                ret = (int) iio_device_buffer_attr_read(dev,
                        attr, buf, BUF_SIZE);

                printf("\t\t\t\tattr %2u: %s ",
                        j, attr);

                if (ret > 0) {
                    printf("value: %s\n", buf);
                } else {
                    iio_strerror(-ret, buf, BUF_SIZE);
                    printf("ERROR: %s\n", buf);
                }
            }
        }

        nb_attrs = iio_device_get_debug_attrs_count(dev);
        if (nb_attrs) {
            printf("\t\t%u debug attributes found:\n", nb_attrs);
            for (j = 0; j < nb_attrs; j++) {
                const char *attr =
                    iio_device_get_debug_attr(dev, j);

                ret = (int) iio_device_debug_attr_read(dev,
                        attr, buf, BUF_SIZE);
                printf("\t\t\t\tdebug attr %2u: %s ",
                        j, attr);
                if (ret > 0) {
                    printf("value: %s\n", buf);
                } else {
                    iio_strerror(-ret, buf, BUF_SIZE);
                    printf("ERROR: %s\n", buf);
                }
            }
        }

        const struct iio_device *trig;
        ret = iio_device_get_trigger(dev, &trig);
        if (ret == 0) {
            if (trig == NULL) {
                printf("\t\tNo trigger assigned to device\n");
            } else {
                name = iio_device_get_name(trig);
                printf("\t\tCurrent trigger: %s(%s)\n",
                        iio_device_get_id(trig),
                        name ? name : "");
            }
        } else if (ret == -ENOENT) {
            printf("\t\tNo trigger on this device\n");
        } else if (ret < 0) {
            iio_strerror(-ret, buf, BUF_SIZE);
            printf("ERROR: checking for trigger : %s\n", buf);
        }
    }

    free_argw(newargc, argw);
    free(buf);
    iio_context_destroy(ctx);
    return EXIT_SUCCESS;
}
