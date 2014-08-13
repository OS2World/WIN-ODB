//----------------------------------------------------
//
//          Implementation of object
//
//----------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "..\source\store.h"

object::object(void){	//Class constructor
	//OID=GLOBAL_OID++;
	props = NULL;
	typeof=NULL;
}
object::object(type *tp, database *db){	//Class constructor
	OID=(*db).generate_oid();
	props = NULL;
	typeof=tp;
}

ODB_OID object::oid(void){
 return OID;
 }


 void object::display(outputbuffer &buf){
 char *bufpos;
	bufpos=buf.write_request(10);
	if (bufpos!=NULL){
		sprintf(bufpos,"[OID:%i(",OID);
		if (props!=NULL) (*props).display(buf);
		bufpos=buf.write_request(2);
		if (bufpos!=NULL) sprintf(bufpos,")]");
	}
 }

void object::displayoid(outputbuffer &buf){
char *bufpos=buf.write_request(10);
	if (bufpos!=NULL) sprintf(bufpos," [OID:%i]",OID);
 }

object *object::add_newproperty(int tp, char *nm){
database *db;
property_value *pv;
	db=(*typeof).getdb();
	pv=new property_value(tp, nm, db);
	(*this).putprop(pv);
	return this;
}

property_value *object::putprop(property_value *newprop){
	if (props==NULL) props=newprop;
	else {
		(*newprop).setnextto(props);
		props=newprop;
	};
	return newprop;
}
property_value *object::getpropertynamed(char *name){
property_value *pr;
char *sname;
database *odb;
	odb=(*typeof).getdb();
	sname=(*odb).getstring(name); //subst for stored
	pr=props;
	while((pr!=NULL)&&((*pr).getname()!=sname)){
		pr=(*pr).nextproperty();
		}; //end while
	return pr;
}

property_value *object::setproperty_value(char *name, ODB_INT val){
// must move ref to obj in allindexes
property_value *pr;
table *indx;
	pr=(*this).getpropertynamed(name);
	if ((pr!=NULL)&&((*pr).gettype()==_INT_)) { //property ok and type ok
		indx=(*typeof).getindexnamed(name);
		(*pr).setintval(val);
		if ((indx!=NULL)&&(strcmp(name,_OID_)!=0)){ //index exist over the propery
			(*indx).remove(this);
			(*indx).put(this,name);
			};//end if index
		};
	return pr;
}

property_value *object::setproperty_value(char *name,ODB_CHAR val){
property_value *pr;
table *indx;
database *odb;
	odb=(*typeof).getdb();
	pr=(*this).getpropertynamed(name);
	if ((pr!=NULL)&&((*pr).gettype()==_CHAR_)) {
		indx=(*typeof).getindexnamed(name);
		(*pr).setcharstringval(val,odb);
		if (indx!=NULL){ //index exist over the propery
			(*indx).remove(this);
			(*indx).put(this,name);
			};//end if index
		};	return pr;
}

property_value *object::setproperty_value(char *name,ODB_REAL val){
// must move ref to obj in allindexes
property_value *pr;
table *indx;
	pr=(*this).getpropertynamed(name);
	if ((pr!=NULL)&&((*pr).gettype()==_REAL_)) { //property ok and type ok
		indx=(*typeof).getindexnamed(name);
		(*pr).setrealval(val);
		if (indx!=NULL){ //index exist over the propery
			(*indx).remove(this);
			(*indx).put(this,name);
			};//end if index
		};
	return pr;
}

property_value *object::setproperty_value(char *name,ODB_REF refobj){
property_value *pr;
table *indx;
	pr=(*this).getpropertynamed(name);
	if ((pr!=NULL)&&((*pr).gettype()==_OBJECT_)) {
		indx=(*typeof).getindexnamed(name);
		(*pr).setelem(refobj);
		if (indx!=NULL){ //index exist over the propery
			(*indx).remove(this);
			(*indx).put(this,name);
			};//end if index
		};	return pr;
}

property_value *object::setproperty_value(char *name,ODB_SET sv){
property_value *pr;
table *indx;
	pr=(*this).getpropertynamed(name);
	if ((pr!=NULL)&&(((*pr).gettype()==_COLLECTION_)||
										((*pr).gettype()==_INT_COLLECTION_)||
										((*pr).gettype()==_REAL_COLLECTION_)||
										((*pr).gettype()==_CHAR_COLLECTION_))) {
		indx=(*typeof).getindexnamed(name);
		(*pr).setsetval(sv);
		if (indx!=NULL){ //index exist over the propery
			(*indx).remove(this);
			(*indx).put(this,name);
			};//end if index
		};	return pr;
}


int object::getoid(void){
	return (*this).OID;
}

object::~object(void){
property_value *curr, *prev;
	curr=props;
	while (curr!=NULL){
		prev=curr;
		curr=(*curr).nextproperty();
		(*prev).~property_value(); //remove property value
		}
	}

void object::deleteobject(void){
property_value *curr, *prev;
	curr=props;
	while (curr!=NULL){
		prev=curr;
		curr=(*curr).nextproperty();
		(*prev).~property_value(); //remove property value
		};
	(*typeof).deleteobject(this); //let the owner be responsible for the
				      //actual deletion, any indexes must be cleaned
	}

int object::delete_instanceproperty(char *propnm){
//This function MUST be called with propnm subst. for a storedstring
//otherwise it wont work since it does a pointer equal to compare property
//names. This is for performance purposes.
property_value *pv, *prevp;
	pv=props;
	if ((*pv).getname()==propnm){  //if first prop is to be deleted.
		props=(*pv).nextproperty();
		(*pv).~property_value(); //delete it from the object;
		return 1;    //ok to quit. Only one prop with same name
		}
	else {
		prevp=pv;
		pv=(*pv).nextproperty();
		while (pv!=NULL){
			if((*pv).getname()==propnm){
				(*prevp).changenextto((*pv).nextproperty());
				(*pv).~property_value();
				return 1;
				}
			else {
				prevp=pv;
				pv=(*pv).nextproperty();
				}
			}
		return 0;
	}
}

//--------------------------------------------------
//------ Database file I/O .------------------------
//--------------------------------------------------

int object::saveobject(ofstream *databasefile, int how){
property_value *ptr;
int dattype;
ODB_OID oid;
object *obj;
char *propnm,*coid;
ODB_SET s;
	coid=(*(*typeof).getdb()).getstring(_OID_); //get _OID_ string
	ptr=props;
	(*databasefile)<<_Objstart_<<" ";
	(*databasefile)<<OID<<" ";
	while((how==WITH_PROPS)&&(ptr!=NULL)){ //save all properties and all values
		propnm=(*ptr).getname();
		if (propnm==coid) ptr=(*ptr).nextproperty(); //dont save OID
		else {
			dattype=(*ptr).gettype();
			if((dattype==_COLLECTION_)||(dattype==_INT_COLLECTION_)||
					(dattype==_CHAR_COLLECTION_)||(dattype==_REAL_COLLECTION_))
					dattype=_COLLECTION_;
			(*ptr).saveprop(databasefile); //saves name and type
			switch(dattype){
			case _OBJECT_:
				obj=(*ptr).getelem();
				if (obj!=NULL){
					oid=(*obj).oid();
					(*databasefile)<<" "<<oid<<" ";
					}
				else (*databasefile)<<0<<" ";
				break;
			case _COLLECTION_:
				s=(*ptr).getsetval();
				if (s!=NULL) (*s).saveset(databasefile);
				else (*databasefile)<<(*ptr).gettype()<<" { }\n";
				break;
			case _INT_:
				(*databasefile)<<(*ptr).getintval()<<" ";
				break;
			case _REAL_:
				(*databasefile)<<(*ptr).getrealval()<<" ";
				break;
			case _CHAR_:
				(*databasefile)<<'\n';
				(*databasefile)<<(*ptr).getcharstringval()<<"\n";
				break;
			}; //end dwitch
			ptr=(*ptr).nextproperty();
			} //end else
		}; //end while
	(*databasefile)<<_Objend_<<"\n";
	return 1;
}
