//----------------------------------------------------
//
//          Implementation of databse class
//
//----------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "..\source\store.h"
#include "..\source\parser.h"

database::database(char *nm){
char *coidn;
	global_oid=1;
	default_tablesize=10; //gloal to database
	strtbl = new stringtable(default_tablesize);
	name=(*this).getstring(nm);
	usertypes.set_belongsto(this);
	coidn=(*this).getstring(_OID_);
	usertypes.add_property(_INT_,coidn);
   validationObject.setDb(this);
   ODBparser.setDatabase(this);
}


database::database(void){
char *coidn;
	global_oid=1;
	default_tablesize=10; //gloal to database
	strtbl = new stringtable(default_tablesize);
   name=(*this).getstring("ODB_Database");
	usertypes.set_belongsto(this);
	coidn=(*this).getstring(_OID_);
	usertypes.add_property(_INT_,coidn);
   validationObject.setDb(this);
   ODBparser.setDatabase(this);
}

database::~database(void){
	this->initialize();
}

ODB_OID database::generate_oid(void){
int return_oid;
	return_oid=global_oid;
	global_oid++;
	return return_oid;
}

int database::get_tablesize(void){
	return (*this).default_tablesize;
}

type *database::get_usertypes(void){
	return &usertypes;
}

type *database::create_type(char *nm){
type *tp;
	if (usertypes.gettypenamed(nm)==NULL) tp=new type(nm, &usertypes, this);
	else tp=NULL;
	return tp;
}

type *database::create_type(char *nm, char *supt){
type *tp,*stp;
	stp=(*this).gettypenamed(supt);
	if ((stp!=NULL)&&(usertypes.gettypenamed(nm)==NULL))
		tp=new type(nm, stp, this);
	else tp=NULL;
	return tp;
}

void database::display(outputbuffer &buf){
char *bufptr=buf.write_request(40);
	sprintf(bufptr,"\n\nDATABASE: %s\nDefault table size: %i\n",name,default_tablesize);
}

type *database::gettypenamed(char *nm){
	return usertypes.gettypenamed(nm);
}

char *database::getstring(char *st){
char *s;
	if (st==NULL) return NULL;
	s=(*strtbl).get(st);
	if (s==NULL) s=(*strtbl).put(st);
	return s;
}

//--------------------------------------------------
//------ Database I/O .-----------------------------
//--------------------------------------------------
/*
int database::makefilename(char *nm, char *filename){
char *strpos, *namestart;
int length, namestartpos;
	namestartpos=0;
	strcpy(filename,nm); //copy characters
	length=0;
	namestart=filename+length;
	while (*namestart!='\0'){
		if ((*namestart!=':')||(*namestart!='\\')){
			namestartpos=length;
			};
		length++;
		namestart++;
	}
	length=strlen(filename);
	while(length>7+namestartpos){ //cut the string to length 1
		strpos=filename+length;
		*strpos='\0';
		length--;
		};
	//strcat(filename,".odb"); //extension is .odb
	return 1;
}
*/
int database::save_database(char *nm){
//returns 0 or !=0
ofstream databasefile;
	//makefilename(name,filename);
	//strcpy(filename,name);
	databasefile.open(nm); // create
	if(databasefile.fail()){
		return 0;
		}
	else{    //Save database on file
   	databasefile<<_DBfileID_<<" ";
		if ((*this).name==NULL) databasefile<<"NoName"<<" ";
		else databasefile<<(*this).name<<" ";
		databasefile<<default_tablesize<<" ";
		databasefile<<global_oid<<"\n";
		usertypes.savetypes(&databasefile);
		databasefile<<_Databaseend_<<" ";
		databasefile<<_Populationstart_<<" ";
		usertypes.saveobjects(&databasefile,NO_PROPS);
		databasefile<<_Populationend_<<" ";
		databasefile<<_Populationstart_<<" ";
		usertypes.saveobjects(&databasefile,WITH_PROPS);
		databasefile<<_Populationend_<<" ";
      databasefile.close();

		};
	return 1;
}

int database::load_database(char *nm){
//Return 0 or !=0 wether ok or fail
char filename[256], keyword[256], dbname[256],charval[256],setstart[256], ch;
ifstream databasefile;
type *currtp, *supt, *tp;
int proptype=0, tblsize, i;
ODB_INT oid, intval;
ODB_REAL realval;
ODB_SET setval;
ODB_REF obj, refobj, relobj;

tblentry *maptable; 	//map table for old oid to new obj. To set up
		   				//rel between objects read from file
int fileerror=0;

	// --------------------------------------
   // Copy the filename and open the file
   // If fail then return immideately
   //---------------------------------------
	strcpy(filename,nm);
	databasefile.open(filename); // create
	if(databasefile.fail()) {
		return -1;
		}

   //-------------------------------------
   // To check that is is an ODB file check
   // that it initiated with the string _DBFile_ID
   // to make sure no crash check this by
   // reading a char at a time.
   //-------------------------------------
	else {
   i=0;
   while (i<_DBfileID_Length){
   	databasefile>>ch;
      charval[i++]=ch;
   }
   charval[i]='\0';
   if (strcmp(charval,_DBfileID_)!=0) return -1; //No string match No ODB file

   //------------------------------------
   // Now the new file can be loaded. Dealloc
   // The existing database before new is loaded
   //-------------------------------------

	(*this).initialize();

   //-----------------------------------------------
	// First get Db Name, table size and nof objects
	//-----------------------------------------------
	databasefile>>dbname;
	name=(*this).getstring(dbname); //subst for stored
	databasefile>>default_tablesize;
	databasefile>>tblsize;

	//-----------------------------------------------
   // start loading type index and property definitions
   // the keyword _Databaseend_ indicates end of
   // the logical definition
   //------------------------------------------------
	databasefile>>keyword;
	while ((fileerror!=1)&&(strncmp(keyword,_Databaseend_,20)!=0)){

		if (databasefile.peek()==EOF) //catch unexpected end of file
			fileerror=1;

      //---------------------------------------------
      // The keyword _typestart_ indicate start of
      // type definition. This includes indexes and
      // property definitions
      //---------------------------------------------
			if (strncmp(keyword,_Typestart_,20)==0) {
         	databasefile>>dbname;
         	databasefile>>keyword; //may denote subtype of

      	//-----------------------------------------
         // dbname does now hold a typename and the
         // type can be created a subtype specification
         // may occur. This is indicated by _subtypestart_
         //-----------------------------------------
				if (strncmp(dbname,_Usertypes_,20)==0)
					currtp=&usertypes; //usertypes exsists just get it
				else {
					if (strncmp(keyword,_Subtypestart_,20)==0){
						databasefile>>keyword; //supertypename
						supt=(*this).gettypenamed(keyword);
						databasefile>>keyword; //read new kwd
						currtp=(*this).create_type(dbname,(*supt).getname());
					} //end if supertype

         else  currtp=(*this).create_type(dbname);
			}; //end else, i.e. type creation clause

		//--------------------------------------------
      // Now, property definitions follow
      // if any properties exist. This is indicated by
      // _propstart keyword
      //---------------------------------------------
		if (strncmp(keyword,_Propstart_,20)==0){
			databasefile>>keyword; //holds the property name

         //-------------------------------------------
         // read properties until _propend keyword
         //------------------------------------------
			while (strncmp(keyword,_Propend_,20)!=0){
				databasefile>>proptype; // holds the property type
				(*currtp).add_property(proptype,keyword);
				databasefile>>keyword;
			}; //end while _Propend found

			databasefile>>keyword;
		}; //end if _Propstart_

      //---------------------------------------------
      // Index definitions follow if any. Keyword
      // _indexstart_ indicate this. All indexes are
      // defined beteween _indexstart and _ indexend
      //---------------------------------------------
		if (strncmp(keyword,_Indexstart_,20)==0){
			databasefile>>keyword;
			while (strncmp(keyword,_Indexend_,20)!=0){
				(*currtp).build_index(keyword);
				databasefile>>keyword;
			}; //end while _indexend found
			databasefile>>keyword;
		}; //end
		}
	}

   //-------------------------------------------------
   // This section loads the objects. First just the
   // Objects then the properties and values to the props
   // for each object.
   //-------------------------------------------------
	maptable = new tblentry[tblsize]; //alloc map table
	i=0;
	while (i<tblsize){
		*(maptable[i].prop)=NULL; //initialize tbl entry
		maptable[i].refoid=0;
		maptable[i].newobj=NULL;
		i++;
   };

	//-------------------------------------------------
   // First, maptable set up. The maptable is indexed
   // with the OIDs from file. New objects are given
   // new OIDs
   // The objects are contained within _populationstart
   // and _populationend with the type of the object
   // specified before the object. The objects are
   // given as OIDs
   //----------------------------------------------
   databasefile>>keyword;//read instances

	if (strncmp(keyword,_Populationstart_,20)==0){
	databasefile>>keyword;
	while((fileerror!=1)&&(strncmp(keyword,_Populationend_,20)!=0)){

		if (databasefile.peek()==EOF) //Catch eof
			fileerror=1;

       //----------------------------------------
       // If a type is specified load the type from db
       //----------------------------------------
		if (strncmp(keyword,_Typestart_,20)==0) {
			databasefile>>keyword;
			tp=(*this).gettypenamed(keyword);
			databasefile>>keyword;
			};

      //-----------------------------------------
      // Each instance is eclosed within _objstart
      // and _objend  creat the object of the
      // type previously specified and insert it
      // into maptable by its OID as spec. in file
      //------------------------------------------
		if (strncmp(keyword,_Objstart_,20)==0){
			obj=(*tp).create_instance();
			databasefile>>oid; //read oid
			maptable[oid].newobj=obj; //store *obj in maptbl
			databasefile>>keyword; //_objend_ or propnm
			if(strncmp(keyword,_Objend_,20)!=0) fileerror=1;
			};

		if ((fileerror!=1)&&(strncmp(keyword,_Objend_,20)==0))
      	 databasefile>>keyword;
		}; //end while populationend ...
	}

   //----------------------------------------------------------
	// Secondly, The properties of all created instances are set up
   // objects are enclosed within Populationstart and populationend
   // just as before. The differenc is that the attribute values
   // are defined here
   //----------------------------------------------------------
	databasefile>>keyword;//read instances
	if (strncmp(keyword,_Populationstart_,20)==0){
	databasefile>>keyword;

	while((fileerror!=1)&&(strncmp(keyword,_Populationend_,20)!=0)){

		if (databasefile.peek()==EOF)
			fileerror=1;

		if (strncmp(keyword,_Typestart_,20)==0) {
			databasefile>>keyword;
			tp=(*this).gettypenamed(keyword);
			databasefile>>keyword;
      };

		if (strncmp(keyword,_Objstart_,20)==0){

			databasefile>>oid;			//read oid
			obj=maptable[oid].newobj;	//get *obj from maptbl

			databasefile>>keyword;		//_objend_ or propnm

         //----------------------------------------------------
         // read all attribute value definitions for the object
         //----------------------------------------------------
			while((fileerror!=1)&&(strncmp(keyword,_Objend_,20)!=0)){

				if (databasefile.peek()==EOF)
					fileerror=1;

				databasefile>>proptype;	//the datatype

            // Treat all collection types similarily. Assume they
            // where written properly to file
				if((proptype==_COLLECTION_)||(proptype==_INT_COLLECTION_)||
					 (proptype==_CHAR_COLLECTION_)||(proptype==_REAL_COLLECTION_))
						proptype=_COLLECTION_;

            //-------------------------------------------
            // Dispatch the correct attribute value assignment
            // procedure depending on the type of the propery
            //----------------------------------------------
            switch(proptype) {
					case _INT_:
						databasefile>>intval;
						(*obj).setproperty_value(keyword,intval);
						break;

					case _REAL_:
						databasefile>>charval;
                  this->validate(charval,realval);
						(*obj).setproperty_value(keyword,realval);
						break;

					case _CHAR_:
						databasefile.get();	//read ' '
						databasefile.get(); 	//read \n
						databasefile.getline(charval,255);
						(*obj).setproperty_value(keyword,charval);
						break;

					case _OBJECT_:
						databasefile>>intval;//old oid
						relobj=maptable[intval].newobj;
						(*obj).setproperty_value(keyword,relobj);
						break;

					case _COLLECTION_:
						databasefile>>intval; //Set element type
						setstart[0]='\0';
						databasefile>>setstart; //should be {
						if (strncmp(setstart,"{",1)!=0) fileerror=1;
						else {
							setval=new collection(intval);
							(*setval).populatefromfile(&databasefile,intval,maptable,this);
							(*obj).setproperty_value(keyword,setval);
							};
						break;

					default: fileerror=1;
					};

				databasefile>>keyword;	//_objend_ or propnm
				}								//end while
			}; 								//end if _Objstart_

		if ((strncmp(keyword,_Populationend_,20)!=0)&&
      	 (strncmp(keyword,_Typestart_,20)!=0))
		      databasefile>>keyword; //_objstart_ or _typestart_

		} 										//end while not _populationend_
		}										//end if _populationstart_

	//set up references stored in tbl.
	i=0;
	if (fileerror==1) return -1;
	while(i<tblsize){
		if (*(maptable[i].prop)!=NULL) { //a reference
			obj=maptable[i].newobj;
			strncpy(keyword,maptable[i].prop,20);
			oid=maptable[i].refoid;
			refobj=maptable[oid].newobj;
			(*obj).setproperty_value(keyword,refobj);
			}; //end if
		i++;

	}; //end while
	}; //end else if fail()
	databasefile.close();
	free(maptable);
	return 1;
}

//--------------------------------------------------------------
//         query interface
//--------------------------------------------------------------
query_stream *database::open_stream(char *tpname){
type *tp;
char *nm;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	tp=(*this).gettypenamed(nm);
	if (tp!=NULL){
		qst=(*tp).open();
		}
	else {
		qst=NULL;
		};
	return qst;
}

query_stream *database::open_stream(char *tpname, char *propnm,char *op,ODB_INT i){
type *tp;
char *nm, *pnm,*cop;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	cop=(*this).getstring(op);
	pnm=(*this).getstring(propnm);
	tp=(*this).gettypenamed(nm);
	if ((tp!=NULL)&&(strcmp(propnm,_OID_)==0)){
		qst=(*tp).open(pnm,cop,i); 
		return qst;
		};
	if ((tp!=NULL)&&((*tp).getpropertynamed(pnm)!=NULL)){
		qst=(*tp).open(pnm,cop,i);
		}
	else {
		qst=NULL;
		};
	return qst;
}

query_stream *database::open_stream(char *tpname, char *propnm,char *op,ODB_REAL f){
type *tp;
char *nm, *pnm,*cop;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	cop=(*this).getstring(op);
	pnm=(*this).getstring(propnm);
	tp=(*this).gettypenamed(nm);
	if ((tp!=NULL)&&((*tp).getpropertynamed(pnm)!=NULL)){
		qst=(*tp).open(pnm,cop,f);
		}
	else {
		qst=NULL;
		};
	return qst;
}

query_stream *database::open_stream(char *tpname, char *propnm,ODB_CHAR ch){
type *tp;
char *nm, *pnm, *chr;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	pnm=(*this).getstring(propnm);
	chr=(*this).getstring(ch);
	tp=(*this).gettypenamed(nm);
	if ((tp!=NULL)&&((*tp).getpropertynamed(pnm)!=NULL)){
		qst=(*tp).open(pnm,chr);
		}
	else {
		qst=NULL;
		};
	return qst;
}

query_stream *database::open_stream(char *tpname, char *propnm,ODB_REF o){
type *tp;
char *nm, *pnm;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	pnm=(*this).getstring(propnm);
	tp=(*this).gettypenamed(nm);
	if ((tp!=NULL)&&((*tp).getpropertynamed(pnm)!=NULL)){
		qst=(*tp).open(pnm,o);
		}
	else {
		qst=NULL;
		};
	return qst;
}

query_stream *database::open_stream(char *tpname, char *propnm,ODB_SET o){
type *tp;
char *nm, *pnm;
query_stream *qst;
	nm=(*this).getstring(tpname); //subst for stored
	pnm=(*this).getstring(propnm);
	tp=(*this).gettypenamed(nm);
	if ((tp!=NULL)&&((*tp).getpropertynamed(pnm)!=NULL)){
		qst=(*tp).open(pnm,o);
		}
	else {
		qst=NULL;
		};
	return qst;
}

select_stream *database::select(query_stream *qst, char *propnm,char *oop,ODB_INT i){
char *pnm, *coop;
type *tp;
property_value *pv;
select_stream *sst;
	coop=(*this).getstring(oop);
	 pnm=(*this).getstring(propnm);  //subst for stored
	 tp=(*qst).get_currtype();
	 if (tp!=NULL) pv=(*tp).getpropertynamed(pnm);
	 if (pv!=NULL){
		sst=(*qst).open_select_stream(pnm,coop, i);
		}
	 else sst=NULL;
	 return sst;
}

select_stream *database::select(query_stream *qst, char *propnm,char *oop,ODB_REAL f){
char *pnm,*coop;
type *tp;
property_value *pv;
select_stream *sst;
	coop=(*this).getstring(oop);
	 pnm=(*this).getstring(propnm);  //subst for stored
	 tp=(*qst).get_currtype();
	 if (tp!=NULL) pv=(*tp).getpropertynamed(pnm);
	 if (pv!=NULL){
		sst=(*qst).open_select_stream(pnm,coop, f);
		}
	 else sst=NULL;
	 return sst;
}

select_stream *database::select(query_stream *qst, char *propnm,ODB_CHAR ch){
char *pnm, *chr;
type *tp;
property_value *pv;
select_stream *sst;
	 pnm=(*this).getstring(propnm);  //subst for stored
	 chr=(*this).getstring(ch);
	 tp=(*qst).get_currtype();
	 if (tp!=NULL) pv=(*tp).getpropertynamed(pnm);
	 if (pv!=NULL){
		sst=(*qst).open_select_stream(pnm, chr);
		}
	 else sst=NULL;
	 return sst;
}

select_stream *database::select(query_stream *qst, char *propnm,ODB_REF o){
char *pnm;
type *tp;
property_value *pv;
select_stream *sst;
	 pnm=(*this).getstring(propnm);  //subst for stored
	 tp=(*qst).get_currtype();
	 if (tp!=NULL) pv=(*tp).getpropertynamed(pnm);
	 if (pv!=NULL){
		sst=(*qst).open_select_stream(pnm, o);
		}
	 else sst=NULL;
	 return sst;
}

select_stream *database::select(query_stream *qst, char *propnm,ODB_SET o){
char *pnm;
type *tp;
property_value *pv;
select_stream *sst;
	 pnm=(*this).getstring(propnm);  //subst for stored
	 tp=(*qst).get_currtype();
	 if (tp!=NULL) pv=(*tp).getpropertynamed(pnm);
	 if (pv!=NULL){
		sst=(*qst).open_select_stream(pnm, o);
		}
	 else sst=NULL;
	 return sst;
}

semi_join_stream *database::semi_join(query_stream *ls,char *p1,query_stream *rs,char *p2){
char *lp, *rp;
type *tpl,*tpr;
property_value *lpv, *rpv;
semi_join_stream *sjs;
	if (ls==NULL) return NULL;
	lp=(*this).getstring(p1); //subst for stored
	rp=(*this).getstring(p2);
	tpl=(*ls).get_currtype();
	tpr=(*rs).get_currtype();
	if (tpl!=NULL) lpv=(*tpl).getpropertynamed(lp); //check that props
	if (tpr!=NULL) rpv=(*tpr).getpropertynamed(rp); //defined
	if ((tpl!=NULL)&&(tpr!=NULL)&&(lpv!=NULL)&&(rpv!=NULL)) {
		sjs=(*ls).open_sj_stream(lp,rs,rp);
		}
	else {
		sjs=NULL;
		};
	return sjs;
}

ODB_REF database::get(query_stream *qst){
object *o;
	if (qst!=NULL) {
		o=(*qst).get();
		}
	else o=NULL;
	return o;
}

int database::close_stream(query_stream *qst){
	if (qst!=NULL) (*qst).close();
	qst=NULL;
	return 1;
}

int database::reset_stream(query_stream *qst){
	if (qst!=NULL) (*qst).reset();
	qst=NULL;
	return 1;
}

//This two functions return the names of all types
char *database::getallsubtypes(void){
	return usertypes.getallsubtypes();
}
char *database::getnextsubtype(void){
	return usertypes.getnextsubtype();
}

//this two functions gets the subtypes of a particular type
char *database::getsubtypes(char *tpnm){
type *tp;
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL)
		return (*tp).getsubtypes();
	else return NULL;
}
char *database::getsubtype(char *tpnm){
type *tp;
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL)		return (*tp).getsubtype();
	else return NULL;
}

int database::subtypeOf(char *supertype, char *subtype){
type *supertp, *subtp;
char *stp;

	
	subtype=getstring(subtype);
	supertp=this->gettypenamed(supertype);
   subtp=this->gettypenamed(subtype);

   if ((supertp==NULL)||(subtp==NULL)) return 0;
   if (supertp==subtp) return 1;

   stp=supertp->getsubtypes();
   while ((stp!=NULL)&& (stp!=subtype)){
   	stp=supertp->getsubtype();
   };

   if (stp==NULL) return 0;
   else return 1;
}

char *database::getfirstprop(char *tpnm){
type *tp;
char *propnm,*coid;
	 coid=(*this).getstring(_OID_);
	 tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL){
		propnm=(*tp).getfirstprop();
		if (propnm==coid) propnm=(*tp).getnextprop();
		}
	else propnm=NULL;
	return propnm;
}

char *database::getnextprop(char *tpnm) {
type *tp;
char *propnm,*coid;
	 coid=(*this).getstring(_OID_);
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL) {
		propnm=(*tp).getnextprop();
		if (propnm==coid) propnm=(*tp).getnextprop();
		}
	else propnm=NULL;
	return propnm;
}

char *database::getsupertype(char *tpnm){
type *tp,*stp;
char *stpnm=NULL;
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL){
		stp=(*tp).getsupertype();
		if (stp!=NULL) stpnm=(*stp).getname();
	}
	return stpnm;
}


int database::getpropertytype(char *tpnm, char *propnm){
type *tp;
property_value *pv;
int datatype=_ERRORTYPE_;
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL){
		pv=(*tp).getpropertynamed(propnm);
		if (pv!=NULL)
			datatype=(*pv).gettype();
		};
	return datatype;
}

int database::add_property(char *tpname, char *prname,int tp){
type *tpp;
property_value *pv;
char *normnm;
	normnm=(*this).getstring(prname);
	tpp=(*this).gettypenamed((*this).getstring(tpname));
	if (tpp==NULL) return -1;

	pv=(*tpp).getpropertynamed(normnm);
	if (pv!=NULL) return -1;

	(*tpp).add_property(tp,normnm);
	return 1;
}


object *database::getobjectwithoid(char *tpnm,ODB_OID oid){
query_stream *qst;
object *o;
char *eq;
	eq=(*this).getstring("=");
	qst=(*this).open_stream(tpnm,_OID_,eq,oid);
	 o=(*qst).get();
	 (*qst).close();
	 return o;
}


object *database::create_instance(char *tpnm){
type *tp;
object *o;
	tp=(*this).gettypenamed(tpnm);
	if (tp!=NULL)
		o=(*tp).create_instance();
	else o=NULL;
	return o;
}

char *database::set_name(char *nm) {
	name=(*this).getstring(nm);
	return name;
}

int database::set_tablesize(int s){
	default_tablesize=s;
	return default_tablesize;
}

char *database::get_dbname(void){
	if (name==NULL) return (*this).getstring("NoName");
	else return name;
}

int database::create_index(char *tpnm, char *propnm){
type *tp;
char *ctpnm,*cpropnm;
table *tbl;
int retval,proptp;
property_value *pv;
	ctpnm=(*this).getstring(tpnm);
	tp=(*this).gettypenamed(ctpnm);
	if (tp==NULL) return -1;

	cpropnm=(*this).getstring(propnm);
	tbl=(*tp).getindexnamed(cpropnm);
	if (tbl!=NULL) return -1; //index already there

	pv=(*tp).getpropertynamed(cpropnm);
	if (pv!=NULL) {
		proptp=(*pv).gettype();
		if ((proptp==_COLLECTION_)||(proptp==_INT_COLLECTION_)||
				(proptp==_REAL_COLLECTION_)||(proptp==_CHAR_COLLECTION_)||
				(proptp==_SET_COLLECTION_)) return -1;

		retval=(*tp).build_index(cpropnm);
		return retval;
		}
	else return -1;
}

int database::drop_index(char *tpnm, char *propnm){
type *tp;
char *ctpnm,*cpropnm;
table *tbl;

	ctpnm=(*this).getstring(tpnm);
	tp=(*this).gettypenamed(ctpnm);
	if (tp==NULL) return -1;

	cpropnm=(*this).getstring(propnm);
	tbl=(*tp).getindexnamed(cpropnm);
	if (tbl!=NULL) {
		(*tp).drop_index(cpropnm);
		return 1;
		};
	return -1;
}

int database::delete_property(char *tpnm,char *propnm){
//Inherited props must be deleted at the place where they are defined
type *tp;
char *ctpnm,*cpropnm;
property_value *pv;
query_stream *qst;
object *obj;
	ctpnm=(*this).getstring(tpnm);
	tp=(*this).gettypenamed(ctpnm);
	if (tp==NULL) return 0;
	cpropnm=(*this).getstring(propnm);
	pv=(*tp).get_noninherited_propertynamed(cpropnm);
	if (pv!=NULL) { //the property exist
		qst=(*this).open_stream(ctpnm);
		obj=(*qst).get();
		while (obj!=NULL){  //scan all objects in deep extent
			(*obj).delete_instanceproperty(cpropnm);
			obj=(*qst).get();
			};
		(*tp).delete_instanceproperty(cpropnm); //delete from type
		(*tp).drop_index(cpropnm);
		return 1;
		}
	else return 0;
}

int database::delete_type(char *tpnm){
type *tp,*utp;
//Type usertypes cannot be deleted
	utp=(*this).gettypenamed((*this).getstring("Usertypes"));
	tp=(*this).gettypenamed((*this).getstring(tpnm));
	if ((tp!=NULL)&&(tp!=utp)) {
		(*tp).delete_type();
		return 1;
		}
	else return 0;
}

int database::delete_object(ODB_REF o){
	if (o!=NULL) {
		(*o).deleteobject();
		return 1;
		};
	return -1;
}


int database::initialize(void){
char *subtpnm;
type *tp;
	subtpnm=(*this).getsubtypes("Usertypes");
	while (subtpnm!=NULL){
		tp=(*this).gettypenamed(subtpnm);
		if (tp!=NULL) (*tp).delete_type();
		subtpnm=(*this).getsubtype("Usertypes");
		}
	this->name=NULL;
	global_oid=1;
	return 1;
}

//Updating property values
int database::setproperty_value(ODB_REF o,char *name,ODB_INT intval) {
	if (o==NULL) return -1;

	if ((*o).setproperty_value(name,intval)!=NULL) return 1;

	return -1;
}

int database::setproperty_value(ODB_REF o,char *name,ODB_REAL realval) {
	if (o==NULL) return -1;

	if ((*o).setproperty_value(name,realval)!=NULL) return 1;

	return -1;
}
int database::setproperty_value(ODB_REF o,char *name,ODB_CHAR charval) {
	if (o==NULL) return -1;

	if ((*o).setproperty_value(name,charval)!=NULL) return 1;

	return -1;
}

int database::setproperty_value(ODB_REF o,char *name,ODB_REF refval) {
	if (o==NULL) return -1;

	if ((*o).setproperty_value(name,refval)!=NULL) return 1;

	return -1;
}

int database::setproperty_value(ODB_REF o,char *name,ODB_SET setval) {
	if (o==NULL) return -1;

	if ((*o).setproperty_value(name,setval)!=NULL) return 1;

	return -1;
}

//retreiving property values
int database::getproperty_value(ODB_REF o,ODB_NAME propnm,ODB_INT *iv){
ODB_PROP pv;
	if (o==NULL) {
		return -1;
	};
	pv=(*o).getpropertynamed(propnm);
	if(pv==NULL) {
		return -2;
	};
	if (pv->gettype()!=_INT_) return -3;
	*iv=pv->getintval();
	return 1;
}

int database::getproperty_value(ODB_REF o,ODB_NAME propnm,ODB_REAL *rv){
ODB_PROP pv;
	if (o==NULL) {
		return -1;
	};
	pv=(*o).getpropertynamed(propnm);
	if(pv==NULL) {
		return -2;
	};
	if (pv->gettype()!=_REAL_) return -3;
	*rv=pv->getrealval();
	return 1;
}

int database::getproperty_value(ODB_REF o,ODB_NAME propnm,ODB_REF *rv){
ODB_PROP pv;
	if (o==NULL) {
		return -1;
	};
	pv=(*o).getpropertynamed(propnm);
	if(pv==NULL) {
		return -2;
	};
	if (pv->gettype()!=_OBJECT_) return -3;
	*rv=pv->getelem();
	return 1;
}
int database::getproperty_value(ODB_REF o,ODB_NAME propnm,ODB_CHAR *cv){
ODB_PROP pv;
	if (o==NULL) {
		return -1;
	};
	pv=(*o).getpropertynamed(propnm);
	if(pv==NULL) {
		return -2;
	};
	if (pv->gettype()!=_CHAR_) return -3;
	*cv=pv->getcharstringval();
	return 1;
}

int database::getproperty_value(ODB_REF o,ODB_NAME propnm,ODB_SET *cv){
ODB_PROP pv;
ODB_TYPE_TAG tt;
	if (o==NULL) {
		return -1;
	};
	pv=(*o).getpropertynamed(propnm);
	if(pv==NULL) {
		return -2;
	};
	tt=pv->gettype();
	if ((tt!=_COLLECTION_)&&(tt!=_INT_COLLECTION_)&&(tt!=_REAL_COLLECTION_)&&
			(tt!=_CHAR_COLLECTION_)&&(tt!=_SET_COLLECTION_)) return -3;
	*cv=pv->getsetval();
	return 1;
}ODB_SET database::create_collection(int tp){
ODB_SET coll;
	coll=new collection(tp);
	return coll;
}


int database::collection_insert(ODB_SET coll,ODB_INT iv){
	if (coll!=NULL) {
		if ((*coll).gettype()==_INT_) {
			(*coll).insert(iv);
			return 1;
			}
		else return -2;
	}
	else return -1;
}

int database::collection_insert(ODB_SET coll,ODB_REAL rv){
	if (coll!=NULL) {
		if ((*coll).gettype()==_REAL_) {
			(*coll).insert(rv);
			return 1;
			}
		else return -2;
	}
	else return -1;
}

int database::collection_insert(ODB_SET coll,ODB_SET sv){
	if (coll!=NULL) {
		if (((*coll).gettype()==_COLLECTION_)||((*coll).gettype()==_INT_COLLECTION_)||
				((*coll).gettype()==_REAL_COLLECTION_)||((*coll).gettype()==_CHAR_COLLECTION_)||
				((*coll).gettype()==_SET_COLLECTION_)) {
			(*coll).insert(sv);
			return 1;
			}
		else return -2;
	}
	else return -1;
}

int database::collection_insert(ODB_SET coll,ODB_CHAR cv){
	if (coll!=NULL) {
		if ((*coll).gettype()==_CHAR_) {
			(*coll).insert(cv);
			return 1;
			}
		else return -2;
	}
	else return -1;
}

int database::collection_insert(ODB_SET coll,ODB_REF ref){
	if (coll!=NULL) {
		if ((*coll).gettype()==_OBJECT_) {
			(*coll).insert(ref);
			return 1;
			}
		else return -2;
	}
	else return -1;
}

int database::delelete_collection(ODB_SET coll){
	if (coll!=NULL) {
		(*coll).delete_set();
		return 1;
		}
	else return -1;
}


int database::operator >> (char *filename){
	return save_database(filename);
}

int database::operator << (char *filename){
	return load_database(filename);
}


//------------------------------------------------------------------
// to validate data
//------------------------------------------------------------------
int database::validate(char *inp,ODB_INT &value)
{
	return validationObject.validate_integer(inp,value);
}

int database::validate(char *inp,ODB_REAL &value)
{
	return validationObject.validate_real(inp,value);
}

int database::validate(char *ch)
{
	return validationObject.validate_name(ch);
}

int database::validate(char *inp,ODB_SET set)
{
	return validationObject.validate_set(inp,set);
}

//------------------------------------------------------------------
// to parse ODBQL files
//------------------------------------------------------------------
int database::parseFile(char *fnm){
int rv;

	initialize();
   
	try {
		rv=ODBparser.parseFile(fnm);
   }
   catch (int i) {

   	initialize();

   	if (i==PARSEERROR) {
         ODBparser.displayError();
      	return 0;
      }
      if (i==PARSERERROR) {
         ODBparser.displayError();
      	return 0;
      }
      if (i==SEMANTICERROR) {
         ODBparser.displayError();
      	return 0;
      }
      return 0;
   }
   ODBparser.closeFile();
   	return 1;
}

int database::mapTypeNameToTypeId(char *tpnm,int set){
	if (strcmp(tpnm,_INT_NM_)==0) 									return _INT_;
   if (strcmp(tpnm,_REAL_NM_)==0) 									return _REAL_;
   if (strcmp(tpnm,_CHAR_NM_)==0) 									return _CHAR_;
   if (strcmp(tpnm,_OBJECT_NM_)==0) 								return _OBJECT_;
   if ((strcmp(tpnm,_COLLECTION_NM_)==0)||
   	((strcmp(tpnm,_OBJECT_NM_)==0)&&(set==1))) 				return _OBJECT_COLLECTION_;
   if ((strcmp(tpnm,_INT_COLLECTION_NM_)==0)||
   	((strcmp(tpnm,_INT_NM_)==0)&&(set==1))) 					return _INT_COLLECTION_;
   if ((strcmp(tpnm,_REAL_COLLECTION_NM_)==0)||
   	((strcmp(tpnm,_REAL_NM_)==0)&&(set==1))) 					return _REAL_COLLECTION_;
   if ((strcmp(tpnm,_CHAR_COLLECTION_NM_)==0)||
   	((strcmp(tpnm,_CHAR_NM_)==0)&&(set==1))) 					return _CHAR_COLLECTION_;
   if ((strcmp(tpnm,_SET_COLLECTION_NM_)==0)||
   	((strcmp(tpnm,_SET_COLLECTION_NM_)==0)&&(set==1))) 	return _SET_COLLECTION_;
   if ((gettypenamed(tpnm)!=NULL)&&(set!=1))						return _OBJECT_;
   if ((gettypenamed(tpnm)!=NULL)&&(set==1))						return _OBJECT_COLLECTION_;
   return _ERRORTYPE_;
}

char *database::mapTypeIdToTypeName(int tpid){
	if (_INT_==tpid) 									return _INT_NM_;
   if (_REAL_==tpid) 								return _REAL_NM_;
   if (_CHAR_==tpid) 								return _CHAR_NM_;
   if (_OBJECT_==tpid) 								return _OBJECT_NM_;
   if (_COLLECTION_==tpid) 						return _COLLECTION_NM_;
   if (_INT_COLLECTION_==tpid) 					return _INT_COLLECTION_NM_;
   if (_REAL_COLLECTION_==tpid) 					return _REAL_COLLECTION_NM_;
   if (_CHAR_COLLECTION_==tpid) 					return _CHAR_COLLECTION_NM_;
   if (_SET_COLLECTION_==tpid) 					return _SET_COLLECTION_NM_;
   return _ERRORTYPE_NM_;
}

char *database::getODBQLError(void){
	return ODBparser.getODBQLError();
}

