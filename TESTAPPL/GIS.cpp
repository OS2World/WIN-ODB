/* ---------------------------------------------------------------------------
// FILE NAME:		gis.cpp
//
// FULL FILE NAME:	gis operations module
//
// AUTHOR / COMPANY:	Staffan Flodin / (C) STN Atlas (Australia)
//
// CREATION DATE:	1998-08-10
//
// HARDWARE/OS:		DEC Alpha, Digital UNIX v4.0
// DESCRIPTION:		Implements the gis operations reg'd for HSS
//			
//
// $Revision: $
// $State: Exp $
// ---------------------------------------------------------------------------
*/

#include <iostream.h>

#include "..\testappl\gis.h"



//----------------------------------------------------------------------------
// some generic list stuff
//----------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List<T>::List(void){
	next=NULL;
	prev=NULL;
	hasElem=0;
	index=0;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List<T>::List(const List &l){

	element=l.getElement();
	prev=l.getPrev();
	index=l.getIndex();
	if (l.getNext()!=NULL){
		next=new List<T>;
		*next=*(l.getNext());
	}
	else next=NULL;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List<T>::List(T &e){
	next=NULL;
	prev=NULL;
	element=e;
	hasElem=1;
	index=0;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List<T>::~List(void){

	if (next!=NULL) delete next;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List *List<T>::getPrev(void) const {

	return prev;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> int List<T>::getIndex(void) const {

	return index;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> List *List<T>::getNext(void) const {

	return next;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> void List<T>::setNext(List *l){

	next=l;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> void List<T>::setPrev(List *l){

	prev=l;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> int List<T>::insertElement(T &e){
List<T> *l=NULL;
int rv;

	if (!hasElem){
		element=e;
		hasElem=1;
		rv=1;
	}
	else {
		l=new List(e);
		l->setNext(next);
		if (next!=NULL) next->setPrev(l);
		l->setIndex(index+1);
		l->setPrev(this);
		next=l;
		rv=1;
	}

	return rv;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> int List<T>::insertElement(T &e,int i){
T te;
int rv;

	if (i==index){
		te=element;
		element=e;
		hasElem=1;
		rv=1;

		if (hasElem){
			insertElement(te);
		}
	}

	if ((i==index+1)&&(next==NULL)) rv=insertElement(e);

	else {
		if (next!=NULL) rv=next->insertElement(e,i);
		else rv=0;
	}

	return rv;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> void List<T>::setIndex(int i){

	if (i!=index) {
		index=i;
		if (next!=NULL) next->setIndex(index+1);
	}
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> void List<T>::deleteElement(void){

	if (next!=NULL) next->setPrev(prev);

	if (prev!=NULL) prev->setNext(next);

	next->setIndex(index);

	next=NULL;
	
	prev=NULL;

	delete this;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> T List<T>::getElement(void) const {

	return element;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> void List<T>::display(void){

	if (hasElem) {
		cout<<endl<<"List Element : ";
		element.display();
	}

	if (next!=NULL) next->display();

}





//----------------------------------------------------------------------------
// some point methods
//----------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point::Point(void){
	setX(0); 
	setY(0); 
	valid=1;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point::Point( float ax, float ay){
	setX(ax); 
	setY(ay); 
	valid=1;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point::Point(const Point &p){

	setX(p.getX()); 
	setY(p.getY());
	if (p.isValid()) validatePoint();
	else invalidatePoint();
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point::~Point(void){
}


void Point::display(void){

	cout<<"Point y="<<getY()<<" ,x="<<getX();

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int Point::operator ==(const Point &p){

	if ((p.getX()==x)&&(p.getY()==y)) return 1;
	else return 0;
}





//----------------------------------------------------------------------------
// some line methods
//----------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Line::Line(void){
	setK(0); 
	setM(0);
	x=0;
	y=0;
	constantY=0;
	constantX=0;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Line::Line(Point p1, Point p2){

	makeLine(p1,p2);

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Line::Line(const Line &l){

	setK(l.getK()); 
	setM(l.getM());
	constantX=l.isConstantX();
	constantY=l.isConstantY();
	if (constantX) x=l.getX(0);
	else x=0;
	if (constantY) y=l.getY(0);
	else y=0;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
float  Line::getX(float y) const {
float retX;

	if (constantX) retX=x;	
	else retX=(y-m)/k;

	return retX;

}
      



// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
float Line::getY(float x) const {
float retY;

	if (constantY) retY=y;
	else retY=k*x+m;

	return retY;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void Line::makeLine(Point p1, Point p2){

	//
	// cater for the special cases where the line is
	// a straight line and cannot be represented by y=kx+m
	// another representation is to represent a line as
	// a point and an angle
	//
	if (p1.getX()==p2.getX()) {
		constantX=1;
		x=p1.getX();
	}
	if (p1.getY()==p2.getY()) {
		constantY=1;
		y=p1.getY();
	}

	if ((p1.getX()!=p2.getX())&&(p1.getY()!=p2.getY())){
		setK((p2.getY()-p1.getY())/(p2.getX()-p1.getX()));
		setM(p1.getY()-k*p1.getX());
	}

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point Line::intersectionPoint(BoundedLine l2){

	return l2.intersectionPoint(*this);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point Line::intersectionPoint(Line l2){
Point ret;
float deltam,deltak;


	//
	// Due to the y=kx+m representation we must first cover 
	// all special cases of combinations of constant x, y's
	//
	if (constantX&&l2.isConstantX()){
		if (x==l2.getX(0)) ret.setX(x);
		else ret.invalidatePoint();
		return ret;
	}
	
	if (constantX&&l2.isConstantY()){
		ret.setX(x);
		ret.setY(l2.getY(x));
		return ret;
	}

	if (constantX){
		ret.setX(x);
		ret.setY(l2.getY(x));
		return ret;
	}

	if (constantY&&l2.isConstantY()){
		if (y==l2.getY(0)) ret.setY(y);
		else ret.invalidatePoint();
		return ret;
	}
	
	if (constantY&&l2.isConstantX()){
		ret.setY(y);
		ret.setX(l2.getX(y));
		return ret;
	}

	if (constantY){
		ret.setY(y);
		ret.setX(l2.getX(y));
		return ret;
	}


	if (!constantX&&!constantY&&!l2.isConstantX()&&!l2.isConstantY()) {
		deltam=l2.getM()-m;
		deltak=k-l2.getK();
		ret.setX(deltam/deltak);
		ret.setY(getY(ret.getX()));
		return ret;
	}

	return l2.intersectionPoint(*this);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void Line::display(void){

	if (constantY) cout<<"Line y="<<y;
	if (constantX) cout<<"Line x="<<x;
	if (!constantY&&!constantX) cout<<"Line y="<<getK()<<"x +"<<getM();

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point Line::operator +(Line l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point Line::operator +(BoundedLine l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int Line::operator ==(const Line &l){

	if ((l.getK()==k)&&(l.getM()==m)&&(getX(0)==l.getX(0))&&
	     (getY(0)==l.getY(0))) return 1;
	else return 0;

}



//-----------------------------------------------------------------------------
// bounded line methods
//----------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine::BoundedLine(void){

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine::BoundedLine(Point p1,Point p2):Line(p1,p2){
	upperBound=p1;
	lowerBound=p2;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine::BoundedLine(const BoundedLine &l){

	upperBound=l.getUpperBound();
	lowerBound=l.getLowerBound();
	setK(l.getK());
	setM(l.getM());
	constantX=l.isConstantX();
	constantY=l.isConstantY();
	if (constantX) x=l.getX(0);
	else x=0;
	if (constantY) y=l.getY(0);
	else y=0;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine::~BoundedLine(void){
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void BoundedLine::makeLine(Point p1, Point p2){

	Line::makeLine(p1,p2);
	upperBound=p2;
	lowerBound=p1;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int BoundedLine::outOfRange(Point p){
float minx, maxx, miny, maxy;

	minx=min(lowerBound.getX(),upperBound.getX());
	maxx=max(lowerBound.getX(),upperBound.getX());
	miny=min(lowerBound.getY(),upperBound.getY());
	maxy=max(lowerBound.getY(),upperBound.getY());

	if (constantX){
		if ((p.getY()>=miny)&&(p.getY()<=maxy))
			return 0; 
		else return 1;
	}

	if (constantY){
		if ((p.getX()>=minx)&&(p.getX()<=maxx))
			return 0; 
		else return 1;
	}
		
	if ((p.getX()>=minx)&&(p.getX()<=maxx)&&
	    (p.getY()>=miny)&&(p.getY()<=maxy))
	   	return 0;

	return 1;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point BoundedLine::intersectionPoint(Line l2){
Point lp;

	lp=Line::intersectionPoint(l2);

	if (outOfRange(lp)) lp.invalidatePoint();

	return lp;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point BoundedLine::intersectionPoint(BoundedLine l2){
Point lp;

	lp=Line::intersectionPoint(l2);

	if (outOfRange(lp)||l2.outOfRange(lp)) 
		lp.invalidatePoint();

	return lp;
}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void BoundedLine::display(void){

	cout<<"Bounded ";
	Line::display();
	cout<<" P1:";
	upperBound.display();

	cout<<" P2: ";
	lowerBound.display();

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point BoundedLine::operator +(Line l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point BoundedLine::operator +(BoundedLine l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int  BoundedLine::operator ==(const BoundedLine &l){
	
	if ((l.getK()==k)&&(l.getM()==m)&&(getX(0)==l.getX(0))&&
	     (getY(0)==l.getY(0))&&(upperBound==l.getUpperBound())&&
	     (lowerBound==l.getLowerBound())) return 1;
	else return 0;
}





//------------------------------------------------------------------
// some polyline methods
//------------------------------------------------------------------
// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
PolyLine::PolyLine(){

	segments=NULL;
	noSegments=0;
	currentelem=NULL;
	currentindex=0;
	checkForCloseness();
}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
PolyLine::PolyLine(const PolyLine &p){

	if (p.getSegments()!=NULL){

		if (segments==NULL) segments=new List<BoundedLine>();

		*segments=*(p.getSegments());
	}

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
PolyLine::~PolyLine(){

	//delete segments;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int PolyLine::setSegment(int no,BoundedLine bl){
int rv;
	if (noSegments==0){
		if (no==0){
			segments = new List<BoundedLine>(bl);
			noSegments++;
			currentelem=segments;
			currentindex=0;
			rv=1;
		}
		else rv=0;
	}
	else {
		
		rv=segments->insertElement(bl,no);
		noSegments++;
	
		//
		// if we insert something before our curr pointer, we
		// better add one to its index to make it correct
		//
		if (no<=currentindex) currentindex++;

	}
	

	//
	// If the line inserted allright update the bounding box
	// definition
	//
	if (rv>0){
		swp.setX(min(swp.getX(),min(bl.getUpperBound().getX(),
				bl.getLowerBound().getX())));
		swp.setY(min(swp.getY(),min(bl.getUpperBound().getY(),
				bl.getLowerBound().getY())));
		nep.setX(max(nep.getX(),max(bl.getUpperBound().getX(),
				bl.getLowerBound().getX())));
		nep.setY(max(nep.getY(),max(bl.getUpperBound().getY(),
				bl.getLowerBound().getY())));
	}

	checkForCloseness();
	return rv;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int PolyLine::advanceToNextLine(void){
int retval=0;

	if (currentelem!=NULL){
		currentelem=currentelem->getNext();
		currentindex++;
		retval=1;
	}

	if (currentelem==NULL){
		
		currentelem=segments;
		currentindex=0;
	}

	return retval;

}






// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int PolyLine::advanceToPreviousLine(void){
int retval=0;

	if (currentelem!=NULL){
		currentelem=currentelem->getPrev();
		currentindex--;
		retval=1;
	}
	if (currentelem==NULL){
		
		currentelem=segments;
		currentindex=0;

	}


	return retval;

}



// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine PolyLine::getNthLine(int i){
BoundedLine bl;

	if (advanceToNthLine(i)) 
		return currentelem->getElement();
	else return bl;
}








// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int PolyLine::advanceToNthLine(int i){


	if ((i<0)||(i>=noSegments)) return 0; // out of range data	

	if (currentindex==i) return 1;        // got it at first shot

	if (currentindex>i){
		while(currentindex>i) advanceToPreviousLine();
	}

	if (currentindex<i){
		while(currentindex<i) advanceToNextLine();
	}

	if (currentindex==i) return 1; 
	else return 0;

}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point PolyLine::intersectionPoint(Line l2){
int i=0;
Point p;

	//
	// Invalidate p just to get in to the loop
	// 
	p.invalidatePoint(); 
	currentelem=segments;
	currentindex=0;

   	while((!p.isValid())&&(i<noSegments)){

		p.validatePoint();

		p=(*this)[i]+l2;

		i++;

      }

      if (i>=noSegments) p.invalidatePoint();

      return p;
}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point PolyLine::intersectionPoint(BoundedLine l2){
int i=0;
Point p;

	//
	// Invalidate p just to get in to the loop
	// 
	p.invalidatePoint(); 
	currentelem=segments;
	currentindex=0;

   	while((!p.isValid())&&(i<noSegments)){

		p.validatePoint();

		p=(*this)[i]+l2;

		i++;

      }

      if (i>=noSegments) p.invalidatePoint();

      return p;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void PolyLine::checkForCloseness(void){
Point pStart,pStop;
List <BoundedLine> *lptr=NULL,*lprev=NULL;

	if (segments==NULL) closed=1;
	else {
		pStart=segments->getElement().getLowerBound();
		
		lptr=segments;
		lprev=lptr;

		while(lptr!=NULL) {

			lprev=lptr;
			lptr=lptr->getNext();

		};

		
		pStop=lprev->getElement().getUpperBound();

		if (pStart==pStop) closed=1;
		else closed=0;
	}
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
void PolyLine::display(void){

	cout<<"Polyline described by ";
	if (segments!=NULL) segments->display();
	else cout<<" No area described ";
	cout<<endl;

}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point PolyLine::operator +(BoundedLine l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Point PolyLine::operator +(Line l2){

	return intersectionPoint(l2);
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
BoundedLine PolyLine::operator[](int i){
    
    return getNthLine(i);

}






//---------------------------------------------------------------------------
// Area stuff
//---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Area::Area(void) : areaDefinition() {
}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Area::Area(PolyLine &pl) : areaDefinition() {

	areaDefinition=pl;
}




// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Area::Area(const Area &a){

	areaDefinition=a.getAreaDefinition();;
}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
Area::~Area(void){

}





// ---------------------------------------------------------------------------
// METHOD:		
//
// DESCRIPTION:		
//			
//			
//
// ARGUMENTS:		
//
// RETURNS:		
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
int Area::overlap(Point p){
BoundedLine northerly,southerly,easterly,westerly,lines[4];
Point sw,ne,north,east,west,south,intersectionPoint;
float x,y;
int i,j,nofCrossings[4]={0,0,0,0},inside=1;

	x=p.getX();
	y=p.getY();

	sw=areaDefinition.getSWP();
	ne=areaDefinition.getNEP();

	//
	// south, east, north and wast are points that together with p
	// will describe the four perpendicular lines out of the point p
	// that we need in order to check intersections with the area
	//
	south.setX(p.getX());
	south.setY(sw.getY());

	north.setX(p.getX());
	north.setY(ne.getY());

	east.setX(ne.getX());
	east.setY(p.getY());

	west.setX(sw.getX());
	west.setY(p.getY());


	//
	// the lines can now be defined
	//
	northerly.makeLine(p,north);
	southerly.makeLine(p,south);
	easterly.makeLine(p,east);
	westerly.makeLine(p,west);
	lines[0]=northerly;lines[1]=southerly;
	lines[2]=easterly;lines[3]=westerly;

	i=0;
	j=0;

	//
	// Now, were ready for action. Count the number of crossings
	// between the 4 lines from the point and each line segment
	// in the polyine
	//
	while(i<areaDefinition.getNoSegments()){
		
		while(j<4){

			intersectionPoint=lines[j]+areaDefinition[i];

			/*cout<<endl;
			lines[j].display(); cout<<endl<<"and "; 
			areaDefinition[i].display(); cout<<"\nintersect at ";
			intersectionPoint.display();
			if (intersectionPoint.isValid()) cout<<" is valid ";
			else cout<<" is outside lines ";*/

			if (intersectionPoint.isValid()){

				//
				// if we get an intersection enumerate
				//
				nofCrossings[j]=nofCrossings[j]+1;

				//
				// the values we want is 0,1 and 2 meaning
				// no crossings, odd number and even number
				//
				if (nofCrossings[j]==3) nofCrossings[j]=1;
			}

			j++;
		}

		i++;
		j=0;
		cout<<endl;

	}

	j=0;
	while(j<4){
		//cout<<endl<<"Result for line "<<j<<" is "<<nofCrossings[j];

		if ((nofCrossings[j]==0)||(nofCrossings[j]==2)) inside=0;
		j++;
	}

	return inside;
}





int Area::overlap(Line l){
Point p;

	//
	// An area and a line overlaps if any of the following holds:
	//   i: the line crosses the border of the area
	//  ii: the end point of the line is in the area
	// iii: the start point of the line is in the area
	//
	// in this case we are dealing Line wich is an indefinite stretch
	// cases ii and iii are not applicable
	// 
	p=areaDefinition+l;
	if (p.isValid()) return 1;

	return 0;

}





int Area::overlap(BoundedLine l){
Point p;

	//
	// An area and a line overlaps if any of the following holds:
	//   i: the line crosses the border of the area
	//  ii: the end point of the line is in the area
	// iii: the start point of the line is in the area
	// 
	p=areaDefinition+l;
	if (p.isValid()) return 1;

	if (overlap(l.getLowerBound())) return 1;

	if (overlap(l.getUpperBound())) return 1;

	return 0;

}







void Area::display(void){

	cout<<endl<<" Area described by ";
	areaDefinition.display();
	cout<<endl;
}




//
// misc
//

float min(float a,float b){
	if (a<b) return a; else return b;
}

float max(float a,float b){
	if (a>b) return a; else return b;
}

