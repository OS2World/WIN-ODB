//----------------------------------------------------
//
//          Implementation of type
//
//----------------------------------------------------
// Type creation is somewhat complicated since there exists an auxiliary
// linked list with structs. Each struct points to the type it belongs to
// By traversing the list from the top a partial order of the type
// hierarchy is obtained. This list is used to retrieve all types in the
// subtree to a particular type. The entire list is the extent of the type
// usertypes.
// Each type has a pointer ext_start which points to a struct which points
// back to the type. Each type has also a pointer ext_end which points to the
// stryct that points to the last type of the extent.
// When a type is created a struct which points to the type has to be linked
// in to the list. All supertypes has to be notified so they can ectend
// their extents by chanign their ext_end ptr if required.
// The functions addsubtype and extend extent takes care of this.

// first a few fns to handle the extent list
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "..\source\store.h"

typestp *makenewtypestp(typestp *curr,type *super){
typestp *newtypestp;
	newtypestp=(struct typestp *)malloc(sizeof(struct typestp));
	newtypestp->next=curr;
	newtypestp->tp=super;
	return newtypestp;
}


type::type(void){
	strncpy(name,"Usertypes",19);
	extent=NULL;
	index=NULL;
	supertypes=NULL;
	subtypes=NULL;
	OID=0;
	inherited_props=NULL;
	ext_start=makenewtypestp(NULL,this);
	ext_end=ext_start;
	typeof=this;
	curr_stp=NULL;
} //Constructor

type::type(char *nm, database *db):object(){   //constructor
	OID=(*db).generate_oid();
	belongsto=db;
	typeof=this;
	strncpy(name,nm,19);
	card=(*db).get_tablesize();
	extent=new table(card);
	index=NULL;
	subtypes=NULL;
	supertypes=makenewtypestp(NULL,(*db).get_usertypes());
	ext_start=makenewtypestp(NULL,this);
	ext_end=ext_start;
	(*(*db).get_usertypes()).addsubtype(this,ext_end);
	inherited_props=NULL;
	curr_stp=NULL;
	//subtype to usertypes;
}

type::type(char *nm,type *tp, database *db):object(){   //constructor
property_value *inh_props;
	OID=(*db).generate_oid();
	belongsto=db;
	typeof=this;
	strncpy(name,nm,19);
	card=(*db).get_tablesize();
	extent=new table(card);
	index=NULL;
	subtypes=NULL;
	supertypes=makenewtypestp(NULL,tp);
	ext_start=makenewtypestp(NULL,this);
	ext_end=ext_start;
	(*tp).addsubtype(this,ext_end);
	//subtype to usertypes;
	inh_props=(*tp).getprops();
	if (inh_props!=NULL)
		props=(*inh_props).copy_prop(db);
	else props=NULL;
	inherited_props=props; //inherit properties
	inh_props=props;
	//Any index defined in supertypes must be
	//inherited to this type
	while(inh_props!=NULL){
		if ((*tp).getindexnamed((*inh_props).getname())!=NULL)
			(*this).build_index((*inh_props).getname());
		inh_props=(*inh_props).nextproperty();
	}; //end while
	curr_stp=NULL;
}

void type::remove_subtype(type *t){
typestp *curr,*prev;
	curr=subtypes;
   
   if (curr==NULL) return;

	if (curr->tp==t) {
		subtypes=curr->next;
		free(curr);
		}
	else {
		while ((curr->tp!=t)&&(curr!=NULL)){
                	prev=curr;
			curr=curr->next;
			} //while
		if (curr->tp==t) {
			prev->next=curr->next;
			free(curr);
			}
		}
}
void type::delete_type(void){
typestp *runner;
indextp *ind;
type *stp;
table *tb;
type *utp;
property_value *pv,*prev;
	runner=subtypes;
	while (runner!=NULL) {  //delete all subtypes
		stp=runner->tp;
		(*stp).delete_type();
		runner=runner->next;
		};
	ind=index;
	while (ind!=NULL) {  //delete all indexes
		tb=ind->tbl;
		(*tb).drop_index();
		(*tb).~table();
		ind=ind->next;
		};
	(*extent).drop_table(); //delte extent and all objects;
	(*extent).~table();
	pv=props;
	while(pv!=NULL){  //delete all props
		prev=pv;
		pv=(*pv).nextproperty();
		(*prev).~property_value();
		};
	free(ext_start);
	(*supertypes->tp).remove_subtype(this);
	utp=(*belongsto).gettypenamed("Usertypes");
	(*utp).rebuild_extentlist();
	(*this).~type();
}

void type::rebuild_extentlist(){
//this method is called whenever a type has been deleted and the
//extent list has to be rebulilt.
typestp *stp;
	ext_end=ext_start; //ext_end set to safe plase
	ext_end->next=NULL;
	if (supertypes!=NULL) (*supertypes->tp).readdsubtype(ext_start);
	stp=subtypes;
	while (stp!=NULL) {
		(*stp->tp).rebuild_extentlist();
		stp=stp->next;
		}
}

int type::readdsubtype(typestp *extptr){
typestp *extp;
	extp=ext_end->next;    //remember rest of extent
	ext_end->next=extptr; //New type in extent
	extptr->next=extp;    //its linked in
	ext_end=extptr;
	if (supertypes!=NULL) //propagate upwards
		(*(supertypes->tp)).extend_extent(extptr);
	return 1;
}
int type::addsubtype(type *tp, typestp *extptr){
typestp *ntp;
typestp *extp;
	ntp=makenewtypestp((*this).subtypes,tp);
	this->subtypes=ntp;
	extp=ext_end->next;    //remember rest of extent
	ext_end->next=extptr; //New type in extent
	extptr->next=extp;    //its linked in
	ext_end=extptr;
	if (supertypes!=NULL) //propagate upwards
		(*(supertypes->tp)).extend_extent(extptr);
	return 1;
}

int type::extend_extent(typestp *extptr){
typestp *ptr;
	ptr=ext_start;
	while ((ptr!=NULL)&&(ptr!=ext_end)&&(ptr!=extptr))
		ptr=ptr->next;
	if (ptr==NULL) return 0;
	if (ptr==ext_end) {
		ext_end=extptr;
		 if ((supertypes!=NULL)&&(supertypes->tp!=NULL)) //propagate upwards
			(*(supertypes->tp)).extend_extent(extptr);		return 1;
		};
	return 1;
}

int type::set_belongsto(database *db){
	belongsto=db;
	return 1;
}


type *type::getsupertype(void){
	if (supertypes!=NULL) return supertypes->tp;
	else return NULL;
}


type *type::add_inheritedproperty(int tp, char *nm){
property_value *pv;
typestp *curr;
ODB_PROP propptr=NULL;

	pv=new property_value(tp, nm, belongsto);
   (*pv).setnextto(inherited_props);

   propptr=props;
   //--------------------------------------------------------
   // forward propptr until the inherited props are reached
   //--------------------------------------------------------
   while ((propptr!=NULL)&&(propptr!=inherited_props)&&(propptr->nextproperty()!=inherited_props)){
   	propptr=propptr->nextproperty();
   }

   //-------------------------------------------------------
   // there may not be any properties
   //-------------------------------------------------------
   if (props==NULL)
   	props=pv;

   //-------------------------------------------------------
   //the last if the non inh props is found, link in the
   //new inh prop as the first inh prop
   //-------------------------------------------------------
   if (propptr->nextproperty()==inherited_props)
   	propptr->changenextto(pv);

   //------------------------------------------------------
   // no non inh props, the new prop is already linked in as
   // the first, set props to this one
   //------------------------------------------------------
	if (propptr==inherited_props)
   	props=pv;

	inherited_props=pv;

	//Add new property to all subtypes as well
	curr=subtypes;
	while (curr!=NULL) {
		(*curr->tp).add_inheritedproperty(tp,nm);
		curr=curr->next;
		};
	return this;
}

type *type::add_property(int tp, char *nm){
property_value *pv;
typestp *curr;
query_stream *qst;
object *obj;
	pv=new property_value(tp, nm, belongsto);
	(*this).putprop(pv);
	//Add new property to all subtypes as well
	curr=subtypes;
	while (curr!=NULL) {
		(*curr->tp).add_inheritedproperty(tp,nm);
		curr=curr->next;
		};
	//add property to all instances
	qst=(*this).open();
	obj=(*qst).get();
	while (obj!=NULL){
		(*obj).add_newproperty(tp,nm);
		obj=(*qst).get();
	}; //end while
	return this;
}


object *type::create_instance(void){
object *no;
char *coidn;
	if (extent!=NULL){
		coidn=(*belongsto).getstring(_OID_);
		no=new object(this, belongsto);
		(*no).putprop((*props).copy_prop(belongsto));
		(*no).setproperty_value(coidn,(*no).oid());
		(*extent).put(no); //insert into extent
	}
	else no=NULL;
	return no;
}

indextp *type::makenewindex(char *nm){
indextp *newindex;
	newindex = (struct indextp *)malloc(sizeof(struct indextp));
	strncpy(newindex->name,nm,19);
	newindex->tbl=new table(card);
	newindex->next=NULL;
	if (index==NULL) index=newindex;
	 else {
		newindex->next=index;
		index=newindex;
		};
	 return index;
}

indextp *type::populateindex(char *nm,indextp *thenewindex){
object *obj;
query_stream qst;
	obj=(*extent).get(&qst);
	while (obj!=NULL) {
		(*thenewindex->tbl).put(obj,nm);//hasha p† prop val
		obj=(*extent).getnext(&qst);
		} //while
	return thenewindex;
}


int type::build_index(char *prop){
property_value *tmp;
table *tbl;
typestp *subtps;
	tmp=(*this).getpropertynamed(prop);
	tbl=(*this).getindexnamed(prop);
	if ((tmp!=NULL)&&(tbl==NULL)) {
		populateindex(prop,makenewindex(prop));
		subtps=subtypes;
		while(subtps!=NULL){ //index inh. to subtps
			(*subtps->tp).build_index(prop);
			subtps=subtps->next;
			};
		return 1;
		}
	else return 0;
}

int type::insertintoallindex(object *obj){
indextp *ind=index;
	while(ind!=NULL){
		(*ind->tbl).put(obj,(ind->name));
		ind=ind->next;
		}; //end while
	return 1;
}

int type::drop_index(char *nm){
char *cnm;
table *ind;
indextp *prev,*curr;
type *tp;
typestp *tpptr;
	cnm=(*belongsto).getstring(nm);
	ind=(*this).getindexnamed(cnm);
	if (ind!=NULL){ //do stuff to delete the index
		curr=index;
		if (curr->tbl==ind){ //the index is the first one
			(*ind).drop_index();
			index=curr->next;
			free(curr);
			}
		else{ //find it and delete it
		while(curr!=NULL){
			prev=curr;
			curr=curr->next;
			if (curr->tbl==ind){ //found it!
				prev->next=curr->next;
				free(curr);
				curr=NULL;
				(*ind).drop_index();
				};
			} //while
		} //end else
	} //end if(ind!=NULL)
	tpptr=subtypes; //drop from all subtypes as well
	while (tpptr!=NULL){
		tp=tpptr->tp;
		(*tp).drop_index(cnm);
		 tpptr=tpptr->next;
	} //while
	return 1;
}

//------------- object deletion ---------------------
int type::deleteobject(object *obj){
//any reference to this object in the database must be nulled
indextp *indexes;
table   *tblptr;
int result;
type *utp;
	indexes=index;
	while(indexes!=NULL){   //Remove from indexes
		tblptr=indexes->tbl;
		(*tblptr).remove(obj);
		indexes=indexes->next;
		};  //while
	result=(*extent).remove(obj);
	utp=(*belongsto).gettypenamed("Usertypes");
	if (utp!=NULL) (*utp).remove_all_references_to(obj);
	(*obj).~object(); //dispose the object
	return result;
}

int type::remove_all_references_to(object *o){
char *pnm;
property_value *pv,*ipv;
query_stream *qst;
object *dbo;
typestp *stp;
ODB_SET setv;
	pv=props;
	while (pv!=NULL){ //check all props
		pnm=(*pv).getname(); //get name to seee if non inh.
		ipv=(*this).get_noninherited_propertynamed(pnm);
		if ((ipv!=NULL)&&(((*ipv).gettype()==_COLLECTION_)||
											((*ipv).gettype()==_OBJECT_))){
			qst=(*belongsto).open_stream((*this).getname(),(*ipv).getname(),o);
			dbo=(*qst).get();
			while (dbo!=NULL){
				if ((*ipv).gettype()==_OBJECT_)
					(*dbo).setproperty_value((*ipv).getname(),(object *)NULL);
				else {
					setv=(*(*dbo).getpropertynamed((*ipv).getname())).getsetval();
					(*setv).null_reference_to(o);
					};
				dbo=(*qst).get();
				};
			}; //end if
		pv=(*pv).nextproperty();
		}; //end while
	stp=subtypes;
	while(stp!=NULL){
		(*stp->tp).remove_all_references_to(o);
		stp=stp->next;
		};
	return 1;
}



property_value *type::get_noninherited_propertynamed(char *nm){
property_value *pr,*ipr;
char *sname;
database *odb;
	odb=(*typeof).getdb();
	sname=(*odb).getstring(nm); //subst for stored
	pr=props;
	while((pr!=NULL)&&((*pr).getname()!=sname)){
		pr=(*pr).nextproperty();
		}; //end while
	ipr=inherited_props;
	while((ipr!=NULL)&&((*ipr).getname()!=sname)){
		ipr=(*ipr).nextproperty();
		}; //end while
	if (ipr!=NULL) pr=NULL; //the property was inherited
	return pr;
}
void type::delete_inherited_props(char *pnm){
typestp *stp;
char *cpnm;
type *tp;
property_value *pv, *prevp;

	cpnm=(*belongsto).getstring(pnm);
	(*this).delete_instanceproperty(cpnm);

	pv=inherited_props;
	if (pv!=NULL)
	if ((*pv).getname()==cpnm){  //if first prop is to be deleted.
		props=(*pv).nextproperty();
		(*pv).~property_value(); //delete it from the object;
		}
	else {
		prevp=pv;
		pv=(*pv).nextproperty();
		while (pv!=NULL){
			if((*pv).getname()==cpnm){
				(*prevp).changenextto((*pv).nextproperty());
				(*pv).~property_value();
				}
			else {
				prevp=pv;
				pv=(*pv).nextproperty();
				}
			} //while
	} //else
	stp=subtypes;
	while (stp!=NULL){
		tp=stp->tp;
		(*tp).delete_inherited_props(pnm);
		stp=stp->next;
		}
}

table *type::getindexnamed(char *nm){
indextp *tmp;
	tmp=index;
	if (strcmp(nm,_OID_)==0) return extent;
	while((tmp!=NULL)&&(strcmp(tmp->name,nm)!=0)){
		tmp=tmp->next;
		};
	if (tmp==NULL) return NULL;
	else return tmp->tbl;
}

table *type::getextent(void){
	return extent;
}

property_value *type::getprops(void){
	return props;
}

database *type::getdb(void){
	return belongsto;
}

void type::displayname(outputbuffer &buf){
char *bufpos=buf.write_request(20);
	sprintf(bufpos," %s ",(*this).name);
}

void type::display(outputbuffer &buf){
indextp *indexes;
typestp *ptr;
char *bufpos=buf.write_request(100);
	sprintf(bufpos,"\n\nType: %s\nOID:%i Name: %s\n",name,OID,name);
	bufpos=buf.write_request(20);
	if (props!=NULL) {
		sprintf(bufpos,"\nProperties: ");
		(*props).display(buf);
		}
	else sprintf(bufpos,"\nNo properties ");
	bufpos=buf.write_request(10);
	sprintf(bufpos,"\nIndexes: ");
	indexes=index;
	while (indexes!=NULL) {
		bufpos=buf.write_request(30);
		sprintf(bufpos," %s ",indexes->name);
		indexes=indexes->next;
	};//end while
	bufpos=buf.write_request(10);
	sprintf(bufpos,"\nSupertype: ");
	if (supertypes!=NULL) (*supertypes->tp).displayname(buf);
	bufpos=buf.write_request(10);
	sprintf(bufpos,"\nSubtypes: ");
	ptr=subtypes;
	while (ptr!=NULL){
		(*ptr->tp).displayname(buf);
		bufpos=buf.write_request(1);
		sprintf(bufpos," ");
		ptr=ptr->next;
	};
	};

void type::displayextent(outputbuffer &buf){
char *bufpos=buf.write_request(40);
	sprintf(bufpos,"Extent of type %s\n",name);
	(*extent).display(buf);
}



//------------------------------------------------
// Search facility submodule of type
//
// functions get and get next invarious variants
//------------------------------------------------

//--------- Integer valued props -----------------

//------------- Query stream interface --------------
query_stream *type::open(void){
query_stream *qst;
	qst=new query_stream();
	(*qst).set_currtype(this);
	(*qst).set_table(extent);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	(*qst).set_prop(NULL);
	if (extent!=NULL) (*extent).openscan(_ALL_,qst);
	return qst;
}

query_stream *type::open(char *nm,char *op,ODB_INT intval){
table *indx=NULL;
query_stream *qst;
char *cop;
property_value *pv;
int dtp;
	cop=(*belongsto).getstring(op);
	qst=new query_stream();
	(*qst).set_currtype(this);
	pv=(*this).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*qst).set_datatype(dtp);	//(*qst).set_datatype(_INT_);
	(*qst).set_prop(nm);
	(*qst).set_intval(intval);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	(*qst).set_operator(cop);
	if (strcmp(cop,"=")==0) indx=(*this).getindexnamed(nm); //only hashindex so far
	else indx=NULL;
	if (indx!=NULL){ //index exist
		(*qst).set_table(indx);
		(*indx).openscan((*indx).hashfn(intval),qst);
		}
	else {
		(*qst).set_table((*this).extent);
		if (extent!=NULL) (*extent).openscan(_ALL_,qst);
		};
	return qst;
}

query_stream *type::open(char *nm,char *op,ODB_REAL realval){
table *indx=NULL;
query_stream *qst;
char *cop;
property_value *pv;
int dtp;
	qst=new query_stream();
   cop=(*belongsto).getstring(op);
	(*qst).set_operator(cop);
	(*qst).set_currtype(this);
	pv=(*this).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*qst).set_datatype(dtp);	//(*qst).set_datatype(_REAL_);
	(*qst).set_prop(nm);
	(*qst).set_realval(realval);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	if (strcmp(cop,"=")==0) indx=(*this).getindexnamed(nm);
	else indx=NULL;
	if (indx!=NULL){ //index exist
		(*qst).set_table(indx);
		(*indx).openscan((*indx).hashfn(realval),qst);
		}
	else {
		(*qst).set_table((*this).extent);
		if (extent!=NULL) (*extent).openscan(_ALL_,qst);
		};
	return qst;
}

query_stream *type::open(char *nm,ODB_CHAR charstringval){
table *indx=NULL;
query_stream *qst;
property_value *pv;
int dtp;	qst=new query_stream();
	(*qst).set_currtype(this);
	pv=(*this).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*qst).set_datatype(dtp);	//(*qst).set_datatype(_CHAR_);
	(*qst).set_prop(nm);
	(*qst).set_charstringval(charstringval);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	indx=(*this).getindexnamed(nm);
	if (indx!=NULL){ //index exist
		(*qst).set_table(indx);
		(*indx).openscan((*indx).hashfn(charstringval),qst);
		}
	else {
		(*qst).set_table((*this).extent);
		if (extent!=NULL) (*extent).openscan(_ALL_,qst);
		};
	return qst;
}

query_stream *type::open(char *nm,ODB_REF objval){
table *indx=NULL;
query_stream *qst;
property_value *pv;
int dtp;
	qst=new query_stream();
	//pv=(*this).getpropertynamed(nm);
	//dtp=(*pv).gettype();
	(*qst).set_currtype(this);
	pv=(*this).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*qst).set_datatype(dtp);
	(*qst).set_prop(nm);
	(*qst).set_objval(objval);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	indx=(*this).getindexnamed(nm);
	if (indx!=NULL){ //index exist
		(*qst).set_table(indx);
		(*indx).openscan((*indx).hashfn(objval),qst);
		}
	else {
		(*qst).set_table((*this).extent);
		if (extent!=NULL)
			(*extent).openscan(_ALL_,qst);
		};
	return qst;
}

query_stream *type::open(char *nm,ODB_SET sv){
table *indx=NULL;
query_stream *qst;
property_value *pv;
int dtp;	qst=new query_stream();
	(*qst).set_currtype(this);
	pv=(*this).getpropertynamed(nm);
	dtp=(*pv).gettype();
	(*qst).set_datatype(dtp);	//(*qst).set_datatype(_COLLECTION_);
	(*qst).set_prop(nm);
	(*qst).set_setval(sv);
	(*qst).set_extentstart(ext_start);
	(*qst).set_extentend(ext_end);
	indx=(*this).getindexnamed(nm);
	if (indx!=NULL){ //index exist
		(*qst).set_table(indx);
		(*indx).openscan((*indx).hashfn(sv),qst);
		}
	else {
		(*qst).set_table((*this).extent);
		if (extent!=NULL)
			(*extent).openscan(_ALL_,qst);
		};
	return qst;
}

type *type::gettypenamed(char *nm){
typestp *subtp;
type *res;
	if (strncmp(nm,(*this).name,20)==0) res=this;
	else {
		subtp=(*this).subtypes;
		res=NULL;
		while ((subtp!=NULL)&&(res==NULL)) {
			res=(*subtp->tp).gettypenamed(nm);
			subtp=subtp->next;
			}; //end while
	} //end else
	return res;
}

//--------------------------------------------------
//------ Database file I/O .------------------------
//--------------------------------------------------

int type::savetypes(ofstream *databasefile){
property_value *proptr;
indextp *indexptr;
typestp *typeptr;
char *propnm,*coid;
	coid=(*belongsto).getstring(_OID_); //get normalized string
	(*databasefile)<<_Typestart_<<" "<<name<<" ";
	if (supertypes!=NULL){
		(*databasefile)<<_Subtypestart_<<" ";
		(*databasefile)<<(*supertypes->tp).getname();
		(*databasefile)<<" ";
	};
	(*databasefile)<<_Propstart_<<" ";
	proptr=props;
	// The system generated OID property is not saved
	while((proptr!=NULL)&&((*this).inherited_props!=proptr)){ //save properties
		 propnm=(*proptr).getname();
		 if (propnm==coid) proptr=(*proptr).nextproperty();
		 else {
			(*proptr).saveprop(databasefile);
			proptr=(*proptr).nextproperty();
			} //end else
		}; //end while
	(*databasefile)<<_Propend_<<" "<<_Indexstart_<<" ";
	indexptr=index;
	while(indexptr!=NULL){
		(*databasefile)<<indexptr->name<<" ";
		indexptr=indexptr->next;
		};
	(*databasefile)<<_Indexend_<<"\n";
	typeptr=subtypes;
	while(typeptr!=NULL){
		(*(typeptr->tp)).savetypes(databasefile);
		typeptr=typeptr->next;
		};
	//(*databasefile)<<_Typeend_<<" ";
	return 1;
}


int type::saveobjects(ofstream *databasefile, int how){
typestp *subtps;
	if (extent!=NULL) {
		(*databasefile)<<_Typestart_<<" "<<(*this).name<<" ";
		(*extent).saveobjects(databasefile,how);
		};
	subtps=(*this).subtypes;
	while(subtps!=NULL){ //save the extents of the subtypes
		(*subtps->tp).saveobjects(databasefile,how);
		subtps=subtps->next;
		}; //subextents saved
	return 1;
	}

// To get all subtype names
char *type::getname(void){
	return name;
}

//this two functons gets all subtypes, the first fn initiates
//the second continues
char *type::getallsubtypes(void){
char *nm;
	curr_ext=ext_start;
	if (curr_ext!=NULL){
		nm=(*curr_ext->tp).getname();
		curr_ext=curr_ext->next;
		}
	else nm=NULL;
	return nm;
}
char *type::getnextsubtype(void){
char *nm;
	if (curr_ext==ext_end){
		nm=(*curr_ext->tp).getname();
		curr_ext=NULL;
		}
		else if (curr_ext!=NULL){
		nm=(*curr_ext->tp).getname();
		curr_ext=curr_ext->next;
		}
	else nm=NULL;
	return nm;
}

//This two functions returns all direct subtypes
char *type::getsubtypes(void){
char *nm;
	this->curr_stp=subtypes;
	if (curr_stp!=NULL){
		nm=(*curr_stp->tp).getname();
		this->curr_stp=(*this).curr_stp->next;
		}
	else nm=NULL;
	return nm;
}

char *type::getsubtype(void){
char *nm;
	if ((*this).curr_stp!=NULL){
		nm=(*curr_stp->tp).getname();
		this->curr_stp=(*this).curr_stp->next;
		}
	else nm=NULL;
	return nm;
}

char *type::getfirstprop(void){
char *nm;
	propptr=props;
	if (propptr!=NULL) {
		nm=(*propptr).getname();
		propptr=(*propptr).nextproperty();
		}
	else nm=NULL;
	return nm;
}

char *type::getnextprop(void){
char *nm;
	if (propptr!=NULL) {
		nm=(*propptr).getname();
		propptr=(*propptr).nextproperty();
		}
	else nm=NULL;
	return nm;
}

int type::validate(char *inp,void *obj) {
int length=strlen(inp);
int currpos=0;
int retval=0;
int factor=pow(10,length-1);
char currch=inp[currpos];
object *reto;
	while((currpos<length)&&(currch!='\0')){
		if (isdigit(currch)) {
			retval=retval+(currch-48)*factor;
			factor=factor/10;
			currpos++;
			currch=inp[currpos];
			}
		else currpos=2*length; //signals shitty input
	}; //end while
	if (currpos==2*length) {
		obj=NULL;
		return 0;
		};
	retval;  //the parsed integer value
	reto=(*belongsto).getobjectwithoid((*this).name,retval);
	if (reto!=NULL) obj=(void *)reto;
	return 1;
}
