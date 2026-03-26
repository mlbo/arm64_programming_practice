#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static int strcmp_c_impl(const char* str1, const char* str2) {
	char c1, c2;
	do {
		c1 = *str1++;
		c2 = *str2++;
	} while (c1 != '\0' && c1 == c2);

	return c1 - c2;
}

int main(void)
{
	int i;
	int ret;

	char *str1 = "sve is good!";
	char *str2 = "sve is good!!";

	ret = strcmp_c_impl(str1, str2);
	printf("C: ret = %d\n", ret);

	ret = strcmp_sve(str1, str2);
	printf("ASM: ret =%d\n", ret);
}
