#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *Encrypt( char *arry, unsigned long key );

int main(){
	char *arry = malloc(500);
	char *new = malloc(500);
	int i = 0;
	int temp = 0;
	unsigned long key = 1;
	printf("Enter message: ");
	fgets( arry, 256, stdin);


	new = Encrypt( arry, key);

	printf("%s\n", new);
	free(arry);
return 0;
}

char *Encrypt( char *arry, unsigned long key ){
	int  i = 0;
	int temp = 0;

	temp = strlen(arry);
	for( i = 0; i < temp; i++ ) arry[i] = ++arry[i];
	return arry;	
}
