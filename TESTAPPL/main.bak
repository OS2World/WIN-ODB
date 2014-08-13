#include <iostream.h>

#include "..\testappl\gis.h"

int main(void) {


Point p1(-2,-1),p2(-2,1), p3(-3,2), p4(3,2), intersectionPoint;
Point plp1(7,8),plp2(5,-8),plp3(-7,-6),plp4(-5,5);
Point tlp1(-3,-2),tlp2(2,1),testP(0.6,0.5);
BoundedLine l1,l2,tl;
BoundedLine pll1,pll2,pll3,pll4;
PolyLine pl;
Area *a=NULL;


	l1.makeLine(p1,p2);
	l2.makeLine(p3,p4);
	tl.makeLine(tlp1,tlp2);
	intersectionPoint=l1+l2;


	intersectionPoint.display();
	cout<<endl<<" is the instersection point of ";
	l1.display();
	l2.display();
	if (intersectionPoint.isValid()) cout<<" And it is on the lines"<<endl;
	else cout<<" But it is outside the ranges of the lines"<<endl;

	cout<<endl<<"--------------------------------------------------"<<endl;
	pll1.makeLine(plp1,plp2);
	pll2.makeLine(plp2,plp3);
	pll3.makeLine(plp3,plp4);
	pll4.makeLine(plp4,plp1);
	pl.setSegment(0,pll1);
	pl.setSegment(1,pll2);
	pl.setSegment(2,pll3);
	pl.setSegment(3,pll4);

	intersectionPoint=pl.intersectionPoint(tl);

	intersectionPoint.display();
	cout<<endl<<" is the instersection point of ";
	pl.display();
	tl.display();
	if (intersectionPoint.isValid()) cout<<" And it is on the lines"<<endl;
	else cout<<" But it is outside the ranges of the lines"<<endl;



	if (pl.isClosed()) cout<<" the line is closed "<<endl;
	else cout<<" the line is open "<<endl; 



	cout<<endl<<"--------------------------------------------------"<<endl;
	cout<<endl<<"--------------------------------------------------"<<endl;
	a=new Area(pl);
	if (a->overlap(testP)) {
		cout<<endl;
		testP.display();
		cout<<" is inside of ";
		a->display();
	}
	else
	{
		cout<<endl;
		testP.display();
		cout<<" is outside of ";
		a->display();
	}

   return 1;
}


