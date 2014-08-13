#ifndef _validation_
#define _validation_

#include "..\source\store.h"

class validation
{
private:
	database *db;
public:
	validation(void);
   validation(database *d);
   ~validation(void);
   void setDb(database *d){ db=d; };
	int validate_integer(char *inp,ODB_INT &value);
	int validate_real(char *inp,ODB_REAL &value);
	int validate_name(char *ch);
	int validate_set(char *inp,ODB_SET set);
};

#endif

