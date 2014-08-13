#include "odb.h"
//#include "odb_imports.h"
#include <iostream.h>


#pragma import(outputbuffer::outputbuffer(),"@outputbuffer@$bctr$qv","ODB",118)
#pragma import(database::database(char*),"@database@$bctr$qpc","ODB",11)
#pragma import(database::database(),"@database@$bctr$qv","ODB",12)
#pragma import(object::~object(),"@object@$bdtr$qv","ODB",84)
#pragma import(validation::~validation(),"@validation@$bdtr$qv","ODB",258)

//import the outputbuffer destructor analogously

int main(void){
   outputbuffer *obuf=0;  //Nothing from the DLL invoked so far

   cout<<"\nSo far so good";

   obuf=new outputbuffer(); // invocation of the constructor

   cout<<"\nConstructed outputbuffer";

   delete obuf; // invocation of the destructor

   cout<<"\nSent your buffer object where it belongs...\n";

   cout<<"Now lets create a database\n";
   database *myodb=0;
//   myodb=new database("TestDB");
   delete myodb;

   cout << "I succeeded!\n";
   cout .flush();

   return 1;
}

