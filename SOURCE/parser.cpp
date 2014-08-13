//----------------------------------------------------
//	Project		:	ODB and parselib
//	File		:	parser.cpp
//	Description	:	Implementation of ODBQL parser
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#include <string.h>
#include <iostream.h>
#include <malloc.h>
#include "parser.h"
#include "store.h"
#include "linkedListTmpl.cpp"




//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
parser::parser(void)
{
  
  memset(fileName,'\0',INPBUFLENGTH);
  memset(errorMsg,'\0',INPBUFLENGTH);
  variables=NULL;
  odb=NULL;
  
}





//----------------------------------------------------
//	Method		:	destructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
parser::~parser(void)
{
  closeFile();

  if (variables != NULL) delete variables;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void parser::closeFile(void)
{

  ODBscanner.closeFile();

  if (variables != NULL) delete variables;

  variables=NULL;

}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void parser::semanticError(int ec, char *em, char *am)
{

  memset(errorMsg,'\0',INPBUFLENGTH);

  switch(ec)
    {
    case UNDEFINEDTYPE:
      {
	sprintf(errorMsg,"\n%s%s for %s%s%i",UNDEFINEDTYPESTR,em,am,
		LINENUMBERSTR,getLineNo());
	break;
      }
    case UNDEFINEDVARIABLE:
      {
	sprintf(errorMsg,"\n%s%s%s%i",UNDEFINEDVARIABLESTR,em,LINENUMBERSTR,
		getLineNo());
	break;
      }
    case ILLEGALSET:
      {
	sprintf(errorMsg,"\n%s%s%s%i",ILLEGALSETSTR,em,LINENUMBERSTR,
		getLineNo());
	break;
      }
    case TYPEMISMATCH:
      {
	sprintf(errorMsg,"\n%s %s %s %s %i",TYPEMISMATCHSTR,em,am,LINENUMBERSTR,
		getLineNo());
	break;
      }
    case CREATETYPEERROR: 
      {
	sprintf(errorMsg,"\n%s%s%s%i",CREATETYPEERRORSTR,em,LINENUMBERSTR,
		getLineNo());
	break;
      }
    case UNKNOWNTYPE: 
      {
	sprintf(errorMsg,"\n%s%s%s%i",UNKNOWNTYPESTR,em,LINENUMBERSTR,
		getLineNo());
	break;
      }
    case PROPERTYEXIST:
      {
	sprintf(errorMsg,"\n%s%s on %s%s%i",PROPERTYEXISTSTR,em,am,
		LINENUMBERSTR,getLineNo());
	break;
      }
    case UNKNOWNPROPERTY:
      {
	sprintf(errorMsg,"\n%s %s.%s %s%i",UNKNOWNPROPERTYSTR,em,am,
		LINENUMBERSTR,getLineNo());
	break;
      }
    default:
      {
	sprintf(errorMsg,"\nInternal error. Error code:%i, %s%s%i",ec,em,
		LINENUMBERSTR,getLineNo());
	break;
      }
    }

  throw SEMANTICERROR;
}






//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void parser::parseError(int et, int at, int error)
{

  expectedToken=et;
  actualToken=at;
  memset(errorMsg,'\0',INPBUFLENGTH);

  if (expectedToken==NAMETOKEN)
    sprintf(errorMsg,"\nPARSE ERROR On line: %i Expected: a name Found: %s",
	    getLineNo(),ODBscanner.tokenToStr(actualToken));
  else
    sprintf(errorMsg,"\nPARSE ERROR On line: %i Expected: %s Found: %s",
	    getLineNo(),ODBscanner.tokenToStr(expectedToken),
	    ODBscanner.tokenToStr(actualToken));

  throw error;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void parser::displayError(void)
{

  cout<<errorMsg;
  cout.flush();

}





//----------------------------------------------------
//	Method		:	
//	Description	:	This is the entry function to ODBQL
//					parsing. The client creates a parser
//					objec and invokes parseFile 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int parser::parseFile(char *fnm)
{
  ODBTokenType nextt=0;
  int result=0;

  strcpy(fileName,fnm);
  ODBscanner.openFile(fileName);

  //
  // Get the first token
  //
  nextt=ODBscanner.getNextToken();


  //
  // In dispatch on what can occur at first level in ODBQL
  // e.g., create variable|instance|type ...
  //       set <variable> = <value>
  //
  while ((nextt!=EOFTOKEN)&&(result!=PARSEERROR)) 
    {
	
      switch(nextt)
	{
	case CREATETOKEN:
	  {
	    result=createRule(nextt,NULL);
	    break;
	  }
        
	case SETTOKEN:
	  {
	    result=setRule(nextt);
	    break;
	  }

	default:
	  {
	    parseError(CREATETOKEN,nextt,PARSEERROR);
	    break;
	  }
	}

      nextt=ODBscanner.getNextToken();
   
    }
 
  closeFile();

  return result;

}






//----------------------------------------------------
//	Method		:	
//	Description	:	set <variable>=<value>; 
//				or set <variable>.<property>=value;
//	Author		:	Staffan Flodin
//----------------------------------------------------
int parser::setRule(ODBTokenType nextt)
{
  ODBTokenType t;
  char vnm[NAMELENGTH],anm[NAMELENGTH],valueField[NAMELENGTH];
  variable *v,*automVar;
  ODB_TYPE tp;
  ODB_PROP prop;
  int propType,valueType=-1;
  ODB_INT intval;
  float realval;

  if (nextt!=SETTOKEN) parseError(SETTOKEN,nextt,PARSEERROR);

  t=ODBscanner.getNextToken();
  if (t!=NAMETOKEN) parseError(NAMETOKEN,t,PARSEERROR);
  else strcpy(vnm,ODBscanner.tokenToStr(t));

  v=variables->getElemNamed(vnm);
  if (v==NULL) semanticError(UNDEFINEDVARIABLE,vnm,NULL);

  t=ODBscanner.getNextToken();
  //-------------------------------------------------------------------
  // Two cases: either a dot meaning an attribute of the object
  //            or the equalsign
  //-------------------------------------------------------------------
  if (t==DOTTOKEN) 
    {
   
      t=ODBscanner.getNextToken();
      if (t!=NAMETOKEN)  parseError(NAMETOKEN,t,PARSEERROR);
      strcpy(anm,ODBscanner.tokenToStr(t));
      if (v->getType()!=_OBJECT_)
	semanticError(TYPEMISMATCH,odb->mapTypeIdToTypeName(v->getType()),
		      odb->mapTypeIdToTypeName(_OBJECT_));

      //-----------------------------------------------------------------
      // Now create a temporary variable of the same type as the
      // property and use it instaed of v. Later on dereference v and
      // assign the property the value of the variable
      //------------------------------------------------------------------
      tp=v->getTypeRef();
      prop=tp->getpropertynamed(anm);

      if (prop!=NULL) propType=prop->gettype();
      else semanticError(UNKNOWNPROPERTY,tp->getname(),anm);

      /* if (propType==_OBJECT_) 
	 automVar=new variable(NULL,propType,prop->getObjectType());
	 else automVar=new variable(NULL,propType,NULL);
	 */
      t=ODBscanner.getNextToken();
         
    }

  if (t!=EQUALTOKEN) parseError(EQUALTOKEN,t,PARSEERROR);

  t=ODBscanner.getNextToken();

  memset(valueField,NAMELENGTH,'\0');

  switch(t)
    {
    case CREATETOKEN:
      createRule(t,v);
      break;
	
    case NAMETOKEN:
      valueType=parseValue(t,valueField);
      t=ODBscanner.getNextToken();
      if (t!=SCOLONTOKEN) parseError(SCOLONTOKEN,t,PARSEERROR);
      break;

    case QUOTETOKEN:
      valueType=parseValue(t,valueField);
      t=ODBscanner.getNextToken();
      if (t!=SCOLONTOKEN) parseError(SCOLONTOKEN,t,PARSEERROR);
      break;

    case LEFTCURLTOKEN:
      valueType=parseValue(t,valueField);
      t=ODBscanner.getNextToken();
      if (t!=SCOLONTOKEN) parseError(SCOLONTOKEN,t,PARSEERROR);			
      break;

    default: parseError(CREATETOKEN,t,PARSEERROR);
   
    }

  //t=ODBscanner.getNextToken(); // should be a ;
  //if (t!=SCOLONTOKEN) parseError(SCOLONTOKEN,t,PARSEERROR);

  switch(valueType)
    {
    case _INT_: 
      odb->validate(valueField,intval);
      odb->setproperty_value(v->getObjval(),anm,intval);
      //automVar->setIntval(intval);
      break;
    case _REAL_: 
      odb->validate(valueField,realval);
      odb->setproperty_value(v->getObjval(),anm,realval);
      //automVar->setRealval(intval);
      break;
    case _CHAR_: 
      //automVar->setCharval(valueField);
      odb->setproperty_value(v->getObjval(),anm,valueField);
      break;
    default: break;
    }

  //delete automVar;

  return 1;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int parser::createRule(ODBTokenType t,variable *v)
{
  ODBTokenType nextt;

  if (t!=CREATETOKEN) parseError(CREATETOKEN,t,PARSERERROR);

  nextt=ODBscanner.getNextToken();

  if ((v!=NULL)&&(nextt!=INSTANCETOKEN)) semanticError(ILLEGALSET,v->getName(),NULL);

  switch(nextt)
    {
    case TYPETOKEN: return createTypeRule(nextt);
    case PROPERTIESTOKEN: return createPropertiesRule(nextt);
    case INSTANCETOKEN: return createInstanceRule(nextt,v);
    case VARIABLETOKEN: return createVariableRule(nextt);
    default: parseError(TYPETOKEN,nextt,PARSEERROR);
    }
  return 1;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
//-----------------------------------------------------------
//
//                 C R E A T E   T Y P E
// The syntax is:
// <create type>:=create type <name> [subtype of <nm>]
//                                   [properties <proplist>];
// <proplist>   :=<name> -> <name> [, <proplist>]
//-----------------------------------------------------------
int parser::createTypeRule(ODBTokenType t)
{
  ODBTokenType nextt;
  char tpnm[NAMELENGTH];
  char stpnm[NAMELENGTH];
  char errorStr[NAMELENGTH];
  char *st=NULL;
  LinkedList<PropertyDef> *props, *p;
  int ttp;
  ODB_TYPE createResult;

  stpnm[0]='\0';
  props=NULL;

  if (t!=TYPETOKEN) parseError(TYPETOKEN,t,PARSERERROR);

  nextt=ODBscanner.getNextToken();

  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else strcpy(tpnm,ODBscanner.tokenToStr(nextt));

  nextt=ODBscanner.getNextToken();

  if (nextt==SUBTYPETOKEN)
    {
      st=subtypeOfRule(nextt);
      strcpy(stpnm,st);
      nextt=ODBscanner.getNextToken();
    };

  if (nextt==PROPERTIESTOKEN)
    {
      props=propertiesRule(nextt,DEFINITION);
      nextt=ODBscanner.getNextToken();
    };

  if (nextt==SCOLONTOKEN)
    {
      if (st!=NULL) createResult=odb->create_type(tpnm,stpnm);
      else createResult=odb->create_type(tpnm);

      if (createResult==NULL) {
	delete props;
	semanticError(CREATETYPEERROR,tpnm,NULL);
      }

      if (props!=NULL)
	{

	  p=props;

	  while (p!=NULL)
	    {

	      ttp=odb->mapTypeNameToTypeId((p->getElem())->getName(),
					   (p->getElem())->getSet());
	      if (ttp!=_ERRORTYPE_)
		{
		  strcpy(stpnm,(p->getElem())->getPropName());
		  ttp=odb->add_property(tpnm,stpnm,ttp);
		}
	      else {
		strcpy(errorStr,(p->getElem())->getName());
		strcpy(tpnm,(p->getElem())->getPropName());
		delete props;
		p=NULL;
		semanticError(UNDEFINEDTYPE,errorStr,tpnm);
	      }

	      p=p->getNextListElem();
	    }// end while

	  delete props;
	}
      cout.flush();
    }
  else {
    if (props!=NULL) delete props;
    parseError(SCOLONTOKEN,nextt,PARSEERROR);
  }
  return 1;
}






//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
char *parser::subtypeOfRule(ODBTokenType t)
{
  ODBTokenType nextt;

  if (t!=SUBTYPETOKEN) parseError(SUBTYPETOKEN,t,PARSERERROR);
  nextt=ODBscanner.getNextToken();
  if (nextt!=OFTOKEN) parseError(OFTOKEN,nextt,PARSEERROR);
  nextt=ODBscanner.getNextToken();
  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else return ODBscanner.tokenToStr(nextt);

  return NULL;
}





//----------------------------------------------------
//	Method		:	setOfRule	
//	Description	:	Parse "set of <typename>" clauses
//				and returns the <typename> string 
//	Author		:	Staffan Flodin
//----------------------------------------------------
char *parser::setOfRule(ODBTokenType t)
{
  ODBTokenType nextt;

  if (t!=SETTOKEN) parseError(SETTOKEN,t,PARSERERROR);
  nextt=ODBscanner.getNextToken();
  if (nextt!=OFTOKEN) parseError(OFTOKEN,nextt,PARSEERROR);
  nextt=ODBscanner.getNextToken();
  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else return ODBscanner.tokenToStr(nextt);

  return NULL;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODBTokenType parser::parseValue(ODBTokenType nextt,char *propnm)
{
  ODB_INT intval;
  ODB_REAL realval;
  int rv;

  //--------------------------------------------------
  // A value has to be read now. Values can be:
  // 1) A variable name -> NAMETOKEN
  // 2) A string        -> QUOTETOKEN <string> QUOTETOKEN
  // 3) An integer      -> NAMETOKEN && all chars are integers
  // 4) A real          -> NAMETOKEN DOTTOKEN NAMETOKEN
  //                       or DOTTOKEN NAMETOKEN
  // 5) A set		 -> LEFTCURLTOKEN [value[,value]*]
  //	         	    RIGHTCURLTOKEN
  // this is taken care of by tpnm=parseValue(nextt);
  //----------------------------------------------------

  if (nextt==QUOTETOKEN) 
    {
   	
      rv=ODBscanner.scanString(nextt,propnm);
      return _CHAR_;

    }


  if (nextt==NAMETOKEN) 
    {
   
      strcpy(propnm,ODBscanner.tokenToStr(nextt));

      rv=odb->validate(propnm,intval);
      
      if (rv>0) 
	{ //an integer which may be a real

	  nextt=ODBscanner.getNextToken();
         
	  if (nextt!=DOTTOKEN)
	    {
	      ODBscanner.putBackToken(nextt);
	      return _INT_;
	    } //end if nextt!=DOTTOKEN
	  else 
	    { //an int followed by a dot
	      strcat(propnm,ODBscanner.tokenToStr(nextt));

	      // get the stuff that follows the dot
	      nextt=ODBscanner.getNextToken();
	      strcat(propnm,ODBscanner.tokenToStr(nextt));

	      //propnm may now hold a real, if not its an error
	      rv=odb->validate(propnm,realval);

	      if (rv>0) return _REAL_;
	      else return PARSEERROR;
         
	    };

      
	} //if rv>o

      else return _VARIABLE_; //no integer, no real
   
    }
   
  if (nextt==LEFTCURLTOKEN)
    {
      //
      // Parse a set value
      //
      return _COLLECTION_;
    }

  return NAMETOKEN;

}





//----------------------------------------------------
//	Method		:	
//	Description	:	Parses a set 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODB_SET	parser::parseSet(ODBTokenType t)
{
  ODBTokenType nextt=0;
  char tpnm[CHARSTRINGLENGTH];
  ODB_SET coll=NULL;
  ODB_REF o=NULL;
  ODB_REF oval=NULL;
  int dt,dtt,subtype;
  ODB_INT iv;
  ODB_REAL rv;
  ODB_TYPE vart;
  ODB_CHAR cv;
  variable *var;
  int fromVariable=0;
  int varType=0;


  if (t!=LEFTCURLTOKEN) parseError(LEFTCURLTOKEN,nextt,PARSERERROR);

  nextt=ODBscanner.getNextToken();

  while ((nextt!=RIGHTCURLTOKEN)&&(nextt!=EOFTOKEN))
    {

      varType=parseValue(nextt,tpnm);
      if (varType==PARSEERROR) 
	{
	  if (coll!=NULL) delete coll;
	  parseError(NAMETOKEN,nextt,PARSEERROR);
	}

      if (varType==_VARIABLE_) 
	{ 
	  var=variables->getElemNamed(tpnm);
			
	  if (var==NULL)
	    semanticError(UNDEFINEDVARIABLE,tpnm,NULL);

	  varType=var->getType();
			
	  if (varType==_OBJECT_)	oval= var->getObjval();
	  else if (varType==_INT_)	iv= var->getIntval();
	  else if (varType==_REAL_)	rv= var->getRealval();
	  else if (varType==_CHAR_)	cv= var->getCharval();
        }
      else 
	{
	  if (varType==_CHAR_)		cv=tpnm;     
	  else if (varType==_INT_)	odb->validate(tpnm,iv);	
	  else if (varType==_REAL_)	odb->validate(tpnm,rv);	    
	  else if (varType==_OBJECT_)	oval=NULL;
			
	}

      if (coll==NULL)		coll=odb->create_collection(varType);
      if (varType==_CHAR_)	coll->insert(cv);    
      else if (varType==_INT_)	coll->insert(iv);			
      else if (varType==_REAL_)	coll->insert(rv);      
      else if (varType==_OBJECT_) coll->insert(oval);
		
      //
      // If a comma follows get next token which will be a value
      // or }
      nextt=ODBscanner.getNextToken();
      if (nextt==COMMATOKEN) nextt=ODBscanner.getNextToken();

    }

  return coll;

}





//----------------------------------------------------
//	Method		:	
//	Description	:	This method returns a linked list of 
//					property definitions The recevier of 
//					the linked list must ensure it is
//					deallocated after use
//	Author		:	Staffan Flodin
//----------------------------------------------------
LinkedList<PropertyDef> *parser::propertiesRule(ODBTokenType t,int how)
{
  ODBTokenType nextt;
  int rv=0;
  char tpnm[CHARSTRINGLENGTH];
  char propnm[NAMELENGTH];
  LinkedList<PropertyDef> *props=NULL;
  PropertyDef *newprops=NULL;
  int setValuedProp =0;
  PropertyDef *pDf;
  ODB_SET coll;

  memset(tpnm,'\0',CHARSTRINGLENGTH);
  memset(propnm,'\0',NAMELENGTH);
  nextt=COMMATOKEN;

  if (t!=PROPERTIESTOKEN) parseError(PROPERTIESTOKEN,nextt,PARSERERROR);
   
  while (nextt==COMMATOKEN)
    {

      nextt=ODBscanner.getNextToken();
   	
      if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
      else strcpy(propnm,ODBscanner.tokenToStr(nextt));

		
      //
      // Definition of type properties
      //
      if (how==DEFINITION)
	{
   		
	  nextt=ODBscanner.getNextToken();
	  if (nextt!=MINUSTOKEN) parseError(MINUSTOKEN,nextt,PARSEERROR);
   		
	  nextt=ODBscanner.getNextToken();
	  if (nextt!=GREATERTHANTOKEN) 
	    parseError(GREATERTHANTOKEN,nextt,PARSEERROR);

	  nextt=ODBscanner.getNextToken();
	  //set of must be handeled!
	  switch (nextt)
	    {
	    case SETTOKEN:
	      strcpy(tpnm,setOfRule(nextt));
	      setValuedProp=1;
	      break;
			
	    case NAMETOKEN:
	      strcpy(tpnm,ODBscanner.tokenToStr(nextt));
	      break;
   		
	    default: parseError(NAMETOKEN,nextt,PARSEERROR);
   		
	    };

         
	  pDf=new PropertyDef(tpnm,propnm);
	  pDf->setSet(setValuedProp);
	  setValuedProp=0;

	  if (props==NULL)
	    props=new LinkedList<PropertyDef>(pDf);
	  else props->insertElem(pDf);
      
	}

      //
      // Assignment of instances properties
      //
      if (how==ASSIGNMENT)
	{
      	
	  nextt=ODBscanner.getNextToken();
	  if (nextt!=EQUALTOKEN) parseError(EQUALTOKEN,nextt,PARSEERROR);
	  nextt=ODBscanner.getNextToken();

	  rv=parseValue(nextt,tpnm);
	  if (rv==PARSEERROR) parseError(NAMETOKEN,nextt,PARSEERROR);

	  if (rv==_COLLECTION_) 
	    {
	      coll=parseSet(nextt);
	    }
				

	  if (props==NULL)
	    {
	      newprops=new PropertyDef(tpnm,propnm,rv);
	      props=new LinkedList<PropertyDef>(newprops);
				
	    }
	  else 
	    {
	      newprops=new PropertyDef(tpnm,propnm,rv);
	      props->insertElem(newprops);
	    }

	  if (rv==_COLLECTION_) 
	    {
	      newprops->setsetval(coll);
	    }
      
	}

      nextt=ODBscanner.getNextToken();
    }

  ODBscanner.putBackToken(nextt);
   
  return props;
}
//-----------------------------------------------------------
//
//           E N D  O F  C R E A T E   T Y P E
//
//-----------------------------------------------------------





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
//-----------------------------------------------------------
//
//                 C R E A T E   P R O P E R T Y
// The syntax is:
// <create prop>:=create property <proplist> on <nm>;
//-----------------------------------------------------------
int parser::createPropertiesRule(ODBTokenType t)
{
  ODBTokenType nextt;
  char tpnm[NAMELENGTH];
  char pnm[NAMELENGTH];
  LinkedList<PropertyDef> *props, *p;
  ODB_TYPE newtp=NULL;
  ODB_PROP dprop=NULL;
  int ttp=0,dttp=0;

  props=NULL;

  props=propertiesRule(t,DEFINITION);

  nextt=ODBscanner.getNextToken();

  if (nextt!=ONTOKEN) parseError(ONTOKEN,nextt,PARSEERROR);

  nextt=ODBscanner.getNextToken();

  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else strcpy(tpnm,ODBscanner.tokenToStr(nextt));

  nextt=ODBscanner.getNextToken();   

  if (nextt==SCOLONTOKEN)
    {

      newtp=odb->gettypenamed(tpnm);
      if (newtp==NULL) {
	delete props;
	semanticError(UNKNOWNTYPE,tpnm,NULL);
      }

      if (props!=NULL)
	{
	  p=props;
	  while (p!=NULL)
	    {
	      dprop=newtp->getpropertynamed((p->getElem())->getPropName());
	      if (dprop!=NULL) {
		strcpy(pnm,(p->getElem())->getPropName());
		delete props;
		semanticError(PROPERTYEXIST,pnm,tpnm);
	      }

	      ttp=odb->mapTypeNameToTypeId((p->getElem())->getName(),
					   (p->getElem())->getSet());
	      if (ttp==_ERRORTYPE_)
		{
		  strcpy(pnm,(p->getElem())->getName());
		  delete props;
		  semanticError(UNKNOWNTYPE,pnm,NULL);
		}

	      strcpy(pnm,(p->getElem())->getPropName());
	      odb->add_property(tpnm,pnm,ttp);
               
	      p=p->getNextListElem();
	    }
	  delete props;
	}
      cout.flush();
    }
  else parseError(SCOLONTOKEN,nextt,PARSEERROR);
  return 1;
}
//-----------------------------------------------------------
//
//           E N D  O F  C R E A T E   P R O P E R T Y
//
//-----------------------------------------------------------





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
//-----------------------------------------------------------
//
//                 C R E A T E   I N S T A N C E
// The syntax is:
// <create inst>:=create instance of <nm> set <propasslist>;
// <propasslist>:=<propnm>=<value>[,<propasslist>]
//-----------------------------------------------------------
int parser::createInstanceRule(ODBTokenType t,variable *v)
{
  ODBTokenType nextt;
  char tpnm[NAMELENGTH], *propnm, *vartpnm=NULL,*cv=NULL,*propnmCopy=NULL;
  LinkedList<PropertyDef> *props=NULL, *p;
  ODB_REF o=NULL;
  ODB_REF oval=NULL;
  int dt,dtt,subtype;
  ODB_INT iv;
  ODB_REAL rv;
  ODB_TYPE vart;
  variable *var;
  int fromVariable=0;
  ODB_SET coll;


  props=NULL;

  if (t!=INSTANCETOKEN) parseError(INSTANCETOKEN,t,PARSERERROR);

  nextt=ODBscanner.getNextToken();
  if (nextt!=OFTOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);

  nextt=ODBscanner.getNextToken();
  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else strcpy(tpnm,ODBscanner.tokenToStr(nextt));

  if (v!=NULL) 
    {
   	
      vart=v->getTypeRef();
      if (vart!=NULL) vartpnm=vart->getname();
      if (vartpnm==NULL) vartpnm=odb->mapTypeIdToTypeName(v->getType());
      if ((odb->mapTypeNameToTypeId(tpnm,0)!=v->getType())||
	  (odb->gettypenamed(tpnm)!=v->getTypeRef())||
	  (!odb->subtypeOf(tpnm,vartpnm)))
	semanticError(TYPEMISMATCH,tpnm,vartpnm);
    }

  nextt=ODBscanner.getNextToken();

  if (nextt==SETTOKEN)
    {
      //
      // Get a list with properties and their values
      //
      props=propertiesRule(PROPERTIESTOKEN,ASSIGNMENT);
      nextt=ODBscanner.getNextToken();
   
    }
  //else parseError(SETTOKEN,nextt,PARSEERROR);

  if (nextt==SCOLONTOKEN)
    {
      //
      // End of rule, create the object and stuff
      //
      o=odb->create_instance(tpnm);
      if (v!=NULL) v->setObjval(o);

      if ((props!=NULL)&&(o!=NULL))
	{
	  p=props;

	  while (p!=NULL)
	    {
	      //
	      // set the properties to the instance
	      //
	      propnm=(p->getElem())->getPropName();
	      dt=(p->getElem())->getDataType();

	      //
	      // If value from var substitute by the variable type
	      //
	      if (dt==_VARIABLE_) 
		{ 
               	
		  var=variables->getElemNamed((p->getElem())->getName());
		  dt=var->getType();
		  if (dt==_OBJECT_)	oval= var->getObjval();
		  if (dt==_INT_)	iv= var->getIntval();
		  if (dt==_REAL_)	rv= var->getRealval();
		  if (dt==_CHAR_)	cv= var->getCharval();
		  fromVariable=1;
		}
	      else fromVariable=0;

	      dtt=odb->getpropertytype(tpnm,propnm);

	      //
	      // is the type property type the same as whats read from file?
	      // if not we have a type error
	      //
	      if (dtt!=dt)
		{
					
		  // 
		  // Must vopy the propname since it is freed before
		  // use in semanticError
		  //
		  propnmCopy=strdup(propnm);
		  delete props;

		  if (dtt<0)
		    {
					
		      semanticError(UNKNOWNPROPERTY,tpnm,propnmCopy);
		    }
					
		  free(propnmCopy);
		  semanticError(TYPEMISMATCH,odb->mapTypeIdToTypeName(dtt),
				odb->mapTypeIdToTypeName(dt));
               
		}

	      //
	      // If value from variable it is laready set to loacal var. if it
	      // is not from variable it is saved in provval object in 
	      // linkedlist
	      //
	      switch (dt)
		{
		case _CHAR_: 
		  {
		    if (!fromVariable) cv=(p->getElem())->getName();
		    odb->setproperty_value(o,propnm,cv);
		    break;
		  }
               	
		case _INT_:
		  {
		    if (!fromVariable) 
		      odb->validate((p->getElem())->getName(),iv);

		    odb->setproperty_value(o,propnm,iv);
		    break;
		  }
               	
		case _REAL_:
		  {
		    if (!fromVariable) 
		      odb->validate((p->getElem())->getName(),rv);

		    odb->setproperty_value(o,propnm,rv);
		    break;
		  }
               	
		case _OBJECT_: 
		  {
		    if (!fromVariable) oval=NULL;
		    odb->setproperty_value(o,propnm,oval);
		    break;
		  }

		case _OBJECT_COLLECTION_ :
		  {
		    coll=p->getElem()->getsetval();

		    //
		    // To prevent the collection from being
		    // deleted at exit of this method we set
		    // the pointer to the collection in the 
		    // proplist to NULL
		    //
		    p->getElem()->setsetval(NULL);
		    odb->setproperty_value(o,propnm,coll);
		    break;
		  }

		case  _INT_COLLECTION_:
		  {
		    coll=p->getElem()->getsetval();
		    odb->setproperty_value(o,propnm,coll);
		    break;
		  }

		case  _REAL_COLLECTION_:
		  {
		    coll=p->getElem()->getsetval();
		    odb->setproperty_value(o,propnm,coll);
		    break;
		  }

		case  _CHAR_COLLECTION_:
		  {
		    coll=p->getElem()->getsetval();
		    odb->setproperty_value(o,propnm,coll);
		    break;
		  }

		case  _SET_COLLECTION_:
		  {
		    coll=p->getElem()->getsetval();
		    odb->setproperty_value(o,propnm,coll);
		    break;
		  }
               	
		default: break;
		};
               
	      p=p->getNextListElem();
            
	    } // while
         
	} //end if ((props!=NULL)&&(o!=NULL)

      delete props;
   
    }
  else parseError(SCOLONTOKEN,nextt,PARSEERROR);

  return OKTOKEN;
}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
//-----------------------------------------------------------
//
//                 C R E A T E   V A R I A B L E
// The syntax is:
// <create var>:=create variable <nm> of <tpnm>;
//-----------------------------------------------------------
int parser::createVariableRule(ODBTokenType t)
{
  ODBTokenType nextt;
  char tpnm[NAMELENGTH], vnm[NAMELENGTH];
  variable *v;
  int setValue=0;


  if (t!=VARIABLETOKEN) parseError(VARIABLETOKEN,t,PARSERERROR);

  nextt=ODBscanner.getNextToken();

  if (nextt!=NAMETOKEN) parseError(NAMETOKEN,nextt,PARSEERROR);
  else strcpy(vnm,ODBscanner.tokenToStr(nextt));

  nextt=ODBscanner.getNextToken();

  if (nextt!=ISTOKEN) parseError(ISTOKEN,nextt,PARSEERROR);

  nextt=ODBscanner.getNextToken();

  switch (nextt)
    {
    case SETTOKEN:
      strcpy(tpnm,setOfRule(nextt));
      strcpy(tpnm,odb->mapTypeNameToCollectionOfTypeName(tpnm));
      setValue=1;
      break;

    case NAMETOKEN:
      strcpy(tpnm,ODBscanner.tokenToStr(nextt));
      break;

    default:
      parseError(NAMETOKEN,nextt,PARSEERROR);
      break;
    }

	
  nextt=ODBscanner.getNextToken();

  if (nextt==SCOLONTOKEN)
    {
	
      if((odb->gettypenamed(tpnm)==NULL)&&
	 (odb->mapTypeNameToTypeId(tpnm,1)==_ERRORTYPE_))
	semanticError(UNKNOWNTYPE,tpnm,NULL);

      //create the variable
      v=new variable(vnm,odb->mapTypeNameToTypeId(tpnm,0),odb->gettypenamed(tpnm));
      
      if (variables==NULL) variables=new LinkedList<variable>(v);
      else variables->insertElem(v);

   
    }

   
  else parseError(SCOLONTOKEN,nextt,PARSEERROR);

   
  return OKTOKEN;

}





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
char *parser::getODBQLError(void)
{

  return errorMsg;

}


















