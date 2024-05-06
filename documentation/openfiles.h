/*!
\mainpage Open Files Software Framework

Open Files is both a SMB Client and Server Network stack and
a lightweight, multi-platform, real-time, event-driven, and 
high performance, subsystem for application development.  The combination
provides a powerful portable, commercial SMB stack for integration with
OEMs and Value added resellers.

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
- It has integrated support for parsing and generating DOM documents
- It has support for persistent configurations
- Built in performance statistics for core constructs.
- Built in Heap Debugging, Leak Detection, Buffer overruns and more.

With the integrated SMB modules, Open Files provides a commercial SMB
stack with support for:

- SMB v2, v3.0, and v3.1
- aes128-ccm and aes128-gcm encryption
- Active Directory/Kerberos Authentication
- NTLM authenticaltion
- Standalone and Domain based DFS
- Client/Server
- Message Signing
- Robust Win32 based file system API
- FUSE file system handlers for Linux, Windows, and MacOS
- Java JNI for Open Files API and SMB
- Android Document Provider
- NetBIOS Name Server and Name Lookup.
- Microsoft lan manager support for Pipes, Mailslots, and DCE

This document details the structure and APIs available to platform 
integrators for initializing, configuring, starting up, and monitoring
the Open Files stack.  This document also details the various APIs utilized
by the Open Files stack to interface with the platform specific routines
(i.e. the porting layer).

This is geared towards two audiences: 
- those wishing to integrate an Open Files package to a target system
- those wishing to develop applications that use the functionality
 provided by Open Files.  

See the following sections for more info:

- \subpage introduction
Who is Open Files for?  What kind of things can I do with it?  How
do I work with it?  What else can it do?  These questions and more
will be answered in the introduction.

- \subpage download
Open Files is not destributed as a binary for target platforms although it
could be.  As such, there isn't an installation step.  That means the first
step in using Open Files is to download or cloan the Open Files repository.

- \subpage configuration
Open Files comes standard with support for Windows, Linux, MacOS, Android
iOS, ThreadX, and NucleusPLUS.  Default configurations for these platforms
are provided.  It is also possible to create configurations for new 
platforms or to customize existing configurations.

- \subpage building
Open Files uses the cmake build system.  There are three development
environments we recommend:
- CLion  <a href=https://www.jetbrains.com/clion/>Jetbrains CLion</a>
 as a multi-target cmake aware IDE.  This is a licensed product and 
although nice, the cost may not be appropriate.
- Emacs with emacs-c-project plugin.  
multi-platform, free, and although it does not have a pretty IDE, it can 
do everything any other IDE can.
- Command Line.  Based on Make/cmake.  This is preconfigured and
easy to use.

- \subpage testing
Open Files is essentially a framework or library that you can link with
an application to generate an executable.  Open Files provides a collection
of test applications that are products of the build.

- \subpage developing
Open Files provides a full set of APIs that allow developers to implement
robust multi-platform, asynchronous or threaded applications. The
facilities provided are discussed in this section.

- \subpage async
A major feature of open files is it's support for asynchronous concurrency
model.  To learn more about the async programming model, read here.
*/

/*!
\page introduction Introducing Open Files

\section History
There's a bit of history to Open Files.  Prior to OpenFiles, the smb
stack was marketed as ConnectedNAS and ConnectedSMB.  These products
were less module with less flexible configuration than OpenFiles.
Before that, there was the Blue Share CIFS Product.  This product only
supported an SMBv2/SMBv2 client.

\section Purpose
As a framework, it provides a portable platform independent abstraction
for Windows, MacOS, Linux, Android, and select RTOSes.  
It provides a sophisticated real-time, asynchronous programming model.
Non-blocking, state-driven applications can be easily deployed.

As an SMB stack, Open Files is a SMBv2/SMBv3 Client/Server stack supporting
Active Directory and NTLM authentication, Distributed File System,
security, and many other features.

\section APIs
Open Files provides it's own C
runtime, thread/process abstraction, time of day, XML, extensible 
persistence and configuration, sockets and networking, file handling and 
multiple plugin file handlers, buffer management, locks, 
plugin heap managers, real-time event scheduling, and much more.  

\section Asynchronous Asynchronous Real-Time Event-Driven
Open Files async scheduling is based on the notion of single run loop 
implemented as a platform thread.  All scheduling occurs within the run loop.
There can be multiple run loops and those run loops can be assigned to 
individual cores for a robust SMP solution.

\section platform Multi-Platform
Open Files native platform is Linux but applications built with it
implicitly supports all platforms.  Open Files is extensible so that 
new platforms, heaps, and file handlers, can easily be added.

\section license Source Licenses
Our core framework is distributed under the Creative Commons, 
Attribution, No Derivitives License.

We want to contribute to the community but wish to avoid 
exploitation.  We are flexible so if you wish to license it differently, 
just contact us via our contact page on our 
<a href=http://www.connectedway.com>website</a>

The SMB modules are distributed under a proprietary Connected Way License.
*/

/*!
\page download Accessing the Source Code.

The Connected Way github page is available at:

<a href=https://github.com/connectedway>https://github.com/connectedway</a>

There are various repositories which can be downloaded or cloaned 
individually although all the necessary repositories are available as
git submodules within the openfiles repository.  You can clone the 
openfiles repository
as follows:

\verbatim
git clone https://github.com/connectedway/openfiles.git
\endverbatim

This will clone and checkout the main branch in the openfiles repository.
The repository will contain tags for specific releases.  If you wish to
run a particular release, checkout that tag as follows:

\verbatim
git checkout <release-tag>
\endverbatim

At the time of this documentation update, the latest release tag is
5.3.0.  There may be more recent tags.  Contact Connected Way Support
for the tag that we recommend for you.  

Open Files is a collection of cooperating repositories allowing for flexible
configuration and build.  Most repositories are licensed under the 
Create Common license (see LICENSE.md).  Some repositories (eg. smb 
related repositories) are licensed separately and may not be accessible to
all developers.  Various platforms require various submodule repositories.
For the purpose of this user manual, we are documenting the steps 
necessary for a linux SMB client platform.  If you require a different 
build environment, contact Connected Way Support for assistance.

\verbatim
make linux-smb-init
make linux-smb-update
\endverbatim

This will bring in all accessible repositories and sync them with the 
openfiles main branch or release tag.
*/

/*!
\page configuration Configuring a Platform Build

Open Files has both runtime and build time configuration settings.

Static Build Settings
===================== 

The build settings are grouped into the following configuration buckets:

- platform (macos, linux, win, android)
- debug (debug, nodebug)
- smb (nosmb, smbclient, smbserver)
- cipher (openssl, gnutls, mbedtls)
- jni (jni, nojni)

The high level makefile contains targets for a build tuple.  Tuples
are formed by concatenating configuration buckets.  For example, the
following tuples are supported:

 * \verbatim
macos-debug-nosmb-openssl-nojni
linux-nodebug-smbclient-openssl-nojni
\endverbatim

A target further appends a build phase.  The phases of a build are:

- clean: clean the build
- config: Configure a build
- build: Perform a build
- install: Install build artifacts into system
- uninstall: Remove build artifats from system
- test: Run Unit Tests on build
- docs: Build this document for that platform
- reinstall: Build and Reinstall
- info: Echo the build tuple
- full: Perform a clean, config, build, install, and test

For instance, to make and install an smbclient package using
opensl on linux for production, one would issue:

\verbatim
make linux-nodebug-smbclient-openssl-nojni-full
\endverbatim

There is a special target that will build all supported tuples for
a platform.  For building all linux targets, one coud type:

\verbatim
make linux-full
\endverbatim

There is another special target for building a production smbclient
using openssl on linux.  

\verbatim
make linux-smb-client-<phase>
\endverbatim

Where phase is any of the build phases (clean, config, etc.)

Configuration Parameter Documentation is available here: \ref config
*/

/*!
\page building Building the Open Files Stack
To build using CLion, simply run the CLion IDE and use the File Open
item to open the directory of the openfiles repository.  Once opened,
it will automatically generate the necessary makefiles for your platform.
Then using the Build Project menu item will generate all targets.

To build first you will need to generate the 
necessary makefiles.  This can be done by running a command shell and 
issuing the following command:

\verbatim
make linux-smb-client-config
\endverbatim

Then you can build the product by issuing:
\verbatim
make linux-smb-client-build
\endverbatim

To install, simply issue the following command.  You will need to
have your user configured as an sudoer.

\verbatim
make linux-smb-client-install
\endverbatim

The following static and shared libraries will be installed in
`/usr/local/lib64/`:

- libof_core_shared.so
- libof_core_static.a
- libof_netbios_shared.so
- libof_netbios_static.a
- libof_smb_shared.so
- libof_smb_static.a
*/

/*!
\page testing Running Generated Test Applications

The Open Files build will generate the following test applications 
and install them at `/usr/local/bin/openfiles`:

- of_core/test/test_stream
- of_core/test/test_waitq
- of_core/test/test_fs_linux
- of_core/test/test_all
- of_core/test/test_timer
- of_core/test/test_thread
- of_core/test/test_event
- of_core/test/test_path
- of_core/test/test_dg
- of_core/test/test_iovec
- of_core/test/test_perf
- of_core/test/test_pipe
- of_core/test/test_subpersist
- of_smb_fs/test/test_fs_smb

\note
There are a set of smb file applications that are installed as a part
of another package `smbcp`.  See \ref smbcp.

test_all will run tests contained within the other test applications.
The individual test applications (other than test_all) are designed to test
specific functionality provided by Open Files.  

By default, Open Files will use a persistent runtime configuration.
There are preconfigured runtime templates in the config subdirectory.
There are default configurations based on the first three components
of the build tuple.  For instance, for the build tuple
linux-nodebug-smbclient-openssl-nojni, the default runtime configuration
is linux-nodebug-smbclient.  When the product is installed
the default runtime configuration is installed as /etc/openfiles.xml.

When the Open Files library is initialzed, it will look at the
OPEN_FILES_HOME environment variable.  If set, it will look at that
path for the runtime configuration.  If not set it will look at
/etc/openfiles.xml.

To set the runtime configuration, execute the following prior to 
running the program that has linked with the Open Files stack:

\verbatim
export OPEN_FILES_HOME=./configs/darwin_debug.xml
\endverbatim
*/

/*!
\page developing Developing with Open Files

Open Files provides a very robust set of platform independent functions
that allow a developer to accelerate implementation of multi-platform 
applications.  If your only focus is to integrate with the SMB APIs, 
this section can be skipped.

Applications can be traditional thread based or can utilize
the event-driven application framework provided by Open Files.  The 
event-driven model allows for scheduling any number of non-blocking
applications scheduling threads.  There can be one scheduling thread per
application, or multiple scheduling threads can be deployed either by
function or by CPU.

The framework supports two application concurrency models: Async and 
Threading.  The async model is where a single run loop dispatches to 
registered event handlers while the threading model is where multiple
threads are used to repeatedly process and then block within some 
application structure.  Open Files supports both models and provides our
own Async model.  The threading model doesn't require much discussion.  It
essentially just follows a 'main' or 'pthread' type structure where APIs
either block or provide events that can be tested or waited on later on.
The Async model is discussed in \ref async.

The various facilities/components available to an application developer are:

- Abstracted Types \ref types
- Versioning
- Event Driven Applications
- Console I/O
- Initialization
- DOM Parser
- Environment Variables
- Events (semaphores)
- File Handling \ref file
- Management Interface \ref framework
- File System Handling
- Object Hiding/Handles
- Heap Management \ref heap
- C like Runtime Library
- Locks (mutexes)
- Network Messages
- Network Interfaces and Discovery
- File System Path Handling \ref path
- Persistence
- Process Management
- Queue Management
- Simple XML Parser
- Event Driven Scheduler
- Network Sockets
- Thread Management
- Time Handling
- Timer Management
- Waitable Queues
- Waitable Sets of Events
*/

/*!
\page async Open Files Asynchronous Programming Model

This section is intended for those contributing to the Open Files stack,
or developing applications that leverage the internal APIs of the 
Open Files stack.  If you are simply integrating the stack with an
application, you need not be concerned with this section.

The two main components that provide the async programming model are the
Open Files Scheduler and the Open Files Application Manager.  These two
components utilize a collection of eventing APIs available to the Treading
model as well.

There can be one or more schedulers.  A scheduler is a run-loop that
dispatches events.  Each run loop is a platform thread.  
Although everything can run in a single run-loop, there
may be a desire to leverage some QoS on platform threads, or there may be
a desire to distribute run-loops onto each processor of an SMP system.

At application load time or any time else in the life of an application,
an async handler (or we call them 'applets' or 'apps') can be created and
added to a scheduler.  Each app has a callback template that declares 
some app secific info as well as dispatch routines for initializing,
syncing of events, processing of events, and destroying.  

The 'syncing of events' we call 'preselect'.  When an app is created and
after every time the app has handled an event, the preselect routine is
called.  The preselect routine typically dispaches on state and determines
what events are waitable.  These events are then registered with the 
scheduler.  There are all sorts of waitable events (see \ref noteevents).

When an event fires, the postselect routine is called with the event that
fired.  Based on the event and app state, the postselect routine can 
perform whatever processing is required, change state or remain, create
new events, signal other events.  As it leaves the postselect routine, 
the updated waitable events are communicated to the scheduler.

Apps have another callback where it can destroy itself and notify any 
waiters on it's app destruction.

You can view the test apps distributed as part of of_core for examples
of using the concurrency models.  

- \ref test_dg.c "Datagram Test App"
- \ref test_event.c "Event Test App"
- \ref test_file.c "File Test App"
- \ref test_path.c "Path Test App"
- \ref test_stream.c "Stream Test App"
- \ref test_thread.c "Thread Test App"
- \ref test_timer.c "Timer Test App"
- \ref test_waitq.c "WaitQ Test App"

Because of the fact that apps are executed on the run-loop, if an application
developer can insure that there is only one app loop, or ensure that the
application context is local to the particular scheduler, then the callbacks
are completely thread-safe so no locking/synchronization is required. 

\section noteevents Waitable Events

A waitable event is one that can be provided to an Async scheduler for an
async app, can be added to a waitset and waited on as part of the group, or
can be individually waited on.

The following events are waitable:

- Wait Queue
- File \ref file
- Network Socket
- Buffer \ref file
- Event (semaphore)
- Timer

\example test_dg.c
This is an example of using datagram sockets and timers in an async
application

\example test_event.c
This is an example of using events and timers in an async
application

\example test_file.c
This is an example of using files and timers in an threaded application.

\example test_path.c
This is an example of using path names in a threaded application.

\example test_stream.c
This is an example of using socket streams and timers in an async
application

\example test_thread.c
This is an example of using Open File Threads

\example test_timer.c
This is an example of using timers in an async application

\example test_waitq.c
This is an example of using wait queues in an async application

\example smbcp.c
This is an example of a application level file copy using the
OpenFiles Libraries

\example smbls.c
This is an example of a application level directory list using the
OpenFiles Libraries

\example smbfree.c
This is an example of a application level disk free space command using the
OpenFiles Libraries

\example smbrm.c
This is an example of a application level file remove using the
OpenFiles Libraries

\example template.xml
This is a template for runtime configuration
*/


/*!
\page smbcp SMB Applications

Openfiles also comes with a set of SMB Applications that can either be
used as examples on how to integrate the SMB APIs into customer
applications or they can be used as binaries that can be integrated
as system calls into a deployed system.

The SMBCP applications are maintained as a separate <a href=https://github.com/connectedway/smbcp.git>git repo</a>:

\verbatim
https://github.com/connectedway/smbcp.git
\endverbatim

The smbcp applications require OpenFiles SMB client libraries to be
installed first.  There are 4 applications:

- smbcp: Copy a file - \ref smbcp.c
- smbrm: Remove a file - \ref smbrm.c
- smbfree: Show freespace - \ref smbfree.c
- smbls: List a directory - \ref smbls.c

The smbcp applications are dynamically linked.  The applications themselves
are built seperately from the openfiles framework and they are simply linked
with the framework on the target system.

================
Command Overview
================

The syntax and decription of the OpenFiles SMB commands are describe
here.

-----
smbcp
-----

\anchor smbcpcmd
Copy a file to a destination file.  The syntax of smbcp is:

\verbatim
$ smbcp [-a] <source> <destination>
\endverbatim

Where -a signifies that the copy operation should be done asynchronously
using multiple overlapped buffers.  The absense of -a specifies that the
copy is done synchronously.  Only one I/O is outstanding at a time in
synchronous mode.

The target file must be fully specified.  In other words, you must provide
the target file name.  Simply specifying the target directory is not
sufficient.

Openfiles can operate in one of two authentication mode: Active Directory 
and NTLM.  Active Directory authentication requires a kerberos stack 
preinstalled on your target system and active domain controllers with
users and target computers.  Before running any of the smbcp applications
in Active Directory mode, you must first obtain a user ticket.  On linux
this can be done with the `kinit` kerberos utility.

\verbatim
$ kinit <user>@<domain>
\endverbatim

The utility will prompt for your domain password.

smbcp can copy a file from local or remote locations to a file that resides
locally or remotely.  A file specification is of the form:

\verbatim
[//username:password:domain@server/share/]path.../file
\endverbatim

If authenticated with Active Directory (or samba ad/dc), you will not
need to specify a username or password in the URL.  And if using 
Active Directory the domain portion of the URL is used to specify the
path to the kerberos ticket cache to use.  If not specified, the default
cache is used.

If authenticating with NTLM (this is called standalone authentication),
the username and password is required.  The domain portion is optional but
allows the target to authenticate the user against the specified domain.

Path can be any depth as long as the total file path is less than
256 bytes.  For example:

\verbatim
$ smbcp //me:secret@remote/share/subdirectory/picture.jpg ./picture.jpg
\endverbatim

This command will access the server named 'remote' and log in using
the username 'me' and the password 'secret'.  It will implicitly mount
the remote share named 'share' and will access the file
'subdirectory/picture.jpg' relative to the share.  It will copy the
file locally to the filename picture.jpg.

The source path and destination path can be either local or remote so
copies from a remote to a local, from a local to a remote, from a local to
a local, and from a remote to a remote are all possible.

-------
smbfree
-------

Find the freespace of the target file system.  The syntax of smbfree
is:

\verbatim
$ smbfree <target>
\endverbatim

Where target is a path that include a remote share.  The format of the URL
is the same as described in the \ref smbcpcmd "smbcp" command.

-----
smbls
-----

List the contents of a target directory.  The syntax of smbls is:

\verbatim
$ smbls <directory>
\endverbatim

Where directory is a path that is a path to a directory.  The format
of the URL is the same as described in the \ref smbcpcmd "smbcp" command.

-----
smbrm
-----

Delete a target file.  The syntax of smbrm is:

\verbatim
$ smbrm <path>
\endverbatim

Where path is a path that is a path to a file.  The format
of the URL is the same as described in the \ref smbcpcmd "smbcp" command.

==============================
Building the smbcp application
==============================

If you are building a yocto based distribution using the of_manifests
repo configuration, the smbcp application will be built and included in
your distribution.

If you are deploying openfiles in a Linux environment, you will need to
clone, build, and install smbcp on your Linux machine.

----------------------------
Cloning the smbcp repository
----------------------------

To clone the repo, simply issue:

\verbatim
$ git clone https://github.com/connectedway/smbcp.git
$ cd smbcp
\endverbatim

------------------------------
Building the smbcp Application
------------------------------

Note that if you wish to build the smbcp application, you will have had to
successfully build and installed the Open Files Framework.  You can find
more <a href=https://github.com/connectedway/openfiles/blob/main/README.md>
here</a> and 
<a href=https://github.com/connectedway/openfiles/blob/main/LINUX.md>here</a>.

Then simply issue:

\verbatim
$ make
\endverbatim

--------------------------------
Installing the smbcp Application
--------------------------------

You can install the application into your system directories by issuing:

\verbatim
$ sudo make install
\endverbatim

The application will be install, by default, in
`/usr/local/bin/openfiles`.  You may wish to add this to your path
variable:

\verbatim
$ export PATH=$PATH:/usr/local/bin/openfiles
\endverbatim

Update your shell startup scripts accordingly

The following applications will be installed:

- /usr/local/bin/openfiles/smbcp
- /usr/local/bin/openfiles/smbfree
- /usr/local/bin/openfiles/smbls
- /usr/local/bin/openfiles/smbrm

----------------------------------
Uninstalling the smbcp Application
----------------------------------

You can uninstall the application from your system directories by
issuing:

\verbatim
$ sudo make uninstall
\endverbatim

-----------------------------
Cleaning your build directory
-----------------------------

You can clean up build artifacts from your build directory by issuing:

\verbatim
$ make clean
\endverbatim

-----------------------------
Running the smbcp application
-----------------------------

You can run the application by following the description in this 
document.

====================
smbcp Implementation
====================

The implementatoin of the smbcp application is relevant for
integrating customer applications with the Linux based openfiles framework.

--------------
smbcp Makefile
--------------

The makefile is a basic make.  The smbcp application consists of one
object file for each utility: smbcp.o, smbfree.o, smbls.o and smbrm.o.
The object file is compiled using default CFLAGS which
includes a specification of the sysroot which will contain the
necessary openfiles header files.  The executable is linked with five
shared libraries: 

- libof_smb_shared 
- libof_core_shared 
- libssl
- libkrb5 
- libgssapi_krb5

The makefile, as written, links with the openssl crypto libraries.  Open
Files supports opensl, mbedtl, and gnutls.  See the (see config) if
you wish to uses a different crypto library and update this Makefile 
accordingly.

We specify the linker option `--no-as-needed` which directs the linker to
include the libraries whether they are explicity referenced in the object
files or not.  This is needed if we wish to utilize implicit library
constructors.  As a conveniece to openfile application developers, we will
initialize the stack implicitly upon library load rather than requiring the
developer to initialize the stack within the application itself.

-----------------
smbcp Source Code
-----------------

This readme will walk through the implemenation of the smbcp.c
application.  The other applications will have similar APIs and
are not described here.

This readme will not go through line by line within the smbcp.c source file.
Rather, we'll call out relevant sections.

An openfiles application can use standard C libraries and with respect to
smbcp, we use five.  The wchar.h file is included so we can convert ASCII
characters to wide characters used as part of the openfiles API.  NOTE that
openfiles exposes an ascii API as well but wide characters is recommended.

There are seven openfiles header files used by this application.
Openfiles provides a robust set of APIs for many services.  We
recommend viewing the /ref openiles documentation for more detail.
The Openfiles recipe and makefiles installs a subset of the available
APIs into the yocto sysroot.  If you find that particular headers are
not available in your sysroot, Connected Way support will be glad to
export them for you.

A brief description of the headers used:

- ofc/config.h - This provides constants that the openfiles framework was
built with.  This will have defines for the maximum buffer size, whether
smb I/O is supported and more.
- ofc/handle.h - This defines a generic handle type used to refer to
openfile objects.  Handles are used throughout openfiles and can refer to
files, queues, events, sockets and more.
- ofc/types.h - This defines the basic types used by openfiles.
- ofc/file.h - The API used by the openfiles file system.
- ofc/waitset.h - This allows aggregation of waitable objects.  This is used
by the asynchronous file copy to manage asynchronous completion of multiple
buffers.
- ofc/queue.h - This is a robust abstraction of a doubly linked list.  It
is purely a convenience facility but is heavily used throughout the openfiles
framework and within the smbcp application to maintain lists of buffers.
- of_smb/framework.h - This is generally not needed.  For smbcp, we include
it to expose the API for setting DFS bootstrap DCs.  This should not
be necesary but is provided or example only.

In the simple case, this is the full set of APIs needed to interact with
the openfiles framework.  The main set of APIs is contained in the header
`file.h`.  For the most part, the file API of Open Files is based on the
Windows File APIs.

The smbcp.c source file contains code for both the asynchronous file copy
and synchronous file copy modes.

The synchronous file copy is simple.  It simply opens up the source
and destination files and then enters a loop where a buffer is read from the
read file, and written to the write file.  The loop continues till an
eof on the read file or an error on either.  The entry point of the synchronous
copy is:

\verbatim
static OFC_DWORD copy_sync(OFC_CTCHAR *rfilename, OFC_CTCHAR *wfilename)
\endverbatim

Pseudo code for the simple file copy follows.  Also see: \ref smbcp.c

\verbatim
Begin

    Open the Read File as read_file

    Open the Write File as write_fil

    while Read from read_file into buffer and length is True

        Write to write_file from buffer and length

    Close write_file

    Close read_file

Done
\endverbatim

The asynchronous code supports arbitrary
queue depth of arbitrary buffer size (up to a max of OFC_MAX_IO).  If you
have questions on the operation of the asynchrous copy, please contact
Connected Way support.  The buffer management of the asynchronous copy
is where most of the complication to this utility lies.  The buffer
management utilizes the OpenFiles framework, thus the need for the 
OpenFiles headers.

The entry point to the asynchronous file copy is

\verbatim
static OFC_DWORD copy_async(OFC_CTCHAR *rfilename, OFC_CTCHAR *wfilename)
\endverbatim

The main entry point parses the arguments, converts the ascii file names
into wide character file names and calls the respective asynchronous or
synchronous entry points.

The APIs used by smbcp are:

APIs for Synchronous File Copy:

- OfcCreateFile - Create or Open a Local or Remote file.
- OfcReadFile - Read from a Local or Remote File
- OfcWriteFile - Write to a Local or Remote File
- OfcGetLasstError - Get the last error code encountered
- OfcCloseHandle - Close a Local or Remote File
- ofc_get_error_string - Convert a last error into a string

APIs for Asynchronous File Copy:

- OfcSetOverlappedOffset - Set the offset for an overlapped buffer
- OfcGetOverlappedResult - Get the result of an overlapped i/o
- OfcDestroyOverlapped - Destroy an overlapped buffer
- OfcCreateOverlapped - Create an overlapped buffer
- OfcReadFile - Read from a local or remote file
- OfcWriteFile - Write to a local or remote file
- OfcCreateFile - Create or Open a local or remote file
- OfcCloseHandle - Close a local or remote file
- OfcGetLastError - Get the last error code encountered
- ofc_waitset_add - Add an object to a list of waitable objects
- ofc_waitset_remove - Remove an object from a list of waitable objects
- ofc_waitset_destroy - Destroy the list of waitable objects
- ofc_waitset_create - Create a list of waitable objects
- ofc_waitset_wait - Wait for a buffer completion
- ofc_handle_get_app - Get a buffer association
- ofc_dequeue - Dequeue a buffer from a buffer list
- ofc_queue_create - Create a buffer list
- ofc_enqueue - Queue a buffer to a buffer list
- ofc_queue_first - Get the first item on the buffer list
- ofc_queue_next - Get the subsequent item on a buffer list
- ofc_get_error_string - Convert a last error into a string

It is clear from the list of APIs used between synchronous and asynchronous
file copy that the asynchronous mode is more complex but it can offer
considerable performance improvements

This is the full list of APIs required for either mode.  This should help in
understanding the level of effort in integrating openfiles with a customer
application.

There are a few features of Open Files which allow this simple set of APIs:

- Implicit Framework Initialization and Destruction.  No explicit
initialization required although explicit and static initialization is
supported.
- Implicit Remote Connections.  Connections to remotes do not need to be
explicitly established.  Remote URLs are examined and if the URL is to a
remote that does not have a connection, a connection will be established
and authenticated.  Connections are closed when idle.
- Implicit Share Mounting.  Mounting of a share is performed implicity when
accessing a file through the URL.  No explicit mounting or unmounting of
a share is required.  Shares are dismounted when idle.
- Integrated local and remote file APIs.  No need to be aware of whether a
file is local or remote.  The framework handles the difference internally.
*/

