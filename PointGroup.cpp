
#include "PointGroup.h"


#define DEFMAXMILE		1
#define OLDMAPCOLOR		"#EED51E"
#define MAPCOLOR		"#F41158"
#define POLYTOKEN		" POLY:"
#define SCORETOKEN		"SCORE:"

using namespace std;


//************************************************************************************************

//constructor:
PointGroup::PointGroup()
{
	MAXMILE=DEFMAXMILE;
	groupID=0;
	PG_removeDups=0;

} //end of constructor

//************************************************************************************************

//constructor:
PointGroup::PointGroup(int sizeHint)
{
	MAXMILE=DEFMAXMILE;
	ptVec.reserve(sizeHint);	
	groupID=0;
	PG_removeDups=0;

} //end of constructor

//************************************************************************************************

void PointGroup::noDups()
{
	PG_removeDups=1;

}//end of noDups

//************************************************************************************************
void PointGroup::reset()
{
	ptVec.erase(ptVec.begin(),ptVec.end());
	polyVec.erase(polyVec.begin(),polyVec.end());
	groupID=0;

}//end of reset

//************************************************************************************************
void PointGroup::allowDups()
{
	PG_removeDups=0;

}//end of allowDups
//************************************************************************************************

void PointGroup::setMaxMileDist(double miles)
{
	MAXMILE=miles;

}//end of setMaxMileDist

//************************************************************************************************

void PointGroup::addPoint(Point pnt )
{

	ptVec.push_back(pnt);

}//end of addPoint

//************************************************************************************************

void PointGroup::addPoint(double x, double y)
{
	//variables
	Point pnt;

	pnt.set(x,y);
	ptVec.push_back(pnt);

}//end of addPoint

//************************************************************************************************

bool compX(Point val1, Point val2) 
{ 	
	return ( val1.x() < val2.x() );

} //end of compX

//************************************************************************************************

bool descX(Point val1, Point val2) 
{ 	
	return (  ( val1.x() > val2.x() )  );

} //end of descX

//************************************************************************************************
bool ascX(Point val1, Point val2) 
{ 	
	return (  ( val1.x() < val2.x() )  );

} //end of descX
//************************************************************************************************

bool compY(Point val1, Point val2) 
{ 	
	return ( val1.y() < val2.y() );

} //end of compY

//************************************************************************************************

bool compG(Point val1, Point val2) 
{ 	
	return ( val1.g() < val2.g() );

} //end of compY

//************************************************************************************************

unsigned int PointGroup::getMaxPolyScore()
{
        //variables
        int loop;
        unsigned int maxScore,pScore;

	maxScore=0;

        for(loop=0; loop<polyVec.size(); loop++)
        {

                pScore=polyVec.at(loop).getPolyScore();
		if (pScore > maxScore)
			maxScore=pScore;

        }

 	return(maxScore);

}//end getMaxPolyScore

//************************************************************************************************

void PointGroup::recordGroupHit(int group, unsigned int *inGroup, int ptIndex )
{

	//set the groupID
	ptVec.at(ptIndex).setg(group);

	//increment the number of points found in group
	*inGroup=*inGroup+1;

	//save off point
	polyVec.at(group).addPoint(ptVec.at(ptIndex));

	//save off min/max points
	polyVec.at(group).checkSetAll(ptVec.at(ptIndex));
	
	
}//end of recordGroupHit

//************************************************************************************************

void PointGroup::groupByRadiusX(unsigned int *inGroup)
{
	// ** assumes vector is sorted by X axis

	// variables
	int outer,inner;		
	double miles;
	int groupFound;
	Poly newPoly;
	int bail;

	// initialize
	outer=0; 
	polyVec.push_back(newPoly);

	//go through each point
	while(outer<(ptVec.size()-1))
	{
		//initialize
		inner=outer+1;
		groupFound=0;
		bail=0;

		//go through section of list that makes sense
		while ((!bail) && (inner<ptVec.size()) )
		{
			//check if within distance
			if(ptVec.at(outer).dist(ptVec.at(inner))<MAXMILE)
			{

				//flag that point was found
				groupFound=1;

				//set the group values 
				if (ptVec.at(outer).g() == -1)
					recordGroupHit(groupID,inGroup,outer);

				if (ptVec.at(inner).g() == -1)
					recordGroupHit(groupID,inGroup,inner);

				//go to next vector entry since it is further out on axis 
				outer=inner;
			}
			else //not in range 
			{
				//check distance of points within MAXMILE of X-axis
				if(ptVec.at(outer).distX(ptVec.at(inner))<MAXMILE)
					//record hint: 
					ptVec.at(inner).setg1(groupID);	
				else
					bail=1;
			}
			//go to next entry 
			inner=inner+1;
		}
		if (groupFound)
		{
			//points are too far on X-axis to be in group 
			groupID=groupID+1;
			polyVec.push_back(newPoly);
		}

		//go to next outer element 
		outer=outer+1;
	}
} //end of groupByRadiusX

//************************************************************************************************

void PointGroup::groupByRadiusY(unsigned int *inGroup)
{

	// ** assumes vector is sorted by Y axis
	// ** assumes vector already passed through groupByRadiusX

	//variables 
	int outer,inner;		
	double miles;
	int groupFound;
	Poly newPoly;
	int bail;

	//initialize 
	outer=0; 

	//go through each point 
	while(outer<(ptVec.size()-1))
	{
		//initialize
		inner=outer+1;
		bail=0;
		groupFound=0;

                while ((!bail) && (inner<ptVec.size()) )
		{

			//if either point needs to be grouped 
			if( (ptVec.at(outer).g() == -1)||(ptVec.at(inner).g() == -1) )
			{
				//if distance is in range: 	
				if(ptVec.at(outer).dist(ptVec.at(inner))<MAXMILE)
				{
					//flag that a new group was found 
					if(  (ptVec.at(outer).g() == -1)&&(ptVec.at(inner).g() == -1) )
					{
						groupFound=1;

						recordGroupHit(groupID,inGroup,outer);
						recordGroupHit(groupID,inGroup,inner);

					}
					else //assign to existing group 
					{
						if( ptVec.at(inner).g() == -1 )	
							recordGroupHit(ptVec.at(outer).g(),inGroup,inner);
						else
							recordGroupHit(ptVec.at(inner).g(),inGroup,outer);
					}

					//go to next vector entry since it is further out on axis 
					outer=inner;
				}
				else //not in range 
				{
					//if still within range for Y axis:
	                                if(ptVec.at(outer).distY(ptVec.at(inner))<MAXMILE)
						//record hint: 
						ptVec.at(inner).setg2(groupID);	
					else
						bail=1;
				}
	
			}
			else //advance x-index since already grouped 
				outer=outer+1;

			//go to next entry 
			inner=inner+1;
		}

		//already traversed every point within range of new group, now increment
		if (groupFound)
		{
			//points are too far on Y-axis to be in group 
			groupID=groupID+1;
			polyVec.push_back(newPoly);
		}


		//go to next outer element 
		outer=outer+1;
	}

	//remove extra point group vector entry
	polyVec.erase(polyVec.end());

} //end of groupByRadiusY

//************************************************************************************************

void PointGroup::groupSparesToSpares(unsigned int *inGroup)
{

        //variables
        int outer,inner;
	Poly newPoly;
	int bail;

        //initialize
        outer=0;
	polyVec.push_back(newPoly);
	

        // go through each pt, looking for those outside a group
        while( (outer<ptVec.size()) )
        {
		//if point not part of group
		if( (ptVec.at(outer).g()<0) )
                {
			//initialize
                	inner=outer+1;
			bail=0;

                	//go through entire array
                	while ( (inner < ptVec.size()) && !bail )
                	{
				//if not comparing with self
                                if(inner != outer)
                                { 
					//if within range:
                        		if ( (ptVec.at(outer).dist(ptVec.at(inner)))<MAXMILE )
					{
						if ( (ptVec.at(inner).g() == -1) && (ptVec.at(outer).g() == -1) )
						{
							recordGroupHit(groupID,inGroup,outer);
							recordGroupHit(groupID,inGroup,inner);

							//advance groupID
							groupID=groupID+1;
							polyVec.push_back(newPoly);
						}

						if (ptVec.at(outer).g() == -1)
							recordGroupHit(ptVec.at(inner).g(),inGroup,outer);

						if (ptVec.at(inner).g() == -1)
							recordGroupHit(ptVec.at(outer).g(),inGroup,inner);


                                        }
					else
					/* if no longer have a chance */
					if ( (ptVec.at(outer).distX(ptVec.at(inner)))>MAXMILE )
						bail=1;
					
                                }

                        	inner=inner+1;
                	}

		}

                //go to next element
                outer=outer+1;
        }

	//remove spare entry
	if( polyVec.at(polyVec.size()-1).points() == 0)
		polyVec.erase(polyVec.end());

}//end of groupSparesToSpares

//************************************************************************************************

void PointGroup::printGroups()
{
	int loop;

	printf("\n\n");		
        for(loop=0; loop<ptVec.size(); loop++)
                printf("\t( %f , %f ) g=%2d   g1=%2d   g2=%2d\n",\
			ptVec[loop].x(),\
			ptVec[loop].y(),\
			ptVec[loop].g(),\
			ptVec[loop].g1(),\
			ptVec[loop].g2());
	printf("\n\n");		

} //end of printGroups

//************************************************************************************************
void PointGroup::rmPointsInGroup()
{
	//variables
	int index;

	if (ptVec.size())
	{
		//sort by Group
		sort(ptVec.begin(), ptVec.end(), compG);

		//find first point in group
		index=0;
		while( (index<ptVec.size()) && (ptVec.at(index).g()<0)  )
			index=index+1;

		//delete the entries
		if(index != ptVec.size())
			ptVec.erase(ptVec.begin()+index,ptVec.end());
	}

}//end of rmPointsInGroup

//************************************************************************************************
void PointGroup::checkGroup(int index, int group, int *found, unsigned int *inGroup)
{
	//variables	
	int start,end;


	start=(polyVec.at(group).getSavedPolySize()-1);
	if (start<0)
		start=0;
	end=(polyVec.at(group).points());

	while( (start<end) && (!*found ) )
	{

		//see if within range for the point 
		if (ptVec.at(index).dist(polyVec.at(group).getPoint(start)) < MAXMILE)
		{
			//save the data
			recordGroupHit(group,inGroup,index);	

			*found=1;
		}

		start=start+1;
	}

}//end of checkGroup

//************************************************************************************************

void PointGroup::checkEdgeProximity(int index, int group, int *found, unsigned int *inGroup)
{


	//check corners and extend graph:

	if (ptVec.at(index).dist(polyVec.at(group).minX()) < MAXMILE)
		*found=1;
	else
		if (ptVec.at(index).dist(polyVec.at(group).minY()) < MAXMILE)
			*found=1;
		else
			if (ptVec.at(index).dist(polyVec.at(group).maxX()) < MAXMILE)
				*found=1;
			else
				if (ptVec.at(index).dist(polyVec.at(group).maxY()) < MAXMILE)
					*found=1;

	if(*found)
		recordGroupHit(group,inGroup,index);

}//end of checkEdgeProximity

//************************************************************************************************

void PointGroup::pointGroupMatch(unsigned int *inGroup)
{
	//variables
	int found, outer, group ;
	
	for(outer=0; outer<ptVec.size(); outer++)
	{
		found=0;

		//check hint1 
		//if size of group has changed
		group=ptVec.at(outer).g1();
		if ( (group != -1) && (group<polyVec.size()) )
			if ( polyVec.at(group).points() != polyVec.at(group).getSavedPolySize())
				//if point is within a rough outline boundary summarizing points
				if( polyVec.at(group).ptInBounds(ptVec.at(outer)))
					checkGroup(outer,group,&found,inGroup);
				else
					checkEdgeProximity(outer,group,&found,inGroup);

		//check hint2 group
		if(!found)
		{
			//if size of group has changed
			group=ptVec.at(outer).g2();
			if ( (group != -1) && (group<polyVec.size()) )
				if ( polyVec.at(group).points() != polyVec.at(group).getSavedPolySize())
					//if point is within a rough outline boundary summarizing points
					if(polyVec.at(group).ptInBounds(ptVec.at(outer)))
						checkGroup(outer,group,&found,inGroup);
					else
						checkEdgeProximity(outer,group,&found,inGroup);
		}	

		//check all groups
		group=0;
		while ( (!found) && (group<polyVec.size()) )
		{
			//skip already checked groups
			if ( (group!= ptVec.at(outer).g1()) && (group!= ptVec.at(outer).g2()) )
			{
				if ( polyVec.at(group).points() != polyVec.at(group).getSavedPolySize())
					//if point is within a rough outline boundary summarizing points
					if(polyVec.at(group).ptInBounds(ptVec.at(outer)))
						checkGroup(outer,group,&found,inGroup);
					else
						checkEdgeProximity(outer,group,&found,inGroup);
			}
			group=group+1;
		}


	}
	
}//end of pointGroupMatch

//************************************************************************************************
bool dupEqual(PG_distStruct val1, PG_distStruct val2) 
{ 	
	float delta;

	
	delta=val1.dist-val2.dist;
	if(delta<0)
		delta=delta*-1;
	
	if( delta <= 0.0001  )
		return(1);
	else
		return(0);

} //end of dupEqual

//************************************************************************************************

bool compDist(PG_distStruct val1, PG_distStruct val2) 
{ 	
	return ( val1.dist < val2.dist );

} //end of compDist

//************************************************************************************************
int PointGroup::findInVec(vector<PG_distStruct> *distVec, int ptIndex)
{
	//variables
	int loop;

	//go through array, see if index is found
	for(loop=0; loop<distVec->size(); loop++)
		if(distVec->at(loop).index == ptIndex)
			return(1);
	return(0);
}
//************************************************************************************************

void PointGroup::getRidOfDups()
{
	//variables
	vector<PG_distStruct> distVec;	
	Point basePoint;
	PG_distStruct oneEntry;
	int loop;
	vector<PG_distStruct>::iterator it;
	int found,leave;
	int ascend;


	if(ptVec.size())
	{

		//initialize	
		distVec.reserve(ptVec.size());
		basePoint.set(ptVec.at(0).x(),ptVec.at(0).y());
		ascend=0;
		if( ptVec.at(0).x() <0 )
			ascend=1;

		//get distance of all points from base point
		for(loop=0; loop<ptVec.size();loop++)
		{
			oneEntry.index=loop;
			oneEntry.dist=ptVec.at(0).dist(ptVec.at(loop));
			distVec.push_back(oneEntry);
		}

		//sort points based on distance
		sort(distVec.begin(), distVec.end(), compDist);

		//sort based on distance:
		it=unique(distVec.begin(),distVec.end(),dupEqual);
		distVec.resize(distance(distVec.begin(),it));

		//invalidate duplicate points:
		for(loop=0; loop<ptVec.size(); loop++)
		{
			found=findInVec(&distVec,loop);
			if(!found)
				ptVec.at(loop).set(0,0);
		}

		//sort points based on X
		if(ascend)
			sort(ptVec.begin(), ptVec.end(), ascX);
		else
			sort(ptVec.begin(), ptVec.end(), descX);

		loop=0; leave=0;
		//go through all points
		while( (loop<ptVec.size()) && (!leave)  )
		{
			//if start of invalid coords:
			if( (ptVec.at(loop).x()==0) && (ptVec.at(loop).y()==0) )
			{
				ptVec.erase(ptVec.begin()+loop,ptVec.end());
				leave=1;
			}
			loop=loop+1;
		}

	}

}//end of getRidOfDups

//************************************************************************************************
void PointGroup::formGroups()
{
	//variables
	unsigned int inGroup, goal, baseline;
	int groupsChanged,loop;

		//if user did now want dups:
		if(PG_removeDups)
			getRidOfDups();

	if(ptVec.size())
	{

		/* initialize */
		inGroup=0;
		goal=ptVec.size();

		// ... PASS1 X-AXIS

		//sort the vector by x-axis 
		sort(ptVec.begin(), ptVec.end(), compX);


		//group by radius using X-axis
		groupByRadiusX(&inGroup);

		//.... ...



		// ... PASS2 Y-AXIS

		//if not all pts in group: 
		if (inGroup != goal)
		{
			//sort the vector by y-axis 
			sort(ptVec.begin(), ptVec.end(), compY);

			//group by radius using Y-axis 
			groupByRadiusY(&inGroup);
		}
		//.... ...



		//try to fit points to group
		if (inGroup != goal)
		{
			groupsChanged=0;
			do{

				//remove excess points from point list	
				rmPointsInGroup();
				baseline=inGroup;


				//place points In Group
				pointGroupMatch(&inGroup);

				if (baseline == inGroup)
					groupsChanged=0;
				else
				{
					//save the current size of groups
					for(loop=0; loop<polyVec.size(); loop++)
						polyVec.at(loop).savePolySize();		


					groupsChanged=1;
				}


			}while(groupsChanged);
		}


			//try to form new groups 
			if (inGroup != goal)
			{
			//sort the vector by x-axis 
			sort(ptVec.begin(), ptVec.end(), compX);

			groupSparesToSpares(&inGroup);
		}



		//potentially merge the point groups
		groupMergeCheck();



		//delete groups of < 2 points
		loop=0;
		while(loop<polyVec.size())
		{
			//if not enough pts for a poly
			if(polyVec.at(loop).points() < 3)
			{
				polyVec.erase(polyVec.begin()+loop);
				//go backwards because entry was deleted
				loop=loop-1;
			}
			loop=loop+1;
		}
	}


} //end of formGroups

//************************************************************************************************

int PointGroup::pointIsLeft(Point A, Point B, Point P)
{
        //variables
        double loc;

        //cross product to determine point is left or right of line defined by A->B
        loc=( ( B.x()-A.x() )*( P.y()-A.y() ) - ( B.y()-A.y() )*( P.x()-A.x() ));

        if (loc>0)
                return(1);
        else
                return(0);

}//end of pointIsLeft

//************************************************************************************************

double PointGroup::pdist(Point A, Point B, Point C)
{
        //variables
        double ABx;
        double ABy;
        double dist;

        //deltas of line A->B
        ABx=B.x()-A.x();
        ABy=B.y()-A.y();

        //pseudo distance
        dist=ABx*( A.y()-C.y() )-ABy*( A.x()-C.x() );

        //return positive distance value
        if (dist<0)
                return(dist*(-1));
        else
                return(dist);

}//end of pdist


//************************************************************************************************

void PointGroup::hullSet(Point A, Point B, vector<Point> *set, Poly *hull)
{
	//variables
	int loop;
	double maxDist,curDist;
	int farPtIndex;
	vector<Point> leftSetAP;
	vector<Point> leftSetPB;
	Point P;

	//initialize
	maxDist=INT_MIN;
	leftSetAP.reserve(set->size());
	leftSetPB.reserve(set->size());



	// ... ZERO POINTS in set

	//if no points in the set:
	if (set->size() == 0)
		//done
		return;

 	//.... ...



	// ... 1 POINTS in set

	//1 point in set, then add to hull
	if (set->size() == 1)
	{
		//add point to hull
		hull->addPoint(set->at(0));

		//remove point from set
		set->erase(set->begin());

		//done
		return;
	}

	//.... ...



	// ... MULTIPLE POINTS in set

	//find furthest point	
	for(loop=0; loop<set->size(); loop++)	
	{
		curDist= ( pdist(A,B,set->at(loop)) );
		if (curDist > maxDist)
		{
			maxDist=curDist;
			farPtIndex=loop;
		}
		
	}

	//add furthest point to hull
	P=set->at(farPtIndex);
	hull->addPoint(P);

	//remove point from set
	set->erase(set->begin()+farPtIndex);

	//determine who's left of A-->P
	for(loop=0; loop<set->size(); loop++)		
	{
		if(  pointIsLeft(A,P,set->at(loop))  )
			leftSetAP.push_back(set->at(loop));
	}

	//determine who's left of P-->B
	for(loop=0; loop<set->size(); loop++)		
	{
		if(  pointIsLeft(P,B,set->at(loop))  )
			leftSetPB.push_back(set->at(loop));
	}

	hullSet(A,P,&leftSetAP,hull);
	hullSet(P,B,&leftSetPB,hull);

	//.... ...


}//end of hullSet

//************************************************************************************************

void* PointGroup::th_quickHull(void* polyIn)
{
	//variables 
	Poly *polyPoints;
	vector<Point> leftSet;
	vector<Point> rightSet;
	int loop;
	Point pnt;

	Poly *convexHull=new Poly();

	//make it easier to access data passed in
	polyPoints=((Poly *)(polyIn));

	//initialize
	leftSet.reserve(polyPoints->points());
	rightSet.reserve(polyPoints->points());

	//add the min and max x-coordinates to convex hull
	convexHull->addPoint(polyPoints->minX());
	convexHull->addPoint(polyPoints->maxX());

	//remove the extreme x-coord points added to convex hull	
	polyPoints->rmPoint(polyPoints->minX());
	polyPoints->rmPoint(polyPoints->maxX());

	//add point to either left or right set
	for(loop=0; loop<polyPoints->points(); loop++)
	{
		//get the point to place in either left or right set
		pnt=polyPoints->getPoint(loop);

		//if point is left of line segment
		if(  pointIsLeft(polyPoints->minX(),polyPoints->maxX(),pnt)  )
			leftSet.push_back(pnt);
		else
			rightSet.push_back(pnt);
	}

	hullSet(polyPoints->maxX(),polyPoints->minX(),&rightSet,convexHull);
	hullSet(polyPoints->minX(),polyPoints->maxX(),&leftSet,convexHull);

	//go around poly in circle to list line segments in proper order
	convexHull->polySort();

	//exit and return poly
	pthread_exit((void*)convexHull);

}//end of th_quickHull

//************************************************************************************************

void PointGroup::quickHull()
{
	//variable declaration
	int loop;
	vector<pthread_t> tidVec;
	Poly *myPoly;	
	pthread_t tid;
	unsigned int pSize;
	

	//initialize
	//polys will be <= number of groups
	tidVec.reserve(groupID);
	polyVec.reserve(groupID);


	//attack quick Hull with multiple threads
	for(loop=0; loop<polyVec.size(); loop++)
	{
		//store the number of points prior to quickHull
		polyVec.at(loop).savePolySize();
		
		pthread_create(&tid,NULL,&PointGroup::th_quickHull,(void *)&polyVec.at(loop));
		tidVec.push_back(tid);
	}


	//wait for all of the threads to terminate	
	for(loop=0; loop<polyVec.size(); loop++)
	{
		//wait for thread
		pthread_join(tidVec.at(loop),(void **)&myPoly);

		//get size of all points in poly
		pSize=polyVec.at(loop).getSavedPolySize();

		polyVec.at(loop)=*myPoly;

		//set the score of poly
		polyVec.at(loop).savePolyScore(pSize);
	}
	
}//end of quickHull
//************************************************************************************************

int PointGroup::calcScore(Point pt)
{

	/*  Score can be negative or positive and is calculated against all polys  */
	/*  per poly, if point is: <10 miles, then 1%-100% of polyscore is added to running total */
	/*  if > 10mile dist, ,then score = negative.  negative multiplier of maxPolyScore - closest polyScore */


	//variable declaration
	int loop;
	int myScore;
	double dist,pct;
	unsigned int maxPScore;
	

	//initialize
	myScore=0;
	maxPScore=getMaxPolyScore();

	//calc points per poly using threads
	for(loop=0; loop<polyVec.size(); loop++)
	{

		//if point is within poly: give score
		if (polyVec.at(loop).isPtInPoly(pt))
			myScore=myScore+polyVec.at(loop).getPolyScore();
		else
		{

			//in miles, distance to poly
			dist=(polyVec.at(loop).shortestDist2Poly(pt));

			//zero value if 10miles away.  negative value if further than 10
			pct=dist*10;
			pct=100-pct;

			if (pct < 0)
				myScore=myScore+((pct/((double)100))* (maxPScore-polyVec.at(loop).getPolyScore()+1) );
			else
				myScore=myScore+((pct/((double)100))*polyVec.at(loop).getPolyScore());
		}

	}

	return(myScore);

}//end of calcScore
//************************************************************************************************
int PointGroup::calcScore(double x, double y)
{
	Point pnt;

	pnt.set(x,y);	
	return calcScore(pnt);

}//end of calcScore


//************************************************************************************************

void PointGroup::printPolys()
{
	string fname;
	fname="stdout";

	//specify that output goes to screen
	printPolysToFile(fname);
}
//************************************************************************************************

void PointGroup::getPolyPoints(vector<Poly> *pVec)
{
	//variables
	int loop;

	for(loop=0; loop<polyVec.size(); loop++)
		pVec->push_back(polyVec.at(loop));
}

//************************************************************************************************

void PointGroup::cpPointGroups(Poly *dest, Poly *src)
{

	//variables
	int loop;

	for(loop=0; loop<src->points(); loop++)
		dest->addPoint(src->getPoint(loop));

	//save the poly size
	dest->savePolySize();
	dest->findAllMinMax();

}//end of mergePointGroups

//************************************************************************************************
bool PointGroup::closePolyExtremals(Poly *poly1, Poly *poly2 )
{
	//variables
	vector<Point> tempVec;
	int loop;

	tempVec.push_back(poly1->minX());
	tempVec.push_back(poly1->minY());
	tempVec.push_back(poly1->maxX());
	tempVec.push_back(poly1->maxY());

	for(loop=0; loop<tempVec.size(); loop++)
	{
		if ( tempVec.at(loop).dist(poly2->minX()) < MAXMILE  )
			return(true);
		if ( tempVec.at(loop).dist(poly2->maxX()) < MAXMILE  )
			return(true);
		if ( tempVec.at(loop).dist(poly2->minY()) < MAXMILE  )
			return(true);
		if ( tempVec.at(loop).dist(poly2->maxY()) < MAXMILE  )
			return(true);
	}

	return(false);
}

//************************************************************************************************

void PointGroup::groupMergeCheck()
{
	//variables
	int outer,inner;

	if(polyVec.size() >0)
	{
		//go through n-1 polys
		for(outer=0; outer<polyVec.size()-1; outer++)
		{
			//traverse every forward poly
			for(inner=outer+1;inner<polyVec.size();inner++)
			{
				if( closePolyExtremals( &polyVec.at(outer),&polyVec.at(inner) ))	
				{
					cpPointGroups(&polyVec.at(outer),&polyVec.at(inner));
					//delete src poly
					polyVec.erase(polyVec.begin()+inner);
				}
			}
		}
	}

}//end groupMergeCheck

//************************************************************************************************

void PointGroup::printPolysToFile(string fname)
{
	//variables
        FILE *fp;
	int loop,loop2;
	Point aPoint;
	unsigned int pScore;

	if (fname == "stdout")
		fp=stdout;
	else
		//open file
        	if((fp = fopen(fname.c_str(), "w")) <= 0)
               		exit(1);

	for(loop=0; loop<polyVec.size(); loop++)
	{

		pScore=polyVec.at(loop).getPolyScore();

		fprintf(fp,"%s %d \n",POLYTOKEN,loop+1);
		fprintf(fp,"%s %d\n",SCORETOKEN,pScore);

		for(loop2=0; loop2<polyVec.at(loop).points(); loop2++)
		{
			aPoint=polyVec.at(loop).getPoint(loop2);
			fprintf(fp,"\t %f , %f \n",aPoint.x(),aPoint.y());
		}
	}

	fflush(fp);
       	fclose(fp);

}//end savePolySToFile

//************************************************************************************************

void PointGroup::printPolysToFileIfNotEmpty(string fname)
{
	if(polyVec.size())
		printPolysToFile(fname);
}

//************************************************************************************************
int PointGroup::getPolyCount()
{
	return(polyVec.size());
}

//************************************************************************************************
unsigned int PointGroup::getPolyScore(int index)
{

	if ( (index>(-1)) && (index<polyVec.size()) )
		return(polyVec.at(index).getPolyScore());
	else
		return(-1);

}

//************************************************************************************************

void PointGroup::loadPolysFromFile(string fname)
{

        //variables
        Point aPoint;
        Poly newPoly;
        ifstream infile;
        string line;
        char *token, *rest, *ptr;
        double x,y;
        char *sCopy;
	int index,loop;


	//initialize
	index=-1;

        //open file
        infile.open(fname.c_str());

        //go through poly file
        while (!infile.eof())
        {
                getline(infile,line);
                sCopy=new char[line.length()+1];
		strcpy(sCopy,line.c_str());


		//if this is a new poly line:
		if (strstr(sCopy,POLYTOKEN))
		{
			index=index+1;
		
			//add a poly:
			polyVec.push_back(newPoly);
		}

		//get score value if this is a score line
		if (strstr(sCopy,SCORETOKEN))
		{
			//remove tag
			token=strtok_r(sCopy,":",&rest);
			ptr=rest;	

			//extract score
			token=strtok_r(ptr,":",&rest);
			polyVec.at(index).savePolyScore(atoi(token));

		}


		//get the coordinates	
		if (strstr(sCopy,","))
		{
			//get x value
			token=strtok_r(sCopy,",",&rest);
			ptr=rest;	
			x=strtod(token,NULL);

			//get y value 
			token=strtok_r(ptr,",",&rest);
			y=strtod(token,NULL);

			//save the point
			aPoint.set(x,y);
			//add point to poly:
			polyVec.at(index).addPoint(aPoint);

		}
        }

        infile.close();


	//set the min/maxs for the polys
	for (loop=0; loop<polyVec.size(); loop++)
		polyVec.at(loop).findAllMinMax();


}//end of loadPolysFromFile

//************************************************************************************************
float PointGroup::distFromClosestPoly(Point pnt)
{
        //variables
        int loop;
	float minMiles;


	minMiles=INT_MAX;

        for(loop=0; loop<polyVec.size(); loop++)
        {
		if (polyVec.at(loop).shortestDist2Poly(pnt) < minMiles)
			minMiles=polyVec.at(loop).shortestDist2Poly(pnt);
	}

	return(minMiles);


}
//************************************************************************************************
