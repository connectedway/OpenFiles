/**
 * \mainpage Open Files Software Framework
 *
 * Open Files is a lightweight (<256K), multi-platform, real-time, 
 * event-driven, and high performance, subsystem for application development.
 *
 * The main advantages of Open Files are:
 *
 * - It is multi-platform.  Open File applications can run on any supported
 * platform including Windows, MacOS, Android, Linux, iOS, and many RTOSes.
 * - It supports a real-time, event-driven programming model as well as
 * traditional thread based models
 * - It supports multiple file system handlers for platform files as well
 * as virtual and networked file systems
 * - It has a well defined platform abstraction layer for easy extensibility
 * to other platforms.
 * - It uses the a cmake generated build system.  Meaning it has a cross 
 * platform build system and has support for leading GUIs
 * - It has a built in unit test framework based on Unity.
 * - It supports IPv6 and IPv4.
 * - Integrated support for parsing and generating DOM documents
 * - Support for persistent configurations
 * - Built in performance statistics for core constructs.
 * - Built in Heap Debugging, Leak Detection, Buffer overruns and more.
 * - Open Source, with restrictions for attribution and derivatives.
 *
 * In addition to the core Open Files facilities, there are extensions
 * based on Open Files to provide for:
 *
 * - Private Security Libraries
 * - SASL Authentication
 * - Message Signing
 * - SMB client and server support
 * - FUSE file system handlers for Linux, Windows, and MacOS
 * - Java JNI for Open Files API and SMB
 * - Android Document Provider
 * - NetBIOS Name Server and Name Lookup.
 * - Microsoft lan manager support for Pipes, Mailslots, and DCE
 * - Kerberos/Active Directory Integration.
 *
 * This document details the structure and APIs available to platform 
 * integrators for initializing, configuring, starting up, and monitoring
 * the Open Files stack.  This document also details the various APIs utilized
 * by the Open Files stack to interface with the platform specific routines
 * (i.e. the porting layer).
 *
 * This is geared towards two audiances: 
 * - those wishing to integrate an Open Files package to a target system
 * - those wishing to develop applications that use the functionality
 *  provided by Open Files.  
 *
 * A separate document describes how to extend Open Files with additional
 * platform and file system support.
 *
 * - \subpage introduction
 * Who is Open Files for?  What kind of things can I do with it?  How
 * do I work with it?  What else can it do?  These questions and more
 * will be answered in the introduction.
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
 * robust multi-platform, asynchronous or threaded applications. The
 * facilities provided are discussed in this section.
 *
 * - \subpage async
 * A major feature of open files is it's support for asynchronous concurrency
 * model.  To learn more about the async programming model, read here.
 */

/**
 * \page introduction Introducing Open Files
 *
 * \section History
 * There's a bit of history to Open Files.  We wrote this and leveraged it
 * within our ConnectedNAS and ConnectedSMB products and before that, our
 * Blue Share CIFS product.  Those products all required platform 
 * abstraction, event driven processing, file handling, persistance, and other 
 * capabilities.  There aren't many simple C language frameworks available on
 * both desktop and embedded platforms. 
 *
 * \section Purpose
 * So, what is it for?  If you want to write a module that will run on Windows,
 * MacOS, Linux, and Android.  You'd rather develop to a single target and
 * then simply validate the product on all targets after integration.  You
 * want a robust model for handling many real-time events with minimal
 * OS interaction and resources.  Or perhaps you're an embedded developer and
 * would like to develop on a desktop before integrating the app with an RTOS.
 * Open Files is essentially for that.  We have plugins for SMB client and
 * server, persistence, Android Document Provider, security, and more all
 * based on this framework.  
 *
 * \section Functions
 * What kind of things can you do with it?  Open Files provides it's own C
 * runtime, thread/process abstraction, time of day, XML, extensible 
 * persistence and configuration, sockets and networking, file handling and 
 * multiple plugin file handlers, buffer management, locks, 
 * plugin heap managers, real-time event scheduling, and much more.  
 *
 * \section Asynchronous 
 * Open Files async scheduling is based on the notion of single run loop 
 * implemented as a platform thread.  All scheduling occurs within the run loop.
 * There can be multiple run loops and those run loops can be assigned to 
 * individual cores for a robust SMP solution.
 *
 * \section Multi Platform
 * Best of all, an application built on one target implicitly supports all
 * platforms.  Open Files is extensible so that new platforms, heaps, 
 * and file handlers, can easily be added.
 *
 * \section Open Source
 * We distribute under the Creative Commons, Attribution, No Derivitives License.
 * We want to provide the software to the community but wish to avoid 
 * exploitation.  We are flexible so if you wish to license it differently, 
 * just contact us via our contact page on our website 
 * (\ref http://www.connectedway.com)
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
mkdir build
cmake -Bbuild -DOPENFILE_CONFIG=./configs/acme.build
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
mkdir build
cmake -Bbuild -DOPENFILE_CONFIG=./configs/default .
\endverbatim
 *
 * To build, simply issue the cmake command with the build argument.
 *
 * \verbatim
cmake --build build
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
 * The individual test applications (other than test_all) are designed to test
 * specific functionality provided by Open Files.  
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
 * The framework supports two application concurrency models: Async and 
 * Threading.  The async model is where a single run loop dispatches to 
 * registered event handlers while the threading model is where multiple
 * threads are used to repeatedly process and then block within some 
 * application structure.  Open Files supports both models and provides our
 * own Async model.  The threading model doesn't require much discussion.  It
 * essentially just follows a 'main' or 'pthread' type structure where APIs
 * either block or provide events that can be tested or waited on later on.
 * The Async model is discussed in \ref async.
 *
 * The various facilities/components available to an application developer are:
 *
 * - Abstracted Types \ref types
 * - Versioning \ref version
 * - Event Driven Applications \ref app
 * - Console I/O \ref console
 * - Initialization \ref core
 * - DOM Parser \ref dom
 * - Environment Variables \ref env
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

/**
 * \page async Open Files Asynchronous Programming Model
 *
 * The two main components that provide the async programming model are the
 * Open Files Scheduler and the Open Files Application Manager.  These two
 * components utilize a collection of eventing APIs available to the Treading
 * model as well.
 *
 * There can be one or more schedulers.  A scheduler is a run-loop that
 * dispatches events.  Each run loop is a platform thread.  
 * Although everything can run in a single run-loop, there
 * may be a desire to leverage some QoS on platform threads, or there may be
 * a desire to distribute run-loops onto each processor of an SMP system.
 * 
 * At application load time or any time else in the life of an application,
 * an async handler (or we call them 'applets' or 'apps') can be created and
 * added to a scheduler.  Each app has a callback template that declares 
 * some app secific info as well as dispatch routines for initializing,
 * syncing of events, processing of events, and destroying.  
 *
 * The 'syncing of events' we call 'preselect'.  When an app is created and
 * after every time the app has handled an event, the preselect routine is
 * called.  The preselect routine typically dispaches on state and determines
 * what events are waitable.  These events are then registered with the 
 * scheduler.  There are all sorts of waitable events (see \ref noteevents).
 *
 * When an event fires, the postselect routine is called with the event that
 * fired.  Based on the event and app state, the postselect routine can 
 * perform whatever processing is required, change state or remain, create
 * new events, signal other events.  As it leaves the postselect routine, 
 * the updated waitable events are communicated to the scheduler.
 *
 * Apps have another callback where it can destroy itself and notify any 
 * waiters on it's app destruction.
 *
 * You can view the test apps distributed as part of of_core for examples
 * of using the concurrency models.  
 * 
 * - \ref test_dg.c "Datagram Test App"
 * - \ref test_event.c "Event Test App"
 * - \ref test_file.c "File Test App"
 * - \ref test_path.c "Path Test App"
 * - \ref test_stream.c "Stream Test App"
 * - \ref test_thread.c "Thread Test App"
 * - \ref test_timer.c "Timer Test App"
 * - \ref test_waitq.c "WaitQ Test App"
 *
 * Because of the fact that apps are executed on the run-loop, if an application
 * developer can insure that there is only one app loop, or ensure that the
 * application context is local to the particular scheduler, then the callbacks
 * are completely thread-safe so no locking/synchronization is required. 
 *
 * \section noteevents Waitable Events
 *
 * A waitable event is one that can be provided to an Async scheduler for an
 * async app, can be added to a waitset and waited on as part of the group, or
 * can be individually waited on.
 *
 * The following events are waitable:
 *
 * - Wait Queue \ref waitq
 * - File \ref file
 * - Network Socket \ref socket
 * - Buffer \ref file
 * - Event (semaphore) \ref event
 * - Timer \ref timer
 *
 * \example test_dg.c
 * This is an example of using datagram sockets and timers in an async
 * application
 *
 * \example test_event.c
 * This is an example of using events and timers in an async
 * application
 *
 * \example test_file.c
 * This is an example of using files and timers in an threaded application.
 *
 * \example test_path.c
 * This is an example of using path names in a threaded application.
 *
 * \example test_stream.c
 * This is an example of using socket streams and timers in an async
 * application
 *
 * \example test_thread.c
 * This is an example of using Open File Threads
 *
 * \example test_timer.c
 * This is an example of using timers in an async application
 *
 * \example test_waitq.c
 * This is an example of using wait queues in an async application
 */
