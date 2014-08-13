//----------------------------------------------------
//	Project		:	ODB
//	File		:	odbset.cpp
//	Description	:	Implementation of collection class
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "..\source\store.h"





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	takes a type definition, e.g. _OBJECT_ 
//	Author		:	Staffan Flodin
//----------------------------------------------------
collection::collection (int tp)
{
  type=tp;
  obj=NULL;
  charstring=NULL;
  setval=NULL;
  intval=0;
  realval=0;
  next=NULL;
  empty=1; //yes its empty
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
collection::collection(void)
{
  type=0;
  obj=NULL;
  charstring=NULL;
  setval=NULL;
  intval=0;
  realval=0;
  next=NULL;
  empty=1;
}





//----------------------------------------------------
//	Method		:	destructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
collection::~collection(void)
{
	
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
collection::is_empty(void)
{
  return empty;
}




//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::insert(ODB_INT i)
{
  ODB_SET ns;
  if (type!=_INT_) return -1;
  if (empty==1)
    {
      empty=-1;
      intval=i;
    }
  else {
    ns=new collection(type);
    (*ns).insert(i);
    (*ns).set_next_to(next);
    next=ns;
  };
  return 1;
}




//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::insert(ODB_REAL r)
{
  ODB_SET ns;
  if (type!=_REAL_) return -1;
  if (empty==1)
    {
      empty=-1;
      realval=r;
    }
  else {
    ns=new collection(type);
    (*ns).insert(r);
    (*ns).set_next_to(next);
    next=ns;
  };
  return 1;
}




//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::insert(ODB_REF o)
{
  ODB_SET ns;
  if (type!=_OBJECT_) return -1;
  if (empty==1)
    {
      empty=-1;
      obj=o;
    }
  else {
    ns=new collection(type);
    (*ns).insert(o);
    (*ns).set_next_to(next);
    next=ns;
  };
  return 1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::insert(ODB_CHAR c)
{
  ODB_SET ns;
  if (type!=_CHAR_) return -1;
  if (empty==1)
    {
      empty=-1;
      charstring=c;
    }
  else {
    ns=new collection(type);
    (*ns).insert(c);
    (*ns).set_next_to(next);
    next=ns;
  };
  return 1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::insert(ODB_SET s)
{
  ODB_SET ns;
  if (type!=_COLLECTION_) return -1;
  if (empty==1)
    {
      empty=-1;
      setval=s;
    }
  else {
    ns=new collection(type);
    (*ns).insert(s);
    (*ns).set_next_to(next);
    next=ns;
  };
  return 1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::set_next_to(ODB_SET sv)
{
  if (next==NULL)
    {
      next=sv;
      return 1;
    }
  else return -1;
}




//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void collection::delete_set(void)
{
  if (next!=NULL)
    {
      (*next).delete_set();
      (*this).~collection();
    }
  else  (*this).~collection();
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void collection::null_reference_to(ODB_REF o)
{
  if (obj==o) obj=NULL;
  else if (next!=NULL) (*next).null_reference_to(o);
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::delete_element(ODB_REF o) {
  ODB_SET tmpset;
  if ((*this).obj==o) {
    if (next!=NULL) {
      obj=(*next).getobj(); //copy reference the next member element
      tmpset=next;
      next=(*next).getnext();
      if (tmpset->is_empty()>0) this->empty=1;
      (*tmpset).~collection(); //delete next set element
      return 1;
    }
    else { //next was null
      obj=NULL;
      empty=1;
      return 1;
    }
  }
  else if (next!=NULL) return (*next).delete_element(o);
  return -1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::delete_element(ODB_INT i) {
  ODB_SET tmpset;
  if ((*this).intval==i) {
    if (next!=NULL) {
      obj=(*next).getobj(); //copy reference the next member element
      tmpset=next;
      if (tmpset->is_empty()>0) this->empty=1;
      next=(*next).getnext();
      (*tmpset).~collection(); //delete next set element
      return 1;
    }
    else { //next was null
      intval=0;
      empty=1;
      return 1;
    }
  }
  else if (next!=NULL) return (*next).delete_element(i);
  return -1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::delete_element(ODB_REAL r) {
  ODB_SET tmpset;
  if ((*this).realval==r) {
    if (next!=NULL) {
      obj=(*next).getobj(); //copy reference the next member element
      tmpset=next;
      if (tmpset->is_empty()>0) this->empty=1;
      next=(*next).getnext();
      (*tmpset).~collection(); //delete next set element
      return 1;
    }
    else { //next was null
      realval=0;
      empty=1;
      return 1;
    }
  }
  else if (next!=NULL) return (*next).delete_element(r);
  return -1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::delete_element(ODB_CHAR c) {
  ODB_SET tmpset;
  if ((*this).charstring==c) {
    if (next!=NULL) {
      obj=(*next).getobj(); //copy reference the next member element
      tmpset=next;
      if (tmpset->is_empty()>0) this->empty=1;
      next=(*next).getnext();
      (*tmpset).~collection(); //delete next set element
      return 1;
    }
    else { //next was null
      charstring=NULL;
      empty=1;
      return 1;
    }
  }
  else if (next!=NULL) return (*next).delete_element(c);
  return -1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void collection::displayelements(outputbuffer &buf)
{
  char *bufp;

  switch(type)
    {
    case _INT_:
      bufp=buf.write_request(10);
      if (bufp!=NULL) sprintf(bufp,"%i",intval);
      break;
    case _REAL_:
      bufp=buf.write_request(10);
      if (bufp!=NULL) sprintf(bufp,"%f",realval);
      break;
    case _CHAR_:
      bufp=buf.write_request(strlen(charstring));
      if (bufp!=NULL) sprintf(bufp,"\"%s\"",charstring);
      break;
    case _OBJECT_:
      if (obj!=NULL) (*obj).displayoid(buf); //(*obj).displayoid(buf);
      break;
    case _COLLECTION_:
      (*setval).display(buf);
      break;
    };
  if (next!=NULL) {
    bufp=buf.write_request(1);
    sprintf(bufp,",");
    (*next).displayelements(buf);
  }
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void collection::display(outputbuffer &buf)
{
  char *bufp;
  bufp=buf.write_request(1);
  if (bufp!=NULL) sprintf(bufp,"{");
  if (empty!=1) (*this).displayelements(buf);
  bufp=buf.write_request(1);
  if (bufp!=NULL) sprintf(bufp,"}");
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::member(ODB_INT i)
{
  if (empty==1) return 0;
  if (i==intval) return 1+((next==NULL)?0:(*next).member(i));
  else if (next!=NULL) return(*next).member(i);
  else return 0;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::member(ODB_CHAR c)
{
  if (empty==1) return 0;
  if (c==charstring) return 1+((next==NULL)?0:(*next).member(c));
  else if (next!=NULL) return(*next).member(c);
  else return 0;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::member(ODB_REAL r)
{
  if (empty==1) return 0;
  if (r==realval) return 1+((next==NULL)?0:(*next).member(r));
  else if (next!=NULL) return(*next).member(r);
  else return 0;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::member(ODB_REF o)
{
  if (empty==1) return 0;
  if (o==obj) return 1+((next==NULL)?0:(*next).member(o));
  else if (next!=NULL) return(*next).member(o);
  else return 0;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::equal(ODB_SET s)
{
  int retval;
  if (s==NULL) return -1;
  if ((empty>0)&&((*s).is_empty()>0)) return 1;
  if (empty>0) return -1;
  //else
  switch(type)
    {
    case _INT_:retval=(*s).member(intval);
      break;
    case _REAL_:retval=(*s).member(realval);
      break;
    case _CHAR_:retval=(*s).member(charstring);
      break;
    case _OBJECT_:retval=(*s).member(obj);
      break;
      //case _COLLECTION_:retval=(*s).member(setval);
      //break;
    } ;
  if (retval<1) return retval;
  if (next!=NULL) return (*next).equal(s);
  if (next==NULL) return 1;
  return -1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::saveset(ofstream *databasefile)
{
  collection *coll, *s;
  object *o;
  ODB_OID oid;
  int a_member=0;
  coll=this;
  (*databasefile)<<type<<" {";
  if (type!=_CHAR_) (*databasefile)<<" ";
  else (*databasefile)<<" \n";
  while ((coll!=NULL)&&((*coll).is_empty()<1))
    {
      a_member=1;
      switch (type)
	{
	case _INT_:
	  (*databasefile)<<(*coll).getintval()<<" ";;
	  break;
	case _REAL_:
	  (*databasefile)<<(*coll).getrealval()<<" ";
	  break;
	case _CHAR_:
	  (*databasefile)<<(*coll).getcharstringval()<<"\n";
	  break;
	case _OBJECT_:
	  o=(*coll).getobj();
	  if (o!=NULL)
	    {
	      oid=(*o).oid();
	      (*databasefile)<<oid<<" ";
	    }
	  else (*databasefile)<<0<<" ";
	  break;
	case _COLLECTION_:
	  s=(*coll).getsetval();
	  (*s).saveset(databasefile);
	  break;
	}; //end case
      coll=(*coll).getnext();
    } //end while
  (*databasefile)<<"}\n";

  return 1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::populatefromfile(ifstream *databasefile,int datatype, tblentry *maptable, database *db)
{
  ODB_INT iv;
  ODB_REAL rv;
  ODB_REF ov;
  char charval[256],keyword[20],ch;

  //
  //initialized to other than }
  //
  keyword[0]='\0'; 

  //
  //The set may be empty scan until } or until some other charcater
  //
  ch=(char)(*databasefile).peek();

  while ((isspace(ch)!=0)&&(ch!='}'))
    {
      //
      //remove the charcter
      //
      (*databasefile).get();  
      
      //
      //peek the next charcter
      //
      ch=(char)(*databasefile).peek(); 

      keyword[0]=ch;

      //read it since empty set
      if (ch=='}') ch=(char)(*databasefile).get(); 
    }

  keyword[1]='\0';

  while (strncmp(keyword,"}",1)!=0)
    {
      switch(datatype)
	{
	case _INT_:
	  (*databasefile)>>iv;
	  (*this).insert(iv);
	  break;
	case _REAL_:
	  (*databasefile)>>rv;
	  (*this).insert(rv);
	  break;
	case _CHAR_:
	  while ((*databasefile).peek()!='}')
	    {
	      //read ' '
	      if ((*databasefile).peek()==' ') ch=(char)(*databasefile).get();
	      
	      //read \n
	      if ((*databasefile).peek()=='\n') ch=(char)(*databasefile).get();

	      (*databasefile).getline(charval,255);
	      (*this).insert((*db).getstring(charval));

	      //read ' '
	      if ((*databasefile).peek()==' ') ch=(char)(*databasefile).get();

	      if ((*databasefile).peek()=='\n') ch=(char)(*databasefile).get();
	    };
	  break;

	case _COLLECTION_:
	  break;
	case _OBJECT_:
	  (*databasefile)>>iv;
	  ov=maptable[iv].newobj;
	  if (ov!=NULL) (*this).insert(ov);
	  else return -1;
	  break;
	};
      ch=(char)(*databasefile).get();
      if (ch=='}')  keyword[0]='}';
      else {
	if (ch==EOF) return -1;
	ch=(char)(*databasefile).peek();
	if(ch=='}') (*databasefile)>>keyword;
      };
    }; //end while
  return 1;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int collection::gettype(void)
{
  return (*this).type;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_INT collection::getintval(void)
{
  return intval;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_CHAR collection::getcharstringval(void)
{
  return charstring;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_REAL collection::getrealval(void)
{
  return realval;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_REF collection::getobj(void)
{
  return obj;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_SET collection::getsetval(void)
{
  return setval;
}





//----------------------------------------------------
//	Method		:
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_SET collection::getnext(void)
{
  return next;
}
