
1) main function
---------------

(gdb) disas main
Dump of assembler code for function main:
   0x0804851a <+0>:	push   %ebp                            save old base pointer
   0x0804851b <+1>:	mov    %esp,%ebp                       create new stack frame
   0x0804851d <+3>:	and    $0xfffffff0,%esp                This aligns the stack to 16 bytes.
   0x08048520 <+6>:	call   0x80484a4 <v>
   0x08048525 <+11>:	leave                              leave is equivalent to: mov %ebp,%esp   pop %ebp
   0x08048526 <+12>:	ret                                Returns to _start.
End of assembler dump.

main() does nothing except call function v().


2) v() function
---------------


Dump of assembler code for function v:
   0x080484a4 <+0>:	    push   %ebp                         Save old base pointer on the stack
   0x080484a5 <+1>:	    mov    %esp,%ebp                    Create new stack frame. Now ebp → start of v() stack frame
   0x080484a7 <+3>:	    sub    $0x218,%esp                  Reserve 0x218 bytes on stack. 0x218 = 536 bytes
   0x080484ad <+9>:	    mov    0x8049860,%eax               Load value stored at address 0x8049860 into eax (eax = stdin)
   0x080484b2 <+14>:	mov    %eax,0x8(%esp)               This sets: 3rd argument of fgets = stdin
   0x080484b6 <+18>:	movl   $0x200,0x4(%esp)             Set second argument of fgets:0x200 = 512 so fgets(buffer, 512, stdin);
   0x080484be <+26>:	lea    -0x208(%ebp),%eax            0x208 = 520 So this is the start of the buffer.
   0x080484c4 <+32>:	mov    %eax,(%esp)
   0x080484c7 <+35>:	call   0x80483a0 <fgets@plt>        Call fgets, so now: User input is stored in buffer, Max 512 bytes read
   0x080484cc <+40>:	lea    -0x208(%ebp),%eax            Load address of buffer again.
   0x080484d2 <+46>:	mov    %eax,(%esp)                  Set first argument for printf.
   0x080484d5 <+49>:	call   0x8048390 <printf@plt>       THIS IS THE VULNERABLE LINE equivalent to printf(buffer);
   0x080484da <+54>:	mov    0x804988c,%eax               
   0x080484df <+59>:	cmp    $0x40,%eax                   if (*(0x804988c) == 64)
   0x080484e2 <+62>:	jne    0x8048518 <v+116>            If NOT equal: Jump to end of function.
   0x080484e4 <+64>:	mov    0x8049880,%eax
   0x080484e9 <+69>:	mov    %eax,%edx
   0x080484eb <+71>:	mov    $0x8048600,%eax
   0x080484f0 <+76>:	mov    %edx,0xc(%esp)
   0x080484f4 <+80>:	movl   $0xc,0x8(%esp)
   0x080484fc <+88>:	movl   $0x1,0x4(%esp)
   0x08048504 <+96>:	mov    %eax,(%esp)

