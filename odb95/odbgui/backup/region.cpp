#include "d:\users\staffan\odb\odb95\odb95.h"

 region::region(void){
 	x1=0;
   x2=0;
   y1=0;
   y2=0;
   next=NULL;
 }

 region::region(int ax1,int ay1, int ax2, int ay2){
 	x1=ax1;
   x2=ax2;
   y1=ay1;
   y2=ay2;
   next=NULL;
 }

 void region::set(int ax1,int ay1, int ax2, int ay2){
 	x1=ax1;
   x2=ax2;
   y1=ay1;
   y2=ay2;
 }

region *region::getNext(void){return next;}

region *region::getNextToSet(void){
	if (x1==-1) return this;

   if (next==NULL){
   	next=new region();
      return next;
      }

   return next->getNextToSet();

}

int region::invalidRegion(void){
	if (x1==-1) return 1;
   else return 0;
}

region *region::findRegion(int mx, int my){
	if (!invalidRegion()&&(x1<mx)&&(x2>mx)&&(y1<my)&&(y2>my)) return this;
   if (next!=NULL) return next->findRegion(mx,my);
   else return NULL;
}

void region::invalidate(void){
	x1=-1;
   if (next!=NULL) next->invalidate();
}


region::~region(){
	if (next!=NULL) delete next;
}

char *region::getName(void){
	return name;
}

void region::setName(char * nm){
	strcpy(name,nm);
}
