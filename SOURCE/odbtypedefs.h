//----------------------------------------------------
//	Project		:	ODB
//	File		:	odbtypedefs.h
//	Description	:	Definition of ODBQL parser
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#ifndef _ODBTYPEDFS_
#define _ODBTYPEDFS_

#define NAME_LENGTH             20
#define FILENAME_LENGTH         255

class object;
class collection;
class query_stream;
class type;
class semi_join_stream;
class select_stream;
class database;
class property_value;

typedef signed long int 	ODB_INT;
typedef float 	       		ODB_REAL;
typedef signed long int 	ODB_OID;
typedef char * 	       		ODB_CHAR;
typedef object *       		ODB_REF;
typedef collection *		ODB_SET;
typedef query_stream *		ODB_QSTREAM;
typedef select_stream *		ODB_SSTREAM;
typedef semi_join_stream *	ODB_SJSTREAM;
typedef database       		ODB_DB;
typedef database * 	       	ODB_DB_HANDLE;
typedef type *		       	ODB_TYPE;
typedef int 		       	ODB_TYPE_TAG;
typedef char * 		       	ODB_NAME;
typedef property_value * 	ODB_PROP;
#endif







