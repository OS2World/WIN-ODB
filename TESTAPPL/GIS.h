/* ---------------------------------------------------------------------------
// FILE NAME:		gis.h
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

#ifndef _GIS_H_
#define _GIS_H_


//#define min(A,B) ((A)<(B)?(A):(B))
//#define max(A,B) ((A)>(B)?(A):(B))

float min(float a,float b);

float max(float a,float b);



class BoundedLine;




// ---------------------------------------------------------------------------
// CLASS:               List		
//
// DESCRIPTION:		Implements a doubly linked list. Elements can be
//                      ordered by inserting them at a particuler position
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
template <class T> class List {
  private:
      T element;
      List *next;
      List *prev;
      int  hasElem;
      int index;
   public:
      List(void);
      List(const List &l);
      List(T &e);
      ~List(void);
      List *getNext() const ;
      List *getPrev(void) const ;
      void setNext(List *l);
      void setPrev(List *l);
      int insertElement(T &e);
      int insertElement(T &e,int i);
      void deleteElement(void);
      void setIndex(int i);
      int  getIndex(void) const ;
      T getElement(void) const ;
      void display(void);
};




// ---------------------------------------------------------------------------
// CLASS:               Point		
//
// DESCRIPTION:		A point is a position in a 2 dimensional space
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
class Point { 
  private:
      float x;
      float y;
      int valid;
   public:
      Point(void);
      Point( float ax, float ay);
      Point(const Point &p);
      ~Point(void);
      float getX(void) const {return x;};
      float getY(void) const {return y;};
      void setX( float ax){x=ax;};
      void setY( float ay){y=ay;};
      void display(void);
      int isValid(void) const {return valid;};
      void invalidatePoint(void){valid=0;};
      void validatePoint(void){valid=1;};
      int operator ==(const Point &p);
};




// ---------------------------------------------------------------------------
// CLASS:               Line
//
// DESCRIPTION:		A line is an indefinite stretch described by y=kx+m  
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
class Line {
  protected:
      float k;
      float m;
      float x;
      float y;
      int constantY;
      int constantX;
   public:
      Line(void);
      Line(Point p1,Point p2);
      Line(const Line &l);
      ~Line(void){};
      float getK(void) const {return k;};
      float getM(void) const {return m;};
      void setK(float ax){k=ax;};
      void setM(float ay){m=ay;};
      float getX(float y) const ;
      float getY(float x) const ;
      Point intersectionPoint(Line l2);
      Point intersectionPoint(BoundedLine l2);
      void makeLine(Point p1, Point p2);
      void display(void);
      int isConstantX(void) const { return constantX; };
      int isConstantY(void) const { return constantY; };

      //
      // some syntactic sugar
      //
      Point operator +(Line l2);         // a + looks like crossing lines ?!
      Point operator +(BoundedLine l2);
      int operator ==(const Line &l);
};




// ---------------------------------------------------------------------------
// CLASS:		BoundedLine
//
// DESCRIPTION:		A Bounded Line is a definite stretch described by
//                      y=kx+m
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
class BoundedLine : public Line {
  private:
      Point upperBound;
      Point lowerBound;
   public:
      BoundedLine(void);
      BoundedLine(Point p1,Point p2);
      BoundedLine(const BoundedLine &bl);
      ~BoundedLine();
      Point intersectionPoint(Line l2);
      Point intersectionPoint(BoundedLine l2);
      int outOfRange(Point p);
      Point getUpperBound(void) const { return upperBound;};
      Point getLowerBound(void) const { return lowerBound; };
      void display(void);
      void makeLine(Point p1, Point p2);
      Point operator +(Line l2);
      Point operator +(BoundedLine l2);
      int   operator ==(const BoundedLine &l2);
};




// ---------------------------------------------------------------------------
// CLASS:		PolyLine
//
// DESCRIPTION:		A Polyline is a set of connected BoundedLines
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
class PolyLine  : public BoundedLine {
  protected:
      List<BoundedLine> *segments;
      List<BoundedLine> *currentelem;
      int                currentindex;
      int                noSegments;
      //
      // A few ones to describe a bounding box
      //
      Point              swp; //south - west coordinate
      Point              nep;
      int                closed;
      void               checkForCloseness(void);
      advanceToNextLine(void);
      advanceToPreviousLine(void);
      int advanceToNthLine(int i);

   public:
      PolyLine();
      PolyLine(const PolyLine &p);
      ~PolyLine();

      int setSegment(int no,BoundedLine bl);
      Point intersectionPoint(Line l2);
      Point intersectionPoint(BoundedLine l2);
      int outOfRange(Point p);
      void display(void);
      int isClosed(void){return closed;};

      Point getSWP(void){return swp;};
      Point getNEP(void){return nep;};

      BoundedLine getNthLine(int i);
      List<BoundedLine> *getSegments(void) const { return segments; };
      int getNoSegments(void) { return noSegments; };

      Point operator +(Line l);
      Point operator +(BoundedLine l);
      BoundedLine operator[](int);

};




// ---------------------------------------------------------------------------
// CLASS:		Area
//
// DESCRIPTION:		Area is a 2 dimesional space
//			
//			
//
// DATE:		1998-08-10
// AUTHOR:		staffan flodin STN Atlas (Australia)
// --------------------------------------------------------------------------
class Area {
  protected:
    PolyLine areaDefinition;
  public:
    Area(void);
    Area(PolyLine &pl);
    Area(const Area &a);
    ~Area(void);

    PolyLine getAreaDefinition(void) const {return areaDefinition;};
    void display(void);
    int overlap(Point p);
    int overlap(Line l);
    int overlap(BoundedLine l);
    int overlap(PolyLine l);
    int overlap(Area a);
};

#endif



