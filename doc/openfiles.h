/**
 * \mainpage Open Files Software Framework
 *
 * Open Files is a multi-platform, real-time, event-driven, high performance, 
 * and small footprint subsystem for application development.
 *
 * This document details the structure and APIs available to platform 
 * integrators for initializing, configuring, starting up, and monitoring
 * the Open Files stack.  This document also details the various APIs utilized
 * by the Open Files stack to interface with the platform specific routines
 * (i.e. the porting layer).
 *
 * The Open Files Documentation leverages the doxygen documentation generation
 * tool.  Doxygen arranges the product documentation in seven or more sections.
 * To make navigation more intuitive, Open Files has added this main page
 * and a collection of subpages that attempt to guide a reader through the
 * APIs and relates the APIs to architectural constructs and workflow 
 * interests.
 *
 * This document has three target audiances: those wishing to port Open Files
 * to a target platform, those wishing to integrate an Open Files
 * package to a target system, and those wishing to develop applications
 * that use the functionality provided by Open Files.  
 *
 * - \subpage download
 * Open Files is not destributed as a binary for target platforms although it
 * could be.  As such, there isn't an instllation step.  That means the first
 * step in using Open Files is to download or cloan the Open Files repository.
 *
 * - \subpage configuration
 * Open Files comes standard with support for Windows, Linux, MacOS, Android
 * iOS, ThreadX, and NucleusPLUS.  Default configurations for these platforms
 * are provided.  It is also possible to create configurations for new platforms
 * or to customize existing configurations.
 *
 * - \subpage building
 * Open Files uses the cmake build system.  We recommend CLion 
 * (https://www.jetbrains.com/clion/) as a multi-target cmake aware IDE but
 * using any properly configured cmake tool for your host system should work.
 *
 * - \subpage testing
 * Open Files is essentially a framework or library that you can link with
 * an application to generate an executable.  Open Files provides a collection
 * of test applications that are products of the build.
 *
 * - \subpage developing
 * Open Files provides a full set of APIs that allow developers to implement
 * robust multi-platform, event-driven or sequential applications. The
 * facilities provided are discussed in this section.
 */

/**
 * \page download Accessing the Source Code.
 *
 * The Connected Way github page is available at:
 *
 * https://github.com/connectedway
 *
 * There are various repositories which can be downloaded or cloaned individually
 * although all the necessary repositories are available as git submodules 
 * within the openfiles repository.  You can clone the openfiles repository
 * as follows:
 *
 * \verbatim
git clone https://github.com/connectedway/openfiles.git
\endverbatim
 *
 * This will clone and checkout the main branch in the openfiles repository.
 * The repository will contain tags for specific releases.  If you wish to
 * run a particular release, checkout that tag as follows:
 *
 * \verbatim
git checkout <release-tag>
\endverbatim
 *
 * This page will be updated with release tags as they are made available.
 *
 * Open Files is a collection of cooperating repositories allowing for flexible
 * configuration and build.  Most repositories are licensed under the 
 * Create Common license (see LICENSE.md).  Some repositories (eg. smb 
 * related repositories) are licensed separately and may not be accessible to
 * all developers.  
 *
 * To bring in all dependent repositories, issue the following git commands:
 *
 * \verbatim
git submodule init
git submodule update
\endverbatim
 *
 * This will bring in all accessible repositories and sync them with the 
 * openfiles main branch or release tag.
 */

/**
 * \page configuration Configuring a Platform Build
 *
 * Open Files provides an easy configuration system to set up the cmake
 * build for your target.  You specify the configuration file to use by
 * passing the cmake variable OPENFILE_CONFIG to the cmake command as follows:
 *
 * \verbatim
cmake -DOPENFILE_CONFIG=./configs/acme.build
\endverbatim
 *
 * Default config files provided include:
 * - default_darwin
 * - default_windows
 * - default_linux
 * - default_android
 * - default_threadx
 * - default_nucleus
 *
 * We recommend starting with an existing platform configuration and adapting
 * it as necessary for your particular build.
 *
 * Configuration Parameter Documentation is available here: \ref config
 */

/**
 * \page building Building the Open Files Stack
 * To build using CLion, simply run the CLion IDE and use the File Open
 * menu item to open the directory of the openfiles repository.  Once opened,
 * it will automatically generate the necessary makefiles for your platform.
 * Then using the Build Project menu item will generate all targets.
 *
 * To build using cmake directly, first you will need to generate the 
 * necessary makefiles.  This can be done by running a command shell and 
 * issuing the following command:
 *
 * \verbatim
cmake -DOPENFILE_CONFIG=./configs/default .
\endverbatim
 *
 * To build, simply issue the cmake command with the build argument.
 *
 * \verbatim
cmake --build .
\endverbatim
 *
 * Output will be written to the source directories.  Expected output includes:
 * - of_core/libof_core_static.a
 * - of_core/libof_core_shared.dylib
 * - of_core/libof_core_shared.5.dylib
 * - of_core/libof_core_shared.5.0.1.dylib
 *
 * as well as various test applications in of_core/test
 *
 * Using other cmake options can place the output in other locations
 */

/**
 * \page testing Running Generated Test Applications
 *
 * The Open Files build will generate the following test applications:
 *
 * - of_core/test/test_stream
 * - of_core/test/test_waitq
 * - of_core/test/test_fs_darwin
 * - of_core/test/test_all
 * - of_core/test/test_timer
 * - of_core/test/test_thread
 * - of_core/test/test_event
 * - of_core/test/test_path
 * - of_core/test/test_dg
 *
 * test_all will run tests contained within the other test applications.
 *
 * The individual test applications (other than test_all) are designed to test
 * specific functionality provided by Open Files.  There are an infinite
 * number of tests we could write to test the functionality of Open Files
 * and additional tests will be added over time.
 *
 * If your build is configured with persistent runtime configuration, then
 * you will need to set an environment variable that points to the 
 * runtime configuration to use.  The environment variable can be set
 * prior to the test run, or can be provided on the command line along with
 * the test.
 *
 * \verbatim
export OPEN_FILES_HOME=./configs/darwin_debug.xml
\endverbatim
 *
 * To run a test, simply execute the test application from the command line.
 * The tests do not take any arguments.
 *
 * \verbatim
OPEN_FILES_HOME=./configs/darwin_debug.xml ./of_core/test/test_event
\endverbatim
 *
 * or if the OPEN_FILES_HOME variable has been set previously in
 * your environment, you can simply execute the test.
 *
 * \verbatim
./of_core/test/test_event
\endverbatim
 *
 * Example output of the test_event follows:
 *
 * \verbatim
Unity test run 1 of 1
OpenFiles (main) 5.0 1.0.30
Loading ./configs/darwin_debug.xml
Device Name: localhost
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
Timer Triggered
Event Triggered
.

-----------------------
1 Tests 0 Failures 0 Ignored 
OK
Total Allocated Memory 0, Max Allocated Memory 9310

Heap is Empty, No leaks detected
\endverbatim
 *
 * Note that each test provided will cleanly shutdown the stack and
 * verify that all memory has been cleaned up.  This is very useful
 * for detecting memory leaks within the Open Files stack.
 */

/**
 * \page developing Developing with Open Files
 *
 * Open Files provides a very robust set of platform independent functions
 * that allow a developer to accelerate implementation of multi-platform 
 * applications.  Applications can be traditional thread based or can utilize
 * the event-driven application framework provided by Open Files.  The 
 * event-driven model allows for scheduling any number of non-blocking
 * applications scheduling threads.  There can be one scheduling thread per
 * application, or multiple scheduling threads can be deployed either by
 * function or by CPU.
 *
 * These facilities are grouped below:
 *
 * - Abstracted Types \ref types
 * - Versioning \ref version
 * - Event Driven Applications \ref app
 * - Console I/O \ref console
 * - Initialization \ref core
 * - DOM Parser \ref dom
 * - Environment Variables \ref dom
 * - Events (semaphores) \ref event
 * - File Handling \ref file
 * - Management Interface \ref framework
 * - File System Handling \ref fs
 * - Object Hiding/Handles \ref handle
 * - Heap Management \ref heap
 * - C like Runtime Library \ref libc
 * - Locks (mutexes) \ref lock
 * - Network Messages \ref message
 * - Network Interfaces and Discovery \ref net
 * - File System Path Handling \ref path
 * - Persistence \ref persist
 * - Process Management \ref process
 * - Queue Management \ref queue
 * - Simple XML Parser \ref sax
 * - Event Driven Scheduler \ref sched
 * - Network Sockets \ref socket
 * - Thread Management \ref thread
 * - Time Handling \ref time
 * - Timer Management \ref timer
 * - Waitable Queues \ref waitq
 * - Waitable Sets of Events \ref waitset
 */


