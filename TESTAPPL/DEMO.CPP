//----------------------------------------------------
//          Author: Staffan Flodin
//                  sflodin@ozemail.com.au
//          ODB Sample file. Link with ODB.LIB
//          Make sure Compdb.odb is in the same
//          directory.
//
//----------------------------------------------------
#include<iostream.h>
#include <fstream.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys\timeb.h>
#include "e:\users\staffan\odb\source\store.h"

outputbuffer obuf=outputbuffer();



void browse_extent(ODB_TYPE tp,database &db){
//this function browses through all objects in the
//extent of the specified type tp
ODB_QSTREAM q_stream;
ODB_CHAR tpname;
ODB_REF o;

	if (tp==NULL) return;    //quit if shitty reference

	tpname=tp->getname();   //this method is not described
													//in the manual, it is found
													//in odb.h

	q_stream=db.open_stream(tpname);

	if (q_stream==NULL) return; //should not happen
														 //(they all say that)

	cout<<"Browsing the extent of type "<<tpname<<'\n';
	obuf.reset_buffer();			//reset the buffer

	//follows is the standard structure of iterating through streams
	//at ****** Any Code ***** application specific code are enterd
	//in below example the object bound to o is just printed to stdout
	//-------------------------------------------------
	//----------- Std stream iteration-----------------
	//-------------------------------------------------
	//--          o=db.get(q_stream);                --
	//--          while (o!=NULL){                   --
	//--            ****** Any Code *****            --
	//--            o=db.get(q_stream);              --
	//--          }                                  --
	//-------------------------------------------------
	//----------- End Std stream iteration-------------
	//-------------------------------------------------
	o=db.get(q_stream);
	while (o!=NULL){
		o->display(obuf);
		cout<<obuf.get_buffer()<<'\n';
		obuf.reset_buffer();
		o=db.get(q_stream);
		}

	//-------------------------------------------------
	//--                I M P O R T A N T            --
	//-------------------------------------------------
	//-- The application MUST deallocate the stream  --
	//-- here, otherwise a memory leak would occur as--
	//-- the stream can not be referenced again      --
	//-- Alternatively, declare it as STATIC         --
	//-------------------------------------------------
	db.close_stream(q_stream);
	}


void display_schema(database &db){
//For each type in the loaded database schema
//display its information by sending the display message to the
//type object and then for each type display its propperties
ODB_CHAR tpname, propname;
ODB_TYPE tphandle;

	obuf.reset_buffer(); 								 //empty any old crap in the buffer

	tpname=db.getallsubtypes(); 				 //get the first typename

	while (tpname!=NULL){       				 //loop over all typenames

		tphandle=db.gettypenamed(tpname); //given a type name the handle
																			//can be fetched
		if (tphandle!=NULL) {
			//display the type object and all its properties
			tphandle->display(obuf);
			cout<<"\n--------------------------------------\n";
			cout<<obuf.get_buffer()<<'\n';
			obuf.reset_buffer();
			cout<<"\n------- Press any key for more -------\n" ;
			getch(); 										//let the user control the flow
		};  													//end while

	tpname=db.getnextsubtype();     //get next typename
	} 															//end while

}         												//end function

void select_type(database &db){
//In this function the user is provided all usertypes
//in the database and then promted to select one whose
//extent are to be browsed
ODB_CHAR tpname;
char usertpnm[20];
ODB_TYPE utp;
tpname=db.getallsubtypes();
	while (tpname!=NULL){
	cout<<tpname<<'\n';
	tpname=db.getnextsubtype();
	}
	cout<<"Enter name of the type whose extent to browse\n";
	cin>>usertpnm;
	utp=db.gettypenamed(usertpnm);
	if (utp==NULL) cout<<"\nThere no type named "<<usertpnm<<" in the database\n";
	else browse_extent(utp,db);
}

void find_all_janitors(database &My_DB){
//In this function all janitors are retreived.
//This is done by first finding the department
//object with "Janitors" as value in to the
//Name attribute
ODB_QSTREAM q_stream;
ODB_SSTREAM s_stream;
ODB_REF o,jo;
char typenm[16]="Department";
char propnm[8]="Name";
char val[16]="Janitors";

	//Open a stream over the department type with the condition that
	//the Name attribute must have Janitors as value
	q_stream=My_DB.open_stream(typenm,propnm,val);

	//The stream may not be opened if so it is best to check before use.
	if (q_stream==NULL) {
		cout<<"\nError when opening stream bla bla bla...\n";
		return;
		};

	//The stream was opened allright. In the remaining we only consider the
	//first found department named Janitors, thus we do not assume the name
	//to be a key to departments

	o=My_DB.get(q_stream);

	//If o==NULL yhen there were no such department in the database
	if (o==NULL) {
		cout<<"No department named "<<val<<" exist in the database\n";
		return;
		}

	//No longer req'd
	My_DB.close_stream(q_stream);

	//reuse q_stream to open the stream to select all employees at
	//the department bound to object reference o

	q_stream=My_DB.open_stream("Employee","EmployedAt",o);
  if (q_stream==NULL) {
		cout<<"\nError when opening stream over employee\n";
		return;
		};
	cout<<"\nThese are the Janitors\n-------------------------\n";
	obuf.reset_buffer();			//reset the buffer
	//Now, we can start browsing the janitors
	jo=My_DB.get(q_stream);
	while (jo!=NULL){
		jo->display(obuf);
		cout<<obuf.get_buffer()<<'\n';
		obuf.reset_buffer();
		jo=My_DB.get(q_stream);
		};

	//Now, Lets select those janitors who earn more than 10000.
	//This is accomplished by opening a select stream over the
	//query stream which selects all janitors.

	//First, reset the stream
	My_DB.reset_stream(q_stream);
	//secondly, open a select stream over q_stream with the
	//condition that the salary should be greater than 10000
	s_stream=My_DB.select(q_stream,"Salary",">",(ODB_INT)10000);

	cout<<"\n Theese are all the rich janitors\n";
	obuf.reset_buffer();			//reset the buffer
	//Now, we can start browsing the rich janitors
	jo=My_DB.get(s_stream);
	while (jo!=NULL){
		jo->display(obuf);
		cout<<obuf.get_buffer()<<'\n';
		obuf.reset_buffer();
		jo=My_DB.get(s_stream);
		};

	My_DB.close_stream(s_stream);}

 void demonstrate_index(database &My_DB){
//In this fn a new type is created. The type is then
//given an integer valued proerty which can be assigned
//random values.
//First create 5000 objects and find the ones with a particular
//value for the attribute. Measure the time for this. Then create
//an index and redo the search and measure time.
struct timeb t1,t2;
double elapsed_time;
ODB_REF o;
int i,v;
ODB_QSTREAM q_stream;

	My_DB.set_tablesize(3000); //3K table size

	//Create a type named testtype
	My_DB.create_type("Testtype");

	//Create an integer valued property named Int_prop
	My_DB.add_property("Testtype","Int_prop",_INT_);

	//Create 5000 objects and assign random values to
	//the attribute named Int_prop for each object.
	cout<<"\nCreating 50000 objects ...";
	i=0;
	while (i++<50000){

		//Create an object and assign o to reference it
		o=My_DB.create_instance("Testtype");

		//generate a random number in [0..4000]
		v=rand()%4000;

		//set the property named Int_prop of the object o
		//to v
		o->setproperty_value("Int_prop",(ODB_INT)v);
	};
	cout<<"Done\n";

	//Now, lets find all objects with Int_prop = 2345
	//the probability that this number is less than 1
	//Thus, it may be the case that no object exists
	//with that property value. To make sure there is
	//atleast one such object we simply assign an object
	//that value for the property.

	//To make sure we do this right we first find out the
	//OID of the last object. Then we subtract a number less
	//than 50000 from the OID. We now have an OID to one of
	//the 50000 object we just generated. Retrieve it and
	//set the value.
	i=0;
	My_DB.getproperty_value(o,"OID", (ODB_INT *)&i);
	i=i-rand()%45000;
	o=My_DB.getobjectwithoid("Testtype",i);
	o->setproperty_value("Int_prop",(ODB_INT)1234);

	//Now that we know there is an object with 1234 as
	//value for the Int_prop property we can start to
	//search for it.
	obuf.reset_buffer();
	q_stream=My_DB.open_stream("Testtype","Int_prop","=",(ODB_INT)1234);

	//All set for test search. Start timer and go for it
	ftime(&t1);
	o=My_DB.get(q_stream);
	while (o!=NULL){
		o->display(obuf);
		cout<<obuf.get_buffer()<<'\n';
		obuf.reset_buffer();
		o=My_DB.get(q_stream);
		};

	//All objects found, stop timer and check time
	ftime(&t2);
	elapsed_time=(t2.time*1000+t2.millitm)-(t1.time*1000+t1.millitm);
	cout<<"\nThe search took "<<elapsed_time<<"millisec without index";

	//Now, build an index and redo the whole procedure.
	cout<<"\nBuilding index ...";
	My_DB.create_index("Testtype", "Int_prop");
	cout<<"Done\n";

	//reset the stream
	My_DB.reset_stream(q_stream);
	obuf.reset_buffer();
	ftime(&t1);
	o=My_DB.get(q_stream);
	while (o!=NULL){
		o->display(obuf);
		cout<<obuf.get_buffer()<<'\n';
		obuf.reset_buffer();
		o=My_DB.get(q_stream);
		};
	ftime(&t2);
	elapsed_time=(t2.time*1000+t2.millitm)-(t1.time*1000+t1.millitm);
	cout<<"\nThe search took "<<elapsed_time<<"millisec with index";

	//-------------------------------------------------
	//--                I M P O R T A N T            --
	//-------------------------------------------------
	//-- Since the application now no longer need the--
	//-- 50000objects etc we now deallocate them     --
	//-- There are several ways of doing this.       --
	//-- 1: Open a stream over all Testtype objects  --
	//-- and deallocate them one by one.             --
	//-- 2: Since the type named Testtype is not     --
	//-- wanted anymore we simply delete the type    --
	//-- and let ODB do the hard work of deallocating--
	//-- everything, e.g. objects indexes ......     --
	//--                                             --
	//-- And also, we allocated a stream local to    --
	//-- to this function. This function must be     --
	//-- deallocated to prevent memory leak.         --
	//-------------------------------------------------

	//My_DB.delete_type("Testtype");
	My_DB.close_stream(q_stream);

	//It is now ok to exit this function

}
int main (void){

  database My_DB("new");

  ifstream databasefile;
 

int retval;
	retval=My_DB.load_database("bigone.odb");
	if (retval<0)
   {
   	cout<<"\n Failed to load company database ";
      return 1;
   }

	else {
		cout<<"\n The company database is loaded";
		My_DB.display(obuf);
		cout<<"\n"<<obuf.get_buffer();
		obuf.reset_buffer();


		//display the schema of the loaded database
		display_schema(My_DB);

		//let the user select and browse a type extent
		select_type(My_DB);

		//demonstrate the search interface
		find_all_janitors(My_DB);

		//show the benefit of indexing when the database
		//is large
		demonstrate_index(My_DB);


      //cout<<endl<<"Start save ";
   	//My_DB.save_database("..\\testappl\\bigone.odb");

	cout<<"\nODB Demo is now finished\n";
		return 1;
   }
   
};
