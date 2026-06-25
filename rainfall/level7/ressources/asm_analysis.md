(gdb) disas main
Dump of assembler code for function main:
   0x08048521 <+0>:	    push   %ebp                          saved EBP
   0x08048522 <+1>:	    mov    %esp,%ebp                     Return addr
   0x08048524 <+3>:	    and    $0xfffffff0,%esp              This aligns the stack to 16 bytes (for performance reasons).
   0x08048527 <+6>:	    sub    $0x20,%esp                    So the stack allocates: 0x20 = 32 bytes
   0x0804852a <+9>:	    movl   $0x8,(%esp)
   0x08048531 <+16>:	call   0x80483f0 <malloc@plt>        ptr1 = malloc(8) Stored at: esp + 0x1c
   0x08048536 <+21>:	mov    %eax,0x1c(%esp)               (ESP + 0x1c) = pointer returned by malloc
   0x0804853a <+25>:	mov    0x1c(%esp),%eax               means EAX = *(ESP + 0x1c) Writing inside ptr1, So memory layout: 
   0x0804853e <+29>:	movl   $0x1,(%eax)                   ptr1: [0x00] = 1
   0x08048544 <+35>:	movl   $0x8,(%esp)
   0x0804854b <+42>:	call   0x80483f0 <malloc@plt>
   0x08048550 <+47>:	mov    %eax,%edx
   0x08048552 <+49>:	mov    0x1c(%esp),%eax
   0x08048556 <+53>:	mov    %edx,0x4(%eax)
   0x08048559 <+56>:	movl   $0x8,(%esp)
   0x08048560 <+63>:	call   0x80483f0 <malloc@plt>
   0x08048565 <+68>:	mov    %eax,0x18(%esp)
   0x08048569 <+72>:	mov    0x18(%esp),%eax
   0x0804856d <+76>:	movl   $0x2,(%eax)
   0x08048573 <+82>:	movl   $0x8,(%esp)
   0x0804857a <+89>:	call   0x80483f0 <malloc@plt>
   0x0804857f <+94>:	mov    %eax,%edx



   Dump of assembler code for function m:
   0x080484f4 <+0>:	push   %ebp
   0x080484f5 <+1>:	mov    %esp,%ebp
   0x080484f7 <+3>:	sub    $0x18,%esp
   0x080484fa <+6>:	movl   $0x0,(%esp)
   0x08048501 <+13>:	call   0x80483d0 <time@plt>
   0x08048506 <+18>:	mov    $0x80486e0,%edx
   0x0804850b <+23>:	mov    %eax,0x8(%esp)
   0x0804850f <+27>:	movl   $0x8049960,0x4(%esp)
   0x08048517 <+35>:	mov    %edx,(%esp)
   0x0804851a <+38>:	call   0x80483b0 <printf@plt>
   0x0804851f <+43>:	leave  
   0x08048520 <+44>:	ret    
