//----------------------------------------------------
//
//          Implementation of outputbuffer
//
//----------------------------------------------------

#include "..\source\store.h"
#include <string.h>

outputbuffer::outputbuffer(void){
	memset(buf,'\0',1024);
	currpos=buf;
	len=0;
	maxsize=1024;
}

char *outputbuffer::write_request(int maxlen){
	while((*currpos!='\0')&&(len!=maxsize)) { //find the first available pos
		currpos++;
		len++;
	};
	if ((len+maxlen)<maxsize) {
		return currpos;
		}
	else return NULL;
}

char *outputbuffer::get_buffer(void) 
{
	return buf;
}


void outputbuffer::reset_buffer(void){
	memset(buf,'\0',1024);
	currpos=buf;
	len=0;
}

