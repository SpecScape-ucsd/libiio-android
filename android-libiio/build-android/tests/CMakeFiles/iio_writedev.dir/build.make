# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android

# Include any dependencies generated for this target.
include tests/CMakeFiles/iio_writedev.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/iio_writedev.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/iio_writedev.dir/flags.make

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o: tests/CMakeFiles/iio_writedev.dir/flags.make
tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o: ../tests/iio_writedev.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o"
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests && /home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android29 --gcc-toolchain=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64 --sysroot=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -L/home/jason/Desktop/libiio-android/android-libiio/ -o CMakeFiles/iio_writedev.dir/iio_writedev.c.o   -c /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/tests/iio_writedev.c

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/iio_writedev.dir/iio_writedev.c.i"
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests && /home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android29 --gcc-toolchain=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64 --sysroot=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -L/home/jason/Desktop/libiio-android/android-libiio/ -E /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/tests/iio_writedev.c > CMakeFiles/iio_writedev.dir/iio_writedev.c.i

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/iio_writedev.dir/iio_writedev.c.s"
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests && /home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang --target=aarch64-none-linux-android29 --gcc-toolchain=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64 --sysroot=/home/jason/Desktop/android-ndk-r20b/toolchains/llvm/prebuilt/linux-x86_64/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -L/home/jason/Desktop/libiio-android/android-libiio/ -S /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/tests/iio_writedev.c -o CMakeFiles/iio_writedev.dir/iio_writedev.c.s

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.requires:

.PHONY : tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.requires

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.provides: tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.requires
	$(MAKE) -f tests/CMakeFiles/iio_writedev.dir/build.make tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.provides.build
.PHONY : tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.provides

tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.provides.build: tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o


# Object files for target iio_writedev
iio_writedev_OBJECTS = \
"CMakeFiles/iio_writedev.dir/iio_writedev.c.o"

# External object files for target iio_writedev
iio_writedev_EXTERNAL_OBJECTS =

tests/iio_writedev: tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o
tests/iio_writedev: tests/CMakeFiles/iio_writedev.dir/build.make
tests/iio_writedev: tests/libiio_tests_helper.a
tests/iio_writedev: libiio.so
tests/iio_writedev: tests/CMakeFiles/iio_writedev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable iio_writedev"
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/iio_writedev.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/iio_writedev.dir/build: tests/iio_writedev

.PHONY : tests/CMakeFiles/iio_writedev.dir/build

tests/CMakeFiles/iio_writedev.dir/requires: tests/CMakeFiles/iio_writedev.dir/iio_writedev.c.o.requires

.PHONY : tests/CMakeFiles/iio_writedev.dir/requires

tests/CMakeFiles/iio_writedev.dir/clean:
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests && $(CMAKE_COMMAND) -P CMakeFiles/iio_writedev.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/iio_writedev.dir/clean

tests/CMakeFiles/iio_writedev.dir/depend:
	cd /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/tests /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests /home/jason/Desktop/Work5-SDR/libiio-android/android-libiio/build-android/tests/CMakeFiles/iio_writedev.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/iio_writedev.dir/depend

