LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := xml2
LOCAL_SRC_FILES += libxml2.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := icuuc
LOCAL_SRC_FILES += libicuuc.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := usb
LOCAL_SRC_FILES += libusb-1.0.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avahi-client
LOCAL_SRC_FILES += libavahi-client.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := avahi-common
LOCAL_SRC_FILES += libavahi-common.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := serialport
LOCAL_SRC_FILES += libserialport.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE += libiio
LOCAL_C_INCLUDES = /home/jason/Desktop/Work5-SDR/aarch-lib/dynamicinclude/libusb-1.0/
 LOCAL_SHARED_LIBRARIES += usb
LOCAL_SRC_FILES := channel.c device.c context.c buffer.c utilities.c
LOCAL_CFLAGS += -Wall -Wextra

LOCAL_CFLAGS += -error-limit=0
LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -DHAVE_IPV6=1
LOCAL_CFLAGS += -D_GNU_SOURCE=1
LOCAL_CFLAGS += -D_POSIX_C_SOURCE=200809L -DLIBIIO_EXPORTS=1
LOCAL_CFLAGS += -DLIBIIO_VERSION_MAJOR=0 -DLIBIIO_VERSION_MINOR=6
LOCAL_CFLAGS += -DLIBIIO_VERSION_GIT=\"0.6\"

LOCAL_CFLAGS += -DWITH_LOCAL_BACKEND=ON
LOCAL_SRC_FILES += libini.c local.c

LIBIIO_USE_NETWORK = 1
ifdef LIBIIO_USE_NETWORK
    LOCAL_CFLAGS += -DNETWORK_BACKEND=1
    LOCAL_SRC_FILES += network.c network-unix.c dns_sd.c

    #LOCAL_SRC_FILES += libxml2.so
    #include $(PREBUILT_SHARED_LIBRARY)
    # network requires XML
    LOCAL_CFLAGS += -DXML_BACKEND=1
    LOCAL_C_INCLUDES += /home/jason/Desktop/Work5-SDR/aarch-lib/dynamicinclude/libxml2/
    LOCAL_SHARED_LIBRARIES += xml2 icuuc
    LOCAL_SRC_FILES += xml.c

    LOCAL_CFLAGS += -DWITH_SERIAL_BACKEND=ON
    LOCAL_C_INCLUDES += /home/jason/Desktop/Work5-SDR/aarch-lib/libserialport/build-android-arm64/out/include/
    LOCAL_SHARED_LIBRARIES += serialport
    LOCAL_SRC_FILES += serial.c

    # Network requires threads
    LOCAL_CFLAGS += -pthread
    LOCAL_SRC_FILES += lock.c

    # Network requires iiod-client
    LOCAL_SRC_FILES += iiod-client.c
    
    LOCAL_SHARED_LIBRARIES += avahi-common avahi-client
    LOCAL_C_INCLUDES += /home/jason/Desktop/Work5-SDR/aarch-lib/avahi
    LOCAL_SRC_FILES += scan.c usb.c sort.c dns_sd_avahi.c backend.c
endif

LOCAL_EXPORT_C_INCLUDES = iio.h

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := iio_info
LOCAL_CFLAGS += -Wall -Wextra
LOCAL_CXXFLAGS += -Weffc++
LOCAL_C_INCLUDES += /home/jason/Desktop/Work5-SDR/aarch-lib/dynamicinclude/
LOCAL_SRC_FILES := tests/iio_info.c tests/iio_common.c
LOCAL_SHARED_LIBRARIES := iio usb

include $(BUILD_EXECUTABLE)
