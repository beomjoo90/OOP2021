#include <iostream>
#include <string.h>

int main()
{
	char data[100] { "hello world" };

	printf("%s\n", data);

	strcpy(data, "hello beomjoo seo");

	printf("%s\n", data);

	return 0;
}