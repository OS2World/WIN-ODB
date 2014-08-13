//----------------------------------------------------
//
//          Implementation of property value
//
//----------------------------------------------------
#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include "..\source\store.h"

int property_value::gettype(void){
	return (*this).valuetype;
}

ODB_INT property_value::getintval(void){
	return intval;
}

ODB_CHAR property_value::getcharstringval(void){
	return charstring;
}

ODB_REAL property_value::getrealval(void){
	return realval;
}


ODB_REF property_value::getelem(void){
	return elem;
}

ODB_SET property_value::getsetval(void){
	return setval;
}

property_value::property_value(void)
{
	valuetype=0;
	name=NULL;
	nextprop=NULL;
	charstring=NULL;
	setval=NULL;
	intval=0;
	realval=0;
   elem=NULL;
}

property_value::property_value(int t, char *nm, database *db){
	valuetype=t;
	name=(*db).getstring(nm);
	nextprop=NULL;
	charstring=NULL;
	setval=NULL;
	intval=0;
	realval=0;
   elem=NULL;
}

int property_value::setintval(ODB_INT val){
	if (valuetype==_INT_){
		intval=val;
		return 1;
		}
	else return 0;
}

int property_value::setcharstringval(ODB_CHAR val, database *db){
	if (valuetype==_CHAR_){
		charstring=(*db).getstring(val);
		return 1;
		}
	else return 0;
}

int property_value::setrealval(ODB_REAL val){
	if (valuetype==_REAL_){
		realval=val;
		return 1;
		}
	else return 0;
}


int property_value::setelem(ODB_REF val){
	if(valuetype==_OBJECT_){
		elem=val;
		return 1;
		}
	else return 0;
}

int property_value::setsetval(ODB_SET sv){
	if((valuetype==_COLLECTION_)||(valuetype==_INT_COLLECTION_)||
		 (valuetype==_CHAR_COLLECTION_)||(valuetype==_REAL_COLLECTION_)){
		setval=sv;
		return 1;
		}
	else return 0;
}


int property_value::setnextto(property_value *next){
	if (nextprop==NULL){
		nextprop=next;
		return 1;
		};
	return 0;
}

int property_value::changenextto(property_value *next){
	nextprop=next;
	return 1;
}

property_value *property_value::copy_prop(database *db){
property_value *np;
	np=new property_value((*this).valuetype,(*this).name,db);
	if (nextprop!=NULL) (*np).setnextto((*nextprop).copy_prop(db));
	return np;
}

void property_value::display(outputbuffer &buf){
char *bufpos;
int len=0,vtp;
	if (strcmp(name,"OID")!=0){
	bufpos=buf.write_request(21);
	if (bufpos!=NULL) sprintf(bufpos,"<%s:",name);
	bufpos=buf.write_request(10);
	vtp=valuetype;
	if((valuetype==_COLLECTION_)||(valuetype==_INT_COLLECTION_)||
		 (valuetype==_CHAR_COLLECTION_)||(valuetype==_REAL_COLLECTION_))
		 vtp=_COLLECTION_;	if (bufpos!=NULL)
		switch (vtp) {
		case _INT_: sprintf(bufpos,"%i>",intval);
			break;
		case _REAL_:sprintf(bufpos,"%f>",realval);
			break;
		case _CHAR_:
			if (charstring!=NULL){
				len=strlen(charstring);
				bufpos=buf.write_request(len);
				if (bufpos!=NULL) sprintf(bufpos,"%s>",charstring);
				}
			else {
				bufpos=buf.write_request(1);
				if (bufpos!=NULL) sprintf(bufpos,">");
				}			break;
		case _OBJECT_:
			if (elem!=NULL) (*elem).displayoid(buf);//sprintf(bufpos,"%i",(*elem).oid());
			else sprintf(bufpos,"NULL");
			bufpos=buf.write_request(1);
			if (bufpos!=NULL) sprintf(bufpos,">");
			break;
		case _COLLECTION_:
			if (setval==NULL) sprintf(bufpos,"{}");
			else(*setval).display(buf);
			bufpos=buf.write_request(1);
			if (bufpos!=NULL) sprintf(bufpos,">");
			break;
		default:
			break;
		} //case
	} //if
	if (nextprop!=NULL) (*nextprop).display(buf);
}

char *property_value::getname(void){
	return name;
}

property_value *property_value::nextproperty(void){
	return nextprop;
}

int property_value::saveprop(ofstream *databasefile){
	(*databasefile)<<name<<" "<<valuetype<<" ";
	return 1;
}

property_value::~property_value(void){
	}
