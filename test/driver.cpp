#include <stdlib.h>
#include <string.h>
#include "../PointGroup.h"
#include <stdio.h>
#include <unistd.h>

#define BUFFMAX		128


using namespace std;

//global
char msg[BUFFMAX];
char myname[BUFFMAX];

//----------------------------------------------------------------------------
void getData(char filename[],PointGroup *pg)
{
        FILE *fp;
        char line[BUFFMAX];
	double x,y;
	char *ptr;
	char *rest;
	char *token;

        if((fp = fopen(filename, "r")) <= 0)
        {   sprintf(msg,"%s: PID=%d, Cannot open (%s)",myname,getpid(),filename);
                                perror(msg);
                                exit(1);}


        while (fgets(line,BUFFMAX,fp))
        {
		//get x coordinate
        	token = strtok_r(line, ",", &rest);
		ptr=rest;
		x=strtod(token,NULL);

		//get y coordinate
        	token = strtok_r(ptr, "\n", &rest);
		y=strtod(token,NULL);

		pg->addPoint(x,y);
        }

        fclose(fp);
	
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//                                  MAIN                                       
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	//variables
	PointGroup pg;

	if(argc!=2)
	{	
		printf("Usage: %s <input_filename>\n",argv[0]);
		exit(0);
	}


	//get the coordinates from file
	getData(argv[1],&pg);

	pg.noDups();
	pg.setMaxMileDist(0.50);
	pg.formGroups();

	
	//form polygons 
	pg.quickHull();


	printf("\n");
	printf("Saved Polygons to File: %s\n","polys.txt");
	pg.printPolysToFile("polys.txt");


	printf("\n");
	printf("Points & Scores\n");
	printf("=================\n");
	printf("(%f , %f)  score=%d\n",30.464687256199685, -97.68490558750138,pg.calcScore(30.464687256199685, -97.68490558750138));
	printf("(%f , %f)  score=%d\n",30.229853411017828, -97.71512242848735,pg.calcScore(30.229853411017828, -97.71512242848735));
	printf("(%f , %f)  score=%d\n",30.374245450148024, -97.77575748528018,pg.calcScore(30.374245450148024, -97.77575748528018));
	printf("(%f , %f)  score=%d\n",30.374245450148024, -97.77575748528018,pg.calcScore(30.374245450148024, -97.77575748528018));
	printf("(%f , %f)  score=%d\n",30.216185169473185, -97.82241655569393,pg.calcScore(30.216185169473185, -97.82241655569393));

	printf("\n");
	exit(0);
}

