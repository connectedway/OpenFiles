
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


To log into the domain from Ubuntu on the Azure Ubuntu:

$ # from a shell on some host somewhere
$ ssh -i ~/.ssh/id_rsa rschmitt@52.182.161.63
#
# also can log in with rschmitt:HamiltonTexas0
#
#
# That brings us to Ubuntu inside the cloud
#
$ kinit rschmitt@CONNECTEDFOREST.COM
# Password is HamiltonTexas0

# To make openfiles for that target:

$ cd git/openfiles
$ make azure-linux-clean
$ make azure-linux
$ sudo make azure-linux-install
#
# To teset it
#
$ smbcp //spiritstore.file.core.windows.net/data/chart.png /home/rschmitt/chart2.png


#
# To add a member to Azure AD (Windows)
# Start up powershell
#
Add-DnsServerResourceRecordA -Name "HOST_NAME" -ZoneName "ZONE_NAME" -IPv4Address "IP_ADDRESS" 
#
# For example
#
Add-DnsServerResourceRecordA -Name "UBUNTU" -ZoneName" "CONNECTEDFOREST.COM" -IPv4Address "10.0.0.6"
#

#
# Now do a djoin
#
# in powershell
djoin /provision /domain "ZONE_NAME" /machine "HOST-NAME" /savefile save.txt
#
# For example
#
djoin /provision /domain "CONNECTEDFOREST.COM" /machine "UBUNTU" /savefile save.txt

If you look at Admin Tool Active Directory Users and COmputers, Computers you'll see UBUNTU
#
# Add the storage account
#
ktpass /princ cifs/ubuntu.connectedforest.com@CONNECTEDFOREST.COM /mapuser rschmitt@connectedforest.com +rndpass /out ubuntu.keytab /crypto all /ptype KRB5_NT_PRINCIPAL /mapop set
#
# Look




#######

# Setting up the Windows Server on aws

Go to EC2, create an instance of a windows server 2022
connect to rdp

Password: BCTfs.j&IQ-3?(y;)=VPjEOWOh.$Gumx

1. Log into server: spiritdc
2. Run Server Manager (if not already running)
3. In Configure Local Server:
4. Click "Add Roles and Features"
5. Click Next
6. Select Role-based or feature-based installation
7. Select server from the server pool, select spiritdc,
8. click next
9. Select Active Directory Domain Services.  If there's a popup, select ok
10. select next
11. Select Features Screen, allow defaults, click next
12. Active Directory Domain services screen, click next
13. Confirm installation selections, click install
14. Can close the screen, or wait for feature installation to succeed before hitting close
15. Should see the server manager dashboad with a yellow triangle with an exclamatoion point
for notifications.  Which also says, post deployment configuration.
16. click on "promote thi server to a domain controller"
17. will bring up deployment configuration
18. click add a new forest, root comain is "doubledoubleu.com" click next
19. will bring up domain controller options, DNS server should be checked.
20. Add password (Hamilton50Texas) for DSRM
21. This will bring up DNS options screen.  There'll be a scary warning that
"A delegation for this DNS server cannot be created"  Don't worry.  Leave the default and click Next
22. Add the NetBIOS name.  DOUBLEDOUBLEU.  Click next
23. Will bring up a paths screen.  Accept defaults, click next
24. Review options screen.  Click next
25. Prerequisites Check screen, click Install
26. System should restart when complete

Verify Installation

1. Log back into spiritdc (doubledoubleu)
2. Go to the run screen and select Windows Power Shell
3. Enter: Get-Service adws,kdc,netlogon,dns.  They should all be running.
4. Enter: Get-ADDomainController.  Should look good
5. Enter: Get-ADDomain doubledoubleu.com.  Should look good

All Done.

Set up Firewall:

Name    Security group rule ID	IP version Type Protocol Port range Source
	
IPv4	DNS (UDP)	UDP	53	0.0.0.0/0	–
IPv4	DNS (TCP)	TCP	53	0.0.0.0/0	–
IPv4	AD      	TCP	135	0.0.0.0/0	–
IPv4	LDAP    	UDP	389	0.0.0.0/0	–
IPv4	LDAP    	TCP	636	0.0.0.0/0	–
IPv4	LDAP	        TCP	389	0.0.0.0/0	–
IPv4	LDAP     	UDP	636	0.0.0.0/0	–
IPv4	RDP	        TCP	3389	0.0.0.0/0	–
IPv4	Kerberos	UDP	88	0.0.0.0/0	–
IPv4	All ICMP - IPv4	ICMP	All	0.0.0.0/0	–
IPv4	SMB	        TCP	445	0.0.0.0/0	–
IPv4	AD      	UDP	135	0.0.0.0/0	–

Add A user to active directory

In DC, click Active Directory Uesrs and Computers

Click on doubledoubleu.com,
Then click on Users
right click add New User
First Name; Spirit
Last Name: Cloud
Full Name: Spirit Cloud
User logon name: spiritcloud
Userlogon name DOUBLEDOUBLEU\spiritcloud

Click Next
Passwowrd: Hamilton50Texas
Set password never expires, clean user must change
Click Next and then finish

Add A share

Simply create a directory,
right click
simple...

# Setting up the Ubuntu Server on aws

This is generally pretty easy.  Get a ubuntu server running

1. install net-tools
   sudo apt install net-tools
2. Find private IP address of windows server
3. ping window server.  If this doesn't work, verify the instances
   security groups
   subnets
   routing tables
   By default, ping is not open on the servers.

Configure routing for AWS.  Two instances, two seperate subnets.  Set
up routing tables so they can talk with each other.

spiritdc instance i-0b09a6a2bcb5a6d67
    IP: 172.31.17.154
    subnet subnet-0edeb56fcce30a13b
    route table rtb-033a9fd6c1bf5500e
    IPv4 CIDR 172.31.16.0/20
    security group: sg-04f063e7de491c75

spiritux instance i-0e8e02d0769cb3e8b
    IP: 172.31.47.9  00101111 (0x2f)
    subnet subnet-0360b3b397250d312
    route table rtb-033a9fd5c1bf5500e
    IPv4 CIDR 172.31.32.0/20
    security group: sg-01dcf21113a7ee1a

VPC vpc-06acf90a1de9c405f
    route table rtb-033a9fd5c1bf5500e
    IPv4 CIDR 172.31.32.0/16

NOTE: They all share the same route table

Update DNS

edit/create /etc/netplan/99-custom-dns.yaml and set to:
network:
  version: 2
  ethernets:
    eth0:
      nameservers:
        addresses:
          - 172.31.17.154
          - 172.31.0.2
        search:
          - doubledoubleu.com
      dhcp4-overrides:
	use-dns: false
	use-domains: false

Then run
netplan generate

then reboot

If you're good to puppies, you should have your DNS setup.

Go back to the dc DNS and add an entry for spiritdc and spiritux

make sure reverse ptr records are setup too.  Make sure the addresses
are correct.  For some reason, ours were set up with a bad address.  That's
why we couldn't connect

Now, back to ubuntu.

Enter: sudo apt install krb5-user sssd-krb5

sudo apt update
apt install realmd sssd sssd-tools adcli samba python3-samba cifs-utils

edit: /usr/lib/x86_64-linux-gnu/sssd/conf/sssd.conf
add:
[domain/doubledoubleu.com]
id_provider = ad
krb5_server = spiritdc.doubledoubleu.com
realmd_server =	spiritdc.doubledoubleu.com
krb5_use_ad_srv_records = yes

make sure /etc/hostname and /etc/host have the hostname

Mount the share
mount //spiritdc/shared /mnt/shared -o username=spiritcloud

restart sssd
systemctl restart sssd

Join the domain
realm join -U spiritcloud doubledoubleu.com --computer-name=spiritux

# Setting up the Windows Server on Azure

1. Go to https://portal.azure.com/#home
2. Click on "Create a Resource"
3. Click Virtual Machine/Create
4. Subscription: Azure subscription 1
   Resource group: spiritcloud
   Virtual machine name: spiritdc (doubledoubleu)
   Region: (US) Central US
   Availability options: Availability zone
   Availability zone: Zones 1
   Security type: Trusted Launch
   Image: Window Server 2022 Datacenter: Azure Edition (WS 2016 DataCenter)
   VM architecture: x64
   Run with azure spot discount:
   Size: Standard B2ms (2 vcpus, 8GiB memory)
   Username: rschmitt
   Password: Hamilton50Texas
   Public inbound ports: Allow selected ports
   Select inbound ports: RDP (3389)
   Licensing: Would you like to use an existing license: 

   Encryption at host: No
   OS disk type: Standard HDD
   Delete with VM: yes
   Key management: Platform-managed
   Enable Ultra Disk compatability: No
   Data disks: None
   Virtual network: aadds-vnet
   Subnet: aadds-subnet
   Public IP: new
   NIC network security group: None
   Delete public IP and NIC when VM i deleted: Yes
   Enabled accelerated networking: no
   Load balncing options: None

   Enable system assigned managed identity: No
   Login with Azure AD: No
   Auto-Shutdown: Yes
   Enable bckup: No
   Enable Disaster Recovery: No
   Enable hotpatch: No
   Patch orchestration: Automatic

   Enable recommended alert rules: No
   Boot diagnostics: Disable
   Enable os guest diagnostics: No

   Extensions:  Didn't set any up.


Create RDP connection.

In azure click on the VM spiritdc.
Then click on "Connect"
Start VM
Then Select "Native RDP"
Make sure prerequiites are all configured.
Open the RDP app
Download the RDP file
Import the RDP file

click on the entry in RDP
Username rschmitt
Password Hamilton50Texas


Pick a domain name.  doubledoubleu.com

1. Log into server: spiritdc (doubledoubleu)
2. Run Server Manager (if not already running)
3. In Configure Local Server:
4. Click "Add Roles and Features"
5. Click Next
6. Select Role-based or feature-based installation
7. Select server from the server pool, select spiritdc,
8. click next
9. Select Active Directory Domain Services.  If there's a popup, select ok
10. select next
11. Select Features Screen, allow defaults, click next
12. Active Directory Domain services screen, click next
13. Confirm installation selections, click install
14. Can close the screen, or wait for feature installation to succeed before hitting close
15. Should see the server manager dashboad with a yellow triangle with an exclamatoion point
for notifications.  Which also says, post deployment configuration.
16. click on "promote thi server to a domain controller"
17. will bring up deployment configuration
18. click add a new forest, root comain is "doubledoubleu.com" click next
19. will bring up domain controller options, DNS server should be checked.
20. Add password (Hamilton50Texas) for DSRM
21. This will bring up DNS options screen.  There'll be a scary warning that
"A delegation for this DNS server cannot be created"  Don't worry.  Leave the default and click Next
22. Add the NetBIOS name.  DOUBLEDOUBLEU.  Click next
23. Will bring up a paths screen.  Accept defaults, click next
24. Review options screen.  Click next
25. Prerequisites Check screen, click Install
26. System should restart when complete

Verify Installation

1. Log back into piritdc (doubledoubleu)
2. Go to the run screen and select Windows Power Shell
3. Enter: Get-Service adws,kdc,netlogon,dns.  They should all be running.
4. Enter: Get-ADDomainController.  Should look good
5. Enter: Get-ADDomain doubledoubleu.com.  Should look good

All Done.

Assign storage to new domain

In AWS, we're going to add storage to the windows server.

Add a share

\\EC2AMAZ-DSDVHC8\SPIRIT


1. In azure portal, select "spiritstore" storage account
2. Look for file shares in explorer pane, then select the share.
3. There'll be an Active Directory section in the properties screen,  In our case it says 'configured'.
4. click 'configured'
5. There'll be Step 1: Enable An Active Directory source.  There'll be Active Directory Enabled with
a "Configure" under it.
6. Click Configure
7. You'll want to Click on Disable Active Directory
8. You'll be back at Step 1.  Click "Set up" for Active Directory
9. Click Step 1: Enable Active Directory Authentication

That tell you to install AzFilesHybrid.  Problem is, it doesn't say where to install it.  First I though
we'd install it on the file share host but I think we really want to do this on the windows server.

On the spiritdc (doubledoubleu), bring up powershell

1. Make sure running powershell 7
$PSVersionTable.PSVersion
2. See if AzureRM is installed
Get-Module -Name AzureRM -ListAvailable
It shouldn't be.  If it is, not sure.
3. Execute: Get-ExecutionPolicy -List
4. Execute: Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
5. Install Az Powershell: Install-Module -Name Az -Repository PSGallery -Force

1. Install Powershell 7.
https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows?view=powershell-7.3#msi
2. Run Powershell 7
1. Enter: Install-Module -Name Az.Storage
If it asks whether to install NuGet, say yes