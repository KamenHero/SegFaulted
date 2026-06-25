main function
(gdb) disas main
Dump of assembler code for function main:
0x080485f4 <+0>: push ebp
0x080485f5 <+1>: mov ebp,esp
0x080485f7 <+3>: push ebx
0x080485f8 <+4>: and esp,0xfffffff0
0x080485fb <+7>: sub esp,0x20

Program setup and stack allocation.

0x080485fe <+10>: cmp DWORD PTR [ebp+0x8],0x1
0x08048602 <+14>: jg 0x8048610
0x08048604 <+16>: mov DWORD PTR [esp],0x1
0x0804860b <+23>: call _exit@plt

Checks argc.
If no argument → exit(1).

First object
0x08048610 <+28>: mov DWORD PTR [esp],0x6c
0x08048617 <+35>: call _Znwj@plt

Allocates 108 bytes on heap.

0x0804861e <+42>: mov DWORD PTR [esp+0x4],0x5
0x08048629 <+53>: call _ZN1NC2Ei

Calls constructor:

N *a = new N(5)
Second object
0x08048632 <+62>: mov DWORD PTR [esp],0x6c
0x08048639 <+69>: call _Znwj@plt

Another heap allocation.

0x08048640 <+76>: mov DWORD PTR [esp+0x4],0x6
0x0804864b <+87>: call _ZN1NC2Ei

Creates:

N *b = new N(6)
setAnnotation call
0x08048664 <+112>: mov eax,[ebp+0xc]
0x0804866a <+118>: mov eax,[eax+4]

Loads:

argv[1]
0x08048677 <+131>: call _ZN1N13setAnnotationEPc

Calls:

a->setAnnotation(argv[1])
Virtual function call
0x0804867c <+136>: mov eax,[esp+0x10]
0x08048680 <+140>: mov eax,[eax]
0x08048682 <+142>: mov edx,[eax]

Gets a function pointer from b's vtable.

0x08048693 <+159>: call edx

Calls:

b->operator+(a)
Constructor
N::N(int)
mov [eax],0x8048848
mov [eax+0x68],edx

Stores:

vtable pointer

integer value (5 or 6)

operator+
return a->value + b->value
operator-
return a->value - b->value
setAnnotation
strlen(input)
memcpy(annotation, input, strlen(input))

No size check -------> buffer overflow

What the program does
N *a = new N(5)
N *b = new N(6)

a->setAnnotation(argv[1])

b->operator+(a)

The overflow in setAnnotation() can overwrite object b in the heap.

If we overwrite b's vtable pointer, we can control the function executed by:

call edx

➡ This leads to code execution.