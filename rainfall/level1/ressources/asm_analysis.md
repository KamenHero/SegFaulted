main
----

0x08048480 <+0>:	push   %ebp
0x08048481 <+1>:	mov    %esp,%ebp                           Standard stack frame setup.
0x08048483 <+3>:	and    $0xfffffff0,%esp                    Align stack to 16 bytes. 
0x08048486 <+6>:	sub    $0x50,%esp                          0x50 = 80 bytes so the function reserves 80 bytes on the stack.
0x08048489 <+9>:	lea    0x10(%esp),%eax                     EAX = ESP + 0x10 So the buffer does NOT start exactly at ESP. ESP + 16 bytes     (0x10 = 16)
0x0804848d <+13>:	mov    %eax,(%esp)                         Set argument for gets() function.
0x08048490 <+16>:	call   0x8048340 <gets@plt>                it reads input, It does NOT check length, It allows unlimited overflow. so gets(eax)
0x08048495 <+21>:	leave  
0x08048496 <+22>:	ret                                   <+21> - <+22> : These two lines allow us to find the state of the registers before executing the function. In others terms we quit the main() function.

run
---

   0x08048444 <+0>: 	push   %ebp
   0x08048445 <+1>:	    mov    %esp,%ebp
   0x08048447 <+3>:	    sub    $0x18,%esp                         24 bytes are allocated to the main function for its local variables.
   0x0804844a <+6>:	    mov    0x80497c0,%eax                     Loads the value stored at address 0x80497c0 into EAX
   0x0804844f <+11>:	mov    %eax,%edx                          Copies that value into EDX
   0x08048451 <+13>:	mov    $0x8048570,%eax                    Loads address 0x8048570 into EAX
   0x08048456 <+18>:	mov    %edx,0xc(%esp)                     Places EDX (stdout) at esp + 12
   0x0804845a <+22>:	movl   $0x13,0x8(%esp)                    Places 0x13 (19 decimal) at esp + 8
   0x08048462 <+30>:	movl   $0x1,0x4(%esp)                     Places 1 at esp + 4
   0x0804846a <+38>:	mov    %eax,(%esp)                        Places string address at esp
   0x0804846d <+41>:	call   0x8048350 <fwrite@plt>             Calls fwrite
   0x08048472 <+46>:	movl   $0x8048584,(%esp)                  Places address 0x8048584 at top of stack
   0x08048479 <+53>:	call   0x8048360 <system@plt>             Calls system with that string.
   0x0804847e <+58>:	leave                                     Restores previous stack frame.
   0x0804847f <+59>:	ret  



-------------------
1️ What Is a Buffer?
-------------------

In C:

char buffer[8];

This means:

👉 Reserve 8 bytes in memory.

Memory might look like:

Address 1000 → buffer[0]
Address 1001 → buffer[1]
...
Address 1007 → buffer[7]

Only 8 bytes are valid.

----------------------------
2️ What Is a Buffer Overflow?
----------------------------

If a function writes more than 8 bytes:

gets(buffer);

And user types:

AAAAAAAAAAAA

12 bytes.

Memory becomes:

buffer[0] = A
...
buffer[7] = A
buffer[8] = A   ← OUTSIDE buffer
buffer[9] = A
buffer[10] = A
buffer[11] = A

Now we are writing into memory that does not belong to the buffer.

That is a buffer overflow.

------------------------
3️ Why Is This Dangerous?
------------------------

Because of how the stack is organized.

When a function runs, stack looks like this:

High memory
+----------------------+
| Return Address       |  ← where function returns
+----------------------+
| Saved EBP            |
+----------------------+
| Local variables      |
| (buffer is here)     |
+----------------------+
Low memory

Important:

Stack grows downward.

But overflow writes upward.

So if buffer is too small and we write too much:

We start overwriting:

Other local variables

Saved EBP

Return address

Return address is the critical one.

-----------------------------
4️ What Is The Return Address?
-----------------------------

When a function is called:

call main

CPU pushes:

Address of next instruction

Onto the stack.

That value is the return address.

When function finishes:

ret

CPU pops that address into EIP and jumps there.

So:

Return address = “where to go next”

If we overwrite it:

We decide where program goes next.

That is control hijacking.


------------------------
5️ How Do We Override It?
------------------------

Imagine:

buffer[64]

Stack:

+----------------------+
| Return Address       |
+----------------------+
| Saved EBP            |
+----------------------+
| buffer[64]           |
+----------------------+

If we send:

"A"*64

Buffer is full.

If we send:

"A"*68

64 fill buffer

4 overwrite saved EBP


If we send:

"A"*72

64 buffer
4 saved EBP
4 return address

Now return address = 0x41414141

When function does ret:

CPU tries to jump to:

0x41414141

Crash.

That crash proves we control EIP.