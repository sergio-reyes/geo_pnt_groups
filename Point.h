#ifndef POINT_H
#define POINT_H
#define NAMEIT_H

#include <math.h>

#define _USE_MATH_DEFINES
#define RADIUS			3961 //earth radius in miles


class Point
{
        public:
                //constructor
                Point();

		//set values
		void set(double,double);
		void set(double,double,int);
		void setg(int);
		void setg1(int);
		void setg2(int);

		//retrieve values 
		double x();
		double y();
		int g();
		int g1();
		int g2();

		//determine if point is left of 2 other points.  1=true
                int pointIsLeft(Point, Point);

		//calculate distances (in miles )
		double dist(Point);
		double distX(Point);
		double distY(Point);

        private:
		double deg2Rad(double);	

			//private Variables
			double xval;
			double yval;
			int group;
			int tryG1;
			int tryG2;
	

};  //end of class Point

#endif
