#ifndef _LIST_H_
#define _LIST_H_

template <class T> class List {
	private:
   	T element;
      List *next;
      List *prev;
      int  hasElem;
      int index;
   public:
   	List(void);
      List(const List &l);
      List(T &e);
      ~List(void);
      List *getNext(){return next;};
      List *getPrev(void){return prev;};
      void setNext(List *l);
      void setPrev(List *l);
      void insertElement(T &e);
      void deleteElement(void);
      void setIndex(int i);
      int  getIndex(void){return index;};
      T getElement(void);
};

#endif
