   0x08048ec0 <+0>:	push   %ebp                                            to save the beginning of the previous function's stackframe
   0x08048ec1 <+1>:	mov    %esp,%ebp                                       Stores the contents of ESP (Stack Pointer), where the address of the top of the stack is contained, in EBP
   0x08048ec3 <+3>:	and    $0xfffffff0,%esp                                Align stack to 16 bytes (and $0xfffffff0)
   0x08048ec6 <+6>:	sub    $0x20,%esp                                      Allocate 0x20 bytes (32 bytes) for local variables
   0x08048ec9 <+9>:	mov    0xc(%ebp),%eax                                  Loads into EAX the value at [ebp + 0xC]                     0xc = 12
   0x08048ecc <+12>:	add    $0x4,%eax                                       Adds 4 to EAX   argv[0]  → argv[1]
   0x08048ecf <+15>:	mov    (%eax),%eax                                     Dereferences argv[1] so EAX = argv[1]
   0x08048ed1 <+17>:	mov    %eax,(%esp)                                     Places argv[1] on top of stack
   0x08048ed4 <+20>:	call   0x8049710 <atoi>                                Calls atoi(argv[1])
   0x08048ed9 <+25>:	cmp    $0x1a7,%eax                                     0x1a7 = 423 (decimal) campaire the input with 423
   0x08048ede <+30>:	jne    0x8048f58 <main+152>                            If input ≠ 423 Jump to <main+152> (failure path).
   0x08048ee0 <+32>:	movl   $0x80c5348,(%esp)                               Pushes address 0x80c5348 onto stack it like "Access Granted"
   0x08048ee7 <+39>:	call   0x8050bf0 <strdup>                              Calls strdup(string).  Allocates memory, Copies string into heap, Returns pointer
   0x08048eec <+44>:	mov    %eax,0x10(%esp)                                 Stores duplicated string pointer at: [esp + 0x10]
   0x08048ef0 <+48>:	movl   $0x0,0x14(%esp)                                 Stores 0 at [esp + 0x14]
   0x08048ef8 <+56>:	call   0x8054680 <getegid>                             Returns Effective Group ID. After return : EAX = effective group ID
   0x08048efd <+61>:	mov    %eax,0x1c(%esp)                                 Stores EGID at [esp + 0x1C]
   0x08048f01 <+65>:	call   0x8054670 <geteuid>                             Returns Effective User ID
   0x08048f06 <+70>:	mov    %eax,0x18(%esp)                                 Stores EUID at [esp + 0x18]
   0x08048f0a <+74>:	mov    0x1c(%esp),%eax                                 Loads saved EGID into EAX
   0x08048f0e <+78>:	mov    %eax,0x8(%esp)                                  Stores EGID at [esp + 0x8]
   0x08048f12 <+82>:	mov    0x1c(%esp),%eax                                 Loads EGID again


1)

0x08048ec0 <+0>:  push   %ebp

0x08048ec0 => This is the virtual memory address of the instruction. 

0x → hexadecimal
08048ec0 → 32-bit address
This is inside the .text section (code section)
In 32-bit Linux, binaries typically load around 0x08048000

<+0> =>  This means: Offset +0 from the beginning of the function.

The push X instruction decrements the stack pointer by 4 bytes (ESP = ESP - 4) to allocate space and then stores the value X at that memory location ([ESP] = X).

In 32-bit architecture:

Register	Purpose
eax	accumulator
ebx	base
ecx	counter
edx	data
esp	stack pointer
ebp	base pointer

Adresse 	Contenu	Description
ebp + 12	Argument 2	Deuxième paramètre                      [ebp+12]  = argv  
ebp + 8	Argument 1	Premier paramètre                       [ebp+8]   = argc
ebp + 4	Return Addr	Adresse pour revenir à l'appelant
ebp	Old EBP	Valeur sauvegardée du cadre précédent
ebp - 4	Local Var 1	Première variable locale (offset négatif)