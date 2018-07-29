
#include "Poly.h"


using namespace std;

//************************************************************************************************

//constructor:
Poly::Poly()
{
	initMinMax();
	prevSize=0;
	score=0;

} //end of constructor

//************************************************************************************************

//constructor:
Poly::Poly(int sizeHint)
{
	polyVec.reserve(sizeHint);	
	initMinMax();
	prevSize=0;
	score=0;

} //end of constructor

//************************************************************************************************

unsigned int Poly::points()
{
	return(polyVec.size());

}//end of points

//************************************************************************************************

void Poly::addPoint(Point pnt)
{

	polyVec.push_back(pnt);

}//end of addPoint

//************************************************************************************************
void Poly::rmPoint(Point pnt)
{
	int loop, found;


	loop=0; found=0;
	//go through vector
	while ( (loop < polyVec.size()) && ( !found  )   )
	{
		//search for matching points
		if ( (pnt.x()==polyVec.at(loop).x()) && (pnt.y()==polyVec.at(loop).y()) )
		{
			polyVec.erase(polyVec.begin()+loop);
			found=1;
		}
		loop=loop+1;
	}


}//end of addPoint

//************************************************************************************************

void Poly::initMinMax()
{
	ptMinX.set(INT_MAX,INT_MAX); ptMinY.set(INT_MAX,INT_MAX);
	ptMaxX.set(INT_MIN,INT_MIN); ptMaxY.set(INT_MIN,INT_MIN);
	
}//end of initMinMax
//************************************************************************************************

Point Poly::minX()
{
	return(ptMinX);

}//end of minX

//************************************************************************************************

Point Poly::maxX()
{
	return(ptMaxX);

}//end of maxX

//************************************************************************************************

Point Poly::minY()
{
	return(ptMinY);

}//end of minY

//************************************************************************************************

Point Poly::maxY()
{
	return(ptMaxY);

}//end of maxY

//************************************************************************************************

void Poly::setMinX(Point pt)
{
	ptMinX=pt;

}//end of setMinX

//************************************************************************************************

void Poly::setMinY(Point pt)
{
	ptMinY=pt;

}//end of setMinY

//************************************************************************************************

void Poly::setMaxX(Point pt)
{
	ptMaxX=pt;

}//end of setMaxX

//************************************************************************************************

void Poly::setMaxY(Point pt)
{
	ptMaxY=pt;

}//end of setMaxY

//************************************************************************************************
void Poly::checkSetMinX(Point pt)
{
	if (pt.x() < ptMinX.x() )
		ptMinX=pt;

}//end checkSetMinX

//************************************************************************************************

void Poly::checkSetMinY(Point pt)
{
	if (pt.y() < ptMinY.y() )
		ptMinY=pt;

}//end checkSetMinY

//************************************************************************************************

void Poly::checkSetMaxY(Point pt)
{
	if (pt.y() > ptMaxY.y() )
		ptMaxY=pt;

}//end checkSetMaxY

//************************************************************************************************

void Poly::checkSetMaxX(Point pt)
{
	if (pt.x() > ptMaxX.x() )
		ptMaxX=pt;

}//end checkSetMaxX

//************************************************************************************************

void Poly::checkSetAll(Point pt)
{
	checkSetMinX(pt);
	checkSetMinY(pt);
	checkSetMaxX(pt);
	checkSetMaxY(pt);
}

//************************************************************************************************

void Poly::findMinX()
{
	//variables
	int loop;

	//initialize
	ptMinX.set(INT_MAX,INT_MAX); 

	//go through the list
	for(loop=0; loop<polyVec.size(); loop++)
		checkSetMinX(polyVec.at(loop));

}//end of findMinX

//************************************************************************************************

void Poly::findMinY()
{
	//variables
	int loop;

	//initialize
	ptMinY.set(INT_MAX,INT_MAX);

	//go through the list
	for(loop=0; loop<polyVec.size(); loop++)
		checkSetMinY(polyVec.at(loop));

}//end of findMinY

//************************************************************************************************

void Poly::findMaxX()
{
	//variables
	int loop;

	//initialize
	ptMaxX.set(INT_MIN,INT_MIN);

	//go through the list
	for(loop=0; loop<polyVec.size(); loop++)
		checkSetMaxX(polyVec.at(loop));

}//end of findMaxX

//************************************************************************************************

void Poly::findMaxY()
{
	//variables
	int loop;

	//initialize
	ptMaxY.set(INT_MIN,INT_MIN);

	//go through the list
	for(loop=0; loop<polyVec.size(); loop++)
		checkSetMaxY(polyVec.at(loop));

}//end of findMaxY

//************************************************************************************************

void Poly::findAllMinMax()
{

	//variables
	int loop;

	//initialize
	ptMaxY.set(INT_MIN,INT_MIN);
	ptMaxX.set(INT_MIN,INT_MIN);
	ptMinX.set(INT_MAX,INT_MAX);
	ptMinY.set(INT_MAX,INT_MAX);

	//go through the list
	for(loop=0; loop<polyVec.size(); loop++)
	{
		if (polyVec.at(loop).y()>ptMaxY.y())	
			ptMaxY=polyVec.at(loop);
		if (polyVec.at(loop).x()>ptMaxX.x())	
			ptMaxX=polyVec.at(loop);
		if (polyVec.at(loop).y()<ptMinY.y())	
			ptMinY=polyVec.at(loop);
		if (polyVec.at(loop).x()<ptMinX.x())	
			ptMinX=polyVec.at(loop);
	}

}//end of findAllMinMax

//************************************************************************************************

Point Poly::getPoint(int index)
{

	if ( (index < polyVec.size()) && (index>-1)  )
		return(polyVec.at(index));
	else
	{
		Point nullPt;

		nullPt.set(INT_MIN,INT_MIN);
		return(nullPt);
	}

}//end of getPoint

//************************************************************************************************

bool compPolyY(Point val1, Point val2)
{

        if (val1.y() == val2.y() )
                return ( val1.x() > val2.x() );
        else
                return ( val1.y() > val2.y() );

} //end of compPolyY

//************************************************************************************************

bool revCompPolyY(Point val1, Point val2)
{

        if (val1.y() == val2.y() )
                return ( val1.x() < val2.x() );
        else
                return ( val1.y() < val2.y() );

} //end of revCompPolyY


//************************************************************************************************

void Poly::polySort()
{
        //variables
        vector<Point> leftVec;
        vector<Point> rightVec;
        int loop,index;

        //no need to sort the lines
        if (polyVec.size() < 4)
                return;

        //find min & max points
        findAllMinMax();


        //seperate points to left or right
        for(loop=0; loop<polyVec.size(); loop++)
        {
                if (!( ( polyVec.at(loop).x() == ptMaxY.x() ) && ( polyVec.at(loop).y() == ptMaxY.y() ) \
                        || ( polyVec.at(loop).x() == ptMinY.x() ) && ( polyVec.at(loop).y() == ptMinY.y() )))

                if (polyVec.at(loop).pointIsLeft(ptMinY,ptMaxY))
                        leftVec.push_back(polyVec.at(loop));
                else
                        rightVec.push_back(polyVec.at(loop));
        }


	//sort the points in each hemisphere
        sort(rightVec.begin(), rightVec.end(), revCompPolyY);
        sort(leftVec.begin(), leftVec.end(), compPolyY);


        //add the minY point
        polyVec.at(0)=ptMinY;

        //add right side of segment
        index=1;
        for(loop=0; loop<rightVec.size(); loop++,index++)
                polyVec.at(index)=rightVec.at(loop);

        //add left sight of segment
        polyVec.at(index)=ptMaxY;
        index=index+1;
        for(loop=0; loop<leftVec.size(); loop++,index++)
                polyVec.at(index)=leftVec.at(loop);
}

//************************************************************************************************

void Poly::savePolySize()
{
	prevSize=polyVec.size();

}//end of savePolySize

//************************************************************************************************

void Poly::savePolyScore(unsigned int newScore)
{
	score=newScore;

}//end of savePolyScore

//************************************************************************************************

unsigned int Poly::getSavedPolySize()
{
	return(prevSize);

}//end of getPrevPolySize

//************************************************************************************************

unsigned int Poly::getPolyScore()
{
	return(score);

}//end of getPolyScore

//************************************************************************************************

bool Poly::ptInBounds(Point pt)
{
	//if point is whithin a rough boundary of the mins/maxs
	if( pt.x() >= ptMinX.x()  )
		if( pt.x() <= ptMaxX.x()  )
			if( pt.y() >= ptMinY.y()  )
				if( pt.y() <= ptMaxY.y()  )
					return(true);

	return(false);

}//end of ptInBounds

//************************************************************************************************
double Poly::shortestDist2Poly(Point p3)
{
	//variables
	double dist,minDist;
	int loop;
	Point p1;
	Point p2;
	Point pu;
	double u,x,y;

	//initialize
	minDist=(double)(INT_MAX);

	//go through all of the points/lines in the poly
	for(loop=0; loop<polyVec.size(); loop++)
	{
		//wrap around if last point
		if (loop==(polyVec.size()-1))
		{
			p1=polyVec.at(loop);
			p2=polyVec.at(0);
		}
		else //all other points
		{	
			//line segment
			p1=polyVec.at(loop);
			p2=polyVec.at(loop+1);
		}

		// find closest point on the line 
		u= ( ((p3.x()-p1.x())*(p2.x()-p1.x())) + ((p3.y()-p1.y())*(p2.y()-p1.y())) ) \
			/ \
			( ((p2.x()-p1.x())*(p2.x()-p1.x())) + ((p2.y()-p1.y())*(p2.y()-p1.y())) );


		if (u < 0 )
			pu=p1;
		else
			if ( u > 1 )
				pu=p2;
			else
			{

				//closest point on line:
				x=p1.x()+(u*(p2.x()-p1.x()));
				y=p1.y()+(u*(p2.y()-p1.y()));
				pu.set(x,y);
			}

			//get distance to segment
			dist=pu.dist(p3);			

		if (dist<minDist)
			minDist=dist;
	}

	return(minDist);

}//end of shortestDistFromPoly

//************************************************************************************************
int Poly::intersectChance(Point v1p1, Point v1p2, Point v2p1, Point v2p2)
{
	double d1,d2;
	double a,b,c;

	//convert vector to line in linear equation standard:  Ax+By+C=0
	a=v1p2.y() - v1p1.y();
	b=v1p1.x() - v1p2.x();
	c=( v1p2.x() * v1p1.y() ) - ( v1p1.x() * v1p2.y()  );

	//insert vector2, points into Ax+By+C equation
	d1=(a*v2p1.x()) + (b*v2p1.y()) + c;
	d2=(a*v2p2.x()) + (b*v2p2.y()) + c;

	//if d1 & d2 have same sign, they are on same side, thus intersection not possible
	if ( (d1 > 0) && (d2 > 0) )
		return(0);
	if ( (d1 < 0) && (d2 < 0) )
		return(0);

	return(1);

}//end of intersectPass

//************************************************************************************************
int Poly::intersect(Point v1p1, Point v1p2, Point v2p1, Point v2p2)
{
	//if there is no chance of intersection:
	if (!intersectChance(v1p1,v1p2,v2p1,v2p2))	
		return(0);

	//check the same segments in other direction
	if (!intersectChance(v2p1,v2p2,v1p1,v1p2))	
		return(0);
	
	//intersects
		return(1);

}//end of intersect

//************************************************************************************************

int Poly::isPtInPoly(Point pt)
{

	//implementation learned from:
	//http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test

	//variables
	Point extPt;	
	Point extPt2;	
	double epsilon;
	int loop,intersectCount;

	//initialize
	intersectCount=0;
	//find min/max x values of bounding polygon
	findMinX();	
	findMaxX();	
	epsilon=(ptMaxX.x()-ptMinX.x())/((double)(100));

	//extend the point to a ray
	extPt.set((ptMinX.x()-epsilon),pt.y());	

	//go through all of the points/lines in the poly
	for(loop=0; loop<polyVec.size()-1; loop++)
		if (intersect(polyVec.at(loop),polyVec.at(loop+1),pt,extPt))
			intersectCount=intersectCount+1;

	//check the last line segment
	if (intersect(polyVec.at(polyVec.size()-1),polyVec.at(0),pt,extPt))
			intersectCount=intersectCount+1;

	//if even, pt not in poly :
	if (  (intersectCount%2)==0 )  
		return(0);
	else
		return(1);

}//isptInPoly

//************************************************************************************************

