//Validation functions

#include <string.h>
#include <math.h>
//#include <ctype.h>
#include <fstream.h>

#include "d:\users\staffan\odb\source\store.h"

#ifndef __BORLANDC__
int pow10(int i){
	if (i==0) return 1;
	if (i==1) return 10;
	else return 10*pow10(i-1);
}
#endif

validation::validation(void){
	db=NULL;
}

validation::validation(database *d){
	db=d;
}

validation::~validation(void){}

int validation::validdigit(char ch)
{
	if ((ch<='9')&&(ch>='0')) return 1;
   else return 0;
}

int validation::validate_integer(char *inp,ODB_INT &value){
int length=strlen(inp);
int currpos=0;
int sign=1;
ODB_INT retval=0;
ODB_INT factor;
char currch=inp[currpos];
	if (currch=='-') {
		sign=-1;
		currpos++;
		currch=inp[currpos];
		factor=pow10(length-2);
		}
	else factor=pow10(length-1);
	while((currpos<length)&&(currch!='\0')){
		if (validdigit(currch)) {
			retval=retval+(currch-48)*factor;
			factor=factor/10;
			currpos++;
			currch=inp[currpos];
			}
		else currpos=2*length; //signals shitty input
	}; //end while
	if (currpos==2*length) {
		return 0;
		};
	value=retval*sign;
	return 1;
}

int validation::validate_real(char *inp,ODB_REAL &value){
int length=strlen(inp);
int  currpos=0,saveddotpos;
ODB_INT dotpos;
ODB_REAL retval=0.0;
ODB_REAL ffactor;
char currch=inp[currpos];
	dotpos=-1;
	while((currpos<length)&&(currch!='\0')){  //where is decimal point
		if (currch=='.') dotpos=currpos;
		currpos++;
		currch=inp[currpos];
		};
	if (dotpos!=-1) currpos=dotpos+1;
	else currpos=3*length;
	ffactor=1.0/10.0;
	currch=inp[currpos];
	while((currpos<length)&&(currch!='\0')){
		if (validdigit(currch)) {
			retval=retval+(currch-48)*ffactor;
			ffactor=ffactor/10.0;
			currpos++;
			currch=inp[currpos];
			}
		else currpos=2*length; //signals shitty input
	}; //end while
	if (currpos==2*length) {
		return 0;
		}
	else {  //get intpart add add it to realpart
		if (dotpos!=-1) inp[dotpos]='\0';
		saveddotpos=dotpos;
		dotpos=0;
		if (validate_integer(inp,dotpos)!=0){ //reuse dotpos;
			retval=retval+dotpos;
			value=retval;
			inp[saveddotpos]='.';
			return 1;
			}
		else {
			inp[saveddotpos]='.';
			return 0;
			}
	}
}

int validation::validate_name(char *ch){
int le,currpos,shiftpos;
char currch;
	le=strlen(ch);
	currpos=0;
	currch=ch[currpos];
	while ((currch!='\0')&&(currpos<=le)){
		if (currch<=' '){ //space disallowed in names, overwrite it
			shiftpos=currpos;
			while (shiftpos<le){  //shift all ch one step left
				ch[shiftpos]=ch[shiftpos+1];
				shiftpos++;
				} //while
			le--; //the string is one ch shorter
			} //end if
		currpos++;
		currch=ch[currpos];
		} //while
	return 1;
}

int validation::validate_set(char *inp,ODB_SET set){
int type,pos,ok,bufpos;
ODB_INT intval;
ODB_REAL realval;
ODB_REF obj;
char buffer[256],ch;
	type=(*set).gettype();
	pos=0;
	ch=inp[pos];
	while ((ch!='{')&&(ch!='\0')) ch=inp[++pos]; //find start bracket;
	if (ch!='{') return -1; //something wrong;
	else ch=inp[++pos];
	while ((ch!='}')&&(ch!='\0')){
		while (((ch==' ')||(ch==','))&&(ch!='}'))
			ch=inp[++pos]; //move to
		switch (type){
			case _INT_:
				bufpos=0;
				ch=inp[pos];
				while ((ch!=' ')&&(ch!=',')&&(ch!='}')) {
					buffer[bufpos++]=ch;
					ch=inp[++pos];
					};
				buffer[bufpos]='\0';
				ok=validate_integer(buffer,intval);
				if (ok>0) (*set).insert((ODB_INT)intval);
				else return -1;
				break;
			case _REAL_:
				bufpos=0;
				ch=inp[pos];
				while ((ch!=' ')&&(ch!=',')&&(ch!='}')) {
					buffer[bufpos++]=ch;
					ch=inp[++pos];
					};
				buffer[bufpos]='\0';
				ok=validate_real(buffer,realval);
				if (ok>0) (*set).insert((ODB_REAL)realval);
				else return -1;
				break;
			case _CHAR_:
				bufpos=0;
				ch=inp[pos];
				while((ch!=' ')&&(ch!=',')&&(ch!='}')&&(ch!='\"')) {
					ch=inp[++pos];
					};
				if (ch=='\"') ch=inp[++pos];
				else return -1;
				while ((ch!='\0')&&(ch!='\"')) {
					buffer[bufpos++]=ch;
					ch=inp[++pos];
					};
				buffer[bufpos]='\0';
				if (ch=='\"'){
					(*set).insert((*db).getstring(buffer));
					ch=inp[++pos];
					}
				else return -1;
				break;
			case _OBJECT_:
				bufpos=0;
				ch=inp[pos];
				while ((ch!=' ')&&(ch!=',')&&(ch!='}')) {
					buffer[bufpos++]=ch;
					ch=inp[++pos];
					};
				buffer[bufpos]='\0';
				ok=validate_integer(buffer,intval);
				if (ok>0) {
					obj=(*db).getobjectwithoid("Usertypes",intval);
					if (obj!=NULL) (*set).insert(obj);
					else return -1;
					}
				else return -1;
				break;
			case _COLLECTION_:
				break;
				};
			ch=inp[pos];
		};
	return 1;
}

/*
int main(void){
ODB_SET ivset,rvset,cvset,ovset;
outputbuffer obuf=outputbuffer();
ivset = new collection(_INT_);
rvset = new collection(_REAL_);
cvset = new collection(_CHAR_);
ovset = new collection(_OTHER_);
database *db;
	db=new database();
	(*db).create_type("Employee");
	(*db).create_instance("Employee");
	(*db).create_instance("Employee");
	(*db).create_instance("Employee");
	(*db).create_instance("Employee");

	validate_set("{5,7}",db,ivset);
	(*ivset).display(obuf);

	validate_set("{5.5  7.890}",db,rvset);
	(*rvset).display(obuf);

	validate_set("{\"N J\"}",db,cvset);
	(*cvset).display(obuf);

	validate_set("{3 4}",db,ovset);
	(*ovset).display(obuf);
	cout<<obuf.get_buffer()<<"\n";
	return 1;
	}
	*/
