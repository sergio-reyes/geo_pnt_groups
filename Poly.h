#include <vector>
#include <climits>
#include "Point.h"
#include <cstddef>
#include <algorithm>
#include <string>


using namespace std;


class Poly
{
        public:
                //constructor
                Poly();

                //constructor specifying hint of size of vector
                Poly(int);

		//return number of points
		unsigned int points();

		//return min/max coordinates
		Point minX();
		Point minY();
		Point maxX();
		Point maxY();

		//set min/max coordinates
		void setMinX(Point);
		void setMinY(Point);
		void setMaxX(Point);
		void setMaxY(Point);

		//check if new min/max and set
		void checkSetAll(Point);
		void checkSetMinX(Point);
		void checkSetMinY(Point);
		void checkSetMaxX(Point);
		void checkSetMaxY(Point);

		//find min/max points
		void findMinX();
		void findMinY();
		void findMaxX();
		void findMaxY();
		void findAllMinMax();

		//add coordinates to list of points
		void addPoint(Point);

		//add coordinates to list of points
		void rmPoint(Point);

		//retrive coordinates at specified index
		Point getPoint(int);

		//save the number of points in vector
		void savePolySize();

		//get the prev saved number of points in vector
		unsigned int getSavedPolySize();

		void savePolyScore(unsigned int);
		unsigned int getPolyScore();

		//sort points of poly to go around perimeter
		void polySort();

		//determine if point is (roughly) within poly boundary 
		//this forms a rough outer boundary
		bool ptInBounds(Point); 

		//in miles
		//finds shortest distance of point to any line in poly
		double shortestDist2Poly(Point);

		//checks to see if point is within poly
		//note: does not handle colinear situations
		int isPtInPoly(Point);


        private:
		//1 if lines intersect, otherwise 0
		int intersect(Point,Point,Point,Point);
		int intersectChance(Point, Point, Point, Point);

		void initMinMax();

			//private variables
			vector<Point> polyVec;
	
			Point ptMinX;
			Point ptMinY;
			Point ptMaxX;
			Point ptMaxY;

			unsigned int prevSize;
			unsigned int score;
	

};  //end of class Poly

