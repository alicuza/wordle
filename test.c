
#include <unistd.h>
#include <stdio.h>

int main()
{
	char c;
	printf("enter a character: ");
	read(STDIN_FILENO, &c, 1);
	printf("%c\n", c);
}
