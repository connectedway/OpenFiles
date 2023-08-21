
# Azure Configuration

The Openfile qualification environment is now based on the Microsoft Azure
Cloud Platform.  Azure allows us to configure robust networks with both
NTLM and Active directory authentication.

## Storage Architecture

The Storage Architecture for our Azure qualification environment has been
designed to provide test coverage of Open Files, and to test the most
common deployment architectures.

Three Primary Storage Nodes:
- spiritstore1
- spiritstore2
- spiritstore3

Three Backup Storage Nodes:
- spiritstore1b
- spiritstore2b
- spiritstore3b

We will implement an "Organization Forest"

Domain Forest Controller (connectedforest.com)
- windows1

Backup Domain Forest Controller
- windows1b

It will have two departments: trees and trails

SubDomain Controllers (connectedtrees.com/connectedtrails.com)
- windows2 (connectedtrees.com)
- windows3 (connectedtrails.com)

Backup Domain Controllers
- windows2b
- windows3b

Linux Test Machines
- ubuntu

The storage nodes with a 1 in their names (spiritstore1/spiritstore1b)
host the namespace root in share root

We only need one linux test machine.  The linux test machine will have
samba on it and can access the storage via samba.  It will also be able
to build and run openfiles.

Currently there are three subdirectories off of the root:
- users
- scans
- data

Users will refer to spiritstore2/spiritstore2b to share users
Scans will refer to spiritstore3/spiritstore3b to share scans
data is hosted on spiritstore1 and is not referred.

The Directory Structure will look like:

/spiritstore1/root
/spiritstore1/root/users -> /spiritstore2/users
/spiritstore1/root/scans -> /spiritstore3/scans
/spiritstore1/root/data -> /spiritstore1/data
/spiritstore1/data

/spiritstore2/users
/spiritstore2/users/happy
/spiritstore2/users/happy/scans -> /spiritstore3/scans/happy
/spiritstore2/users/happy/mobile -> /happymobile
/spiritstore2/users/grumpy
/spiritstore2/users/grumpy/scans -> /spiritstore3/scans/grumpy
/spiritstore2/users/grumpy/mobile -> /grumpymobile
/spiritstore2/users/sleepy
/spiritstore2/users/sleepy/scans -> /spiritstore3/scans/sleepy
/spiritstore2/users/sleepy/mobile -> /sleepymobile
/spiritstore2/users/dopey
/spiritstore2/users/dopey/scans -> /spiritstore3/scans/dopey
/spiritstore2/users/dopy/mobile -> /dopyymobile

/spiritstore3/scans
/spiritstore3/scans/happy
/spiritstore3/scans/grumpy
/spiritstore3/scans/sleepy
/spiritstore3/scans/dopey

- Root
  - data 
  - Users 
    - happy
      - Scans: /Root/Scans/happy
    - grump
      - Scans: /Root/Scans/UserB
    - UserC
      - Scans: /Root/Scans/UserC
    - UserD
      - Scans: /Root/Scans/UserD
  - Scans
    - UserA
    - UserB
    - UserC
    - UserD

There will be two other pairs of domain controllers configured as subdomains
of the domain forest (connectedforest): connectedtrees.com and
connectedtrails.com.

connectedtrees will serve UserA and UserB
connectedtrails will serve UserC and UserD

Questions we need to explore:
- What is Azure AD.  We have it configured but we also host our own
  AD domain controllers.  Do we need the Azure AD?
- How to set up a VPN from inside a firewall in a home/office to the
  Azure cloud.
- How to hook up github pipelines to azure
- How to set up replication
- What are subdomains used for

Some Tests:

- if primary domain forest controller is down
- if both primary and secondary domain forest controllers are down
- if primary subdomain controller is down
- if both primary and secondary subdomain controller is down
- If primary storage node is down
- If both primary and secondary storage node is down

- Access storage directly:
- /spiritstore1/data
- /spiritstore2/users/usera
- /spiritstore2/users/userb
- /spiritstore2/users/userc
- /spiritstore2/users/userd
- /spiritstore3/scans/usera
- /spiritstore3/scans/userb
- /spiritstore3/scans/userc
- /spiritstore3/scans/userd

- Access storage through DFS
- /spiritstore1/root/users/usera
- /spiritstore1/root/users/usera/scans
- /spiritstore1/root/users/userb
- /spiritstore1/root/users/userb/scans
- /spiritstore1/root/users/userc
- /spiritstore1/root/users/userc/scans
- /spiritstore1/root/users/userd
- /spiritstore1/root/users/userd/scans
- /spiritstore1/root/scans/users/usera
- /spiritstore1/root/scans/users/userb
- /spiritstore1/root/scans/users/userc
- /spiritstore1/root/scans/users/userd
- /spiritstore1/root/data


# Non-Azure test configuration

In my laptop there is a VM manager called Parallels.  There are two
VMs in it: ubuntu and DC.

DC is the domain controller
Ubuntu is the linux client/server

Username/Passwords of DC:
    rschmitt/happy

Username/Passwords of Ubuntu:
    rschmitt/happy

Active Directory Accounts/Passwords

Administrator@SPIRITCLOUD.APP
    Hamilton50Texas

Spirit@SPIRITCLOUD.APP
    Corpus100Christi

Not sure if this is obsolete or not but I'm assuming the following is
for NTLM authentication of the Ubuntu SAMBA client:

Username: rschmitt
Password: Summer10Sun

If need to change ad-dc password, log into dc and type:
samba-tool user setpassword <user>
It will prompt for password and confirm

To get started.  First:

Make sure kerbero server at 192.168.1.192 is up and running.  May need
to restart samba-ad-dc:

```
systemctl restart samba-ad-dc
```

Log in to active directory from host

```
kinit spirit@SPIRITCLOUD.APP
```

Password for now is Corpus100Christi

First make sure that client is working

```
smbcp ./cmake.out //dc1.spiritcloud.app/spiritcloud/cmake.out
```

Then try that against self.  The host is called ubuntu.

The spiritcloud share on ubuntu allows the SPIRITCLOUD\spirit user.

So, technically, this should work.  This is where we want to start

```
smbcp ./cmake.out //ubuntu.spiritcloud.app/spiritcloud/cmake.out
```

Provisioning.

Offline provisioning of the server is required.  See the LINUX.md file
for descriptions on how to do that.

Configuration of the server:

SMB snippet from /etc/openfiles.xml.
Also is in openfiles/configs/linux_debug.xml

Provisioning info you will get from the DC:
- fqdn
- bootstrap_dc
- keytab

For NTLM authentication we specify:
- serveruser
- serverpass

Servers lists the ports to start up a server at.  Currently all ports
share the same exports.  We can have a server support multiple vapornets.
Each vapornet may be at a special port.  The exports could be vapornet/
server specific.  Probably fqdn, bootstrap_dc, and keytab would be
vapornet specific too.


```
    <smb>
      <fqdn>ubuntu.spiritcloud.app</fqdn>
      <bootstrap_dc>dc1.spiritcloud.app</bootstrap_dc>
      <keytab>/home/rschmitt/samba.keytab</keytab>
      <serveruser>rich</serveruser>
      <serverpass>rich</serverpass>
      <servers>
	<port>4445</port>
      </servers>
      <exports>
	<export>
	  <share>spiritcloud</share>
	  <comment>test share</comment>
	  <nativefs>fat</nativefs>
	  <path>/home/rschmitt/spiritcloud/</path>
	  <exporttype>DISK</exporttype>
	</export>
      </exports>
    </smb>


Some historical info:

There's a gssapi test in the kerberos repo that can be used to test the
provisioning:

git/krb5-1.20.1/src/appl/gss-sample

```
sudo bash
KRB5_TRACE=/dev/stderr ./gss-server -keytab /home/rschmitt/cifs.keytab cifs/ubuntu.spiritcloud.app
```

Things to look at:

1. is there a strdup in the params structure.  Or should we
change all calls to go direct? (NO)
2. the reason for the crashes is that first, the client crashes
because it is expecting something in the setup reponse from
the server which it doesn't get.  It trips.  Fix that (DONE)
3. When it does trip, it closes the connection to the server
prematurely which causes the server to crash.  fix that. (DONE)
4. Then fix the server so that it sends what the client
expects. (DONE)
5. Remove the obsolete stuff in the android app now that
manages multiple servers.  We really only need one for now.
6. Remove the stuff in persistence which supports multiple
servers.
7. Remove the code that starts up the various servers.
8. figure out the best way to get the server's keytab entry.
see more below.  Need to figure this out for both samba-ad-dc
and windows
9. Do memory leak testing
10. test the server with the following:
samba ad/dc, openfiles server, openfiles client
samba ad/dc, openfiles server, linux client
samba ad/dc, openfiles server, windows client
windows dc, openfiles server, openfiles client
windows dc, openfiles server, linux client
windows dc, openfiles server, windows client


