#ifndef _LINKEDLISTTMPLATE_
#define _LINKEDLISTTMPLATE_
#include "parser.h"
#include <string.h>



//-----------------------------------------------------------------------
// generic parse tree stuff
//-----------------------------------------------------------------------

template <class T> LinkedList<T>::LinkedList(){
	next=NULL;
   element=NULL;
}

template <class T> LinkedList<T>::LinkedList(T *elem){
	next=NULL;
   element=elem;
}

template <class T> LinkedList<T>::~LinkedList(){
	if (next!=NULL) delete next;
   delete element;
}

template <class T> LinkedList<T> *LinkedList<T>::getNextListElem(void){
	return next;
}

template <class T> void LinkedList<T>::setNextListElem(LinkedList *ne){
	next=ne;
}

template <class T> T *LinkedList<T>::getElem(void){
	return element;
}

template <class T> int LinkedList<T>::insertElem(T *elem){
LinkedList *ptr, *ne;
	ptr=this;
   	while(ptr->getNextListElem()!=NULL){
      	ptr=ptr->getNextListElem();
      }

      ne=new LinkedList(elem);
      ptr->setNextListElem(ne);
      return 1;
}


template <class T> T *LinkedList<T>::getElemNamed(char *nm){
	if (element==NULL) return NULL;
	if (strcmp(element->getName(),nm)==0) return element;
   if (next!=NULL) return next->getElemNamed(nm);
   else return NULL;
}

#endif