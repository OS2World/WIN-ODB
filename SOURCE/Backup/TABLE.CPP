//----------------------------------------------------
//
//          Implementation of table
//
//----------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "..\source\store.h"

table::table(void){
	tbl=NULL;
	tablesize=0;
}
table::table(int size){
	tbl=new linked_list[size];
	tablesize=size;
}

void table::drop_index(void){
int index, end;
linked_list *iter,*prev;
	end=tablesize-1;
	index=0;
	iter=tbl;
	while(index<=end){
		if ((*iter).empty()==0){
			while(iter!=NULL){
				prev=iter;
				iter=(*iter).next();
				(*prev).~linked_list();
				}
			}
		else (*iter).~linked_list();
		index=index+1;
		iter=tbl+index;
	}//end while
}

void table::drop_table(void){
int index, end;
linked_list *iter,*prev;
object *o;
	end=tablesize-1;
	index=0;
	iter=tbl;
	while(index<=end){
		if ((*iter).empty()==0){
			while(iter!=NULL){
				prev=iter;
				o=(*prev).elem();
				iter=(*iter).next();
				(*prev).~linked_list();
				(*o).~object();
				}
			}
		else (*iter).~linked_list();
		index=index+1;
		iter=tbl+index;
	}//end while
}
int table::hashfn(object o){

	return (o.oid() % tablesize);
}

 int table::hashfn(ODB_OID oid){
	return (oid % tablesize);
}

int table::hashfn(ODB_REAL realval){
int val;
	val=(int) realval*(tablesize +1);
	return (val % tablesize);
}
int table::hashfn(ODB_REF obj){
int val;
	val=(int) obj;
	return (val % tablesize);
}

int table::hashfn(ODB_CHAR prop){
int sum=0;
int i=0;
	if (prop!=NULL) {
		while(prop[i]!='\0') sum=prop[i++]+sum;
		};
	return (sum % tablesize);
}

int table::hashfn(ODB_SET sv){
int sum=0;
collection *curr;
	if (sv==NULL) return 0;
	curr=sv;
	while (curr!=NULL){
		switch((*curr).gettype()){
		case _INT_:sum=sum+(*curr).getintval();
			break;
		case _REAL_:sum=sum+(*curr).getrealval();
			break;
		case _CHAR_:sum=sum+(int)(*curr).getcharstringval();
			break;
		case _OBJECT_:sum=sum+(int)(*curr).getobj();
			break;
		case _COLLECTION_:sum=sum+hashfn((*curr).getsetval());
		}
	curr=(*curr).getnext();
	};
	return (sum % tablesize);
}

object table::put(object *o){
int bucket;
linked_list *iter;
	bucket=(*this).hashfn(*o);
	iter=tbl+bucket;
	(*iter).insert(o);
	return *o;
};

object table::put(object *o,char *prop){
int bucket;
linked_list *iter;
ODB_INT hashval;
property_value *pv;
	pv=(*o).getpropertynamed(prop);
	switch((*pv).gettype()){
	case _INT_:	hashval=(*pv).getintval();
			bucket=(*this).hashfn(hashval);
			break;
	case _REAL_:	bucket=(*this).hashfn((*pv).getrealval());
			break;
	case _CHAR_:	bucket=hashfn((*pv).getcharstringval());
			break;
	case _OBJECT_:   bucket=(*this).hashfn((*pv).getelem());
			break;
	case _COLLECTION_:bucket=(*this).hashfn((*pv).getsetval());
			break;	 };//end switch
	iter=tbl+bucket; //pointer arithmetic
	(*iter).insert(o);
	return *o;
}



void table::display(outputbuffer &buf){
int index, end;
linked_list *iter;
char *bufpos=buf.write_request(10);
	end=tablesize-1;
	index=0;
	iter=tbl;
	while(index<=end){
		sprintf(bufpos,"\n[%i]:",index);
		if ((*iter).empty()!=0) {
			bufpos=buf.write_request(4);
			sprintf(bufpos,"NULL");
			}
		else {
			(*iter).display(buf);
		}; //else
		index=index+1;
		iter=iter+1;
	}//end while
}; //end table::display();

//-----------------------------------------------------------
// Search facilities for tables
//
// functions get and getnext in various configurations
//
//-----------------------------------------------------------

int table::openscan(int how,query_stream *qst) {
	if (how==_ALL_){
		(*qst).set_currbuck(-1);
		(*qst).set_currlst(tbl);
		(*qst).set_scantype(_ALL_ );
		}
	else {
		(*qst).set_currbuck(how);
		(*qst).set_currlst(tbl+how);
		(*qst).set_scantype(_BUCKET_);
		};//end else
	return 1;
}


object *table::getnextobj(query_stream *qst){
object *obj;
	obj=NULL;
	if ((((*qst).get_currlst()==NULL)&&((*qst).get_scantype()!=_ALL_))||
		((*qst).get_currbuck()==-2)){ //end of table reached
		obj=NULL;
		(*qst).set_currobj(obj);
		return obj;
		};
	if ((*qst).get_currbuck()==-1){  //Scan not begun yet
		(*qst).set_currbuck(0); //init scan vars
		(*qst).set_currlst(tbl+(*qst).get_currbuck());
		};
	while((obj==NULL)&&((*qst).get_currbuck()!=-2)){
		if ((*qst).get_currlst()!=NULL){
			obj=(*(*qst).get_currlst()).elem();  //retrieve object
			(*qst).set_currlst((*(*qst).get_currlst()).next()); //move forward in lst
		};
		if (((*qst).get_currlst()==NULL)&&((*qst).get_scantype()==_ALL_)){ //end of bucket list
			if ((*qst).get_currbuck()<(tablesize-1)){
				(*qst).set_currbuck((*qst).get_currbuck()+1); //move to next bucket
				(*qst).set_currlst(tbl+(*qst).get_currbuck());
			}
			else {
				(*qst).set_currbuck(-2); //signals end of table
			}; //end else
		}; //end currlst==NULL
		if (((*qst).get_currlst()==NULL)&&((*qst).get_scantype()!=_ALL_))
			(*qst).set_currbuck(-2);
	}; //end while
	(*qst).set_currobj(obj);
	return obj;
}

object *table::get(query_stream *qst){
object *obj;
 // scan extent
	(*this).openscan(_ALL_,qst);
	obj=(*this).getnextobj(qst);
	return obj;
}

object *table::getnext(query_stream *qst){
object *obj;
	obj=(*this).getnextobj(qst);
	return obj;
}

int table::remove(object *obj){
ODB_OID oid;
int bucket;
linked_list *lst, *prev;
object *o;
int found=0;
	oid=(*obj).getoid();
	bucket=0;
	lst=tbl+bucket;
	while((found!=1)&&(bucket<=(tablesize-1))){
		prev=lst;
		o=(*lst).elem();
		while((found!=1)&&(lst!=NULL)&&(o!=NULL)){
			o=(*lst).elem();
			if((*o).getoid()==oid){
				found=1;
				(*prev).set_next_to((*lst).next());
				if (lst!=prev) {
					(*lst).~linked_list();
					}
				else (*lst).null_elem(); //first in bucket
				}
			else {
				prev=lst;
				lst=(*lst).next();
				};
			}; //end while
		bucket++;
		lst=tbl+bucket;
		}; //end while
	return found;
}


//--------------------------------------------------
//------ Database file I/O .------------------------
//--------------------------------------------------

int table::saveobjects(ofstream *databasefile,int how){
linked_list *bucket;
linked_list *ptr;
int offset=0, anysaved=0;
object *o;
	bucket=tbl+offset;
	while(offset<tablesize){
		if ((*bucket).elem()!=NULL){
			ptr=bucket;
			while(ptr!=NULL){
         	anysaved=1;
				o=(*ptr).elem();
				(*o).saveobject(databasefile,how);
				ptr=(*ptr).next();
				}; //bucket saved
			};
		offset++;
		bucket=tbl+offset;
		}; //all buckets saved;
	return 1;
}

