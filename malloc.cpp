#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(){
	char* p = NULL;

	p = (char*) malloc(sizeof(char) * 4);
//	p = (char*) calloc( 4, sizeof(char) );
	if(!p){
		return 0;
	}

	p[0] = 'a';
	p[1] = 'b';
	p[2] = 'c';
	p[3] = 0;

	printf("%s\n", p);

	free(p);

	return 0;
}

