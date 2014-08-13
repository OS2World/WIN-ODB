#include "parser.h"
#include "store.h"
#include <string.h>

PropertyDef::PropertyDef(){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
   setval=NULL;
}

PropertyDef::PropertyDef(char *t, char *n, int typ){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
   setName(t);
   setPropName(n);
   setDataType(typ);
   setval=NULL;
}

PropertyDef::PropertyDef(char *t, char *n){
	memset(tp,'\0',NAMELENGTH);
   memset(nm,'\0',NAMELENGTH);
   setName(t);
   setPropName(n);
   setDataType(_VARIABLE_);
   setval=NULL;
}

PropertyDef::~PropertyDef(){

	if (setval!=NULL) delete setval;

}

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

int PropertyDef::setsetval(ODB_SET sv){

	if((datatype==_COLLECTION_)||(datatype==_INT_COLLECTION_)||
		 (datatype==_CHAR_COLLECTION_)||(datatype==_REAL_COLLECTION_))
	{
		setval=sv;
		return 1;
	}
	else return 0;
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
   //variable::autoEnum=0;
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
   //variable::autoEnum=0;
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

