//----------------------------------------------------
//
//          Implementation of query_stream
//
//----------------------------------------------------
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "..\source\store.h"

query_stream::query_stream(void){
	bucket=0;
	scantype=0;
	lst=NULL;
	ctp=NULL;
	currobj=NULL;
	prop=NULL;
	intval=0;
	realval=0;
	charstringval=NULL;
	objval=NULL;
	setval=NULL;
}

void query_stream::close(void){
	//free(prop);
	(*this).~query_stream();
}

int query_stream::reset(void){
table *ltbl;
	curre=exs; //reset extent pointer

	ltbl=(*ctp).getindexnamed(prop);
	tbl=ltbl;
	if (ltbl==NULL) { //no index exist
		ltbl=(*ctp).getextent();
		tbl=ltbl;
		(*tbl).openscan(_ALL_,this);
		}
	else
		switch(datatype) {
			case _INT_: (*ltbl).openscan((*ltbl).hashfn(intval),this);
				    break;
			case _REAL_:(*ltbl).openscan((*ltbl).hashfn(realval),this);
				    break;
			case _CHAR_:(*ltbl).openscan((*ltbl).hashfn(charstringval),this);
				    break;
			case _OBJECT_:(*ltbl).openscan((*ltbl).hashfn(objval),this);
				    break;
			case _COLLECTION_:(*ltbl).openscan((*ltbl).hashfn(setval),this);
				    break;
			case _INT_COLLECTION_:(*ltbl).openscan((*ltbl).hashfn(setval),this);
				    break;
			case _REAL_COLLECTION_:(*ltbl).openscan((*ltbl).hashfn(setval),this);
				    break;
			case _CHAR_COLLECTION_:(*ltbl).openscan((*ltbl).hashfn(setval),this);
				    break;

				    }
			return 1;
}


select_stream 	*query_stream::open_select_stream(ODB_NAME prop, ODB_NAME oop, ODB_CHAR charstringval)
{
	select_stream *ss;

	ss=new select_stream(this,prop,oop,charstringval);
	return ss;
}

select_stream *query_stream::open_select_stream(char *pr,char *oop,ODB_INT iv){
select_stream *ss;
	ss=new select_stream(this,pr,oop,iv);
	return ss;
}

select_stream *query_stream::open_select_stream(char *pr,char *oop,ODB_REAL rv){
select_stream *ss;
	ss=new select_stream(this,pr,oop,rv);
	return ss;
}

select_stream *query_stream::open_select_stream(char *pr,ODB_CHAR chstr){
select_stream *ss;
	ss=new select_stream(this,pr,chstr);
	return ss;
}

select_stream *query_stream::open_select_stream(char *pr,ODB_REF ov){
select_stream *ss;
	ss=new select_stream(this,pr,ov);
	return ss;
}


select_stream *query_stream::open_select_stream(char *pr,ODB_SET sv){
select_stream *ss;
	ss=new select_stream(this,pr,sv);
	return ss;
}

semi_join_stream *query_stream::open_sj_stream(char *prop1, query_stream *st2, char *prop2){
semi_join_stream *sjs;
	sjs=new semi_join_stream(this,prop1,st2,prop2);
	return sjs;
}

//----------- constructors ---------------------------
int query_stream::set_operator(char *oop){
	this->op=oop;
	return 1;
}
int query_stream::set_currbuck(int i){
	bucket=i;
	return i;
}

int query_stream::set_currlst(linked_list *l){
	lst=l;
	return 1;
}

int query_stream::set_scantype(int s){
	scantype=s;
	return 1;
}

int query_stream::set_currobj(object *o){
	currobj=o;
	return 1;
}

int query_stream::set_currtype(type *t){
	ctp=t;
	return 1;
}

int query_stream::set_table(table *t){
	tbl=t;
	return 1;
}

int query_stream::set_extentstart(typestp *es){
	exs=es;
	curre=es;
	return 1;
}

int query_stream::set_extentend(typestp *ee){
	exe=ee;
	return 1;
}

int query_stream::set_datatype(int i){
	datatype=i;
	return 1;
}

int query_stream::set_prop(char *p){
database *odb;
	if (p!=NULL){
		odb=(*ctp).getdb();
		prop=(*odb).getstring(p);
		}
	else prop=NULL;
	return 1;
}

int query_stream::set_intval(ODB_INT i){
	intval=i;
        return 1;
}

int query_stream::set_realval(ODB_REAL f){
	realval=f;
	return 1;
}

int query_stream::set_charstringval(ODB_CHAR cv){
database *odb;
	odb=(*ctp).getdb();
	charstringval=(*odb).getstring(cv);
	return 1;
}

int query_stream::set_objval(ODB_REF o){
	objval=o;
	return 1;
}

int query_stream::set_setval(ODB_SET s){
	setval=s;
	return 1;
}

//-------------retrievers-------------------------------
char *query_stream::get_operator(void){
	return op;
}

int query_stream::get_currbuck(void){
	return bucket;
}

linked_list *query_stream::get_currlst(void){
	return lst;
}

int query_stream::get_scantype(void){
	return scantype;
}

object *query_stream::get_currobj(void){
	return currobj;
}

type *query_stream::get_currtype(void){
	return ctp;
}

table *query_stream::get_table(void){
	return tbl;
}

char *query_stream::get_prop(void){
	return prop;
}

ODB_INT query_stream::get_intval(void){
	return intval;
}

ODB_REAL query_stream::get_realval(void){
	return realval;
}

ODB_CHAR query_stream::get_charstringval(void){
	return charstringval;
}

ODB_REF query_stream::get_objval(void){
	return objval;
}

ODB_SET query_stream::get_setval(void){
	return setval;
}

int query_stream::get_datatype(void){
	return datatype;
}
//--------------query interface------------------------
object *query_stream::get(void){
object *o;
type *tptr;
	o=NULL;
	if (prop!=NULL)
		switch(datatype){
			case _INT_ : return (*this).getint(intval);
			case _REAL_: return (*this).getreal(realval);
			case _CHAR_: return (*this).getcharstring(charstringval);
			case _OBJECT_: return (*this).getobj(objval);
			case _COLLECTION_: return (*this).getset();
			case _INT_COLLECTION_: return (*this).getset();
			case _REAL_COLLECTION_: return (*this).getset();
			case _CHAR_COLLECTION_: return (*this).getset();
			case _SET_COLLECTION_: return (*this).getset();
			} //switch
	else {
	if (tbl!=NULL)
		o=(*tbl).getnext(this);
	while ((o==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		curre=curre->next; //move to next extent;
		tptr=curre->tp;
		tbl=(*tptr).getextent();//table to scan
		(*tbl).openscan(_ALL_,this);
		o=(*tbl).getnext(this);
		} //end if
	}; //end else
	return o;
}

void query_stream::move_to_next_extent(type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getextent();
	(*tbl).openscan(_ALL_,this);

}
void query_stream::move_to_next_extent(ODB_INT iv,type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getindexnamed(prop);
	if (tbl!=NULL){ //index exist
		(*tbl).openscan((*tbl).hashfn(iv),this);
		}
	else {
		tbl=(*tptr).getextent();
		(*tbl).openscan(_ALL_,this);
		}
}

void query_stream::move_to_next_extent(ODB_REAL rv,type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getindexnamed(prop);
	if (tbl!=NULL){ //index exist
		(*tbl).openscan((*tbl).hashfn(rv),this);
		}
	else {
		tbl=(*tptr).getextent();
		(*tbl).openscan(_ALL_,this);
		}
}

void query_stream::move_to_next_extent(ODB_CHAR cv,type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getindexnamed(prop);
	if (tbl!=NULL){ //index exist
		(*tbl).openscan((*tbl).hashfn(cv),this);
		}
	else {
		tbl=(*tptr).getextent();
		(*tbl).openscan(_ALL_,this);
		}
}

void query_stream::move_to_next_extent(ODB_REF ov,type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getindexnamed(prop);
	if (tbl!=NULL){ //index exist
		(*tbl).openscan((*tbl).hashfn(ov),this);
		}
	else {
		tbl=(*tptr).getextent();
		(*tbl).openscan(_ALL_,this);
		}
}


void query_stream::move_to_next_extent(ODB_SET sv,type *tptr){
	curre=curre->next; //move to next extent;
	tptr=curre->tp;
	tbl=(*tptr).getindexnamed(prop);
	if (tbl!=NULL){ //index exist
		(*tbl).openscan((*tbl).hashfn(sv),this);
		}
	else {
		tbl=(*tptr).getextent();
		(*tbl).openscan(_ALL_,this);
		}
}


int query_stream::evalsetexpr(ODB_SET sv){
int stype,retval;
	if (sv==NULL) return -1;
	stype=(*sv).gettype();
	switch(stype){
	case _INT_:retval=eval(sv,intval);
		break;
	case _REAL_:retval=eval(sv,realval);
		break;	
	case _CHAR_:retval=eval(sv,charstringval);
		break;		
	case _OBJECT_:retval=eval(sv,objval);
		break;	
	case _COLLECTION_:retval=eval(sv,setval);
		break;
	case _INT_COLLECTION_:retval=eval(sv,setval);
		break;
	case _REAL_COLLECTION_:retval=eval(sv,setval);
		break;
	case _CHAR_COLLECTION_:retval=eval(sv,setval);
		break;	
	default: retval=-1;
	}
return retval;
}

int query_stream::eval(ODB_SET sv,ODB_INT cmpval){
	return (*sv).member(cmpval);
}

int query_stream::eval(ODB_SET sv,ODB_REAL cmpval){
	return (*sv).member(cmpval);
}

int query_stream::eval(ODB_SET sv,ODB_CHAR cmpval){
	return (*sv).member(cmpval);
}
int query_stream::eval(ODB_SET sv,ODB_REF cmpval){
	return (*sv).member(cmpval);
}

int query_stream::eval(ODB_SET sv,ODB_SET cmpval){
	return (*sv).equal(cmpval);
}


int query_stream::eval_op(char *oop,ODB_CHAR chval,ODB_CHAR cmpval){
int retval;
	if (strcmp(oop,"=")==0) retval=(chval==cmpval);
	else if (strcmp(oop,"substr")==0)  retval=subString(chval,cmpval);
	else retval=(chval==cmpval);
	return retval;
}

int query_stream::eval_op(char *oop,ODB_INT obval,ODB_INT cmpval){
int retval;
	if (strcmp(oop,"=")==0) retval=(obval==cmpval);
	else if (strcmp(oop,"<")==0)  retval=(obval<cmpval);
	else if (strcmp(oop,"<=")==0)   retval=(obval<=cmpval);
	else if (strcmp(oop,">")==0)   retval=(obval>cmpval);
	else if (strcmp(oop,">=")==0)   retval=(obval>=cmpval);
	else if (strcmp(oop,"!=")==0)   retval=(obval!=cmpval);
	else retval=(obval==cmpval);
	return retval;
}

int query_stream::eval_op(char *oop,ODB_REAL obval,ODB_REAL cmpval){
int retval;
	if (strcmp(oop,"=")==0) retval=(obval==cmpval);
	else if (strcmp(oop,"<")==0)  retval=(obval<cmpval);
	else if (strcmp(oop,"<=")==0)   retval=(obval<=cmpval);
	else if (strcmp(oop,">")==0)   retval=(obval>cmpval);
	else if (strcmp(oop,">=")==0)   retval=(obval>=cmpval);
	else if (strcmp(oop,"!=")==0)   retval=(obval!=cmpval);
	else retval=(obval==cmpval);
	return retval;
}


object *query_stream::getint(ODB_INT iv){
property_value *pv=NULL;
object *obj;
type *tptr=NULL;
int allscanned=0;
	if (tbl==NULL) obj=NULL;
	else obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	else //move to next extent;
	while((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		(*this).move_to_next_extent(iv,tptr);
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	};  //end while ((obj==NULL...
	//while((obj!=NULL)&&((*pv).getintval()!=iv)){
	//while((allscanned==0)&&(pv!=NULL)&&((*pv).getintval()!=iv)){
	while((allscanned==0)&&(pv!=NULL)&&(eval_op((*this).get_operator(),(*pv).getintval(),iv)==0)){
		obj=(*tbl).getnextobj(this);
		if ((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
			(*this).move_to_next_extent(iv,tptr);
			obj=(*tbl).getnextobj(this);
		      }  //end if ((obj==NULL...
		if ((obj==NULL)&&(curre!=NULL)&&(curre==exe)) allscanned=1;
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		}   //end while
	return obj;
}

object *query_stream::getreal(ODB_REAL rv){
property_value *pv;
object *obj;
type *tptr=NULL;

	obj=(*tbl).getnextobj(this);
	if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	else //move to next extent;
	while((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		(*this).move_to_next_extent(rv,tptr);
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	};  //end while ((obj==NULL...
	while((obj!=NULL)&&(eval_op((*this).get_operator(),(*pv).getrealval(),rv)==0)){
		obj=(*tbl).getnextobj(this);
		if ((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
			(*this).move_to_next_extent(rv,tptr);
			obj=(*tbl).getnextobj(this);
		      };  //end if ((obj==NULL...
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		  };   //end while
	return obj;
}

object *query_stream::getcharstring(ODB_CHAR chr){
property_value *pv;
object *obj;
type *tptr=NULL;
	obj=(*tbl).getnextobj(this);
	if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	else //move to next extent;
	while((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		(*this).move_to_next_extent(charstringval,tptr);
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	};  //end while ((obj==NULL...
	while((obj!=NULL)&&(eval_op((*this).get_operator(),(*pv).getcharstringval(),charstringval)==0)){
		obj=(*tbl).getnextobj(this);
		if ((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
			(*this).move_to_next_extent(charstringval,tptr);
			obj=(*tbl).getnextobj(this);
		      };  //end if ((obj==NULL...
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		  };   //end while
	return obj;
}

object *query_stream::getobj(ODB_REF ov){
property_value *pv;
object *obj;
type *tptr=NULL;

	obj=(*tbl).getnextobj(this);
	if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	else //move to next extent;
	while((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		(*this).move_to_next_extent(ov,tptr);
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	};  //end while ((obj==NULL...
	while((obj!=NULL)&&((*pv).getelem()!=ov)){
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		else   //move to next extent;
		      if ((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
			(*this).move_to_next_extent(charstringval,tptr);
			obj=(*tbl).getnextobj(this);
			if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		      };  //end if ((obj==NULL...
		  };   //end while
	return obj;
}

object *query_stream::getset(){
property_value *pv;
object *obj;
type *tptr=NULL;
	obj=(*tbl).getnextobj(this);
	if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	else //move to next extent;
	while((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
		(*this).move_to_next_extent(tptr);
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
	};  //end while ((obj==NULL...
	while((obj!=NULL)&&((*this).evalsetexpr((*pv).getsetval())<1)){
		obj=(*tbl).getnextobj(this);
		if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		else   //move to next extent;
		      if ((obj==NULL)&&(curre!=NULL)&&(curre!=exe)) {
			(*this).move_to_next_extent(charstringval,tptr);
			obj=(*tbl).getnextobj(this);
			if (obj!=NULL) pv=(*obj).getpropertynamed(prop);
		      };  //end if ((obj==NULL...
		  };   //end while
	return obj;
}


//-----Misc.--------
void query_stream::display(outputbuffer &buf){
char *bufpos=buf.write_request(100);
	sprintf(bufpos,"\nQuery_stream status\nBucket  :%i\nScantype: %i\nList pos: ",
		bucket,scantype);
	if (lst==NULL) sprintf(bufpos,"NULL\n");
	else  sprintf(bufpos,"not NULL\n");
}



//
// data operators
//

//-----------------------------------------------------------------------------
// DESCRIPTION: This method returns 1 if second arg is a substring of
//              first argument.
//-----------------------------------------------------------------------------
int query_stream::subString(ODB_CHAR mains, ODB_CHAR subs)
{
	char *i1=NULL, *i2=NULL, *it=NULL;
   int found=0;

   i1=mains; i2=subs;

   //
   // As long as the main string is not processed and we've not found
   // the substring keep looking
   //
   while ((i1!=NULL)&&(found==0)&&(*i1!='\0'))
   {
   	//
      // Keep a reference to where we started to compare for
      // reset if not found
      //
   	it=i1;

      //
      // As long as the substring have not been compared
      // from current position in main string keep looking
      //
      while ((i2!=NULL)&&(found==0))
      {
      	//
         // If the two strings have a common character
         // enumerate the positions and if that enumeratons
         // results in the end of substring we've got it!
         // if the two positions dont match quit
         //
      	if ((i2!=NULL)&&(i1!=NULL)&&(toupper(*i2)==toupper(*i1)))
         {
         	i2++; i1++;
            if (*i2=='\0') found=1;
            if (*i1=='\0') { i1=NULL; i2=NULL; }
         }
         else i2=NULL;
      }

      //
      // If we did not find it enumerate main string and
      // reset other string
      //
      if (found==0)
      {
      	i2=subs;
         i1=++it;
      }

   }

   return found;

}
