//----------------------------------------------------
//	Project		:	ODB
//	File		:	parser.h
//	Description	:	Definition of ODBQL parser
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#ifndef _ODBPARSER_
#define _ODBPARSER_


#include <stdio.h>

//
// Define the scanner tokens
//
#define OKTOKEN 		-2
#define NOTOKEN 		-1
#define ERRORTOKEN      0
#define NAMETOKEN		1
#define CREATETOKEN		2
#define DATABASETOKEN	3
#define TABLETOKEN      4
#define SIZETOKEN       5
#define EQUALTOKEN      6
#define SCOLONTOKEN     7
#define INTEGERTOKEN    8
#define REALTOKEN       9
#define LOADTOKEN       10
#define SAVETOKEN       11
#define FROMTOKEN       12
#define ASTOKEN         13
#define TYPETOKEN       14
#define SUBTYPETOKEN    15
#define OFTOKEN         16
#define PROPERTIESTOKEN 17
#define ARROWTOKEN      18
#define ONTOKEN         19
#define INSTANCETOKEN   20
#define SETTOKEN        21
#define INDEXTOKEN      22
#define DCOLONTOKEN     23
#define BANGTOKEN		24
#define QUOTETOKEN		25
#define PLUSTOKEN		26
#define MINUSTOKEN		27
#define STARTOKEN		28
#define SLASHTOKEN		29
#define LESSTHANTOKEN	30
#define GREATERTHANTOKEN 31
#define EQUALSTOKEN		32
#define LEFTPARTOKEN	33
#define RIGHTPARTOKEN	34
#define LEFTBRACKTOKEN	35
#define RIGHTBRACKTOKEN	36
#define LEFTCURLTOKEN	37
#define RIGHTCURLTOKEN	38
#define COMMATOKEN      39
#define DOTTOKEN        40
#define EOFTOKEN        41
#define VARIABLETOKEN   42
#define ISTOKEN         43



//
// Token names corr to tokens above
//
#define NAMESTRG
#define CREATESTRG		"create"
#define DATABASESTRG	"database"
#define TABLESTRG      	"table"
#define SIZESTRG        "size"
#define EQUALSTRG       "="
#define SCOLONSTRG      ";"
#define INTEGERSTRG
#define REALSTRG
#define LOADSTRG        "load"
#define SAVESTRG        "save"
#define FROMSTRG        "from"
#define ASSTRG          "as"
#define TYPESTRG        "type"
#define SUBTYPESTRG     "subtype"
#define OFSTRG          "of"
#define PROPERTIESSTRG  "properties"
#define ARROWSTRG       "->"
#define ONSTRG          "on"
#define INSTANCESTRG    "instance"
#define SETSTRG         "set"
#define INDEXSTRG       "index"
#define ISSTRG          "is"
#define VARIABLESTRG    "variable"
#define DCOLONSTRG      ":"
#define BANGSTRG		"!"
#define QUOTESTRG		"\""
#define PLUSSTRG		"+"
#define MINUSSTRG		"-"
#define STARSTRG		"*"
#define SLASHSTRG		"/"
#define LESSTHANSTRG	"<"
#define GREATERTHANSTRG	">"
#define EQUALSTRG		"="
#define LEFTPARSTRG		"("
#define RIGHTPARSTRG	")"
#define LEFTBRACKSTRG	"["
#define RIGHTBRACKSTRG	"]"
#define LEFTCURLSTRG	"{"
#define RIGHTCURLSTRG	"}"
#define COMMASTRG       ","
#define DOTSTRG         "."
#define EOFSTRG         "EOF"

#define PARSEERROR 		-1
#define PARSERERROR 	-2
#define SEMANTICERROR	-3




//
//Syntax Description
//
#define NOF_KEYWORDS    44
#define NOF_DELIMITERS  19
#define INPBUFLENGTH    256
#define CHARSTRINGLENGTH 1024
#define NAMELENGTH		64
#define OPEN 			1
#define CLOSED 			0



//
// The following are to tell createPropertiesrule how to parse
//
#define ASSIGNMENT 		0
#define DEFINITION		1

//
// Semantic errors
//
#define UNDEFINEDTYPE			0
#define CREATETYPEERROR			1
#define UNKNOWNTYPE				2
#define UNKNOWNPROPERTY			3
#define TYPEMISMATCH			4
#define PROPERTYEXIST			5
#define UNDEFINEDVARIABLE		6
#define ILLEGALSET				7
//#define TYPEMISMATCH      8

#define LINENUMBERSTR			"In statement ending at line "
#define UNDEFINEDTYPESTR		"Undefined return type: "
#define CREATETYPEERRORSTR		"Cannot Create type: "
#define UNKNOWNTYPESTR			"Undefined type: "
#define UNKNOWNPROPERTYSTR		"Undefined property: "
//#define TYPEMISMATCHSTR		  "Type mismatch: "
#define PROPERTYEXISTSTR		"Property already exist: "
#define UNDEFINEDVARIABLESTR	"Undefined variable: "
#define ILLEGALSETSTR			"Illegal assignment of: "
#define TYPEMISMATCHSTR			"Type mismatch, incompatible types: "

#include "..\source\odbtypedefs.h"


//
// fwd refferences
//
class database; 
class type;
class object;
class collection;

typedef int ODBTokenType;





//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
class variable
{
private:
	char			name[NAMELENGTH];
   int				tp;
   ODB_TYPE			typeref;
   ODB_INT			intval;
   ODB_REAL			realval;
   ODB_SET		  	setval;
   ODB_CHAR			charval;
   ODB_REF			objval;
   void				generateName(void);

public:
	int				autoEnum;
	variable(void);
	variable(char *tpnm, int tp, ODB_TYPE r);
   ~variable(void);
   char 			*getName(void);
   int				getType(void);
   ODB_TYPE			getTypeRef(void);
   ODB_INT			getIntval(void);
   ODB_REAL			getRealval(void);
   ODB_SET			getSetval(void);
   ODB_CHAR			getCharval(void);
   ODB_REF			getObjval(void);
   void 			setName(char *);
   void				setType(int);
   void				setTypeRef(ODB_TYPE);
   void				setIntval(ODB_INT);
   void				setRealval(ODB_REAL);
   void	  			setSetval(ODB_SET);
   void				setCharval(ODB_CHAR);
   void				setObjval(ODB_REF);
};





//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
class PropertyDef
{
private:
	char 			tp[NAMELENGTH];
   char 			nm[NAMELENGTH];
   int				datatype;
   int				set;
   ODB_SET			setval;
public:
					PropertyDef();
					PropertyDef(char *t, char *n);
					PropertyDef(char *t, char *n, int tp);
					~PropertyDef();
   void setSet(int s){set=s;};
   int getSet(void)	{return set;};
   char 			*getName(void);
   void 			setName(char *t);
   char 			*getPropName(void);
   void 			setPropName(char *p);
   int				getDataType(void);
   void				setDataType(int i);
   int				setsetval(ODB_SET sv);
   ODB_SET			getsetval(void){return setval;};
};




//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
//---------------------------------------------------------------
// Templates which could be moved to separate file
//---------------------------------------------------------------
template <class T> class LinkedList {
private:
	T 				*element;
   LinkedList 		*next;
public:
					LinkedList();
					LinkedList(T *elem);
					~LinkedList();
   LinkedList 		*getNextListElem();
   void 			setNextListElem(LinkedList *ne);
   T 				*getElem();
   int 				insertElem(T *elem);
   T			    *getElemNamed(char *nm);
};





//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
#ifdef __DLL__
class _export scanner
#else
class scanner
#endif
{
private:
	FILE 			*inputfile;
   char 			*fileName;
   char 			inpbuf[INPBUFLENGTH];
   char 			currentName[INPBUFLENGTH];
   char 			currentCh;
   int 				state;
   char 			delimiters[NOF_DELIMITERS];
   char 			*keyWord[NOF_KEYWORDS];
	int 			whiteChar(char ch);          // returns 0 if ch char or number
   int 				delimitingChar(char ch);     // returns non 0 if char is a delimiter
   ODBTokenType 	mapStringToToken(char *str); // maps the string into a token
   ODBTokenType 	nextToken;
   void 			setNextToken(ODBTokenType t){ nextToken=t;};
   int 				hasNextToken(void);
   ODBTokenType 	useNextToken(void);
   void 			removeComment(void);
   int 				comment;
   int 				lineNo;
   void 			setLineNo(int i){lineNo=i;};
   void 			enumLineNo(void){lineNo++;};

public:
	scanner();
   ~scanner();
   ODBTokenType 	getNextToken();
   int 				openFile(char *fnm);
   char 			*tokenToStr(int t){ return keyWord[t];};
   void 			closeFile(void);
   int 				getLineNo(void){ return lineNo;};
   int 				putBackToken(ODBTokenType t);
   int 				scanString(ODBTokenType t,char * str);
};





//--------------------------------------------------
// Class
// Description
// Author		staffan flodin
//--------------------------------------------------
#ifdef __DLL__
class _export parser
#else
class parser
#endif
{
private:
	char 			fileName[INPBUFLENGTH];
   char				errorMsg[INPBUFLENGTH];
	scanner 		ODBscanner;

   //-----------------------------------
   // Parse rules
   //-----------------------------------
   int 				createTypeRule(ODBTokenType t);
   int 				createRule(ODBTokenType t,variable *);
   char 			*subtypeOfRule(ODBTokenType t);
   char				*setOfRule(ODBTokenType t);
   int 				createPropertiesRule(ODBTokenType t);
   LinkedList<PropertyDef> *propertiesRule(ODBTokenType t,int how);
   LinkedList<variable> *variables;
   int 				createInstanceRule(ODBTokenType t,variable *);
   int 				createVariableRule(ODBTokenType t);
   ODBTokenType 	parseValue(ODBTokenType nextt,char *propnm);
   int              setRule(ODBTokenType nextt);
   ODB_SET			parseSet(ODBTokenType t);
   //-----------------------------------
   // Error handling
   //-----------------------------------
   int 				expectedToken;
   int 				actualToken;
   void 			parseError(int et, int at, int error);
   void				semanticError(int ec, char *em, char *am);

   database 		*odb;
public:
	parser(void);
   ~parser(void);
	void           setDatabase(database *d){odb=d;};
   int 				parseFile(char *fnm);
   void 			closeFile(void);
   int 				getLineNo(void){ return ODBscanner.getLineNo();};
   void 			displayError(void);
   char 			*getODBQLError(void);
};




#endif





