//----------------------------------------------------
//
//          Implementation of select_stream
//
//----------------------------------------------------

#include "..\source\store.h"

select_stream::select_stream(void):query_stream() {
	currobj=NULL;
	prop=NULL;
	intval=0;
	realval=0;
	charstringval=NULL;
	objval=NULL;
	qs=NULL;
	setval=NULL;
}

select_stream::select_stream(query_stream *q,char *nm,char *oop,ODB_INT i):query_stream() {
property_value *pv;
int dtp;
	ctp=(*q).get_currtype();
	pv=(*ctp).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*this).set_datatype(dtp);
	(*this).set_prop(nm);
	(*this).set_operator(oop);
	qs=q;
	intval=i;
	//datatype=_INT_;
}

select_stream::select_stream(query_stream *q,char *nm,char *oop,ODB_REAL r):query_stream(){
property_value *pv;
int dtp;
	ctp=(*q).get_currtype();
	pv=(*ctp).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*this).set_datatype(dtp);
	(*this).set_prop(nm);
	(*this).set_operator(oop);
	qs=q;
	realval=r;
	//datatype=_REAL_;
}

select_stream::select_stream(query_stream *q,char *nm,ODB_CHAR chrstr):query_stream()  {
property_value *pv;
int dtp;
	ctp=(*q).get_currtype();
	pv=(*ctp).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*this).set_datatype(dtp);
	(*this).set_prop(nm);
	(*this).set_charstringval(chrstr);
	qs=q;
	//datatype=_CHAR_;
}

select_stream::select_stream(query_stream *q,char *nm,ODB_REF ob):query_stream()  {
property_value *pv;
int dtp;
	ctp=(*q).get_currtype();
	pv=(*ctp).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*this).set_datatype(dtp);
	(*this).set_prop(nm);
	qs=q;
	objval=ob;
	//datatype=_OTHER_;
}


select_stream::select_stream(query_stream *q,char *nm,ODB_SET sv):query_stream()  {
property_value *pv;
int dtp;
	ctp=(*q).get_currtype();
	pv=(*ctp).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*this).set_datatype(dtp);
	(*this).set_prop(nm);
	qs=q;
	setval=sv;
	//datatype=_COLLECTION_;
}

object *select_stream::get(void){
	switch(datatype){
		case _INT_:return (*this).getint();
		case _REAL_:return (*this).getreal();
		case _CHAR_:return (*this).getcharstring();
		case _OBJECT_:return (*this).getobj();
		case _COLLECTION_:return (*this).getset();
		case _INT_COLLECTION_: return (*this).getset();
		case _REAL_COLLECTION_: return (*this).getset();
		case _CHAR_COLLECTION_: return (*this).getset();
		case _SET_COLLECTION_: return (*this).getset();
		};
	return NULL;
}

object *select_stream::getint(void){
object *o;
property_value *pv;
ODB_INT objintval;
	o=(*qs).get();
	if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	if (pv!=NULL) objintval=(*pv).getintval();
	//while((o!=NULL)&&((*pv).getintval()!=intval)){
	while((o!=NULL)&&(eval_op((*this).get_operator(),objintval,intval)==0)){
		o=(*qs).get();
		if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
		if (pv!=NULL) objintval=(*pv).getintval();
	};
	return o;
}

object *select_stream::getreal(void){
object *o;
property_value *pv;
	o=(*qs).get();
	if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	while((o!=NULL)&&(eval_op((*this).get_operator(),(*pv).getrealval(),realval)==0)){
		o=(*qs).get();
		if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	};
	return o;
}

object *select_stream::getcharstring(void){
object *o;
property_value *pv;
	o=(*qs).get();
	if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	while((o!=NULL)&&((*pv).getcharstringval()!=charstringval)){
		o=(*qs).get();
		if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	};
	return o;
}
object *select_stream::getobj(void){
object *o;
property_value *pv;
	o=(*qs).get();
	if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	while((o!=NULL)&&((*pv).getelem()!=objval)){
		o=(*qs).get();
		if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	};
	return o;
}

object *select_stream::getset(void){
object *o;
property_value *pv;
	o=(*qs).get();
	if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	while((o!=NULL)&&(evalsetexpr((*pv).getsetval())<1)){
		o=(*qs).get();
		if (o!=NULL) pv=(*o).getpropertynamed((*this).get_prop());
	};
	return o;
}
void select_stream::close(void){
	//free(&charstringval);
	//free(&prop);
	(*qs).close();
	(*this).~select_stream();
}

int select_stream::reset(void){
	(*qs).reset();
	return 1;
}
