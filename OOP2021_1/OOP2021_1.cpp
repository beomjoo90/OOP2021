#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cstring> // string.h
#include <cstdlib> // stdlib.h
#include <string> // c++ string class



int main()
{
	char data[100] { "hello world" };

	printf("%s\n", data);

	strncpy(data, "world", 5);
	strncat(data, " You should win.", 3);
	printf("%s\n", data);

	if (strncmp(&data[6], "world", 5) == 0) {
		printf("matched\n");
	} else 
		printf("unmatched\n");
	

	

	

	
	return 0;
}