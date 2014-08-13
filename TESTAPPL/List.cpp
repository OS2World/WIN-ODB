#include "..\testappl\List.h"

template <class T> List<T>::List(void){
	next=NULL;
   prev=NULL;
   hasElem=0;
   index=0;
}


template <class T> List<T>::List(const List &l){

	element=l.getElement();
   prev=l.getPrev();
   index=l.getIndex();
   if (l.getNext!=NULL){
   	next=new List<T>;
      *next=*(l.getNext());
   }
   else next=NULL;
}

template <class T> List<T>::List(T &e){
	next=NULL;
   prev=NULL;
   elem=e;
   hasElem=1;
   index=0;
}

template <class T> List<T>::~List(void){

	if (next!=NULL) delete *next;

}

template <class T> void List<T>::setNext(List *l){

	next=l;

}

template <class T> void List<T>::setPrev(List *l){

	prev=l;

}


template <class T> void List<T>::insertElement(T &e){
List<T> *l=NULL;

	if (!hasElem){
   	elem=e;
      hasElem=1;
   }
   else {
   	l=new List(e);
		l->setNext(next);
   	if (next!=NULL) next->setPrev(l);
      l->setIndex(index+1);
   	l->setPrev(this);
   	next=l;
   }

}


template <class T> void List<T>::setIndex(int i){

	if (i!=index) {
   	index=i;
      if (next!=NULL) next->setIndex(index+1);
   }
}


template <class T> void List<T>::deleteElement(void){

	if (next!=NULL) next->setPrev(prev);

   if (prev!=NULL) prev->setNext(next);

   next->setindex(index);

   next=NULL;
   prev=NULL;

   delete *this;

}

template <class T> T List<T>::getElement(void){

	return element;

}


