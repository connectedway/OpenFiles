openfiles for Linux
===================

# Introduction

This repository contains the source code for the Openfiles Framework
and SMB Extensions

# Supported Distros

There are two supported distributions of Openfiles for Linux:

- Ubuntu 20.04
- Ubuntu 21.04

Others are likely supported but not currently qualified.

# Background

This readme is specific to Linux deployments of Openfiles.  You can view
[the Main Readme](https://github.com/connectedway/openfiles/blob/main/README.md)
to learn about support for other platforms.

# Deploying Openfiles on Linux

## Registering for the Open Files git repos

In order to build the openfiles smb distribution, you will need to be
registered for the https://github.com/connectedway repos.  If you are only
building and deploying the core support, you do not need to register.  In
this case, you can skip directly to [Setting up Your Worspace](#setting-up-your-linux-workspace)

If you already have a public/private keypair you would like to use, you can
skip the creation step.

### Create a public/private key pair

Only do this if you do not already have a public/private key pair that you
would like to use for git.

You can find out more info [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)

### Send the public key to Connected Way.

Once we receive your public keys, we will add as deploy keys for Connected
Way's protected git repos.  A public key will typically have a suffix of
.pub.  Protect your private key from unauthorized access.

## Setting up your Linux workspace

Clone the openfiles repository

```
$ git clone https://github.com/connectedway/openfiles.git
```

Initialize the core submodules.

```
$ git submodule init of_core_cheap of_core_binheap of_core Unity \
> of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
$ git submodule update of_core_cheap of_core_binheap of_core Unity \
> of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
```

If you have registered for the private repos and wish to include the smb
support, initialize the smb submodules:

```
$ git submodule init of_smb of_smb_fs of_smb_client of_security \
> of_smb_browser
$ git submodule update of_smb of_smb_fs of_smb_client of_security \
> of_smb_browser
```

NOTE: There is a separate readme if you wish to include the JNI support or
the smb server support.

To generate an smb build of openfiles on hardknott, issue:

# Building A Linux Deployment of OpenFiles

If you wish to build just a core OpenFiles version (i.e. without smb support),
issue the following command:

```
make linux
```

If you wish to build a Version of OpenFiles with SMB support, issue the
following command:

```
make linux-smbfs
```

# Testing the Linux Build

You can run a Unity orchestrated test of Openfiles by issuing:

```
make linux-test
```

# Installing Linux Artifacts

You can install the Linux artifacts into the system directories by
issuing the following command:

```
sudo make linux-install
```

# Uninstalling the Linux Artifacts

You can uninstall the Linux artifacts from the system directories by
issuing the following command:

```
sudo make linux-uninstall
```

# Cleaning up the Build Directory

You can clean up all the build artificats by issuing:

```
make linux-clean
```

# Explore the File System

Openfiles and openfile artificts will be installed in the following locations:

- /usr/bin/openfiles/smbcp: an openfiles smb aware copy utility (see below).  Only installed on the smb variant of openfiles.
- /usr/bin/openfiles/test_dg: a Datagram loopback test application
- /usr/bin/openfiles/test_event: A openfiles event test
- /usr/bin/openfiles/test_iovec: Tests of vector based messages
- /usr/bin/openfiles/test_path: A path manipulation test
- /usr/bin/openfiles/test_perf: A test of the performance measurement facility
- /usr/bin/openfiles/test_stream: A TCP loopback test application
- /usr/bin/openfiles/test_thread: A test of openfiles threading
- /usr/bin/openfiles/test_timer: A test of openfiles timers
- /usr/bin/openfiles/test_waitq: A test of openfiles wait queues.
- /usr/bin/openfiles/test_fs_linux: A test of local file I/O
- /usr/bin/openfiles/test_fs_smb: A test of remote smb file I/O.  Only installed on an smb variant of openfiles.
- /usr/bin/openfiles/test_all: An aggregate of all tests
- /usr/lib/libof_core_shared.so.1*: The openfiles framework
- /usr/lib/libof_smb_shared.so.1*: The openfiles smb support.  Only installed on an smb variant of openfiles

NOTE: the test_* applications are designed as continuous integration (CI)
applications.  As such they are statically linked and configured statically
during the build.  The static configuration is fine for CI deployment but
not really appropriate in a deployed system.  This becomes an issue with
the test_fs_linux and test_fs_smb applications.  The path to use for the
test is statically configured during the build.  For test_fs_linux it is
configured with `/tmp/openfiles`.  For test_fs_smb it is configured with
`//****:****@192.168.1.206:445/spiritcloud/`.  The static configuration
for test_fs_linux may be fine for running in a non CI environment but
obviously the test_fs_smb configuration will be inappropriate in a non-ci
environment.

Fortunately for both, the default test location can be overridden in the
runtime configuration file which is installed by yocto in `/etc/openfiles.xml`.
If this file hasn't been modified previously, there will be a section in the
file that appears as:

```
  <drives></drives>
```

Simply change this to:

```
  <drives>
    <map>
      <drive>test</drive>
      <description>directory for test_fs_linux or test_fs_smb</description>
      <path>PATH_TO_DIR</path>
    </map>
  </drives>
```

Where PATH_TO_DIR is the path to the temporary directory to use.  Both
test_fs_linux and test_fs_smb will use the same map and the path can specify
either a local or remote file.  A file specification is of the form:

```
[//username:password:domain@server/share/]path.../file
```

Where the presence of a leading `//` signifies a remote location.  The
absence of a leading `//` signifies a local location.  So in the case of
test_fs_linux, we may want the path element to be:

```
      <path>/tmp/openfiles</path>
```

and for test_fs_smb, we may want the path element to be:

```
      <path>//username:password@server/share/openfiles</path>
```

Replace `username`, `password`, `server`, and `share` with values appropriate
for you network topology.

Note that both test_fs_linux and test_fs_smb will create the directory if
it doesn't exist but will only create one level.  In other words, if the
parent directory doesn't exist, the test will fail.

For those curious, test_fs_linux and test_fs_smb are nearly identical
programs.  The main difference between the two is in the way they are
linked.  test_fs_smb will link with the smb support while test_fs_linux will
not.  test_fs_linux can only be run with local paths.  test_fs_smb can be
run with either. Therefore, running test_fs_smb with a local file will
result in the the same test as teset_fs_linux.  Running test_fs_linux with a
remote file will result in a failure.

```
$ ./build-yocto-smbfs/of_core/test/test_fs_linux
1502715539 OpenFiles (main) 5.0 1
1502715539 Loading /etc/openfiles.xml
1502715539 Device Name: localhost
Unity test run 1 of 1
1502715539 Starting File Test with //*****:*****@192.168.1.206/spiritcloud/openfiles

1502716543 Failed to Validate Destination //****:****@192.168.1.206/spiritcloud/openfiles, Not Supported(50)
.

-----------------------
1 Tests 0 Failures 0 Ignored 
OK
Total Allocated Memory 0, Max Allocated Memory 8767
```

# smbcp Example Application

Once the openfiles framework has been installed onto your linux machine,
you can begin to develop applications that utilize it.  ConnectedWay provides
the smbcp application as an example of integrating OpenFiles into a new
or existing custom application.  You can find out more
[Here]((https://github.com/connectedway/smbcp/blob/main/README.md)

