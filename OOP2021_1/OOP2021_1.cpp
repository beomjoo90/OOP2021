#include <iostream>
#include <string.h>

int main()
{
	char data[100] { "hello world" };

	printf("%s\n", data);

	strcpy(data, "hello beomjoo seo");

	// 한글을 어떻게

	printf("%s\n", data);

	strcat(data, ", tomorrow");

	printf("%s\n", data);


	return 0;
}