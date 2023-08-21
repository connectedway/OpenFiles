
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
