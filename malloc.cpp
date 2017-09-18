#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// struct strct {...}
// strct xxx;

typedef struct {
	int num;
	char *str;
} strct;

struct Point {
    int x;
    int y;
};

int main(){
	char* p = NULL;
	strct* entity;
	struct Point *a;

	cout<< entity <<endl;
	cout<< sizeof *entity <<endl;
	
//	entity->num = 1;
//	cout<< entity->num <<endl;
	p = (struct Point *)malloc( sizeof(struct Point) );
	entity = (struct strct *)malloc( sizeof(struct strct) );
	entity->num = 55;
//	cout<< entity->num <<endl;
//	cout<< entity.num <<endl;

	free(p);
	free(entity);

/*
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
*/
//	free(entity);

	return 0;
}

