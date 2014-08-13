//----------------------------------------------------
//	Project		:	ODB
//	File		:	scanner.cpp
//	Description	:	Implementation of ODBQL parser
//	Environment	:	Borland C++ or Visual C++
//	Author		:	Staffan Flodin
//----------------------------------------------------
#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <stdlib.h>





//----------------------------------------------------
//	Method		:	
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::openFile(char *fnm)
{

  state=CLOSED;
  fileName=strdup(fnm);
  inputfile=fopen(fileName,"r");
	
  if (inputfile!=NULL) 
    {
      state=OPEN;
      setLineNo(0);
      enumLineNo();
    }

  return state;

}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void scanner::closeFile(void)
{

  if (inputfile!=NULL)
    {
      fclose(inputfile);
      inputfile=NULL;
    }

}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::whiteChar(char ch)
{
  if ((ch<=' ')||(ch>'~')) {
    if (ch=='\n') enumLineNo();
    return 1;
  }
  else return 0;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::delimitingChar(char ch)
{
  int i=0;
  while (i<NOF_DELIMITERS)
    if (delimiters[i++]==ch) return 1;

  return 0;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
void scanner::removeComment(void)
{
  char ch;
  int rv;

  rv=fscanf(inputfile,"%c",&ch);
  while((ch!='\n')&&(rv!=EOF))
    {
      rv=fscanf(inputfile,"%c",&ch);
    }

  if (ch=='\n') enumLineNo();
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::scanString(ODBTokenType t,char * str)
{
  char c,rv;
  int i=0;

  if (state!=OPEN) return ERRORTOKEN;
  if (t!=QUOTETOKEN) return ERRORTOKEN;

  rv=fscanf(inputfile,"%c",&c);

  while((c!='\"')&&(rv!=EOF))
    {
      str[i++]=c;
      rv=fscanf(inputfile,"%c",&c);
    }

  if (rv==EOF) return EOFTOKEN;
  else return OKTOKEN;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODBTokenType scanner::getNextToken(void)
{
  int i=0,rv;
  ODBTokenType t,token;



  if (state!=OPEN) return ERRORTOKEN;


  //-----------------------------------------------
  // A token may have been saved from previous call
  // If so, use it
  //-----------------------------------------------
  if (hasNextToken()) return useNextToken();


  //-----------------------------------------------
  // A character may be saved from previous call
  // If so, use it if it is a token in itself
  //-----------------------------------------------
  if (delimitingChar(inpbuf[0]))
    {
      token=mapStringToToken(inpbuf);
      memset(inpbuf,'\0',INPBUFLENGTH);
      return token;
    }

  rv=fscanf(inputfile,"%c",&currentCh);


  //-----------------------------------------------
  // remove all white spaces
  //-----------------------------------------------
  while((whiteChar(currentCh))&&(rv!=EOF)) 
    rv=fscanf(inputfile,"%c",&currentCh);
  if (rv==EOF) return EOFTOKEN;


  //-----------------------------------------------
  // read characters until whitespace or delimiter
  // is reached. This will be matched to a token
  //-----------------------------------------------
  inpbuf[i++]=currentCh;
  while(!whiteChar(currentCh)&&!delimitingChar(currentCh))
    {
      rv=fscanf(inputfile,"%c",&currentCh);
      inpbuf[i++]=currentCh;
    }

  inpbuf[--i]='\0';

  if (strlen(inpbuf)>0) token=mapStringToToken(inpbuf);
  else if (delimitingChar(currentCh))
    {
      inpbuf[0]=currentCh;
      token=mapStringToToken(inpbuf);
      currentCh='\0';
    }

  if (token==NAMETOKEN) 
    {
      memset(currentName,'\0',INPBUFLENGTH);
      strcpy(currentName,inpbuf);
      keyWord[NAMETOKEN]=currentName;
    }

  memset(inpbuf,'\0',INPBUFLENGTH);

  if (delimitingChar(currentCh)) inpbuf[0]=currentCh;

  //-----------------------------------------------------
  // Comments are SLASHTOKEN SLASHTOKEN ....... newline
  // remove comments
  //-----------------------------------------------------

  if ((comment==0)&&(token==SLASHTOKEN))
    {
      comment=1; 	

      //
      //signals that a comment is currently investigated. 
      //Ie this
      //code is not eneterd in the following getnexttoken
      //

      t=getNextToken();
      comment=0;

      if (t==SLASHTOKEN) 
	{
	  removeComment();
	  token=getNextToken();
	}
      else setNextToken(t);
    }
   
  return token;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODBTokenType scanner::mapStringToToken(char *str)
{
  int i=0;
  while ((i<NOF_KEYWORDS)&&
	 ((keyWord[i]==NULL)||
	  (strcmp(str,keyWord[i])!=0))) i++;

  if ((i>=NOF_KEYWORDS)&&(strlen(str)>0)) return NAMETOKEN;
  else return i;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
scanner::~scanner(void)
{
  if (fileName!=NULL) free(fileName);
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
scanner::scanner(void)
{
  
  
  keyWord[0]=NULL;
  keyWord[1]=NULL;
  keyWord[2]=CREATESTRG;
  keyWord[3]=DATABASESTRG;
  keyWord[4]=TABLESTRG;
  keyWord[5]=SIZESTRG;
  keyWord[6]=EQUALSTRG;
  keyWord[7]=SCOLONSTRG;
  keyWord[8]=NULL;
  keyWord[9]=NULL;
  keyWord[10]=LOADSTRG;
  keyWord[11]=SAVESTRG;
  keyWord[12]=FROMSTRG;
  keyWord[13]=ASSTRG;
  keyWord[14]=TYPESTRG;
  keyWord[15]=SUBTYPESTRG;
  keyWord[16]=OFSTRG;
  keyWord[17]=PROPERTIESSTRG;
  keyWord[18]=ARROWSTRG;
  keyWord[19]=ONSTRG;
  keyWord[20]=INSTANCESTRG;
  keyWord[21]=SETSTRG;
  keyWord[22]=INDEXSTRG;
  keyWord[23]=DCOLONSTRG;
  keyWord[24]=BANGSTRG;
  keyWord[25]=QUOTESTRG;
  keyWord[26]=PLUSSTRG;
  keyWord[27]=MINUSSTRG;
  keyWord[28]=STARSTRG;
  keyWord[29]=SLASHSTRG;
  keyWord[30]=LESSTHANSTRG;
  keyWord[31]=GREATERTHANSTRG;
  keyWord[32]=EQUALSTRG;
  keyWord[33]=LEFTPARSTRG;
  keyWord[34]=RIGHTPARSTRG;
  keyWord[35]=LEFTBRACKSTRG;
  keyWord[36]=RIGHTBRACKSTRG;
  keyWord[37]=LEFTCURLSTRG;
  keyWord[38]=RIGHTCURLSTRG;
  keyWord[39]=COMMASTRG;
  keyWord[40]=DOTSTRG;
  keyWord[41]=EOFSTRG;
  keyWord[42]=VARIABLESTRG;
  keyWord[43]=ISSTRG;


  memset(delimiters,'\0',NOF_DELIMITERS);
  delimiters[0]='!';
  delimiters[1]='"';
  delimiters[2]='+';
  delimiters[3]='-';
  delimiters[4]='*';
  delimiters[5]='/';
  delimiters[6]='<';
  delimiters[7]='>';
  delimiters[8]='=';
  delimiters[9]='(';
  delimiters[10]=')';
  delimiters[11]='[';
  delimiters[12]=']';
  delimiters[13]='{';
  delimiters[14]='}';
  delimiters[15]=',';
  delimiters[16]=';';
  delimiters[17]='.';
  delimiters[18]=':';

  state=CLOSED;
  currentCh='\0';
  memset(inpbuf,'\0',INPBUFLENGTH);
  fileName=NULL;
  nextToken=NOTOKEN;
  comment=0;
  lineNo=0;

  inputfile=NULL;
  
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
ODBTokenType scanner::useNextToken(void)
{
  ODBTokenType temp;
  temp=nextToken;
  nextToken=NOTOKEN;
  return temp;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::hasNextToken(void)
{
  if (nextToken!=NOTOKEN) return 1;
  else return 0;
}





//----------------------------------------------------
//	Method		:	constructor
//	Description	:	 
//	Author		:	Staffan Flodin
//----------------------------------------------------
int scanner::putBackToken(ODBTokenType t)
{
  if (nextToken!=NOTOKEN) return 0;
  else 
    {
      nextToken=t;
      return 1;
    }
}
