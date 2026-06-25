The critical part of the code

At this section:

0x080485e4: movl $0x4,(%esp)
0x080485eb: call malloc
0x080485f0: mov %eax,0x8049aac

This means:

global_ptr = malloc(4)

So a 4‑byte heap object is allocated.

Next instruction
0x080485f5: mov 0x8049aac,%eax
0x080485fa: movl $0x0,(%eax)

Meaning:

*global_ptr = 0

Memory layout:

global_ptr → [ 0x00000000 ]