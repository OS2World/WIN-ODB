//----------------------------------------------------
//
// Main file to project store
//
// In Store.h all types and classes and globals are
// defined
//
// Each class implementation in separate files included
// into this file
//
//----------------------------------------------------
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "..\source\store.h"

outputbuffer obuf=outputbuffer();

int populatedb(database *db){
int i=0;
ODB_INT a=0;
ODB_REAL b;
table *tbl;
object *obj;
type *tp, *tp2;

	(*db).create_type("Person");
	(*db).add_property("Person","Age",_INT_);
	tp=(*db).gettypenamed("Person");
	(*tp).build_index("Age");
	// Person type Metadata created

	(*db).create_type("Employee","Person");
	(*db).add_property("Employee","Salary",_INT_);
	tp=(*db).gettypenamed("Employee");
	(*tp).build_index("Salary");
	// Employee matadata created

	tp=(*db).gettypenamed("Person");
	i=0;
	srand(50);
	while(i++<1000) {
		a=rand()%100;
		obj=(*db).create_instance("Person");
		(*obj).setproperty_value("Age",a);
		};
	//Person pupulated

	tp=(*db).gettypenamed("Employee");
	i=0;
	srand(50);
	while(i++<1000) {
		a=rand()%100;
		obj=(*db).create_instance("Employee");
		(*obj).setproperty_value("Age",a);
		a=rand()%100;
		(*obj).setproperty_value("Salary",2*a);
		};
	//employee populated
	return 1;
} //end populatedb

void find_all_janitors(database My_DB){
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

	My_DB.close_stream(s_stream);
	}


int main(void){
database odb;
ODB_SET c;
query_stream *qst;
ODB_REF o;



	odb.parseFile("test.ocr");
   //odb.save_database("parsed.odb");
   //odb<<"settest.odb";
   //qst=odb.open_stream("type1","setp","=",(ODB_INT)2);
   //o=odb.get(qst);
   //while(o!=NULL){
	//	(*o).display(obuf);
	//	cout<<obuf.get_buffer()<<"\n";
	//	obuf.reset_buffer();
	//	cout.flush();
	//	o=odb.get(qst);
   //};
   return 1;
}


/*int main(void) {
database My_DB("");

int retval;
 float b;
 table *tbl;
 object *obj,*refobj, *searchobj;
 database db("Large");
 property_value *pv;
 query_stream *qst, *qst2;
 select_stream *sst, *sst2;
 semi_join_stream *sjs;
 char *tpnm;
 type *tp;
 object *o,*kid;
 ODB_REF o1,o2,o3;

 collection *coll;
 ODB_INT ok=-10,iv;
 ODB_REAL rv;
 ODB_CHAR cv;

 	//retval=db.load_database("HSS.odb");
	//qst=db.open_stream("SensorData");
	//obj=db.get(qst);
	//while(obj!=NULL){
	//	(*obj).display(obuf);
	//	cout<<obuf.get_buffer()<<"\n";
	//	obuf.reset_buffer();
	//	cout.flush();
	//	obj=db.get(qst);
   //};
   //cout<<" All objects scanned";
   //db.close_stream(qst);


 //if (retval) find_all_janitors(My_DB);
 //else cout<<"\nError reading file"<<"Compdb.odb";
 //return 1;
	populatedb(&db);
	db.save_database("Large.odb");
	return 1;
	if (db.load_database("compdb.odb")==0)
		cout<<"\nError reading file";
	else {
		cout<<"\nLoaded allright";
		obuf.reset_buffer();
		db.display(obuf);
		cout<<obuf.get_buffer()<<"\n";
		obuf.reset_buffer();
	};	db.add_property("Bitmap","Name",_OBJECT_);
	db.create_index("Bitmap","Name");
	cout<<"\nIndex created\n";





	//db.create_index("Person","Friend");
	//db.delete_property("Person","Age");
	//o=db.getobjectwithoid("Usertypes",56);
	//if (o!=NULL) {
	//	(*o).display(obuf);
	//	cout<<obuf.get_buffer()<<"\n";
	//	obuf.reset_buffer();
	//	cout.flush();
	//	}
	//else cout<<"No object found n";
	db.add_property("Bitmap","Chars",_COLLECTION_);
	tp=db.gettypenamed("Bitmap");
	o=db.create_instance("Bitmap");
	coll=new collection(_OBJECT_);
	o3=db.create_instance("Bitmap");
	coll->insert(o3);
	o1=db.create_instance("Bitmap");
	(*coll).insert(o1);
	o2=db.create_instance("Bitmap");
	(*coll).insert(o2);
	(*o).setproperty_value("Chars",coll);
	(*o).display(obuf);
	cout<<obuf.get_buffer()<<"\n";
	obuf.reset_buffer();
	(*coll).delete_element(o2);
	(*o).display(obuf);
	cout<<"\nObject deleted\n"<<obuf.get_buffer()<<"\n";
	obuf.reset_buffer();
	(*coll).delete_element(o1);
	(*o).display(obuf);
	cout<<"\nObject deleted\n"<<obuf.get_buffer()<<"\n";
	obuf.reset_buffer();
	(*coll).delete_element(o3);
	(*o).display(obuf);
	cout<<"\nObject deleted\n"<<obuf.get_buffer()<<"\n";
	obuf.reset_buffer();
	coll->insert(o3);

	db.add_property("Bitmap","INTProp",_INT_);
	db.add_property("Bitmap","REALProp",_REAL_);
	db.add_property("Bitmap","OBJProp",_OBJECT_);
	db.add_property("Bitmap","CHARProp",_CHAR_);
	db.add_property("Bitmap","SETProp",_COLLECTION_);

	db.setproperty_value(o2,"INTProp",(ODB_INT)5);
	db.setproperty_value(o2,"REALProp",(ODB_REAL)7.78);
	db.setproperty_value(o2,"OBJProp",o3);
	db.setproperty_value(o2,"CHARProp","Kalle Kodd");
	db.setproperty_value(o2,"SETProp",coll);

	(*o2).display(obuf);
	 cout<<"\nThe object\n"<<obuf.get_buffer()<<"\n";
	obuf.reset_buffer();
	ok=db.getproperty_value(o2,"REALProp", &iv);	if (ok>0) cout<<"\nRetreived integer value "<<iv<<"\n";
	else cout<<"\Retreiving iv went shitty error:"<<ok<<"\n";
	if (ok==-1) cout<<"The object was null\n";
	if (ok==-2) cout<<"The property did not exist\n";
	if (ok==-3) cout<<"Not integer valued prop\n";

	ok=db.getproperty_value(o2,"REALProp", &rv);
	if (ok>0) cout<<"\nRetreived real value "<<rv<<"\n";
	else cout<<"\Retreiving iv went shitty error:"<<ok<<"\n";
	if (ok==-1) cout<<"The object was null\n";
	if (ok==-2) cout<<"The property did not exist\n";

	ok=db.getproperty_value(o2,"OBJProp",&o);
	if (ok>0) {
		cout<<"\nRetreived object value ";
		o->display(obuf);
		cout<<obuf.get_buffer()<<"\n";
		obuf.reset_buffer();
		}
	else cout<<"\Retreiving iv went shitty error:"<<ok<<"\n";
	if (ok==-1) cout<<"The object was null\n";
	if (ok==-2) cout<<"The property did not exist\n";

	ok=db.getproperty_value(o2,"CHARProp", &cv);
	if (ok>0) cout<<"\nRetreived real value "<<cv<<"\n";
	else cout<<"\Retreiving iv went shitty error:"<<ok<<"\n";
	if (ok==-1) cout<<"The object was null\n";
	if (ok==-2) cout<<"The property did not exist\n";

	ok=db.getproperty_value(o2,"SETProp",&coll);
	if (ok>0) {
		cout<<"\nRetreived set value ";
		coll->display(obuf);
		cout<<obuf.get_buffer()<<"\n";
		obuf.reset_buffer();
		}
	else cout<<"\Retreiving iv went shitty error:"<<ok<<"\n";
	if (ok==-1) cout<<"The object was null\n";
	if (ok==-2) cout<<"The property did not exist\n";	/*
	cout<<"\nLooking for R”jjne";
	qst=db.open_stream("Employee","NickNames","=",(ODB_REAL)2.5);
	obj=db.get(qst);
	while(obj!=NULL){
		(*obj).display(obuf);
		cout<<obuf.get_buffer()<<"\n";
		obuf.reset_buffer();
		obj=db.get(qst);
		 };
	db.close_stream(qst);

	/*
	cout<<"\nOpening streams\n";
	cout.flush();
	qst=db.open_stream("Employee");
	qst2=(*qst).open_select_stream("Kids",kid);
	//qst=db.open_stream("Employee","Name","Staffan Flodin");
	//sjs=db.semi_join(qst,"Age",qst2, "Age");
	//sst=db.select(qst,"Salary",">",(ODB_INT)180);
	//sst2=(*qst).open_select_stream("Licence","ToKill");
	cout<<"\nScanning\n";
	cout.flush();
	obj=db.get(qst2);
	while(obj!=NULL){
		(*obj).display(obuf);
		cout<<obuf.get_buffer()<<"\n";
		obuf.reset_buffer();
		cout.flush();
		//(*obj).deleteobject();
		obj=db.get(qst2);
			 };
			 cout<<"All scanned \n";
			 db.close_stream(qst2);
			 //db.save_database("Compdb.odb");

   //
	return 1;
 }

 */
