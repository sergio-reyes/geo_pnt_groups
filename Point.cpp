#include "Point.h"



//************************************************************************************************

//constructor:
Point::Point()
{

}// end constructor

//************************************************************************************************

void Point::set(double x, double y)
{
	xval = x;
	yval = y;
	group=-1;
	tryG1=-1;
	tryG2=-1;

}// end set

//************************************************************************************************
void Point::set(double x, double y, int g)
{
	xval = x;
	yval = y;
	group = g;

}// end set overload

//************************************************************************************************

void Point::setg(int val)
{
	group=val;
}//end setg

//************************************************************************************************

void Point::setg1(int val)
{
	tryG1=val;
}//end setg1

//************************************************************************************************

void Point::setg2(int val)
{
	tryG2=val;
}//end setg2

//************************************************************************************************

double Point::x()
{
	return(xval);

} // end x

//************************************************************************************************

double Point::y()
{
        return(yval);

} // end y

//************************************************************************************************

int Point::g()
{
        return(group);

} // end g

//************************************************************************************************
int Point::g1()
{
        return(tryG1);

} // end g1
//************************************************************************************************
int Point::g2()
{
        return(tryG2);

} // end g2
//************************************************************************************************

double Point::dist(Point otherPt)
{
	//variables
	double deltaX;
	double deltaY;
	double x1,x2,y1,y2;
	double a,c;

	x1=deg2Rad(xval);
	y1=deg2Rad(yval);
	x2=deg2Rad(otherPt.x());
	y2=deg2Rad(otherPt.y());

	deltaX=x2-x1;
	deltaY=y2-y1;


	a = pow((sin(deltaX/((double)2))),2) + cos(x2) * cos(x1) * pow((sin(deltaY/((double)2))),2);
	c=2*atan2(sqrt(a),sqrt(1-a));
	return(RADIUS*c);


}//end of dist

//************************************************************************************************
double Point::distX (Point otherPt)
{

	//variables
	double deltaX;
	double deltaY;
	double x1,x2;
	double a,c;

	x1=deg2Rad(xval);
	x2=deg2Rad(otherPt.x());

	deltaX=x2-x1;
	//keep Y constant at zero, only comparing x-distance 
	deltaY=0;

	a = pow((sin(deltaX/((double)2))),2) + cos(x2) * cos(x1) * pow((sin(deltaY/((double)2))),2);
	c=2*atan2(sqrt(a),sqrt(1-a));
	return(RADIUS*c);

} //end of distX 

//************************************************************************************************

double Point::distY(Point otherPt)
{
	//variables
	double deltaX;
	double deltaY;
	double x1,x2,y1,y2;
	double a,c;

	x1=deg2Rad(xval);
	y1=deg2Rad(yval);
	//keep x constant, only comparing y-distance
	x2=x1;
	y2=deg2Rad(otherPt.y());

	deltaX=x2-x1;
	deltaY=y2-y1;

	a = pow((sin(deltaX/((double)2))),2) + cos(x2) * cos(x1) * pow((sin(deltaY/((double)2))),2);
	c=2*atan2(sqrt(a),sqrt(1-a));
	return(RADIUS*c);


}//end of distY

//************************************************************************************************

double Point::deg2Rad(double deg)
{
	return(deg*((M_PI)/180));

} //end deg2Rad
//************************************************************************************************

int Point::pointIsLeft(Point A, Point B)
{
        //variables
        double loc;

        //cross product to determine point is left or right of line defined by A->B
        loc=( ( B.x()-A.x() )*( yval-A.y() ) - ( B.y()-A.y() )*( xval-A.x() ));

        if (loc>0)
                return(1);
        else
                return(0);

}//end of pointIsLeft

//************************************************************************************************
