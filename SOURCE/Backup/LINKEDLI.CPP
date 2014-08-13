//----------------------------------------------------
//
//          Implementation of linked_list
//
//----------------------------------------------------

#include "..\source\store.h"

linked_list::linked_list(object *o){
	element=o;
	nextobj=NULL;
};

linked_list::linked_list(void){
	element=NULL;
	nextobj=NULL;
};

linked_list::~linked_list(void){
}

linked_list *linked_list::next(void){
	if (nextobj==NULL) return NULL;
	else return nextobj;
};

object *linked_list::elem(void){
	return element;
};

void linked_list::set_next_to(linked_list *li){
	nextobj=li;
};

linked_list *linked_list::insert(object *o){
linked_list *tmp;
	if (element==NULL) element=o;
	else {
		tmp = new linked_list(o);
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

void linked_list::display(outputbuffer &buf){
linked_list *succ;
	if (element!=NULL){
		(*element).display(buf);
		succ=(*this).next();
		if (succ!=NULL) (*succ).display(buf);
		}; //end if
};

int linked_list::empty(void){
	if (element==NULL) return 1;
	else return 0;
};

object *linked_list::findOID(ODB_OID oid){
	if ((*element).oid()==oid) return element;
	else {
		if (nextobj==NULL) return NULL;
		else return (*nextobj).findOID(oid);
		};//end else
}

void linked_list::null_elem(void){
	element=NULL;
}



