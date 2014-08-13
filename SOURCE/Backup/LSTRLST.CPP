//----------------------------------------------------
//
//          Implementation of linked stringlist
//
//----------------------------------------------------

#include <string.h>
#include "..\source\store.h"

linked_stringlist::linked_stringlist(char *st){
int i;
	i=strlen(st);
	str = new char[i+1];
	strcpy(str,st);
	nextobj=NULL;
}

linked_stringlist::linked_stringlist(void){
	str=NULL;
	nextobj=NULL;
}
linked_stringlist *linked_stringlist::next(void){
	if (nextobj==NULL) return NULL;
	else return nextobj;
};

char *linked_stringlist::getstring(void){
	return str;
}

void linked_stringlist::set_next_to(linked_stringlist *li){
	nextobj=li;
}

linked_stringlist *linked_stringlist::insert(char *st){
linked_stringlist *tmp;
int i;
	if (str==NULL) {
		i=strlen(st);
		str=new char[i+1];
		strcpy(str,st);
		tmp=this;
		}
	else {
		tmp = new linked_stringlist(st);
		if (nextobj==NULL) {
			nextobj=tmp;
			}
		else {
			(*tmp).set_next_to(nextobj);
			nextobj=tmp;
			}; //end else  next0bj==NULL
		};//emd element==NULL
	return tmp;
}; //end insert
