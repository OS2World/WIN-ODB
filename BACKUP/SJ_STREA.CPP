//----------------------------------------------------
//
//          Implementation of semi_join_stream
//
//----------------------------------------------------

//A semi join is a join where the objects in the left operand
//stream constitutes the result.

semi_join_stream::semi_join_stream(void):query_stream() {
	currobj=NULL;
	prop=NULL;
	rprop=NULL;
	intval=0;
	realval=0;
	charstringval=NULL;
	objval=NULL;
	lops=NULL;
	rops=NULL;
}

semi_join_stream::semi_join_stream(query_stream *lop,char *lnm,query_stream *rop,char *rnm):query_stream() {
database *odb;
char *nlnm,*nrnm;
int t1,t2;
	ctp=(*lop).get_currtype();
	odb=(*ctp).getdb();
	nlnm=(*odb).getstring(lnm);
	nrnm=(*odb).getstring(rnm);
	(*this).set_prop(nlnm);
	(*this).set_rprop(nrnm);
	lops=lop;
	rops=rop;
	t1=(*(*(*lop).get_currtype()).getpropertynamed(nlnm)).gettype();
	t2=(*(*(*rop).get_currtype()).getpropertynamed(nrnm)).gettype();
	if (t1==t2) datatype=t1;
	else datatype=_OTHER_;
}

void semi_join_stream::close(void){
	(*lops).close();
	(*rops).close();
	(*this).~semi_join_stream();
}

int semi_join_stream::reset(void){
	(*lops).reset();
	(*rops).reset();
	return 1;
}

//----------- Constructors -------------------------
int semi_join_stream::set_rprop(char *p){
int i;
database *odb;
	odb=(*ctp).getdb();
	rprop=(*odb).getstring(p);
	return 1;
}
//-------- selectors -------------------------------
char *semi_join_stream::get_rprop(void){
	return rprop;
}
//-------- Query primitives ------------------------

object *semi_join_stream::get(void){
//Dispatch the correct operation depending on the type
//of the properties
	switch(datatype){
		case _INT_:return (*this).getint();
		case _REAL_:return (*this).getreal();
		case _CHAR_:return (*this).getcharstring();
		case _OTHER_:return (*this).getobj();
		};
	return NULL;
}

object *semi_join_stream::getint(void){
object *lo,*ro;
property_value *lpv,*rpv;
	lo=(*lops).get();
	if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
	(*this).set_currobj(lo);
	ro=(*rops).get();
	if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	else {
              (*rops).reset();
	      ro=(*rops).get();
	      if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	}	while((lo!=NULL)&&((*rpv).getintval()!=(*lpv).getintval())) {
		while((ro!=NULL)&&((*rpv).getintval()!=(*lpv).getintval())){
			ro=(*rops).get();
			if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		}; //end inner while
	if (ro==NULL){ //reset rstream and try nex from lstream
		(*rops).reset();
		ro=(*rops).get();
		if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		lo=(*lops).get();
		if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
		(*this).set_currobj(lo);
		};
	}; //end outer while
	return lo;
}

object *semi_join_stream::getreal(void){
object *lo,*ro;
property_value *lpv,*rpv;
	lo=(*lops).get();
	if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
	(*this).set_currobj(lo);
	ro=(*rops).get();
	if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	else {
              (*rops).reset();
	      ro=(*rops).get();
	      if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	}
	while((lo!=NULL)&&((*rpv).getrealval()!=(*lpv).getrealval())) {
		while((ro!=NULL)&&((*rpv).getrealval()!=(*lpv).getrealval())){
			ro=(*rops).get();
			if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		}; //end inner while
	if (ro==NULL){ //reset rstream and try nex from lstream
		(*rops).reset();
		ro=(*rops).get();
		if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		lo=(*lops).get();
		if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
		(*this).set_currobj(lo);
		};	}; //end outer while
	return lo;
}

object *semi_join_stream::getcharstring(void){
object *lo,*ro;
property_value *lpv,*rpv;
	lo=(*lops).get();
	if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
	(*this).set_currobj(lo);
	ro=(*rops).get();
	if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	else {
              (*rops).reset();
	      ro=(*rops).get();
	      if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	}
	while((lo!=NULL)&&((*rpv).getcharstringval()!=(*lpv).getcharstringval())) {
		while((ro!=NULL)&&((*rpv).getcharstringval()!=(*lpv).getcharstringval())){
			ro=(*rops).get();
			if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		}; //end inner while
	if (ro==NULL){ //reset rstream and try nex from lstream
		(*rops).reset();
		ro=(*rops).get();
		if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		lo=(*lops).get();
		if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
		(*this).set_currobj(lo);
		};	}; //end outer while
	return lo;
}

object *semi_join_stream::getobj(void){
object *lo,*ro;
property_value *lpv,*rpv;
	if((*this).get_rprop()==NULL) return (*this).getobjspecial();
	lo=(*lops).get();
	if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
	(*this).set_currobj(lo);
	ro=(*rops).get();
	if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	else {
              (*rops).reset();
	      ro=(*rops).get();
	      if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
	}
	while((lo!=NULL)&&((*rpv).getelem()!=(*lpv).getelem())) {
		while((ro!=NULL)&&((*rpv).getelem()!=(*lpv).getelem())){
			ro=(*rops).get();
			if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		}; //end inner while
	if (ro==NULL){ //reset rstream and try nex from lstream
		(*rops).reset();
		ro=(*rops).get();
		if (ro!=NULL) rpv=(*ro).getpropertynamed((*this).get_rprop());
		lo=(*lops).get();
		if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
		(*this).set_currobj(lo);
		};	}; //end outer while
	return lo;
}

object *semi_join_stream::getobjspecial(void){
//This semi-join method compares the specified left operand attribute
//with the right operand object, as opposed to getobj whivh compares
//atribute values of the objects of both operands
object *lo,*ro;
property_value *lpv;
	lo=(*lops).get();
	if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
	(*this).set_currobj(lo);
	ro=(*rops).get();
	if (ro==NULL) {
              (*rops).reset();
	      ro=(*rops).get();
	}
	while((lo!=NULL)&&(ro!=(*lpv).getelem())) {
		while((ro!=NULL)&&(ro!=(*lpv).getelem())){
			ro=(*rops).get();
		}; //end inner while
	if (ro==NULL){ //reset rstream and try nex from lstream
		(*rops).reset();
		ro=(*rops).get();
		if (lo!=NULL) lpv=(*lo).getpropertynamed((*this).get_prop());
		(*this).set_currobj(lo);
		};	}; //end outer while
	return lo;
}
