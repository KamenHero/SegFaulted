#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void	n(void)
{
	system("/bin/cat /home/user/level7/.pass");
}

void	m(void)
{
	puts("Nope");
}

int		main(int argc, char **argv)
{
	char		*buffer = malloc(64);
	void (**fp)(void)     = malloc(sizeof(*fp)); // malloc(4)


	
	//	0x080484a5 <+41>:    mov    edx,0x8048468
	//	0x080484aa <+46>:    mov    eax,DWORD PTR [esp+0x18]
	//	0x080484ae <+50>:    mov    DWORD PTR [eax],edx
	
	*fp = m;
	strcpy(buffer, argv[1]);
	
	//	0x080484ca <+78>:    mov    eax,DWORD PTR [esp+0x18]
	//	0x080484ce <+82>:    mov    eax,DWORD PTR [eax]
	//	0x080484d0 <+84>:    call   eax
	
	(*fp)();

	return (0);
}