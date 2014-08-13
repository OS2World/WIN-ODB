#include "..\source\parser.h"
#include "..\source\store.h"
#include <string.h>

PropertyDef::PropertyDef(){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
}

PropertyDef::PropertyDef(char *t, char *n, int typ){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
   setName(t);
   setPropName(n);
   setDataType(typ);
}

PropertyDef::PropertyDef(char *t, char *n){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
   setName(t);
   setPropName(n);
   setDataType(_VARIABLE_);
}

PropertyDef::~PropertyDef(){}

char *PropertyDef::getName(void){
	return tp;
}

void PropertyDef::setName(char *t){
	strcpy(tp,t);
}

char *PropertyDef::getPropName(void){
	return nm;
}

void PropertyDef::setPropName(char *p){
	strcpy(nm,p);
}

int PropertyDef::getDataType(void){
	return datatype;
}

void PropertyDef::setDataType(int i){
	datatype=i;
}

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

template <class T> LinkedList * LinkedList<T>::getNextListElem(){
	return next;
}

template <class T> void LinkedList<T>::setNextListElem(LinkedList *ne){
	next=ne;
}

template <class T> T *LinkedList<T>::getElem(){
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


//-----------------------------------------------------------------------------
// variable stuff
//-----------------------------------------------------------------------------
variable::variable(void){
	memset(name,'\0',NAMELENGTH);
   tp=-1;
   typeref=NULL;
   intval=0;
   realval=0;
   setval=NULL;
   charval=NULL;
   objval=NULL;
}

variable::variable(char *tpnm, int t, ODB_TYPE r){
	if (tpnm!=NULL) strcpy(name,tpnm);
   else generateName();
   tp=t;
   typeref=r;
   intval=0;
   realval=0;
   setval=NULL;
   charval=NULL;
   objval=NULL;
}

void variable::generateName(void){

	sprintf(name,"_ODBVariable_%i_",variable::autoEnum);
   variable::autoEnum++;
}

variable::~variable(void){
}

char *variable::getName(void){
	return name;
}

int variable::getType(void){
	return tp;
}

ODB_TYPE variable::getTypeRef(void){
	return typeref;
}

ODB_INT variable::getIntval(void){
return intval;
}

ODB_REAL variable::getRealval(void){
	return realval;
}

ODB_SET	variable::getSetval(void){
	return setval;
}

ODB_CHAR	variable::getCharval(void){
	return charval;
}

ODB_REF	variable::getObjval(void){
	return objval;
}

void variable::setName(char *n){
	strcpy(name,n);
}

void variable::setType(int t){
	tp=t;
}

void variable::setTypeRef(ODB_TYPE t){
	typeref=t;
}

void variable::setIntval(ODB_INT i){
	intval=i;
}

void variable::setRealval(ODB_REAL r){
	realval=r;
}

void variable::setSetval(ODB_SET s){
	setval=s;
}

void variable::setCharval(ODB_CHAR c){
	charval=c;
}

void variable::setObjval(ODB_REF r){
	objval=r;
}

