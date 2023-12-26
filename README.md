# Mini Bittorrent - Peer to Peer group based file share
## Distributed File System // Peer to peer group file transfering

-  Should be able to download_file​ from various peers in the network.

**Software Requirement**

- For calculating the SHA of the file 

1 OpenSSL library

   - **To install OpenSSL library :** `sudo apt-get install openssl`
    - #include <openssl/sha.h>


## How to Run

```
1 cd client
2 make
3 cd ../tracker
4 make
```

### Tracker

1 Run Tracker:

```
cd tracker
./tracker​ <TRACKER INFO FILE> <TRACKER NUMBER>
ex: ./tracker tracker_info.txt 1
```

`<TRACKER INFO FILE>` contains the IP, Port details of all the trackers.

```
Ex:
127.0.0.1
5000
127.0.0.1
6000
```

2 Close Tracker:

```
quit
```

### Client:

1 Run Client:

```
cd client
./client​ <IP>:<PORT> <TRACKER INFO FILE>
ex: ./client 127.0.0.1:18000 trackerInfo.txt
```

2 Create user account:

```
create_user​ <user_id> <password>
```

3 Login:

```
Login​ <user_id> <password>
```

4 Create Group:

```
create_group​ <group_id>
```

5 Join Group:

```
join_group​ <group_id>
```

6 Leave Group:

```
leave_group​ <group_id>
```

7 List pending requests:

```
list_requests ​<group_id>
```

8 Accept Group Joining Request:

```
accept_request​ <group_id> <user_id>
```

9 List All Group In Network:

```
list_groups
```

10 List All sharable Files In Group:

```
list_files​ <group_id>
```

11 Upload File:

```
​upload_file​ <file_path> <group_id​>
```

12 Download File:​

```
download_file​ <group_id> <file_name> <destination_path>
```

13 Logout:​

```
logout
```

## Working

1 User should create an account and register with tracker.

2 Login using the user credentials.

3 Tracker maintains information of clients with their files(shared by client) to  assist the clients for the communication between peers.

4 User can create Group and hence will become admin of that group.

5 User can fetch list of all Groups in server.

6 User can join/leave group.

7 Group admin can accept group join requests.

8 Share file across group: Shares the filename and SHA1 hash of the complete file.

9 Fetch list of all sharable files in a Group.

10 Download:
    
    1. Retrieve peer information from tracker for the file.
    
    2. Download file from multiple peers (different pieces of file from different peers - ​piece selection algorithm​) simultaneously and all the files which client downloads will be shareable to other users in the same group. File integrity is ensured using SHA1 comparison.

11 Piece selection algorithm used: Selects rarest piece and then downloads it from a random peer having that piece.


12 Logout - stops sharing all files and remove user entry from group.


## Assumption
 1 Files name are unique through out the group.

 2 File name and group name shouldn't contains space.

 3 All the Upload path are valid.

 4 User can be part of only one group.

 5 You can't be able to join group​ if you are already in group.

 6 User can upload the file only if that user have whole file.

 7 User can't create group if it will be part of some group.

 8 If admin leaves the group, at this moment group doesn't have any admin. Once user (admin) joins the same group, he will get the admin role back.

