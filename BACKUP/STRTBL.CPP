//----------------------------------------------------
//
//          Implementation of stringtable
//
//----------------------------------------------------
#include <string.h>
#include "..\source\store.h"

stringtable::stringtable(void){
	tbl=NULL;
	tablesize=0;
}
stringtable::stringtable(int size){
	tbl=new linked_stringlist[size];
	tablesize=size;
}

int stringtable::hashfn(char *prop){
int sum=0;
int i=0;
	while(prop[i]!='\0')
		sum=prop[i++]+sum;
	return (sum % tablesize);
}

char *stringtable::put(char *str){
int bucket;
linked_stringlist *iter;
	bucket=(*this).hashfn(str);
	iter=tbl+bucket;
	iter=(*iter).insert(str);
	return (*iter).getstring();
};

char *stringtable::get(char *str){
int bucket;
linked_stringlist *iter;
char *storedstr;
	bucket=(*this).hashfn(str);
	iter=tbl+bucket;
	storedstr=(*iter).getstring();
	while((storedstr!=NULL)&&(strcmp(str,storedstr)!=0)&&(iter!=NULL)){
		iter=(*iter).next();
		if (iter!=NULL)
			storedstr=(*iter).getstring();
		else storedstr=NULL;
		};
	if (iter==NULL) {
		return (*this).put(str);
		};
	if ((storedstr!=NULL)&&(strcmp(str,storedstr)==0)) return storedstr;
	return NULL;
};
