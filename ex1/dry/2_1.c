//
// Created by 97250 on 21/11/2021.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* stringduplicator(char* s, int times)
{
	///no have test if argument s is good ans=d remove "!" from the s
	assert(s);
	assert(times > 0);
	///remove stare from the s and change from LEN to len
	unsigned len = strlen(s);
	/// need to add +1 to len * times
	char* out = malloc(len * times + 1);
	///no have test if the malloc is good
	assert(out);
	for (int i = 0; i < times; i++) {
		///out += len; ???
		strcpy(out+i*len, s);
	}
	return out;
}

int main()
{
	char* s="hello";
	printf("%s",stringduplicator(s,3));

	return 0;
}
