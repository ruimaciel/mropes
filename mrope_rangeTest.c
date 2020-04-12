#include <mropes/rope.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main () {
	mrope_t rope;
	mrope_t rope2;
	mrope_init(&rope);
	const char* text="01234";
	mrope_init(&rope2);
	for(int i=2;i!=4;i++) {
		char* buffer=malloc(2);
		buffer[1]=0;
		buffer[0]=text[i-2];
		mrope_append_text(&rope,buffer);
	}
	for(int i=0;i!=2;i++) {
		char* buffer=malloc(2);
		buffer[1]=0;
		buffer[0]=text[2+i];
		mrope_append_text(&rope2,buffer);
	}
	mrope_prepend_rope(&rope,&rope2);
	int length2Copy=4;
	char outputBuffer[length2Copy+1];
	outputBuffer[length2Copy]=0;
	mrope_index_range(&rope,outputBuffer,0,4);
	putchar(mrope_index(&rope,1));
	putchar('\n');
	printf("Size%i:\n",mrope_length(&rope));
	printf("Slice:%s\n",outputBuffer);
	mrope_free(&rope);
	return 0;
}
