# linux_permissions
This repo describes linux permissions


# What are the default permissions for a file and directory

Default permission for a directory is 0777, for files the permissions are 0666 from which the default umask value 0022 is deducted to get the newly created files or directory permission.

# How to see the defaults

To see the default permissions you can initiate the `umask` command
```
vagrant@vagrant:~$ umask
0002
```

The mask is subtracted from the default file permission 0666 and default directory permissions 0777 to get the actual current default permissions for newly created files

0666
-
0002
-----
0664(rw-rw-r--)


# How to change the defaults 

To temporarily change your `umask` value, run the `umask VALUE` command. e.g.

```
vagrant@vagrant:~$ umask
0002
vagrant@vagrant:~$ umask 0022
vagrant@vagrant:~$ umask
0022
```
The new default permissions will be the umask subtracted from 0666(-rw-rw-rw) for files or from 0777(drwxrwxrwx) for directories

0666
-
0022
-----
0644(rw-r--r--)

vagrant@vagrant:~$ touch newfile
vagrant@vagrant:~$ ls -la newfile
-rw-r--r-- 1 vagrant vagrant 0 Jun 11 07:50 newfile

# Extended permissions - what are they, when do you need them, how they work

## setuid=4

### What is it?

Access rights flag that allows users to run  an executable with the permissions of the executable's owner

### Why it is used?

It is used so that a file can be executed as the owner even when logged in as a different user

### How it works?

Using a script that shows what is the current user id and the effective user id we can demonstrate who is running the binary file because of its permissions. The script is in this repo - `uid.c` and myuid is a compiled binary of that script.

```
vagrant@vagrant:~$ sudo chmod 4755 myuid
vagrant@vagrant:~$ ls -la myuid
-rwsr-xr-x 1 root root 8392 Jun  8 14:58 myuid
vagrant@vagrant:~$ ./myuid
The REAL UID =: 1000
The EFFECTIVE UID =: 0
```

## setgid=2

### What is it?

Access rights flag that allows users to run  an executable with the permissions of the executable's group

### Why it is used?

It is used so that a file can be executed as a user in the owners group even when logged in as a user not in that group

### How it works?

Using a script that shows the current group id of the user executing and the effective group id we can demonstrate how setgid works.
The script is in this repo - `gid.c` and mygid is a compiled binary of that script.

```
vagrant@vagrant:~$ chmod 2755 mygid
vagrant@vagrant:~$ ls -la mygid
-rwxr-sr-x 1 vagrant vagrant 8392 Jun 11 13:14 mygid
vagrant@vagrant:~$ sudo su
root@vagrant:/home/vagrant# ./mygid
The REAL GID =: 0
The EFFECTIVE GID =: 1000
```

## sticky=1

###  What is it?

Sticky bit is a user ownership access right flag that can be assigned to files and directories

### Why it is used?

When a directory's sticky bit is set, the filesystem treats the files in such directories in a special way so only the file's owner, the directory's owner, or root user can rename or delete the file.

### How it works?

```
vagrant@vagrant:~$ mkdir sticky_dir
vagrant@vagrant:~$ cd sticky_dir/
vagrant@vagrant:~/sticky_dir$ touch file{1..3}
vagrant@vagrant:~/sticky_dir$ ls -la
total 8
drwxrwxr-x 2 vagrant vagrant 4096 Jun 11 13:40 .
drwxr-xr-x 9 vagrant vagrant 4096 Jun 11 13:36 ..
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file1
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file2
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file3
vagrant@vagrant:~/sticky_dir$ cd ..
vagrant@vagrant:~$ ls -la sticky_dir
drwxrwxr-x 2 vagrant vagrant 4096 Jun 11 13:40 sticky_dir
vagrant@vagrant:~$ chmod 1777 sticky_dir/
vagrant@vagrant:~$ ls -la sticky_dir
drwxrwxrwt 2 vagrant vagrant 4096 Jun 11 13:40 sticky_dir
vagrant@vagrant:~$ cd sticky_dir/
vagrant@vagrant:~/sticky_dir$ rm -rf file1
vagrant@vagrant:~/sticky_dir$ ls -la
total 8
drwxrwxrwt 2 vagrant vagrant 4096 Jun 11 13:41 .
drwxr-xr-x 9 vagrant vagrant 4096 Jun 11 13:36 ..
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file2
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file3
root@vagrant:/home/vagrant/sticky_dir# su wheel
wheel@vagrant:/home/vagrant/sticky_dir$ rm -rf file2
rm: cannot remove 'file2': Operation not permitted
wheel@vagrant:/home/vagrant/sticky_dir$ exit
vagrant@vagrant:~/sticky_dir$ cd ..; chmod 0777 sticky_dir
vagrant@vagrant:~$ ls -la sticky_dir
drwxrwxrwx 2 vagrant vagrant 4096 Jun 11 13:41 sticky_dir
vagrant@vagrant:~$ sudo su
root@vagrant:/home/vagrant# su wheel
wheel@vagrant:/home/vagrant$ cd sticky_dir/
wheel@vagrant:/home/vagrant/sticky_dir$ ls -la
total 8
drwxrwxrwx 2 vagrant vagrant 4096 Jun 11 13:41 .
drwxr-xr-x 9 vagrant vagrant 4096 Jun 11 13:36 ..
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file2
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file3
wheel@vagrant:/home/vagrant/sticky_dir$ rm -rf file2
wheel@vagrant:/home/vagrant/sticky_dir$ ls -la
total 8
drwxrwxrwx 2 vagrant vagrant 4096 Jun 11 13:42 .
drwxr-xr-x 9 vagrant vagrant 4096 Jun 11 13:36 ..
-rw-rw-r-- 1 vagrant vagrant    0 Jun 11 13:40 file3
```



