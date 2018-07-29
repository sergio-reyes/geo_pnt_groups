#include <vector>
#include "Poly.h"
#include <algorithm>
#include <pthread.h>
#include <stdio.h>
#include <limits.h>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>



using namespace std;

struct PG_distStruct{
	float dist;
	int index;
};


class PointGroup
{
        public:
                //constructor
                PointGroup();

                //constructor specifying hint of size of list
                PointGroup(int);

		//add coordinates to list of points
		void addPoint(Point);
		void addPoint(double, double);

		//specify largest distance between points in a group
		void setMaxMileDist(double);

		//classify points into groups 
		void formGroups();

		//simplify the set of points using a quickHull
		void quickHull();

		//removes all existing entry and group information
		void reset();

		//show Group answer
		void printGroups();

		//show Poly answer
		void printPolys();

		//allow or remove duplicate points  when forming group
		void allowDups();
		void noDups();

		//calculate the score for a point against all polys
		int calcScore(Point);
		int calcScore(double,double);

		//file I/O
		void printPolysToFile(string);
		void printPolysToFileIfNotEmpty(string);
                void loadPolysFromFile(string);

		//get vector of points representing poly
		void getPolyPoints(vector<Poly> *);

		//get number of polys that were formed
		int getPolyCount();

		//get score for a given poly
		unsigned int getPolyScore(int);

		//get max score of all polys, max valued poly
		unsigned int getMaxPolyScore();

		float distFromClosestPoly(Point);


        private:

		//Point Grouping Functions
		void groupByRadiusX(unsigned int*);
		void groupByRadiusY(unsigned int*);
		void rmPointsInGroup();
		void pointGroupMatch(unsigned int*);
		void checkGroup(int,int,int*,unsigned int*);
		void checkEdgeProximity(int,int,int*,unsigned int*);
		void groupSparesToSpares(unsigned int *);
		void recordGroupHit(int, unsigned int*, int);

		//Merge Poly point Groups
		void cpPointGroups(Poly *,Poly *);

		//Check Point Groups For Possible Mergers
		void groupMergeCheck();

		//check if edge points of poly groups are close to one another
		bool closePolyExtremals(Poly *, Poly *);

		void getRidOfDups();

		//Quick Hull Functions
		static void hullSet(Point,Point,vector<Point>*,Poly*);


		//multi threaded functions
		static void* th_quickHull(void*);

		//1 if point left of line segment; else 0
                static int pointIsLeft(Point, Point, Point);


		int findInVec(vector<PG_distStruct> *, int );


                //pseudo distance of point to line segment
                static double pdist(Point, Point, Point);

			//PRIVATE VARIABLES  

			//vector of points
       	 		vector<Point> ptVec;

			//total number of found groups
			int groupID;	

			//maximum distance in miles that pts can be apart 
			//& still be part of group	
			double MAXMILE;

			//vector of polygons
			vector<Poly> polyVec;

			int PG_removeDups;
			
};  //end of class PointGroup

