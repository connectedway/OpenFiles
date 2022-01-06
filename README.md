# OpenFiles

Open Files is a lightweight (<256K), multi-platform, real-time, 
event-driven, and high performance, subsystem for application development.

The main advantages of Open Files are:

- It is multi-platform.  Open File applications can run on any supported
platform including Windows, MacOS, Android, Linux, iOS, and many RTOSes.
- It supports a real-time, event-driven programming model as well as
traditional thread based models
- It supports multiple file system handlers for platform files as well
as virtual and networked file systems
- It has a well defined platform abstraction layer for easy extensibility
to other platforms.
- It uses the a cmake generated build system.  Meaning it has a cross 
platform build system and has support for leading GUIs
- It has a built in unit test framework based on Unity.
- It supports IPv6 and IPv4.
- Integrated support for parsing and generating DOM documents
- Support for persistent configurations
- Built in performance statistics for core constructs.
- Built in Heap Debugging, Leak Detection, Buffer overruns and more.
- Open Source, with restrictions for attribution and derivatives.

In addition to the core Open Files facilities, there are extensions
based on Open Files to provide for:

- Private Security Libraries
- SASL Authentication
- Message Signing
- SMB client and server support
- FUSE file system handlers for Linux, Windows, and MacOS
- Java JNI for Open Files API and SMB
- Android Document Provider
- NetBIOS Name Server and Name Lookup.
- Microsoft lan manager support for Pipes, Mailslots, and DCE
- Kerberos/Active Directory Integration.

The documentation for Open Files is can be found at
http://www.connectedway.com/openfiles

The document will provide an introduction to Open Files and guide the reader
through downloading, configuring, building, testing, and developing applications
using Open Files.

# Building on Android (Release on ARMv8)

cmake -DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-23 -DCMAKE_SYSTEM_VERSION=23 -DOPENFILE_CONFIG=./configs/android -Bandroid-build
cmake --build android-build

# Building on Android (Debug in simulator)

cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=/Users/rschmitt/Library/Android/sdk/ndk/23.1.7779620/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DANDROID_PLATFORM=android-23 -DCMAKE_SYSTEM_VERSION=23 -DOPENFILE_CONFIG=./configs/android -Bandroid-build
cmake --build android-build

run a simulator

adb push android-build/of_core/test/test_* /data/local/tmp
adb push configs/android_debug.xml /data/local/tmp
adb shell
cd /data/local/tmp
chmod +x test_*
export OPEN_FILES_HOME=/data/local/tmp/android_debug.xml


