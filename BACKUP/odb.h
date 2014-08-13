//----------------------------------------------------
//	Project		:	ODB
//	File		:	store.h
//	Description	:	Definition of database engine
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#ifdef _SOODB_
#else
#define _SOODB_

//
//Type identifiers
//
#define _INT_ 					0
#define _REAL_ 					1
#define _CHAR_ 					3
#define _OBJECT_ 				4
#define _COLLECTION_ 			5
#define _OBJECT_COLLECTION_ 	5
#define _INT_COLLECTION_ 		6
#define _REAL_COLLECTION_ 		7
#define _CHAR_COLLECTION_ 		8
#define _SET_COLLECTION_ 		9
#define _VARIABLE_ 				10

//
//Type names
//
#define _INT_NM_ 				"integer"
#define _REAL_NM_ 				"real"
#define _CHAR_NM_ 				"charstring"
#define _OBJECT_NM_ 			"object reference"
#define _COLLECTION_NM_ 		"object collection"
#define _INT_COLLECTION_NM_ 	"integer collection"
#define _REAL_COLLECTION_NM_ 	"real collection"
#define _CHAR_COLLECTION_NM_ 	"charstring collection"
#define _SET_COLLECTION_NM_ 	"set collection"
#define _ERRORTYPE_NM_			"Unknown type"

#define _ERRORTYPE_ 			-1
#define _ALL_ 					-3    //_ALL_ for scanning whole table
#define _BUCKET_ 				-4 		// for scanning bucket only
#define _NoName_ 				"NoName"
#define NO_PROPS 				0
#define WITH_PROPS 				1

//
//Tags for saving and loading database to file
//
#define _DBfileID_ 				"Odb_Version_0.1_Database_file"
#define _DBfileID_Length 		29
#define _Typestart_ 			"Type"
#define _Typeend_ 				"/Type"
#define _Propstart_ 			"Props"
#define _Propend_ 				"/Props"
#define _Indexstart_ 			"Index"
#define _Indexend_ 				"/Index"
#define _Usertypes_ 			"Usertypes"
#define _Subtypestart_ 			"Subtype_of"
#define _Databaseend_ 			"/Database"
#define _Populationstart_ 		"Objects"
#define _Populationend_ 		"/Objects"
#define _Objstart_ 				"Instance"
#define _Objend_ 				"/Instance"
#define _Setstart_ 				"{"
#define _Setend_ 				"}"
#define _NULL_ 					"NULL"
#define _OID_ 					"OID"

#define FULL					1
#define PARTIAL					2


#include <iostream.h>
#include <fstream.h>
#include <ctype.h>
#include "..\source\odbtypedefs.h"
#include "..\source\parser.h"

//
//forward declarations
//
class type;
class object;
class database;
class query_stream;
class select_stream;
class semi_join_stream;
class collection;
class property_value;



//class definitios

//--------------------------------------------------
// Class		validation
// Description	A validation object can validate any
//				string wrt to the following types:
//				integer,real,ODB names, sets
// Author		staffan flodin
//--------------------------------------------------

class _import validation

{
private:
	database *db;
   int 					validdigit(char ch);
public:
	validation(void);
   validation(database *d);
   ~validation(void);
   void 					setDb(database *d){ db=d; };
	int 					validate_integer(char *inp,ODB_INT &value);
	int 					validate_real(char *inp,ODB_REAL &value);
	int 					validate_name(char *ch);
	int 					validate_set(char *inp,ODB_SET set);
};





//--------------------------------------------------
// Class			outputbuffer
// Description		A buffer into wich all ODB kernel
//					methods write, the buffer can then
//					be written into a widget such as
//					a datafield etc.
// Author			staffan flodin
//--------------------------------------------------

class _import outputbuffer

{
private:
	char 					buf[1024];
	char 					*currpos;
	int 					len,maxsize;
public:
	outputbuffer(void);
	char 					*write_request(int maxlen);
	char 					*get_buffer(void);
	void 					reset_buffer(void);
};





//--------------------------------------------------
// Class		collection
// Description	ODB set class
// Author		staffan flodin
//--------------------------------------------------

class _import collection

{
private:
	int 					type;
	ODB_REF 				obj;
	ODB_CHAR 				charstring;
	ODB_SET 				setval;
	ODB_INT 				intval;
	ODB_REAL 				realval;
	ODB_SET 				next;
	int 					empty;
	void 					displayelements(outputbuffer &buf);
public:
	collection (int tp);
   collection(void);
   ~collection(void);

	int 					is_empty(void);
	int 					insert(ODB_REF obj);
	int 					insert(ODB_CHAR charstring);
	int 					insert(ODB_SET setval);
	int 					insert(ODB_INT intval);
	int 					insert(ODB_REAL realval);
	int 					delete_element(ODB_REF obj);
	int 					delete_element(ODB_INT i);
	int 					delete_element(ODB_REAL r);
	int 					delete_element(ODB_CHAR c);
	void 					delete_set(void);
	void 					null_reference_to(ODB_REF o);
	int 					gettype(void);
	ODB_INT 				getintval(void);
	ODB_CHAR 			getcharstringval(void);
	ODB_REAL 			getrealval(void);
	ODB_REF 				getobj(void);
	ODB_SET 				getsetval(void);
	ODB_SET 				getnext(void);
	int 					set_next_to(ODB_SET sv);
	void 					display(outputbuffer &buf);
	int 					member(ODB_INT i);
	int 					member(ODB_CHAR c);
	int 					member(ODB_REAL r);
	int 					member(ODB_REF o); //not impl
	int 					member(ODB_SET s);
	int 					equal(ODB_SET s);
	int 					saveset(ofstream *databasefile);
	int 					populatefromfile(ifstream *databasefile,int datatype,struct tblentry *maptable,database *db);
};





//--------------------------------------------------
// Class		property value
// Description	a linked list of object and type props
// Author		staffan flodin
//--------------------------------------------------
class property_value
{
private:
	int 				valuetype;
	ODB_INT 			intval;
	ODB_REAL 			realval;
	ODB_CHAR 			charstring;
	ODB_REF 			elem;
	ODB_SET 			setval;
	ODB_NAME 			name;
	ODB_PROP 			nextprop;
	ODB_TYPE			objectType;
	int 				setsize(int val);
public:
						property_value(int t, ODB_NAME nm, database *db);
						property_value(void);
						~property_value(void);

	int 				gettype(void);
	ODB_INT 			getintval(void);
	ODB_CHAR 			getcharstringval(void);
	ODB_REAL 			getrealval(void);
	ODB_REF 			getelem(void);
	ODB_SET 			getsetval(void);
	ODB_NAME 			getname(void);
	ODB_PROP 			nextproperty(void);
	int 				setintval(ODB_INT val);
	int 				setcharstringval(ODB_CHAR val, database *db);
	int 				setrealval(ODB_REAL val);
	int 				setelem(ODB_REF val);
	int 				setsetval(ODB_SET sv);
	int 				setnextto(ODB_PROP next);
	int 				changenextto(ODB_PROP next);
	ODB_PROP 			copy_prop(database *db);
	void 				display(outputbuffer &buf);
	void				display(outputbuffer &buf,int how);
	void 				fulldisplay(outputbuffer &buf);
	int 				saveprop(ofstream *databasefile);
	ODB_TYPE			getObjectType(void){return objectType;};
};





//--------------------------------------------------
// Class		object
// Description	the basic ODB object
// Author		staffan flodin
//--------------------------------------------------

class _import object

{
protected:
	ODB_OID 			OID;
	ODB_PROP 			props;
	type 				*typeof;

 public:
 	object(void);
	object(type *tp, database *db);
   ~object(void);

	ODB_OID 			oid(void);
	void 				display(outputbuffer &buf);
	void 				displayoid(outputbuffer &buf);
	void				fulldisplay(outputbuffer &buf);
	int 				getoid(void);
	ODB_REF 			add_newproperty(int tp, ODB_NAME nm);
	ODB_PROP 			putprop(ODB_PROP newprop);
	ODB_PROP 			setproperty_value(ODB_NAME name,ODB_INT intval);
	ODB_PROP 			setproperty_value(ODB_NAME name,ODB_REAL val);
	ODB_PROP 			setproperty_value(ODB_NAME name,ODB_CHAR val);
	ODB_PROP 			setproperty_value(ODB_NAME name,ODB_REF refobj);
	ODB_PROP 			setproperty_value(ODB_NAME name,ODB_SET sv);
	ODB_PROP 			getpropertynamed(ODB_NAME name);
	void 				deleteobject(void);
	int 				saveobject(ofstream *databasefile,int how);
	int 				delete_instanceproperty(ODB_NAME propnm);
	ODB_TYPE			getTypeofObject(void){return typeof;};
};




//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
class linked_stringlist
{private:
	linked_stringlist	*nextobj;
	ODB_NAME 			str;
	void 				set_next_to(linked_stringlist *li);
public:
						linked_stringlist();
						linked_stringlist(ODB_NAME st);
	linked_stringlist	*insert(ODB_NAME st);

	ODB_NAME 			getstring(void);
	linked_stringlist	*next(void);
};





//--------------------------------------------------
// Class		linked list
// Description	Holds the elements in hach tables
// Author		staffan flodin
//--------------------------------------------------
class linked_list
{private:
	linked_list 		*nextobj;
	ODB_REF 			element;

public:
						~linked_list(void);
						linked_list(ODB_REF o);
						linked_list(void);

	void 				set_next_to(linked_list *li);
	linked_list 		*next(void);
	ODB_REF 			elem(void);
	linked_list 		*insert(ODB_REF o);
	ODB_REF 			findOID(ODB_OID OID);
	void 				display(outputbuffer &buf);
	int 				empty(void);
	void 				null_elem(void);
};





//--------------------------------------------------
// Class		string table
// Description	thew ODB string table where all strings
//				are stored
// Author		staffan flodin
//--------------------------------------------------
class stringtable
{private:
	int 				tablesize;
	int 				elemsize;
	linked_stringlist	*tbl;
public:
	stringtable(int size);
   stringtable(void);
   ~stringtable(void){};

	hashfn(ODB_NAME prop);
	ODB_NAME 		put(ODB_NAME str);
	ODB_NAME 		get(ODB_NAME str);
};





//--------------------------------------------------
// Class		table
// Description	hash table
// Author		staffan flodin
//--------------------------------------------------
class table
{private:
	int 				tablesize;
	int 				elemsize;
	linked_list 		*tbl;
	//for scanning, used by scan, openscan
	//int currbuck;
	//int scantype;
	//linked_list *currlst;

public:
						table(int size);
						table(void);
						~table(void){};

	int 				hashfn(object o);
	int 				hashfn(ODB_REF obj);
	int 				hashfn(ODB_INT oid);
	int 				hashfn(ODB_REAL realval);
	int 				hashfn(ODB_CHAR str);
	int 				hashfn(ODB_SET sv);
	int 				openscan(int how,query_stream *qst);
						//how =_ALL_ scans table
			      		// otherwise just bucket number how
	ODB_REF 			getnextobj(query_stream *qst);
	object 				put(ODB_REF o);
	object 				put(ODB_REF o, ODB_NAME prop);//for index
	void 				drop_index(void); //no object deletion
	void 				drop_table(void); //deletes all objects in the table
	void 				display(outputbuffer &buf);
	ODB_REF 			get(query_stream *qst);
	ODB_REF 			getnext(query_stream *qst);
	int 				remove(ODB_REF obj);
	int 				saveobjects(ofstream *databasefile,int how);
};






//--------------------------------------------------
// Struct		indextp
// Description  list of indexes of a certain type
// Author		staffan flodin
//--------------------------------------------------
struct indextp {
	char 				name[20];
	table 				*tbl;
	indextp 			*next;
};






//--------------------------------------------------
// Struct		typestp
// Description	the auxiliary extent list built over
//				the type tree
// Author		staffan flodin
//--------------------------------------------------
struct typestp {
	type 				*tp;
	typestp 			*next;
};





//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
struct tblentry {
	ODB_REF 			newobj;
	char 				prop[20];
	int 				refoid;
};






//--------------------------------------------------
// Class		type
// Description	the ODB type class
// Author		staffan flodin
//--------------------------------------------------

class _import type : public object

{ private:
	int 				card;
	char 				name[20];
	table 				*extent;
	indextp 			*index;
	typestp 			*supertypes;
	typestp 			*subtypes;
	indextp 			*makenewindex(ODB_NAME nm);
	indextp 			*populateindex(ODB_NAME nm,indextp *thenewindex);
	int 				insertintoallindex(ODB_REF obj);
	database 			*belongsto;
	ODB_PROP 			inherited_props;
	typestp 			*ext_start, *ext_end, *curr_ext;
	typestp 			*curr_stp;  //for scanning all subtypes, one by one
	ODB_PROP 			propptr; //for scanning all properties

 public:
	type(void);
	type(ODB_NAME nm, database *db);
	type(ODB_NAME nm, type *super, database *db);
   ~type(void);

	void 				delete_type(void);
	void 				remove_subtype(type *t);
	type 				*getsupertype(void);
	int 				set_belongsto(database *db);
	int 				addsubtype(type *tp, typestp *extp);
	int 				readdsubtype(typestp *extp);
	int 				extend_extent(typestp *extptr);
	void 				rebuild_extentlist(void);
	type 				*add_property(int tp, ODB_NAME nm);
	type 				*add_inheritedproperty(int tp, ODB_NAME nm);
	int 				drop_index(ODB_NAME propnm);
	void 				display(outputbuffer &buf);
	void 				displayname(outputbuffer &buf);
	ODB_PROP 			getprops(void);
	ODB_PROP 			get_noninherited_propertynamed(ODB_NAME nm);
	void 				delete_inherited_props(ODB_NAME pnm);
	ODB_REF 			create_instance(void);
	void 				displayextent(outputbuffer &buf);
	int 				build_index(ODB_NAME prop);
	table 				*getindexnamed(ODB_NAME nm);
	table 				*getextent(void);
	query_stream 		*open(void);
	query_stream 		*open(ODB_NAME nm,ODB_NAME op,ODB_INT intval);
	query_stream 		*open(ODB_NAME nm,ODB_NAME op,ODB_REAL realval);
	query_stream 		*open(ODB_NAME nm,ODB_CHAR charstringval);
	query_stream 		*open(ODB_NAME nm,ODB_REF objval);
	query_stream 		*open(ODB_NAME nm,ODB_SET sv);
	int 				remove_all_references_to(ODB_REF o);
	int 				deleteobject(ODB_REF obj);
	int 				savetypes(ofstream *databasefile);
	int 				saveobjects(ofstream *databasefile,int how);
	type 				*gettypenamed(ODB_NAME nm);
	database 			*getdb(void);
	ODB_NAME 			getname(void);
	//next a bunch of fns to scan subtypes, properties, deep extent ...
	ODB_NAME 			getallsubtypes(void);
	ODB_NAME 			getnextsubtype(void);
	ODB_NAME 			getsubtypes(void);
	ODB_NAME 			getsubtype(void);
	ODB_NAME 			getfirstprop(void);
	ODB_NAME 			getnextprop(void);
	int 				validate(ODB_NAME inp,void *obj);
};





//--------------------------------------------------
// Class		query strem
// Description	the general stream class
// Author		staffan flodin
//--------------------------------------------------

class _import query_stream

{protected:
	int 				bucket;		//offset in table
	int 				scantype;	//index use or scan
	linked_list 		*lst;		//actual object holder in table
	type 				*ctp;		//current type being accessed
	table 				*tbl;
	typestp 			*exs, *exe, *curre;
	ODB_REF 			currobj;
	int 				datatype;
	ODB_NAME 			prop;
	ODB_INT 			intval;
	ODB_REAL 			realval;
	ODB_CHAR 			charstringval;
	ODB_REF 			objval;
	ODB_SET 			setval;
	ODB_NAME 			op;
	int 				eval_op(ODB_NAME op,ODB_INT objval,ODB_INT cmpval);
	int 				eval_op(ODB_NAME op,ODB_REAL objval,ODB_REAL cmpval);
	int 				eval(ODB_SET sv,ODB_INT cmpval);
	int 				eval(ODB_SET sv,ODB_REAL cmpval);
	int 				eval(ODB_SET sv,ODB_CHAR cmpval);
	int 				eval(ODB_SET sv,ODB_REF cmpval);
	int 				eval(ODB_SET sv,ODB_SET cmpval);
	int 				evalsetexpr(ODB_SET sv);
	ODB_REF 			getint(ODB_INT inval);
	ODB_REF 			getreal(ODB_REAL realval);
	ODB_REF 			getcharstring(ODB_CHAR charstringval);
	ODB_REF 			getobj(ODB_REF objval);
	ODB_REF   			getset();
	void   				move_to_next_extent(ODB_INT iv,type *tptr);
	void   				move_to_next_extent(ODB_REAL rv,type *tptr);
	void   				move_to_next_extent(ODB_CHAR cv,type *tptr);
	void   				move_to_next_extent(ODB_REF ov,type *tptr);
	void   				move_to_next_extent(ODB_SET sv,type *tptr);
	void  				move_to_next_extent(type *tptr);
public:
	//initializers and modifiers
	query_stream(void);
   ~query_stream(void){};

	int  				set_operator(ODB_NAME op);
	int  				set_currbuck(int i);
	int  				set_currlst(linked_list *l);
	int  				set_scantype(int s);
	int  				set_currobj(ODB_REF o);
	int  				set_currtype(type *t);
	int  				set_table(table *t);
	int  				set_extentstart(typestp *es);
	int  				set_extentend(typestp *ee);
	int  				set_datatype(int i);
	int  				set_prop(ODB_NAME p);
	int  				set_intval(ODB_INT i);
	int 				set_realval(ODB_REAL f);
	int 				set_charstringval(ODB_CHAR cv);
	int 				set_objval(ODB_REF o);
	int 				set_setval(ODB_SET s);

	//retrievers
	ODB_NAME 			get_operator(void);
	int 				get_currbuck(void);
	linked_list 		*get_currlst(void);
	int 				get_scantype(void);
	ODB_REF 			get_currobj(void);
	type 				*get_currtype(void);
	table 				*get_table(void);
	ODB_NAME 			get_prop(void);
	ODB_INT 			get_intval(void);
	ODB_REAL 			get_realval(void);
	ODB_CHAR 			get_charstringval(void);
	ODB_REF 			get_objval(void);
	ODB_SET 			get_setval(void);
	int 				get_datatype(void);

	//query primitives
	virtual ODB_REF		get(void);
	virtual void 		close(void);
	virtual int 		reset(void);
	//open other streams on this
	select_stream 		*open_select_stream(ODB_NAME prop,ODB_NAME oop,ODB_INT intval);
	select_stream 		*open_select_stream(ODB_NAME prop,ODB_NAME oop, ODB_REAL realval);
	select_stream 		*open_select_stream(ODB_NAME prop,ODB_CHAR charstringval);
	select_stream 		*open_select_stream(ODB_NAME prop,ODB_REF objval);
	select_stream 		*open_select_stream(ODB_NAME pr,ODB_SET sv);
	semi_join_stream	*open_sj_stream(ODB_NAME prop1, query_stream *st2, ODB_NAME prop2);

	//misc
	void 				display(outputbuffer &buf);
};





//--------------------------------------------------
// Class		select stream
// Description	a stream that selects over properties
// Author		staffan flodin
//--------------------------------------------------

class _import select_stream : public query_stream

{private:
	query_stream 		*qs;
	ODB_REF 			getint(void);
	ODB_REF 			getreal(void);
	ODB_REF 			getcharstring(void);
	ODB_REF 			getobj(void);
	ODB_REF 			getset(void);
public:
	select_stream(void);
   ~select_stream(void){};
	select_stream(query_stream *q,ODB_NAME nm,ODB_NAME oop,ODB_INT i);
	select_stream(query_stream *q,ODB_NAME nm,ODB_NAME oop,ODB_REAL r);
	select_stream(query_stream *q,ODB_NAME nm,ODB_CHAR chrstr);
	select_stream(query_stream *q,ODB_NAME nm,ODB_REF ob);
	select_stream(query_stream *q,ODB_NAME nm,ODB_SET sv);

	ODB_REF 			get();
	void 				close(void);
	int 				reset(void);
};





//--------------------------------------------------
// Class		semi join strem
// Description	a stream that joins two streams
// Author		staffan flodin
//--------------------------------------------------

class _import semi_join_stream : public query_stream

{private:
	query_stream 		*lops; //left operand stream;
	query_stream 		*rops; //right operand stream
	ODB_NAME 			rprop;
	ODB_REF 			getint(void);
	ODB_REF 			getreal(void);
	ODB_REF 			getcharstring(void);
	ODB_REF 			getobj(void);
	object				*getobjspecial(void);
	int 				set_rprop(ODB_NAME p);
	ODB_NAME 			get_rprop(void);
public:
	semi_join_stream(void);
	semi_join_stream(query_stream *lop,ODB_NAME lnm,query_stream *rop,ODB_NAME rnm);
   ~semi_join_stream(void){};

	ODB_REF 			get();
	void 				close(void);
	int 				reset(void);
};





//--------------------------------------------------
// Class		database
// Description	the umbrella class that ties everything
//				together
// Author		staffan flodin
//--------------------------------------------------

class _import database
{
private:
	ODB_NAME 			name;
	ODB_OID 			global_oid;
	int 				default_tablesize;
	type 				usertypes;
	int 				makefilename(ODB_NAME nm, ODB_NAME filename);
	stringtable 		*strtbl;
	char 				inpbuf[1024];
   validation 			validationObject;
   parser				ODBparser;

public:
	database(ODB_NAME nm);		//constructor
	database(void);				//default constructor
   ~database(void);

	ODB_OID 			generate_oid(void);
	int 				get_tablesize(void);
	ODB_NAME 			get_dbname(void);
	type 				*get_usertypes(void);
	type 				*create_type(ODB_NAME nm);
	type 				*create_type(ODB_NAME nm, ODB_NAME supt);
	void 				display(outputbuffer &buf);
	int 				save_database(ODB_NAME nm);
	int 				load_database(ODB_NAME nm);
	type 				*gettypenamed(ODB_NAME nm);
   int					subtypeOf(char *supertype, char *subtype);
	int 				initialize(void);
	ODB_NAME 			getstring(ODB_NAME st);

	query_stream 		*open_stream(ODB_NAME tpname);
	query_stream 		*open_stream(ODB_NAME tpname, ODB_NAME propnm,ODB_NAME op,ODB_INT i);
	query_stream 		*open_stream(ODB_NAME tpname, ODB_NAME propnm,ODB_NAME op,ODB_REAL f);
	query_stream 		*open_stream(ODB_NAME tpname, ODB_NAME propnm,ODB_CHAR ch);
	query_stream 		*open_stream(ODB_NAME tpname, ODB_NAME propnm,ODB_REF o);
	query_stream 		*open_stream(ODB_NAME tpname, ODB_NAME propnm,ODB_SET o);
	select_stream 		*select(query_stream *qst,ODB_NAME propnm,ODB_NAME oop,ODB_INT i);
	select_stream 		*select(query_stream *qst,ODB_NAME propnm,ODB_NAME oop,ODB_REAL f);
	select_stream 		*select(query_stream *qst,ODB_NAME propnm,ODB_CHAR ch);
	select_stream 		*select(query_stream *qst,ODB_NAME propnm,ODB_REF o);
	select_stream 		*select(query_stream *qst,ODB_NAME propnm,ODB_SET o);

	semi_join_stream	*semi_join(query_stream *ls,ODB_NAME p1,query_stream *rs,ODB_NAME p2);

	ODB_REF 			get(query_stream *qst);
	int 				close_stream(query_stream *qst);
	int 				reset_stream(query_stream *qst);

	//The following pais of functions is for get metainfo
	//in a ony by one fasion.
	ODB_NAME 			getallsubtypes(void); //all subtypes
	ODB_NAME 			getnextsubtype(void); //continues the above
	ODB_NAME 			getsubtypes(ODB_NAME tpnm);//direct subtypes
	ODB_NAME 			getsubtype(ODB_NAME tpnm);
	ODB_NAME 			getfirstprop(ODB_NAME tpnm);
	ODB_NAME 			getnextprop(ODB_NAME tpnm);
	ODB_NAME 			getsupertype(ODB_NAME tpnm);
	int 				getpropertytype(ODB_NAME tpnm, ODB_NAME propnm);
	int 				add_property(ODB_NAME tpname, ODB_NAME prname,int tp);
	ODB_REF 			getobjectwithoid(ODB_NAME tpnm,ODB_OID oid);
	ODB_REF 			create_instance(ODB_NAME tpnm);
	ODB_NAME 			set_name(ODB_NAME nm);
	int 				set_tablesize(int s);
	int 				create_index(ODB_NAME tpnm, ODB_NAME propnm);
	int 				drop_index(ODB_NAME tpnm, ODB_NAME propnm);
	int 				delete_property(ODB_NAME tpnm,ODB_NAME propnm);
	int 				delete_type(ODB_NAME tpnm);
	int 				delete_object(ODB_REF o);

	//Updating property values
	int 			setproperty_value(ODB_REF o,ODB_NAME name,ODB_INT intval);
	int 			setproperty_value(ODB_REF o,ODB_NAME name,ODB_REAL realval);
	int 			setproperty_value(ODB_REF o,ODB_NAME name,ODB_CHAR charval);
	int 			setproperty_value(ODB_REF o,ODB_NAME name,ODB_REF refval);
	int 			setproperty_value(ODB_REF o,ODB_NAME name,ODB_SET setval);

	//retreiving property values
	int 			getproperty_value(ODB_REF o,ODB_NAME propnm, ODB_INT *iv);
	int 			getproperty_value(ODB_REF o,ODB_NAME propnm, ODB_REAL *rv);
	int 			getproperty_value(ODB_REF o,ODB_NAME propnm, ODB_REF *ov);
	int 			getproperty_value(ODB_REF o,ODB_NAME propnm, ODB_CHAR *nm);
	int 			getproperty_value(ODB_REF o,ODB_NAME propnm, ODB_SET *nm);

	//collection stuff
	ODB_SET 		create_collection(int tp);
	int 			collection_insert(ODB_SET coll,ODB_INT iv);
	int 			collection_insert(ODB_SET coll,ODB_REAL rv);
	int 			collection_insert(ODB_SET coll,ODB_SET sv);
	int 			collection_insert(ODB_SET coll,ODB_CHAR cv);
	int 			collection_insert(ODB_SET coll,ODB_REF ref);
	int 			delelete_collection(ODB_SET coll);

   //operators
   int 				operator >> (char *filename);
   int 				operator << (char *filenmae);

   //validation functions
   int 				validate(char *inp,ODB_INT &value);
   int 				validate(char *inp,ODB_REAL &value);
   int 				validate(char *ch);
   int 				validate(char *inp,ODB_SET set);

   // to parse ODBQL files
   int 				parseFile(char *fnm);
   int 				mapTypeNameToTypeId(char *tpnm,int set);
   char 			*mapTypeIdToTypeName(int tpid);
   char				*mapTypeNameToCollectionOfTypeName(char *tpnm);
   char 			*getODBQLError(void);

};



#endif
