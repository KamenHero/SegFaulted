Level4
-----

level4@RainFall:~$ ls
level4

level4@RainFall:~$ ./level4
efedf
efedf

we can see that level4 contain the file level4, and after checking it with gdb "check asm_analysis.md" we can see that the main function calls n() function and that function calls p() function

and the vulnerability is located inside the p() function, it's the printf()
so the mistake is, instead of "printf("%s", buffer);" they are doing "printf(buffer);"

so if if (global == 0x1025544)

1_ let's find the offset
------------------------

level4@RainFall:~$ python -c 'print "aaaa" + " %x" * 15' | ./level4
aaaa b7ff26b0 bffff794 b7fd0ff4 0 0 bffff758 804848d bffff550 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078


from this we can assume that aaaa is located in position 12 so the offset is 12 meaning %12n

and the condition says if (*(0x8049810) == 0x1025544)
so 0x1025544 = 16930116 (decimal)
so we must make *(0x8049810) = 16930116

in printf %10d means Print a decimal number using at least 10 characters width. If the number is shorter, it pads with spaces.

python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' | ./level4


In printf, the %n specifier means: write the number of characters printed so far into the given address

Conclusion:
The first 4 bytes \x10\x98\x04\x08 represent the address 0x08049810, which is the variable the program later checks. When printf processes the input, %16930112d forces it to print 16,930,112 characters, so the internal counter of printed characters becomes 16,930,116 (including the first 4 bytes). Then %12$n tells printf to take the 12th argument from the stack as an address and write the number of printed characters to it. Because the stack layout places the first 4 bytes of our input at position 12, printf writes 16930116 (0x01025544) into 0x08049810. After printf returns, the program compares that variable to 0x01025544; the condition becomes true, so it calls system and executes the protected command (revealing the next level password).