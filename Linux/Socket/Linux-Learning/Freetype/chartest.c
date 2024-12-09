#include<stdio.h>
#include<string.h>
#include<wchar.h>

int main(int argc, char **argv){
	wchar_t *chinese_str = L"中gif";
	unsigned int *p = (wchar_t *)chinese_str;
	int i;

	printf("size of (wchar_t)=%d \n", (int)sizeof(wchar_t));
	printf("the unicode: \n");
	for(i = 0; i < wcslen(chinese_str); i++){
		printf("0x%x, ", p[i]);
	}
	printf("\n");
	return 0;
}
