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

There is a separate readme for DFS support.  You can view
[DFS Readme](https://github.com/connectedway/openfiles/blob/main/DFS.md)
to learn more about DFS support.

# Deploying Openfiles on Linux

## Registering for the Open Files git repos

If you are using only the core openfiles support, you can skip directly to
(#setting-up-your-linux-workspace)

In order to build the openfiles smb distribution, you will need to be
registered for the https://github.com/connectedway repos.  If you are only
building and deploying the core support, you do not need to register.  In
this case, you can skip directly to [Setting up Your Worspace](#setting-up-your-linux-workspace)

### Getting an account on Github

If you already have a github account, you can skip this step.  Otherwise,
go to https://github.com and click the `sign up` box near the top right of
the screen.  It will ask you for your email address and will send you an invite
by email.  Click the invite, add a password, and you will be all set.

### Generating a public/private key pair

If you already have a public/private key pair that you wish to use on your
github account, you can skip this step.

You can find out more info [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh)

If you wish to create a new keypair for accessing the Connected Way repos,
open a linux command shell and issue the command:

```
$ ssh-keygen -t ed25519 -C "your_email@example.com"
```

This will ask you for a file name and a password.  The file name is arbitrary.
You can accept the one it suggests or override with a more friendly name.
For password, you can leave it blank or provide a password.  One issue with
providing a password is you will have to type in the password every time you
use the key which may make automated runs cumbersome.

The keygen utility should place your keys in your `$HOME/.ssh` directory.  If
it placed them somewhere else, you should move them into your .ssh directory.

### Uploading a public key to your account on Github

If you have previously uploaded a key to github, and you wish to use that
key for access to Connected Way's repos, you can skip this step.

After you have logged in, click on your account icon near the top right of
the screen, then click `Settings`.

Then click on "SSH and GPG keys" in the explorer pane on the left of the
screen.  Click on  `New SSH Key` in the main window.  Upload the contents
of the public key you are using and give it an arbitrary title.

You can simply `cat` the file to your screen, select the contents, and then
paste it into the box in the "SSH Keys" screen.  Then select `Add SSH Key`

### Add your private key to your SSH client

If you already have added a github entry to your ssh config file with the key
you are using as the identity, you can skip this step.

Create or edit a file named `config` in `$HOME/.ssh`.  Add an entry similar
to:

```
Host github.com
     IdentityFile ~/.ssh/<private-key-file-name>

```

If you have multple identities on github and wish to use a specific keypair
for accessing connected way repos, you could add an entry like:

```
Host connectedway
     HostName github.com
     IdentityFile ~/.ssh/<private-key-file-name>
```

We don't recommend doing this though.  You will have to change the URLs
of all the submodules in the the openfiles git repo (you'll see this
in a following step).

If your network requires a proxy for github access, you will need to add
additional fields to your config entry.  See your system admin for more
info.

## Accept an Invite to the Connected Way Repos

You may recieve invites to collaborate on a few connected way repos.  You
may have accepted those invites prior to setting up your github account or
your account may have been previously set up.  If you have already accepted
them, you can skip this step.

Log into your github account and click on the notification icon near the top
right of your window.  The notification screen will show pending invites.
Click on each invite, and accept it.

## Setting up your Linux workspace

Clone the openfiles repository

```
$ git clone https://github.com/connectedway/openfiles.git
$ cd openfiles
```

Initialize the core submodules.

```
$ git submodule init of_core_cheap of_core_binheap of_core Unity \
of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
$ git submodule update of_core_cheap of_core_binheap of_core Unity \
of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
```

Note: There is a shortcut for manually specifying the submodules
to initialize and update:

```
$ make linux-init
$ make linux-update
```

If you have registered for the private repos and wish to include the smb
client support, initialize the smb submodules:

```
$ git submodule init of_smb of_smb_fs of_smb_client of_security \
of_smb_browser
$ git submodule update of_smb of_smb_fs of_smb_client of_security \
of_smb_browser
```

Note: There is a shortcut for manually specifying the submodules
to initialize and update for the smbclient.  The following make
targets will init and update the core as well as those required
for the smb client:

```
$ make linux-smbfs-init
$ make linux-smbfs-update
```

If you've been good to puppies, you will successfully have access to the
private repos.

NOTE: There is a separate readme if you wish to include the JNI support or
the smb server support.

# Installing the latest version of Cmake

Before building openfiles, you will need to be running a version of cmake
that is greater than 3.20.  Openfiles can be built on many platforms and to
insure our CMakeLists.txt files are compatible, we require cmake to be 3.20
or above.  You can view your cmake version by typing:

```
$ cmake --version
```

To update to 3.25.1, you can visit

https://cmake.org/download/

and download

https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.sh

This will install cmake version 3.25.1 in /usr/local/bin/cmake.

# Install Openssl

```
# sudo apt-get install libssl-dev

# Install Mbedtls

By default, the linux-smbfs build will use openssl.
If you wish you use MbedTLS instead, you will need to update the
`openfiles/configs/linux-smbfs` config file, turn off `OF_OPENSSL`,
and turn on `OF_MBEDTLS`.  You will also need to build and
install mbedtls for linux.

Create a workspace for mbedtls

```
$ mkdir mbedtls
$ cd mbedtls
```

Clone Upstream

```
$ git clone https://github.com/Mbed-TLS/mbedtls
$ cd mbedtls
```

Checkout the 3.2.1 tag

```
$ git checkout mbedtls-3.2.1
```

Then issue make

```
$ SHARED='y' make
```

Then issue make install

```
$ sudo make install
```

# Install Kerberos

The linux-smbfs build of openfiles uses kerberos.  You can install
this with:

```
$ sudo apt install krb5-user
$ sudo apt install libkrb5-dev
```

# Updating a workspace

Periodically, it's a good idea to pull in new upstream changes.  You can
discuss with Connected Way support when an update is recommended.

To perform the update, cd to the root of your workspace:

```
$ cd openfiles
```

Clean the workspace.  If you are working with a core openfiles release rather
than the SMB release, you can issue the following:

```
$ make linux-clean
```

If you are working with an SMB release, you can issue the following:

```
$ make linux-smbfs-clean
```

Review the current state of your workspace

```
$ git status
```

This will output the branch you are on and any local changes.  There shouldn't
be a need for local changes.  If there are, discuss them with Connected
Way support.  If they are needed to your workspace only, Connected Way can
integrate your changes into a private branch.

The git status may indicate that submodules are modified.  If so, you can
cd into the submodule and perform a git status from there.

If there are changes you wish to save, you can stash your changes, update the
repository, and pop the stashed changes.

To pull down the latest repository changes, do a git pull:

```
$ git pull origin
```

Now you need to update any submodule that have been updated upstream:

```
$ git submodule update of_core_cheap of_core_binheap of_core Unity \
of_core_fs_bookmarks of_core_fs_linux of_core_linux of_core_fs_pipe
```

If you have access to the SMB repositories, you should update those as well.

```
$ git submodule update of_smb of_smb_fs of_smb_client of_security \
of_smb_browser
```

You can use the make shortcut to accomplish the above as well.

```
$ make linux-update
```

or:

```
$ make linux-smbfs-update
```

# Building A Linux Deployment of OpenFiles

If you wish to build just a core OpenFiles version (i.e. without smb support),
issue the following command:

```
make linux
```

If you wish to build a Version of OpenFiles with SMB client support, issue the
following command:

```
make linux-smbfs
```

# Testing the Linux Build

You can run a Unity orchestrated test of Openfiles by issuing:

```
make linux-test
```

You can run a Unity orchestrated test of Openfiles with SMB by issuing:

```
make linux-smbfs-test
```

# Installing Linux Artifacts

You can install the Linux artifacts into the system directories by
issuing the following command:

```
sudo make linux-install
```

You can install the Linux SMB artifacts into the system directories by
issuing the following command:

```
sudo make linux-smbfs-install
```

# Uninstalling the Linux Artifacts

You can uninstall the Linux artifacts from the system directories by
issuing the following command:

```
sudo make linux-uninstall
```

You can uninstall the Linux SMB artifacts from the system directories by
issuing the following command:

```
sudo make linux-smbfs-uninstall
```

# Cleaning up the Build Directory

You can clean up all the build artificats by issuing:

```
make linux-clean
```

You can clean up all the build artificats of the SMB build by issuing:

```
make linux-smbfs-clean
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

# Active Directory Authentication on Linux

Throughout this discussion, we will use Active Directory and Kerberos
interchangeably.  Active Directory is a Microsoft Brand and an implementation
of the Kerberos protocol.  MIT Kerberos 5 is the code base used by
Open Files for Kerberos authentication.

Secure authentication is not really for the weak of heart.  Open Files
utilizes a total of five protocols involved with authentication:

1. SMBv2/v3 
2. SPNEGO
3. GSSAPI

Then either:

4. NTLM
5. Kerberos

Whether we use NTLM or Kerberos should be negotiated by SPNEGO.  Open Files
currently makes that decision on the client before negotiating authentication
and the decision is based on whether we are negotiating within a domain
and whether a username and password has been provided.  A future version
of Open Files may delegate this decision to SPNEGO.

We will use NTLM if a a username, or password is
provided in the file URL.   We will use Kerberos authentication if the
username or password are not specified.  This rule is somewhat
arbitrary and is used for simplicity.  This implies that on Linux, Kerberos
authentication will only be successful if an authentication ticket already
exists in the credential cache for the user.

Open Files is capable of authenticating with either NTLM or Kerberos and
regardless of whether credentials are specified in the URL or not.  Discuss
your particular use case with Connected Way if our default mechanism is
insufficient.

## Authentication Overview

When using Kerberos for authentication, the following steps should be used:

1. Add domain controller and target SMB server names to DNS
2. Configure Kerberos on the SMB client for the desired domain
3. Log into the domain by utilizing the 'kinit' tool
4. Use file urls in the form //server/share/path.  i.e., no credentials as
part of the URL.
5. When session is complete, log out of the domain using 'kdestroy'

When using NTLM for authentication, simply use urls in the form
//user:password[:domain]@server/share/path.  

## Samba DNS

Active Directory/Kerberos is tightly coupled with DNS.  So to set up Active
Directory, you need to administer DNS.  There is an excellent
[article](https://wiki.samba.org/index.php/DNS_Administration).  There
is no requirement to use samba DNS as your domain system.  We document it
simply as an example.

DNS manages "zones".  Consider a "zone" as a domain name.  In in
"spiritcoud.app".

### View configuration

To see what is currently configured, do:

```
$ samba-tool dns query <ip-address of DNS server> <zone-name> @ ALL -U administrator
```

where:
  - ip-address is the address of your samba DNS server.
  - zone-name is the name of your domain

For example:

```
$ samba-tool dns query 192.168.1.192 spiritcloud.app @ ALL -U administrator
```

### Add a New DNS Record

```
$ samba-tool dns 192.168.1.192 <domain-name> <name> A <IP-Address> -U administrator
```

For example:

```
$ samba-tool dns 192.168.1.192 spiritcloud.app openfiles a 192.168.1.206 -U administrator
```

### Managing Existing DNS server

Listing all zones on the server:

```
samba-tool dns zonelist 10.211.55.6 -U administrator
```

Listing Info for a particular zone:

```
samba-tool dns zoneinfo 10.211.55.6 spiritcloud.app -U administrator
```

Querying all records for a zone

```
samba-tool dns query 10.211.55.6 spiritcloud.app @ ALL -U administrator
```

Updating the zone:

```
samba-tool dns update 10.211.55.6 spiritcloud.app dc1 A 192.168.1.206 10.211.55.6 -U administrator
samba-tool dns update 10.211.55.6 spiritcloud.app @ A 192.168.1.206 10.211.55.6 -U administrator
```

Creating the reverse zone

```
samba-tool dns zonecreate 192.168.1.192 1.168.192.in-addr.arpa -U administrator
```

Adding PTR recors to reverse zone

```
samba-tool dns add 192.168.1.192 1.168.192.in-addr.arpa 158 PTR ubuntu.spiritcloud.app -U administrator
samba-tool dns add 192.168.1.192 1.168.192.in-addr.arpa 192 PTR dc1.spiritcloud.app -U administrator
```

### Troubleshooting the DNS server

If it appears that the dns server is not responding to queries:

```
$ sudo systemctl stop systemd-resolved
$ sudo systemctl disable systemd-resolved
```

## Kerberos Configuration

Open Files integrates with a Kerberos library that performs authentication.
This Kerberos framework configuration requires minimal configuration in
order to provide the expected functionality.

On Linux, the kerberos configuration file is stored at /etc/krb5.conf.  On
other platforms, it may be in other locations.

Once the krb5.conf file is updated to support your domain, users will be
able to authenticate within the domain within Open Files.

### Default Realm

Open Files has been qualified against kerberos configurations that specify
a default realm and that the default realm references the domain that
all authentications will be performed within.  To specify this default
realm, add the domain to the [libdefaults] section of the configuration file.
In the example below, we are using the domain `SPIRITCLOUD.APP`.

```
[libdefaults]
	default_realm = SPIRITCLOUD.APP
```

### Domain Realm


The FQDN of the domain controller for the default realm must also be
specified..  This is done in the [realms] section of the configuration file.
Add the following:

```
[realms]
	SPIRITCLOUD.APP = {
		kdc = dc1.spiritcloud.app
		default_domain = spiritcloud.app
	}
```

In this example, the domain is still SPIRITCLOUD.APP.  The kdc
(kerberos domain controller) is the FQDN to a computer on the network
that will actually perform (or delegate) the authentication.  This FQDN
must be a DNS name that is resolved through the DNS subsystem.  In other
words, the kdc cannot be an IP address.  Further, you must also have your
domain controller registered in DNS that is being used by the Linux system.

### Reverse Domain Realm

There is one more section in the kerberos configuration file called
[domain_realm].   The domain_realm section maps server hostnames back to the
realm.  In other words, it is a reverse mapping.  We have added the following:

```
[domain_realm]
	dc1 = SPIRITCLOUD.APP
```

Use the correct host name and domain name for your configuration.

## Adding a domain member

```
samba-tool domain join spiritcloud.app MEMBER -U administrator
```

## Client Configuration

### DNS Setup

For a client to connect to the server domain, the first thing is the
client must be able to resolve the DNS names of the server.  This is
platform specific but described here for a Ubuntu client.

In /etc/systemd/resolved.conf specify

```
[Resolve]
DNS=192.168.1.192
FallbackDNS=1.1.1.1
Domains=spiritcloud.app
#LLMNR=no
#MulticastDNS=no
#DNSSEC=no
#DNSOverTLS=no
#Cache=no-negative
#DNSStubListener=yes
#ReadEtcHosts=yes
```

Where the DNS address is the address of the DNS server.  FallbackDNS is
where you want to fallback to if your host cannot reach the dns server.
Domains is the search list of domains that you want to use to create a FQDN
from a unqualified name.  In other words, if the host you are accessing is
DC1, what must you append to it to get a FQDN.  In the example, DC1 would
resolve to DC1.spiritcloud.app.

After setting this up, restart the resolver by issuing:

```
systemctl restart systemd-resolved
```

### Logging into the Domain

Logging into the domain is generally system dependent.  In some systems,
it is accomplished by some login routine executed by the windowed operating
system.  Ultimately though, it comes down to creating a credential ticket
and storing that ticket in the credential cache for the user.

Stripping away the platform friendliness, this can be accomplished using
the Kerberos `kinit` utility.

To login to the kerberos domain, issue the following command at a shell
prompt:

```
$ kinit <username>@<realm>
```

Where username is the name of the user you wish to log into within the
desired domain.  In our example, we have added a user "spirit" to the
domain controller dc1.spiritcloud.app for the domain "SPIRITCLOUD.APP".  So
we will log in with:

```
$ kinit spirit@SPIRITCLOUD.APP
```

It will prompt for a password which we had specified when we created the user
within the domain controller.

## Verifing Login Status

Typically you don't need to do this, but it's helpful to verify that things
are progressing as expected.  You can verify the login status by issuing
the following command:

```
$ klist
```

You will see output similar to:

```
Ticket cache: FILE:/tmp/krb5cc_1000
Default principal: spirit@SPIRITCLOUD.APP

Valid starting       Expires              Service principal
01/15/2023 12:45:01  01/15/2023 22:45:01  krbtgt/SPIRITCLOUD.APP@SPIRITCLOUD.APP
	renew until 01/16/2023 12:44:56
```

This is telling you that there is a kerberos ticket that can authenticate
within the SPIRITCLOUD.APP domain and that, in this case, that ticket is good
for a day.

## Using alternative cache

The previous section used the default cache for storing kerberos tickets.
Openfiles supports the use of alternative caches.  If you would like to
use an alternative cache for authenticating file operations, use the following
syntax in the file URLs:

```
//[::cache@]server/share/path
```

The credentials portion of the URL is that portion preceding the ‘@’.  In the
above example, that would mean `::cache` is the credential.  In a
credential there are three fields as follows:
 
```
[username][:[password][:domain]]. 
```

That looks confusing but basically says all the following are valid:
 
- username:password:domain   
- username:password
- username::domain
- :password:domain
- ::domain
 
The credential cache is specified in the domain field of the URL.  
When specifying the credential cache, we use an escaped form of the cache
syntax as defined in
https://web.mit.edu/kerberos/krb5-1.12/doc/basic/ccache_def.html
 
That is an escaped form of `TYPE:value`
 
Where TYPE can be:
 
- API
- DIR
- FILE
- KEYRING
- MEMORY
- MSLSA
 
The `escaped` form implies replace the `:` with `%3A`.  Therefore, the
following is a correct syntax specifying a file based credential cache:

```
//::FILE%3A/tmp/mycache@myserver/myshare/ypath
```

Below is an example session using default and alternative caches.
 
```
$ # Using default cache
$ # start with empty cache
$ kdestroy -A
 
$ # do copy, should fail
$ /usr/local/bin/openfiles/smbcp cmake.out //dc1.spiritcloud.app/spiritcloud/cmake.out
Copying cmake.out to //dc1.spiritcloud.app/spiritcloud/cmake.out: [failed]
Bad Network Response
 
$ # Add a ticket to the default cache
$ kinit spirit@SPIRITCLOUD.APP
$ # do copy using default cache, should succeed
$ /usr/local/bin/openfiles/smbcp cmake.out //dc1.spiritcloud.app/spiritcloud/cmake.out
Copying cmake.out to //dc1.spiritcloud.app/spiritcloud/cmake.out: [ok]
 
$ # Remove cache
$ kdestroy
 
$ # Add a ticket to a file based session cache
$ kinit -c FILE:/tmp/krbof spirit@SPIRITCLOUD.APP
 
$ # see if we can do a copy using default cache, should fail
$ /usr/local/bin/openfiles/smbcp cmake.out //dc1.spiritcloud.app/spiritcloud/cmake.out
Copying cmake.out to //dc1.spiritcloud.app/spiritcloud/cmake.out: [failed]
Bad Network Response
 
$ # see if we can do a copy using the session cache
$ /usr/local/bin/openfiles/smbcp cmake.out //::FILE%3A/tmp/krbof@dc1.spiritcloud.app/spiritcloud/cmake.out
Copying cmake.out to //::FILE%3A/tmp/krbof@dc1.spiritcloud.app/spiritcloud/cmake.out: [ok]
```
 
Of NOTE: We are using an escaped form ‘FILE%3A/tmp/krbof’, for the cache name.
The unescaped name of the cache is therefore `FILE:/tmp/krbof`.
That form is what would be used in kinit and kdestroy.  The colon needs
to be escaped in the cache name so that it is not confused with other
colons in the syntax of the file name URL.

Here is a session using a KEYRING based credential cache:

``` 
$ # add a ticket to the KEYRING of cache
$ kinit -c KEYRING:of spirit@SPIRITCLOUD.APP
$ klist -c KEYRING:of
Ticket cache: KEYRING:legacy:of:of
Default principal: spirit@SPIRITCLOUD.APP
 
Valid starting       Expires              Service principal
01/27/2023 11:15:58  01/27/2023 21:15:58  krbtgt/SPIRITCLOUD.APP@SPIRITCLOUD.APP
     renew until 01/28/2023 11:15:53
$
$ # Let’s do a file copy using the keyring instead of a file cache
$ /usr/local/bin/openfiles/smbcp cmake.out //::KEYRING%3Aof@dc1.spiritcloud.app/spiritcloud/cmake.out
Copying cmake.out to //::KEYRING%3Aof@dc1.spiritcloud.app/spiritcloud/cmake.out: [ok]
``` 

## Ticket Renewal

At the current time, renewing the kerberos tickets is not within the scope
of Open Files and is left as an issue for the Open Files deployment to resolve.
The deployment platform should periodically perform a kinit sequence to
insure the credentials are current.

If this behavior is undesireable, please discuss your requirements with
Connected Way.  As mentioned earlier, the Open Files authentication support
is able to add and renew tickets on it's own, but it needs to have
the credentials and needs to know that it is authenticating with Kerberos.

## Logging out of the domain

To log out of the domain, simply issue:

```
$ kdestroy
```

or if you are using alternative caches:

```
$ kdestroy -c <cache-name>
```

## SMB Sessions using Kerberos Authentication

SMB Sessions are initialized using information specified in file URLs passed
into Open Files APIs.  For reference, a URL is of the form:

```
[//username:password:domain@server/share/]path.../file
```

Where the presence of a leading `//` signifies a remote location.  The
absence of a leading `//` signifies a local location.

With a remote URL, a username, and password are required for NTLM
authentication.  For Kerberos authentication, using our policy for determining
whether we are performing Kerberos authentication or not, the username and
password should be blank.  For example:

```
$ smbcp //server/share/subdirectory/picture.jpg ./picture.jpg
```

This will direct the Open Files SMB stack to authenticate with the active
ticket for default domain using the default credential cache.
Note that the server must be a domain name from the domain name system and
NOT an IP address.

If you are running the CI tests such as test_fs_smb, you will want an
entry in the /etc/openfiles.xml file such as:

```
      <map>
        <drive>test</drive>
        <description>Remote for test_file</description>
        <path>//dc1.spiritcloud.app/spiritcloud</path>
      </map>
```

Do not be confused by the example above where we use the target destination
of the domain controller itself.  It doesn't have to be.  It just needs
to be a host that has been registered within the domain.  Our configuration
is accessing files on the domain controller itself so we specify the
FQDN of the domain controller.

To generate a keytab entry.  on the dc do:

sudo samba-tool spn add host/UBUNTU@SPIRITCLOUD.APP Guest
sudo samba-tool domain exportkeytab ubuntu.keytab -principal host/UBUNTU@SPIRITCLOUD.APP

move this over to ubuntu

sudo samba-tool computer add ubuntu
sudo samba-tool computer delete ubuntu
sudo samba-tool computer list

Also can use adcli instead:

adcli --preset-computer --domain=SPIRITCLOUD.APP ubuntu.spiritcloud.app -V cifs

So I can adcli, export keytab

on dc
kadmin.local

add_principal cifs/ubuntu@SPIRITCLOUD.APP
ktadd cifs/ubuntu@SPIRITCLOUD.APP

on member
kdb5_util create -r SPIRITCLOUD.APP -s
add_principal cifs/ubuntu@SPIRITCLOUD.APP
ktadd cifs/ubuntu@SPIRITCLOUD.APP




Add other info to krb5.conf under realms for SPIRITCLOUD
admin_server = dc1.spiritcloud.app
master_kdc = dc1.spiritcloud.app

instead:

take one that works:

adcli --preset-computer --domain=SPIRITCLOUD.APP pixel6.spiritcloud.app -V cifs

or brief

adcli preset-computer pixel6.spiritcloud.app

then

samba-tool add computer doesn't seem to work.  

samba-tool spn add cifs/ubuntu.spiritcloud.app@SPIRITCLOUD.APP Administrator
samba-tool domain exportkeytab cifs.keytab --principal cifs/ubuntu@SPIRITCLOUD.APP

copy that over
add to our keytab



start server with:
./gss-server cifs/ubuntu

start client with
KRB5_TRACE=/dev/stderr ./gss-client ubuntu.spiritcloud.app cifs msg

get rid of any obsolete entries on dc

samba-tool spn delete cifs/ubuntu.spiritcloud.app@SPIRITCLOUD.APP

take two


samba-tool spn add cifs/ubuntu.spiritcloud.app@SPIRITCLOUD.APP Spirit
samba-tool domain exportkeytab cifs.keytab --principal cifs/ubuntu@SPIRITCLOUD.APP

copy that over to the client

on the client

Then login as user

kinit spirit@SPIRITCLOUD.APP

start server with:
KRB5_TRACE=/dev/stderr ./gss-server -keytab cifs.keytab cifs/ubuntu.spiritcloud.app

start client with
KRB5_TRACE=/dev/stderr ./gss-client ubuntu.spiritcloud.app cifs msg


Works

So, given this, how will we make this work?  When adding a server, we need to add a keytab entry in the app.

In the Domain tab, when adding or edit a domain, we need to add a keytab file.
what is the friendliest way to do this?  the DC procedure will be:

samba-tool spn add cifs/ubuntu.spiritcloud.app@SPIRITCLOUD.APP Spirit
samba-tool domain exportkeytab cifs.keytab --principal cifs/ubuntu@SPIRITCLOUD.APP
openssl base64 -in cifs.keytab -out cifs.keyout

In the domain dialog, there'll be a button to enable server.  Then you will
be able to add a server keytab.  They keytab will be stored in the files
directory with the name of the domain.keytab.

cifs.keyout should be copied/pasted into the "Server Keytab" entry in the
domain dialog.

When the user log's in, if the server is enabled and a keytab entry is
provided, the server for that domain entry will start with that keytab entry.
it will call krb5_acquire_server with the server_fqdn and the keytab.
The server_fqdn needs to also be added to the domain dialog.  So what we
have now, is:
Domain:
KDC:
Enable Server:
FQDN:
Server Keytab:

The server needs to be added to the DC using procedure above.
The server's FQDN needs to be added to DNS

When we start the app, if we have a logged in user, and there is a keytab
we will automatically start the server.

If we edit an existing domain entry, the user is automatically logged out.
perhaps when they hit "ok", we check if the user is already logged in.  If
so, we give a popup that says, logout user?

When we log out a user, we need to shutdown the server.

We can DNS query (type PTR, Class IN, "QM" question:
_smb._tcp.local
Returns (UBUNTU, openfile, DC1, "Richard's MacBook Air", and "Pixel 6 Pro")

TO dO

We need a server app.

Need to verify if the keytab works on other machines

See if error message is correct if we're not logged in when server comes up

See if we can acquire multiple servers

Need to add the actual SASL piece

Need a better way to share keytabs.  Would be better if we simply entered a
password.

Need to double check the way we do core load.

Need to figure out why test_smbpersist hangs on shutdown of a test.
