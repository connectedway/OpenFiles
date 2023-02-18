OpenFiles DFS Support
=====================

# Introduction

OpenFiles supports both the standalone and domain based Distributed
Filesystem capability.  

# Configuring OpenFiles for DFS

By default, if you are building the OpenFiles SMB
client for Linux, DFS will already be configured.
This section is helpful to understand the configuration
options or to configure DFS for another platform.

NOTE: The OpenFiles SMB Server does NOT support
DFS.

## Compile Time Configuration

Openfiles support for DFS is configured via the config file:

```
of_smb/configs/default
```

Relevant options are:

- `SMB_SUPPORT_DFS`: Turn out support for DFS in the OpenFiles SMB Client.
Default is ON.
- `DFS_SUPPORT_GET_DFS_REFERRAL_EX`: Turns on support for the
GET_DFS_REFERRAL_EX packet.  It is not clear whether this is
supported anymore.  Enabling this will cause DFS to fail.  It
is included for future compatability.  Default is OFF.
- `DFS_SUPPORT_SYSVOL`: Support for SYSVOL referrals.  This is used
to communicate to DCs that have a SYSVOL or NETLOGON share.  We
have not tested this capability and so we recommend not enabling
it.  Default is OFF.
- `DFS_MAX_REFERRAL_VERSION`: Maximum Referal Version (Level) Supported.
OpenFiles supports versions 1 through 4.  NOTE: Domain referrals are only
supported in levels 3 and 4.  Default is 4.
- `DFS_BOOTSTRAP_DC_TIMEOUT`: Number of seconds between Domain Cache Refresh.
Default is 10.
- `DFS_MAX_TTL`: The max number of seconds to allow a referral entry to
be valid regardless of what the server reports.
Servers typically report a value on the order of 10 minutes.
The max is mostly used so we can test short referral entry
expiration times.  Default is 86400 seconds, or 1 day.  

## Runtime Configuration

Runtime configuration of installed deployments of OpenFiles
is performed through the `/etc/openfiles.xml` file.  Local
deployments will use their own runtime configuration files.

### Path Variable

If you will be running the test_fs_smb test utility, you should update
the path variable to refer to a dfs target.  

A path variable is typically `//<server>/<share>/<path>`.  When using
dfs, the server refers to either a server or a domain, share refers to
a dfs root.  The first directory in path refers to a dfs link.
Remaining directories in path refer to paths relative to the dfs link.

Example path variables:
```
    (1) <path>//rschmitt:happy@10.211.55.7/spiritcloud/openfiles</path>
    (2) <path>//SPIRITCLOUD/dfs/spiritdfs/openfiles</path>
    (3) <path>//dc1.spiritcloud.app/dfs/spiritdfs/openfiles</path>
    (4) <path>//DC1/dfs/spiritdfs/openfiles</path>
    (5) <path>//dc1.spiritcloud.app/spiritcloud/openfiles<path>
    (6) <path>//DC1/spiritcloud/openfiles</path>
```

For `(1)`, This is a non-DFS, non-domain path which specifies a username, and password
using a server at a specified IP address, a share called `spiritcloud`, and path called
`openfiles`.  Authentication is through NTLMSSP.

For `(2)`, This is a domain based DFS path using the domain `SPIRITCLOUD`
the root `dfs`, the link `spiritdfs`, and path called `openfiles`

For `(3)`, This is a non-domain based DFS using a domain controller
at `dc1.spiritcloud.app`, a root of `dfs`, a link of `spiritdfs` and a
path of `openfiles`.

for `(4)`, This is a non-domain based DFS using a domain controller
of `DC1` (get's extended into a fqdn using the DNS search suffix), a root
of `dfs`, a link of `spiritdfs` and a path of `openfiles`.

for `(5)`, This is a non-dfs, non-domain path which does not
specify a username or password implying that authentication is performed
through the domain.  the server is `dc1.spiritcloud.app`, the
share is `spiritcloud` and the path is `openfiles`.

for `(6)`, it is similar to `(5)`, but the server is specified as `DC1`
which gets extended into a fqdn using the DNS search suffix.

### Bootstrap DC

The bootstrap DC informs the OpenFiles DFS stack which DC to contact to
optain information about the Domain.  It is required for domain based
DFS.

It is specified in the OpenFiles.xml file as follows:

```
    <smb>
      <fqdn>ubuntu.spiritcloud.app</fqdn>
      <bootstrap_dc>dc1.spiritcloud.app</bootstrap_dc>
    </smb>
```

The bootstrap_dc should be a FQDN to the DC, or it can be a shortened
name which will be extended into a FQDN by the DNS search suffix.

# Configuring DFS on a Samba Server

For informational purposes, we provide detail on how to setup a
a domain based DFS namespace.  This namespace will consist of
one domain controller and one dfs root on that one DC.

The configuration, therefore, is performed on the one domain
controller.

You need to create a directory to be the dfs root

```
# mkdir /srv/dfsroot
# chmod 755 /srv/dfsroot
```

First add the shares that your samba server will be
serving to the `smb.conf` file.

```
[spiritcloud]
    path = srv/spiritcloud
    read only == No
    writeable = yes
    force group = spiritcloud
    valid users = @SPIRITCLOUD@
```    

Create that directory

```
# mkdir /srv/spiritcloud
```

Then you need to create links in the dfs root to shares

```
# ln -sf msdfs:dc1.spiritcloud.app\\spiritcloud spiritdfs
```

In `smb.conf` in the `global` section add:

```
[global]
host msdfs = yes
```

And in a `dfs` section add the dfs root

```
[dfs]
path = /srv/dfsroot
msdfs root = yes
```

You must then restart your samba server:

```
$ sudo systemctl restart samba-ad-dc
```

# Running the OpenFiles File Test

The assumption here is that the openfiles tests will be
run from a server that has been configured with kerberos
and that the user has logged into the domain and has
an active ticket:

```
$ kinit spirit@SPIRITCLOUD.APP
```

See [the Linux Readme](https://github.com/connectedway/openfiles/blob/main/LINUX.md)
for setting up kerberos and building openfiles on the local client.

Once openfiles has been built you can run the `test_fs_smb` 
The openfiles file test utility `test_fs_smb` can be run from a
shell as follows:

```
$ ./build-linux-smbfs/of_smb_fs/test/test_fs_smb
```

You should see output similar to:

```
 ./build-linux-smbfs/of_smb_fs/test/test_fs_smb 
Unity test run 1 of 1
1692058530 Starting File Test with //SPIRITCLOUD:445/dfs/spiritdfs/openfiles

<snip>

1692073162 File Test Succeeded

.

-----------------------
1 Tests 0 Failures 0 Ignored 
OK
Total Allocated Memory 1336, Max Allocated Memory 1409935
```

# Running the smbcp utility

To run smbcp, you must first build and install openfiles.  The procedures
are outlined in
[the Linux Readme](https://github.com/connectedway/openfiles/blob/main/LINUX.md).
You then must build and install the smbcp application.  Those procedures are
[Here](https://github.com/connectedway/smbcp/blob/main/README.md)

NOTE: Installation will overwrite /etc/openfiles.xml.  Be sure to configure the
path and bootstrap dc variables after installation.

Then run smbcp to copy a file of your choosing to the remote
domain DFS share.  You may wish to run wireshark during
the session to verify the transfer occured as expected.

```
$ smbcp ./spiritcloud.png \
  //SPIRITCLOUD/dfs/spiritdfs/openfiles/spiritcloud.png
Copying ./spiritcloud.png to //SPIRITCLOUD/dfs/spiritdfs/openfiles/spiritcloud.png: [ok]
Total Allocated Memory 1240, Max Allocated Memory 142149
```

Wireshark notes:

1. Verify IPv6 Addressing is being used
2. Verify that after the Negotiate handshake, Kerberos handshake occurs.
3. Verify a session setup and tree connect to the IPC$ share
4. Verify a domain DFS_GET_REFERRALS request is sent
   1. Version is 4
   2. Filename is empty which specifies a domain referral request
5. Verify the DFS_GET_REFERRALS response
   1. Verify one or more domain names are returned.
   2. note that the domain name used in the smbcp url
   should be listed if you are performing a domain based
   dfs transaction.
6. Verify a DC DFS_GET_REFERRAL request
   1. Filename is the domain name preceeded by a backslash
   this signifies a DC referral request.
7. Verify the DFS_GET_REFERRAL response
   1. The number of referrals should be 1
   2. The Expanded Name should be the domain controller of
   the desired domain.  The name may be shortened rather
   than a FQDN.  The DNS search path suffix will be appended
   to this to create a full FQDN for the DC.
8. Verify a new connection is established to the DC
9. Verify a negotiate, setup, treeconnect and create of
   the srvsvc pipe
10. verify a root referral request to the DC
   1. Verify that the filename is of the form \\<dc>\\<root>
11. verify the root referral response
   1. Note that the root referral NODE entry may
   be the same as that used in the root request.  This
   happens if the root is hosted by the domain controller.
12. If the root referral returned a NODE signifying that
   the root is hosted by a different node than the DC, you
   should see a connection, negotiate, and setup to the root
   node.  If the root is hosted by the DC, you will reuse the
   same session.
13. verify a tree connect to the root note dfs root share.
14. note an attempt to open the directory you wish to copy the
   file into
15. note a PATH_NOT_COVERED response
16. Verify a link referral request to the root node
   1. Filename will be the full path to the target file on
   the root node
17. Verify the link referral response
   1. the consumed field will indicate how many of the bytes
   of the filename used in the request should be substituted
   2. The NODE field will contain a path that should replace
   the characters in the requested filename that have been
   identified to be substituted.  For example:
```   
   Requested File Name: \DC1\dfs\spiritdfs\openfiles\spiritcloud.png
   consumed: 36 (NOTE: this is number of bytes so actual consumed
   is half)
   Requested File Name with consumed removed: \openfiles\spiritcloud.png
   NODE in response: \dc1.spiritcloud.app\spiritcloud
   NODE response prepended to path with consumed removed:
   \dc1.spiritcloud.app\spiritcloud\openfiles\spiritcloud.png
```
18. Note a tree connect to the target share
19. Not a successful create on the target file and a successfil file copy.

Verify the file can be copied back

```
$ smbcp //SPIRITCLOUD/dfs/spiritdfs/openfiles/spiritcloud.png \
  ./spiritcloud2.png
Copying //SPIRITCLOUD/dfs/spiritdfs/openfiles/spiritcloud.png to ./spiritcloud2.png: [ok]
Total Allocated Memory 1240, Max Allocated Memory 146174
$ md5sum spiritcloud.png spiritcloud2.png
7b9f3781fdbc7155383fb28eda40d015  spiritcloud.png
7b9f3781fdbc7155383fb28eda40d015  spiritcloud2.png
```

# DFS QA Test Plan and Results

OpenFiles DFS has supports both standalone and domain based DFS.  This
results in a large number of scenarios to test.  They are documented
here.

## Unqualified Features

Due to IT requirements for testing more advanced features of DFS,
some features are unqualified.  Please work with Connected Way to
schedule QA for any of the following features if needed:

- Domain Forests.  A forest consists of one or more domains.
Each domain shares the same authentication database, but not all
users have access to each domain.  OpenFiles DFS has only been
qualified against a single domain.

- Referral Site Costing.  DFS supports the ability to cluster
content through multiple sites.  The cost to access that content
through the vaious sites may not be equivalent.  Ideally, a
client would access content through the site that offers the
least cost.  OpenFiles DFS does not consider a site's cost
when ordering sites.

- DFS Interlinks.  Link targets from a DFS Link referral response
typically link to a destination within the DFS root node that
the referral was initially sent to.  An Interlink redirects
the link to a separate domain.  Although OpenFiles DFS has
implemented support for interlinks, they have not been qualified.

- Client Target Failure.  DFS root and link referral requests
my contain more than one target.  When a client attempts to
access one of these targets, the target may not be available.
In which case, the client can attempt to access an alternative
target in the list.  OpenFiles DFS implements client target
failover but the feature has not been qualified.

- Client Target Failback.  This is an optional feature that
directs a client to periodically attempt to access a failed
target that resulted in a previous failover and revert back
to the preferred target if successful.  OpenFiles does not
support this feature.

- Target Sets.  This is a feature where a referral response
may return multiple sets grouped by various policies.  OpenFiles
DFS does not support target sets.  It treats all targets as if
they are in the same set.

- SYSVOL referrals.  DFS supports referral for special shares on
a dfs root target which are used for domain based logins.  These
are above and beyond the kerberos or active directory authentication.
Although OpenFiles has implemented support for SYSVOL and NETLOGON
referrals, they have not been qualified.

- REQ_GET_DFS_REFERRAL_EX.  There are two referral requests that
a client can send a DFS server: REQ_GET_DFS_REFERRAL and
REQ_GET_DFS_REFERRAL_EX.  It is not clear us who uses the
REQ_GET_DFS_REFERRAL_EX packet.  On reading, it appears that
the EX variant is supposed to be used if the SMB session has
been established using the SMBv3 protocol.  Upon observation,
this is not the case.  Although Open Files DFS has implemented
the REQ_GET_DFS_REFERRAL_EX support, we have not found a
server that supports it.  So we have not qualified this support.
The capability is disabled via a compile time constant.

## Qualification Results

This section details the various tests performed on the
Open Files DFS Feature along with the results of the
tests.

All tests involve the following steps:

- Configure Compile Time Settings (optional)
- Rebuild OpenFiles (only if configuration changed)
- Setting target path
- Setting Bootstrap Domain Controller
- Running test_fs_smb

The configuration of runtime parameters and the
success/failure of the test will be depicted in
a table similar to the following:

|  Ticket State  |  Bootstrap_dc  |  Target Path  | Result   |
|----------------|----------------|---------------|----------|
| ticket-state   | bootstrap-dc   | target-path   | result   |
|----------------|----------------|---------------|----------|

Where:
- ticket-state is one of
    - no ticket
    - expired
    - active
- bootstrap-dc is the DNS name of the initial DC
to perform the domain referral on.  Typically it
will be one of the following:
    - DC1: The DNS host name of the DC.  The DNS
    search path will be appended to this to form
    the FQDN of the DC
    - dc1.spiritcloud.app: The FQDN of the DC.
    - empty: No bootstrap DC set.
- target-path: The URL to the directory to run
the DFS tests against.  In our testing, the
target path will be one of:
    - `//rschmitt:happy@10.211.55.7/spiritcloud/openfiles`:
    Non-domain access to non-DFS Path.
    - `//rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles`:
    Non-domain access to DFS Path.
    - `//10.211.55.6/dfs/spiritdfs/openfiles`:
    Domain Access to non-FQDN DFS Path.
    - `//SPIRITCLOUD/dfs/spiritdfs/openfiles`:
    Domain Access to Domain Based DFS Path.
    - `//dc1.spiritcloud.app/dfs/spiritdfs/openfiles`
    Domain Access to DC Root Target Path (FQDN).
    - `//DC1/spiritcloud/openfiles`:
    Domain Access to DC Root Target Path (DNS).
    - `//dc1.spiritcloud.app/spiritcloud/openfiles`:
    Domain Access to Non-DFS Path
- result: The expected result of the test.  Will be one of:
    - Success
    - Long Failure
    - Path Not Found
    - Not Same Device
    - File Server Not Found
    
NOTE: Domain Access to DC Root Target Paths (both FQDN and DNS)
is equivalent to Standalone DFS.

All tests are run using the following command issued from the
openfiles workspace directory:

```
$ ./build-linux-smbfs/of_smb_fs/test/test_fs_smb
```

### DFS Turned Off

This test verifies that the Open Files SMB client can
access remote non-dfs resident files without regression.

Procedure:

1. Turn off DFS support in configuration file

In `of_smb/configs/default` relative to the base of the
openfiles workspace, set `SMB_SUPPORT_DFS` as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" OFF)
```

2. Clean, Configure, and Build OpenFiles
3. Run

Result:

|  Ticket State  |  Bootstrap_dc  |  Target Path  | Result   |
|----------------|----------------|---------------|----------|
| no ticket      | empty          | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |

### DFS Turned ON, DFS Path

This test verifies that the Open Files SMB client can
access remote dfs standalone and domain resident files.

Procedure:

1. Turn On DFS support in configuration file

In `of_smb/configs/default` relative to the base of the
openfiles workspace, set DFS variables as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" ON)
option(DFS_SUPPORT_GET_DFS_REFERRAL_EX "v3 Servers support the GET_DFS_REFERREL_EX" OFF)
option(DFS_SUPPORT_SYSVOL "DFS Stack supports SYSVOL requests" OFF)
set(DFS_MAX_REFERRAL_VERSION "4" CACHE STRING "Max Referall Version to Support")
set(DFS_BOOTSTRAP_DC_TIMEOUT "10" CACHE STRING "Refresh Interval for Bootstrap DC")
```

2. Clean, Configure, and Build OpenFiles
3. Login, Logout or Expire the Kerberos Ticket as follows:
    - No Ticket: `$ kdestroy`
    - Expired Ticket: `$ kinit spirit@SPIRITCLOUD.APP -l 1m`
    - Active Ticket: `$ kinit spirit@SPIRITCLOUD.APP`
3. Iteratively step through the following table updating the `<path>` and
`<bootstrap_dc>` variables in `/etc/openfiles.xml` according to the
values specified in the table.
3. Run

Result:

|  Ticket State  |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| no ticket      | DC1                 | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | empty               | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | DC1                 | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| no ticket      | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| no ticket      | empty               | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| no ticket      | DC1                 | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | dc1.spiritcloud.app | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | empty               | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| no ticket      | DC1                 | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| no ticket      | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| no ticket      | empty               | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| no ticket      | DC1                 | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| no ticket      | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| no ticket      | empty               | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| expired        | DC1                 | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | empty               | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | DC1                 | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| expired        | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| expired        | empty               | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Logon Failure |
| expired        | DC1                 | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | dc1.spiritcloud.app | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | empty               | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| expired        | DC1                 | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| expired        | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| expired        | empty               | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| expired        | DC1                 | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| expired        | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| expired        | empty               | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| active         | DC1                 | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |
| active         | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |
| active         | empty               | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | File Server Not Found (1) |
| active         | DC1                 | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |
| active         | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |
| active         | empty               | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |
| active         | DC1                 | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| active         | dc1.spiritcloud.app | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| active         | empty               | //10.211.55.6/dfs/spiritdfs/openfiles         | Logon Failure |
| active         | DC1                 | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| active         | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| active         | empty               | //rschmitt:happy@10.211.55.7/dfs/spiritdfs/openfiles | Path Not Found |
| active         | DC1                 | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| active         | dc1.spiritcloud.app | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |
| active         | empty               | //rschmitt:happy@10.211.55.7/spiritcloud/openfiles | Success |

`(1)` File server not found because we can't resolve SPIRITCLOUD to a target host

### DFS Turned ON, Non-DFS Path

This test verifies that the Open Files SMB client can
access non dfs files on a remote DC

Procedure:

1. Follow the steps from above test

Result:

|  Ticket State  |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| active         | DC1                 | //DC1/spiritcloud/openfiles                   | Success       |
| active         | dc1.spiritcloud.app | //DC1/spiritcloud/openfiles                   | Success       |
| active         | DC1                 | //dc1.spiritcloud.app/spiritcloud/openfiles   | Success       |
| active         | dc1.spiritcloud.app | //dc1.spiritcloud.app/spiritcloud/openfiles   | Success       |
| active         | empty               | //DC1/spiritcloud/openfiles                   | Success       |
| active         | empty               | //dc1.spiritcloud.app/spiritcloud/openfiles   | Success       |

### DFS Turned ON, Using EX request

This test shows the results if we have the REQ_GET_DFS_REFERRAL_EX enabled.
This is expected to fail.

Procedure:

1. Turn On DFS support in configuration file

In `of_smb/configs/default` relative to the base of the
openfiles workspace, set DFS variables as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" ON)
option(DFS_SUPPORT_GET_DFS_REFERRAL_EX "v3 Servers support the GET_DFS_REFERREL_EX" ON)
option(DFS_SUPPORT_SYSVOL "DFS Stack supports SYSVOL requests" OFF)
set(DFS_MAX_REFERRAL_VERSION "4" CACHE STRING "Max Referall Version to Support")
set(DFS_BOOTSTRAP_DC_TIMEOUT "10" CACHE STRING "Refresh Interval for Bootstrap DC")
```

2. Clean, Configure, and Build OpenFiles
3. Login to the domain
    - Active Ticket: `$ kinit spirit@SPIRITCLOUD.APP`
3. updating the `<path>` and `<bootstrap_dc>` variables in
`/etc/openfiles.xml` according to the values specified in the table.
3. Run

Result:

|  Ticket State  |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| active         | DC1                 | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Not Same Device (2) |

`(2)` This occurs if GET_DFS_REFERRAL_EX isn't supported, so the referral request fails.  We're unable to resolve
the referral so we fail with the last error we've received, which is not same device.  We could fall back and use
GET_DFS_REFERRAL but until we can verify that there are servers out there that support it, we recommend setting
DFS_SUPPORT_GET_DFS_REFERRAL_EX to OFF

### DFS Testing with Different Referral Levels

This test shows the results when we run with differing DFS referral
levels.  The vald levels are 1-4.

Procedure:

1. Turn On DFS support in configuration file.  Vary the setting of
`DFS_MAX_REFERRAL_VERSION` between 1 and 4 on subsequent runs.
In `of_smb/configs/default` relative to the base of the
openfiles workspace, set DFS variables as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" ON)
option(DFS_SUPPORT_GET_DFS_REFERRAL_EX "v3 Servers support the GET_DFS_REFERREL_EX" ON)
option(DFS_SUPPORT_SYSVOL "DFS Stack supports SYSVOL requests" OFF)
set(DFS_MAX_REFERRAL_VERSION <referral-level> CACHE STRING "Max Referall Version to Support")
set(DFS_BOOTSTRAP_DC_TIMEOUT "10" CACHE STRING "Refresh Interval for Bootstrap DC")
```
Where: <referral-level> is a value between 1 and 4.
2. Clean, Configure, and Build OpenFiles
3. Login to the domain
    - Active Ticket: `$ kinit spirit@SPIRITCLOUD.APP`
3. updating the `<path>` and `<bootstrap_dc>` variables in
`/etc/openfiles.xml` according to the values specified in the table.
3. Run

Note that all the previous tests were against version 4.  So version 4 not retested.  We are also
only interested in the logged in case, so rather than show Ticket State,
we are showing Referral Level.


Results:

| Referral Level |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| 1              | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | File Server Not Found (3) |
| 2              | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | File Server Not Found (3) |
| 3              | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |
| 1              | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |
| 2              | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |
| 3              | dc1.spiritcloud.app | //dc1.spiritcloud.app/dfs/spiritdfs/openfiles | Success       |

`(3)` File Server not found because since we are running with version 1 of referrals, we cannot
resolve the domain name

### Verifying DFS through IPv4 and IPv6

By default, ubuntu will configure both ipv4 and ipv6 addresses and use
ipv6 by default.  So, assuming the server is utilizing ipv6 and that DNS
reports ipv6 as well as ipv4 addresses, OpenFiles will use ipv6 by default.
The above testing, as a result, has all been done against IPv6.  This test
verifies that DFS will work with IPv4.

You can verify by running wireshark during test checking the source and destination
addresses.  

Procedure:

1. Turn On DFS support in configuration file.  Make sure
the referral level from the previous step is reset to 4.
In `of_smb/configs/default` relative to the base of the
openfiles workspace, set DFS variables as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" ON)
option(DFS_SUPPORT_GET_DFS_REFERRAL_EX "v3 Servers support the GET_DFS_REFERREL_EX" ON)
option(DFS_SUPPORT_SYSVOL "DFS Stack supports SYSVOL requests" OFF)
set(DFS_MAX_REFERRAL_VERSION 4 CACHE STRING "Max Referall Version to Support")
set(DFS_BOOTSTRAP_DC_TIMEOUT "10" CACHE STRING "Refresh Interval for Bootstrap DC")
```
2. Clean, Configure, and Build OpenFiles
3. Login to the domain
    - Active Ticket: `$ kinit spirit@SPIRITCLOUD.APP`
4. updating the `<path>` and `<bootstrap_dc>` variables in
`/etc/openfiles.xml` to the value in the following table
5. Set the value of the Linux configuration value for IPv6
as shown below
6. Run

To effectively disable IPv4 and use IPv6, do the following:

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0
```

Note: This doesn't actually disable IPv4, It just enables IPv6
which is used by default so DFS will occur over IPv6.

To disable IPv6 and use IPv4, do the following:

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=1
```

Results:

| IPv4/IPv6      |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| ipv4           | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |
| ipv6           | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |

When finished, don't forget to revert the sysctl setting to allow both
IPv4 and IPv6:

```
$ sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0
```

### Test DFS Referral Level Expiration

When a DFS referral entry expires, the DFS client should obtain a new referral entry
but preserve any target_hint from the ole entry (the preferred target in the set of targets)
in the new entry.

To test this, we have provided a DFS_MAX_TTL value in the compile time configuration
of OpenFiles.  Setting this value to 1 second and running the test_fs_smb test should
cause the referral entry to expire between some of the subtests.  For instance, assuming
a Create of a file, Read of a file, or Copy of a file take longer than 1 second, the
referral discovered for each of those subtests will have expired by the time the next
subtest is executed.

Procedure:

1. Set the DFS_MAX_TTL value in the configuration setings
as follows:

```
option(SMB_SUPPORT_DFS "Add Support for DFS" ON)
option(DFS_SUPPORT_GET_DFS_REFERRAL_EX "v3 Servers support the GET_DFS_REFERREL_EX" ON)
option(DFS_SUPPORT_SYSVOL "DFS Stack supports SYSVOL requests" OFF)
set(DFS_MAX_REFERRAL_VERSION <referral-level> CACHE STRING "Max Referall Version to Support")
set(DFS_BOOTSTRAP_DC_TIMEOUT "10" CACHE STRING "Refresh Interval for Bootstrap DC")
set(DFS_MAX_TTL "<ttl>" CACHE STRING "Max TTL for a Referral Entry")
```
Where <ttl> is 1 or 86400
2. Clean, Configure, and Build OpenFiles
3. Login to the domain
    - Active Ticket: `$ kinit spirit@SPIRITCLOUD.APP`
3. updating the `<path>` and `<bootstrap_dc>` variables in
`/etc/openfiles.xml` according to the values specified in the table.
4. Write a TAG to the syslog
```
$ logger -t START "Start of Test"
```
5. Run

Results:

| TTL Value      |     Bootstrap_dc    |  Target Path                                  | Result        |
|----------------|---------------------|-----------------------------------------------|---------------|
| 1              | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |
| 86400          | dc1.spiritcloud.app | //SPIRITCLOUD/dfs/spiritdfs/openfiles         | Success       |

Syslog Notes:

At the end of the test, view `/var/log/syslog`.  Search for the last
start tag.  You will see entries similar to:

```
Feb 18 16:47:51 ubuntu openfiles[112686]: DFS Referral Expiration for \DC1\dfs\spiritdfs
Feb 18 16:48:00 ubuntu openfiles[112686]: message repeated 2 times: [ DFS Referral Expiration for \DC1\dfs\spiritdfs]
```

This implies a total of three expirations occurred during the test.

When run with a DFS_MAX_TTL of 86400, you will notice no expirations following the start tag.

