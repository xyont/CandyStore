/***************************************************************************
 *   Copyright (C) 2008 by Patrick Prokopczuk   *
 *   prokopczuk@gmx.de   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "femodel.h"
#include <math.h>
#include "node.h"
#include "pvdhandler.h"
#include "gauss_solver.h"
//#include "result.h"
namespace nglib {
#include "./Netgen/libsrc/interface/nglib.h"
#include "octree.h"
}
//***********************************************************************************************
/*! Standard constructor, creates and intialises an empty database  */ 
    femodel::femodel()	
    {
                fileflag=-1;
                currentSet="all";
                currentNsetIndex=-1;
                clearDatabase();
		resultData.clearResultData();
		dispAvailable=false;
		dispRatio=0;
		openFile="";
		currentMax=0;
		currentMin=0;

        nodeOctree= new octree(this);
        timeAnimation= new timeAnimationClass();
    }
//***********************************************************************************************
/*! Clears the database*/ 	
void femodel::clearDatabase()
	{
        bcList.clear();
        mpcList.clear();
        setList.clear();
        nodeList.clear();
		elementList.clear();
		faceList.clear();
		edgeList.clear();
		interpolatorList.clear();
		setMaxCoords(0,0,0);
		resultList.clear();
		resultDataList.clear();
                resultData.clearResultData();
                ResultTitles.clear();
                ResultComponents.clear();
                tempResultIndex.resultStep=-1;
                tempResultIndex.frqtime=-1;
                dispAvailable=false;
                dispRatio=0;
                openFile="";
                currentMax=0;
                currentMin=0;
        borderEdges.clear();
	}
//***********************************************************************************************
/*! Sets the maximum model coordinates*/ 	
void femodel::setMaxCoords(double x, double y, double z)
	{
	
		maxCoords[0]=x;
		maxCoords[1]=y;
		maxCoords[2]=z;
	}
//***********************************************************************************************
/*! Returns the maximum model coordinates*/ 
vector3d femodel::getMaxModelCoords()
	{
		vector3d coord;
		
		maxCoords[0]=1;
		maxCoords[1]=1;
		maxCoords[2]=1;
		
		for (long int i=0; i< nodeList.size(); ++i)
		{
			coord=nodeList[i].getCoordinates();
			
			if ((coord[0] > maxCoords[0]) || (coord[0]*-1 > maxCoords[0]))
			{
				maxCoords[0]=coord[0];
			}
			if ((coord[1] > maxCoords[1]) || (coord[1]*-1 > maxCoords[1]))
			{
				maxCoords[1]=coord[1];
			}
		    if ((coord[2] > maxCoords[2]) || (coord[2]*-1 > maxCoords[2]))
			{
				maxCoords[2]=coord[2];
			}
	  
		}
		
		return(maxCoords);
	}
//***********************************************************************************************
/*! Returns the node index in the nodelist ot the node with the given label*/ 
/*! Returns the negative value of the possible insert location if the node does not exist*/
long femodel::getNodeIndex(long label)
{
unsigned long links=1;
unsigned long rechts=nodeList.size();
unsigned long mitte;
	while(1)
	{
        if (links>rechts) return (-1*rechts);
	mitte=(links+rechts)/2;
	if (nodeList[mitte-1].getLabel()==label) return(mitte-1);
	
    /*if (label<nodeList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
    */
    label<nodeList[mitte-1].getLabel() ? (rechts=mitte-1):links=mitte+1;
    }
}
//*************************************************************
/*! Returns the face index in the facelist of the searchface or the possible insert location*/
/*! the exists flag shows, wether the face is present in the list*/
long femodel::getFaceIndex(face searchface, int* exists)
{
unsigned long links=1;
unsigned long rechts=faceList.size();
unsigned long mitte;

        while(1)
        {
        if (links>rechts)
            {
            *exists=0;
            return(rechts);
            }
        mitte=(links+rechts)/2;
        if (faceList[mitte-1]==searchface)
        {
            *exists=1;
            return(mitte-1);
        }

        if (searchface<faceList[mitte-1])
                rechts=mitte-1;
        else
                links=mitte+1;
        }
}
//*************************************************************
/*! Returns the edge index in the edgelist of the searchedge or the possible insert location*/
/*! the exists flag shows, wether the edge is present in the list*/
long femodel::getEdgeIndex(edge searchedge, int* exists)
{
unsigned long links=1;
unsigned long rechts=edgeList.size();
unsigned long mitte;

        while(1)
        {
        if (links>rechts)
            {
            *exists=0;
            return(rechts);
            }
        mitte=(links+rechts)/2;
        if (edgeList[mitte-1]==searchedge)
        {
            *exists=1;
            return(mitte-1);
        }

        if (searchedge<edgeList[mitte-1])
                rechts=mitte-1;
        else
                links=mitte+1;
        }
}
//***********************************************************************************************
/*! Returns the element index in the elementlist of the element with the given label*/ 
long femodel::getElementIndex(long label)
{
unsigned long links=1;
unsigned long rechts=elementList.size();
unsigned long mitte;
	while(1)
	{
	if (links>rechts) return (-1);
	mitte=(links+rechts)/2;
	if (elementList[mitte-1].getLabel()==label) return(mitte-1);
	
	if (label<elementList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//***********************************************************************************************
/*! Returns the coordinates (vector3d) of the node with the given  label*/
vector3d femodel::getNodeCoords(long label)
	{
	vector3d errorCoords;
		errorCoords[0]=-99;
		errorCoords[1]=-99;
		errorCoords[2]=-99;
	
	
	
unsigned long links=1;
unsigned long rechts=nodeList.size();
unsigned long mitte;
	while(1)
	{
	if (links>rechts) return (errorCoords);
	mitte=(links+rechts)/2;
	if (nodeList[mitte-1].getLabel()==label) return(nodeList[mitte-1].getCoordinates());
	
	if (label<nodeList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//***********************************************************************************************
/*! Returns the displacements of a the node with nodelabel label */
vector3d femodel::getDisplacements(long label)
{
	vector3d errorCoords;
		errorCoords[0]=-99;
		errorCoords[1]=-99;
		errorCoords[2]=-99;
	
	
	
unsigned long links=1;
unsigned long rechts=nodeList.size();
unsigned long mitte;
	while(1)
	{
	if (links>rechts) return (errorCoords);
	mitte=(links+rechts)/2;
	if (nodeList[mitte-1].getLabel()==label) return(nodeList[mitte-1].getDisplacements());
	
	if (label<nodeList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//**************************************************************************************
/*! Returns the value of a the node with nodelabel label */
double femodel::getNodeResults(long label)
	{
		
unsigned long links=1;
unsigned long rechts=nodeList.size();
unsigned long mitte;
	while(1)
	{
	if (links>rechts) return (0);
	mitte=(links+rechts)/2;
	if (nodeList[mitte-1].getLabel()==label) return(nodeList[mitte-1].getResultValue());
	
	if (label<nodeList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//**************************************************************************************
/*! Returns the index of a the node with nodelabel label in the resultlist */
long femodel::getNodeResultListIndex(long label)
    {

unsigned long links=1;
unsigned long rechts=nodeList.size();
unsigned long mitte;
    while(1)
    {
    if (links>rechts) return (-1);
    mitte=(links+rechts)/2;
    if (nodeList[mitte-1].getLabel()==label) return(mitte-1);

    if (label<nodeList[mitte-1].getLabel())
        rechts=mitte-1;
    else
        links=mitte+1;
    }
}
	
//*************************************************************************************************
void femodel::setModelCenter()
//sets the modelcenter and model radius
{
		vector3d tempVec;
		vector3d tempVec1;
		vector3d tempVec2;
		vector3d corner1;
		vector3d corner2;
                double minX,maxX;
                double minY,maxY;
                double minZ,maxZ;

		long size=nodeList.size();
		modelRadius=1;
		
	switch (size)
                {
                    case 0:
				modelCenter[0]=0;
				modelCenter[1]=0;
				modelCenter[2]=0;
				//modelRadius=1;
				//no nodes
                    break;
                    case 1:
				//one node
                    modelCenter=nodeList[0].getCoordinates();
				//modelRadius=1;
                    break;
                    /*case 2:
				// two nodes
				modelCenter=nodeList[1].getCoordinates()-((nodeList[1].getCoordinates()-nodeList[0].getCoordinates())/2);
				tempVec=(corner1-modelCenter);
				//modelRadius=tempVec.norm();
                    break;*/
                    default:
				//more than two nodes
                                long minindex=0;
                                long maxindex=0;
                                minX=nodeList[0].getCoordinate1();
                                maxX=nodeList[0].getCoordinate1();
                                minY=nodeList[0].getCoordinate2();
                                maxY=nodeList[0].getCoordinate2();
                                minZ=nodeList[0].getCoordinate3();
                                maxZ=nodeList[0].getCoordinate3();
                                double min=nodeList[0].getCoordinates().norm();
                                double max=nodeList[0].getCoordinates().norm();
                                for (long l=1; l<nodeList.size();l++)
                                {
                                    if (nodeList[l].getCoordinate1() <minX)
                                    {
                                        minX=nodeList[l].getCoordinate1();
                                    }
                                    if (nodeList[l].getCoordinate1() >maxX)
                                    {
                                        maxX=nodeList[l].getCoordinate1();
                                    }
                                    if (nodeList[l].getCoordinate2() <minY)
                                    {
                                        minY=nodeList[l].getCoordinate2();
                                    }
                                    if (nodeList[l].getCoordinate2() >maxY)
                                    {
                                        maxY=nodeList[l].getCoordinate2();
                                    }
                                    if (nodeList[l].getCoordinate3() <minZ)
                                    {
                                        minZ=nodeList[l].getCoordinate3();
                                    }
                                    if (nodeList[l].getCoordinate3() >maxZ)
                                    {
                                        maxZ=nodeList[l].getCoordinate3();
                                    }
                                }

                                modelCenter.x=(maxX+minX)/2;
                                modelCenter.y=(maxY+minY)/2;
                                modelCenter.z=(maxZ+minZ)/2;

                                vector3d radius;
                                radius.x=maxX-minX;
                                radius.y=maxY-minY;
                                radius.z=maxZ=minZ;
                                modelRadius=radius.norm();

                        break;
			}
		
	}
//*******************************************************
//TODO: Check wether the following is faster:
//      -Put all faces in a list
//      -sort the list with qsort
//      -check the faces next to each other
//      -set the outside flag when no duplicate exists
//**************************************
void femodel::getFaces()
//gets Faces from elements
{
        faceList.clear();
	
	for (long i=0; i<elementList.size();i++)
	{
		switch (elementList[i].type)
		{
		case 1:
			getHex8Faces(i);
		break;
		case 2:
			getPenta6Faces(i);
		break;
		case 3:
			getTet4Faces(i);
		break;
		case 4:
			getHex20Faces(i);
		break;
		case 5:
			getPenta15Faces(i);
		break;
		case 6:
			getTet10Faces(i);
		break;
        case 7:
            getTria3Faces(i);
        break;
        case 8:
            getTria6Faces(i);
        break;
        case 10:
            getQuad8Faces(i);
        break;
		default:
			//unknown element type
		;
		}
	}
    //******************
    //Testing
    //qsort(faceList.begin(),faceList.end());
if (faceList.size()>0)
{
    qSort(faceList);
    //sortFaceList(0,faceList.size()-1);
    //******************
    emit sendMessage("Calculating face centroids");
    calcFaceCentroids();
    emit sendMessage("Calculating face normals");
    calcFaceNormals();
    //**************************************************
    bool setVorhanden=false;
    int setIndex=-1;
    
    //Check wether the outside set already exists
    for (int g=0;g<setList.size();g++)
    {
        if (setList[g].name=="outside")
        {
            setVorhanden=true;
            setIndex=g;
        }
    }
    if (setVorhanden==false)
    {
        tempSet.name="outside";
        tempSet.faceIndexList.clear();
        setList.prepend(tempSet);
        setList[0].qCommandFlag=false;
        setIndex=0;
    }
    else
    {
        setList[setIndex].faceIndexList.clear();
    }


           for (long f=0; f<faceList.size()-1;f++)
           {
                   if ((faceList[f]==faceList[f+1]) && (faceList[f].parentElementDimension == faceList[f+1].parentElementDimension))
                   {
                       faceList[f].outside=false;
                       faceList[f+1].outside=false;
                       f++;
                   }
                   else
                   {
                       setList[setIndex].faceIndexList.append(f);
                   }
           }
           //if the second last face is outside the last has to be outside too
           if (faceList.size()>1)
           {
           if (faceList[faceList.size()-2].outside==true)
           {

               setList[setIndex].faceIndexList.append(faceList.size()-1);
           }
            }
           /*else
           {               
               setList[setIndex].faceIndexList.append(0);
            }*/
       }
}
//***************************************************************
void femodel::getHex8Faces(long elementListNumber)
{
    long i=elementListNumber;
		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=1;
		tempFace.type=4;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[3]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[1]);
		//edge4
		tempFace.sortNodes();
        faceList.append(tempFace);
		//**********************************
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=2;
		//face2
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[4]);
		tempFace.nodeList[1]=(elementList[i].nodeList[5]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[6]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[7]);
		//edge4
		tempFace.sortNodes();
        faceList.append(tempFace);
		//**************************************
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[4]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[7]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[3]);
		//edge4
		tempFace.sortNodes();
        faceList.append(tempFace);
		//**************************************
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=4;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[3]);
		tempFace.nodeList[1]=(elementList[i].nodeList[7]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[6]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[2]);
		//edge4
		tempFace.sortNodes();
        faceList.append(tempFace);
		//**************************************
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=5;
		//face5
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[2]);
		tempFace.nodeList[1]=(elementList[i].nodeList[6]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[1]);
		//edge4
		tempFace.sortNodes();       
        faceList.append(tempFace);
		//**************************************
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=6;
		//face6
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[1]);
		tempFace.nodeList[1]=(elementList[i].nodeList[5]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[4]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[0]);
		//edge4
		tempFace.sortNodes();
        faceList.append(tempFace);
}
//*************************************************************
void femodel::getQuad8Faces(long elementListNumber)
//gets Faces from quad8elements
{

    long i=elementListNumber;

                tempFace.outside=true;
                tempFace.parentElementDimension=2;
                //
                tempFace.parentElement=elementList[i].label;
                tempFace.faceNumber=1;
                tempFace.type=8;
                tempFace.centroidValue=0;
                tempFace.centroidColor=0;
                //face1
                //edge1
                tempFace.nodeList[0]=(elementList[i].nodeList[0]);
                tempFace.nodeList[1]=(elementList[i].nodeList[4]);
                tempFace.nodeList[2]=(elementList[i].nodeList[1]);
                //edge2
                tempFace.nodeList[3]=(elementList[i].nodeList[5]);
                tempFace.nodeList[4]=(elementList[i].nodeList[2]);
                //edge3
                tempFace.nodeList[5]=(elementList[i].nodeList[6]);
                tempFace.nodeList[6]=(elementList[i].nodeList[3]);
                //edge4
                tempFace.nodeList[7]=(elementList[i].nodeList[7]);
                tempFace.sortNodes();
                faceList.append(tempFace);
}
//***************************************************************************
void femodel::getHex20Faces(long elementListNumber)
//gets Faces from hex20elements
{
	
    long i=elementListNumber;

		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=1;
		tempFace.type=8;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[11]);
		tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[10]);
		tempFace.nodeList[4]=(elementList[i].nodeList[2]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[9]);
		tempFace.nodeList[6]=(elementList[i].nodeList[1]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[8]);
		tempFace.sortNodes();
      /*          index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                faceList.append(tempFace);
		
		//**********************************
		//tempFace.nodeList.clear();
		//
                tempFace.outside=true;
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=2;
		//face2
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[4]);
		tempFace.nodeList[1]=(elementList[i].nodeList[16]);
		tempFace.nodeList[2]=(elementList[i].nodeList[5]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[17]);
		tempFace.nodeList[4]=(elementList[i].nodeList[6]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[18]);
		tempFace.nodeList[6]=(elementList[i].nodeList[7]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[19]);
                tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                faceList.append(tempFace);

                //**********************************
                //tempFace.nodeList.clear();
                //
                tempFace.outside=true;
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[12]);
		tempFace.nodeList[2]=(elementList[i].nodeList[4]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[19]);
		tempFace.nodeList[4]=(elementList[i].nodeList[7]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[15]);
		tempFace.nodeList[6]=(elementList[i].nodeList[3]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[11]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                faceList.append(tempFace);

                //**********************************
                //tempFace.nodeList.clear();
                //
                tempFace.outside=true;
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=4;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[3]);
		tempFace.nodeList[1]=(elementList[i].nodeList[15]);
		tempFace.nodeList[2]=(elementList[i].nodeList[7]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[18]);
		tempFace.nodeList[4]=(elementList[i].nodeList[6]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[14]);
		tempFace.nodeList[6]=(elementList[i].nodeList[2]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[10]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                faceList.append(tempFace);

                //**********************************
                //tempFace.nodeList.clear();
                //
                tempFace.outside=true;
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=5;
		//face5
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[2]);
		tempFace.nodeList[1]=(elementList[i].nodeList[14]);
		tempFace.nodeList[2]=(elementList[i].nodeList[6]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[17]);
		tempFace.nodeList[4]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[13]);
		tempFace.nodeList[6]=(elementList[i].nodeList[1]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[9]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                faceList.append(tempFace);

                //**********************************
                //tempFace.nodeList.clear();
                //
                tempFace.outside=true;
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=6;
		//face6
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[1]);
		tempFace.nodeList[1]=(elementList[i].nodeList[13]);
		tempFace.nodeList[2]=(elementList[i].nodeList[5]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[16]);
		tempFace.nodeList[4]=(elementList[i].nodeList[4]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[12]);
		tempFace.nodeList[6]=(elementList[i].nodeList[0]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[8]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
        faceList.append(tempFace);
}
//***************************************************************************
void femodel::getTria3Faces(long elementListNumber)
{
        //int vorhanden=0;
        long i=elementListNumber;
        //long index=0;

        tempFace.outside=true;
        tempFace.parentElementDimension=2;
        //
        tempFace.parentElement=elementList[i].label;
        tempFace.faceNumber=1;
        tempFace.type=3;
        tempFace.centroidValue=0;
        tempFace.centroidColor=0;
        //face1
        //edge1
        tempFace.nodeList[0]=(elementList[i].nodeList[0]);
        tempFace.nodeList[1]=(elementList[i].nodeList[1]);
        //edge2
        tempFace.nodeList[2]=(elementList[i].nodeList[2]);
        //edge3
        tempFace.sortNodes();
          /*      index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

}
//***************************************************************************
void femodel::getTria6Faces(long elementListNumber)
{
        //int vorhanden=0;
        long i=elementListNumber;
        //long index=0;

        tempFace.outside=true;
        tempFace.parentElementDimension=2;
        //
        tempFace.parentElement=elementList[i].label;
        tempFace.faceNumber=1;
        tempFace.type=3;
        tempFace.centroidValue=0;
        tempFace.centroidColor=0;
        //face1
        //edge1
        tempFace.nodeList[0]=(elementList[i].nodeList[0]);
        tempFace.nodeList[1]=(elementList[i].nodeList[3]);
        tempFace.nodeList[1]=(elementList[i].nodeList[1]);
        //edge2
        tempFace.nodeList[3]=(elementList[i].nodeList[4]);
        tempFace.nodeList[2]=(elementList[i].nodeList[2]);
        //edge3
        tempFace.nodeList[5]=(elementList[i].nodeList[5]);
        //
        tempFace.sortNodes();
        //
        faceList.append(tempFace);

}
//***************************************************************************
void femodel::getTet4Faces(long elementListNumber)
{
        //int vorhanden=0;
		long i=elementListNumber;
        //long index=0;

		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
        tempFace.faceNumber=2;
		tempFace.type=3;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
        tempFace.nodeList[1]=(elementList[i].nodeList[1]);
		//edge2
        tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge3
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************/
        tempFace.faceNumber=4;
		//face2
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
        tempFace.nodeList[1]=(elementList[i].nodeList[3]);
		//edge2
        tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge3
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[2]);
        tempFace.nodeList[1]=(elementList[i].nodeList[3]);
		//edge2
        tempFace.nodeList[2]=(elementList[i].nodeList[1]);
		//edge3
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
        tempFace.faceNumber=1;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
        tempFace.nodeList[1]=(elementList[i].nodeList[2]);
		//edge2
        tempFace.nodeList[2]=(elementList[i].nodeList[1]);
		//edge3
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
        faceList.append(tempFace);

}
//*********************************************************************
void femodel::getTet10Faces(long elementListNumber)
//gets Faces from tet10 elements

{
                //int vorhanden=0;
		long i=elementListNumber;
                //long index=0;


		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
        tempFace.faceNumber=2;
		tempFace.type=6;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[4]);
		tempFace.nodeList[2]=(elementList[i].nodeList[1]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[8]);
		tempFace.nodeList[4]=(elementList[i].nodeList[3]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[7]);
		tempFace.sortNodes();
             /*   index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
               // emit sendMessage(QString("FaceIndex %1 %2 %3 %4 %5 %6 %7 %8 %9").arg(vorhanden).arg(index).arg(tempFace.orderedNodeList[0]).arg(tempFace.orderedNodeList[1]).arg(tempFace.orderedNodeList[2]).arg(tempFace.orderedNodeList[3]).arg(tempFace.orderedNodeList[4]).arg(tempFace.orderedNodeList[5]).arg(tempFace.type));
                faceList.append(tempFace);
		//**********************************/
                tempFace.outside=true;
                tempFace.faceNumber=4;
		//face2
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[7]);
		tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[9]);
		tempFace.nodeList[4]=(elementList[i].nodeList[2]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[6]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //emit sendMessage(QString("FaceIndex %1 %2 %3 %4 %5 %6 %7 %8 %9").arg(vorhanden).arg(index).arg(tempFace.orderedNodeList[0]).arg(tempFace.orderedNodeList[1]).arg(tempFace.orderedNodeList[2]).arg(tempFace.orderedNodeList[3]).arg(tempFace.orderedNodeList[4]).arg(tempFace.orderedNodeList[5]).arg(tempFace.type));
                faceList.append(tempFace);
		//**********************************
                tempFace.outside=true;
                tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[2]);
		tempFace.nodeList[1]=(elementList[i].nodeList[9]);
		tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[8]);
		tempFace.nodeList[4]=(elementList[i].nodeList[1]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[5]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //emit sendMessage(QString("FaceIndex %1 %2 %3 %4 %5 %6 %7 %8 %9").arg(vorhanden).arg(index).arg(tempFace.orderedNodeList[0]).arg(tempFace.orderedNodeList[1]).arg(tempFace.orderedNodeList[2]).arg(tempFace.orderedNodeList[3]).arg(tempFace.orderedNodeList[4]).arg(tempFace.orderedNodeList[5]).arg(tempFace.type));
                faceList.append(tempFace);
		//**********************************
        tempFace.outside=true;
        tempFace.faceNumber=1;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[6]);
		tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[5]);
		tempFace.nodeList[4]=(elementList[i].nodeList[1]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[4]);
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //emit sendMessage(QString("FaceIndex %1 %2 %3 %4 %5 %6 %7 %8 %9").arg(vorhanden).arg(index).arg(tempFace.orderedNodeList[0]).arg(tempFace.orderedNodeList[1]).arg(tempFace.orderedNodeList[2]).arg(tempFace.orderedNodeList[3]).arg(tempFace.orderedNodeList[4]).arg(tempFace.orderedNodeList[5]).arg(tempFace.type));
            faceList.append(tempFace);
		
}
//*******************************************************************
void femodel::getPenta6Faces(long elementListNumber)
{
        //int vorhanden=0;
		long i=elementListNumber;
        //long index=0;

		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=1;
		tempFace.type=3;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[1]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge3
		tempFace.sortNodes();
              /*  index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************/
		tempFace.faceNumber=2;
		//face2
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[3]);
		tempFace.nodeList[1]=(elementList[i].nodeList[4]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.sortNodes();
                /*index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.type=4;
		tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[1]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[4]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[3]);
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.faceNumber=4;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[1]);
		tempFace.nodeList[1]=(elementList[i].nodeList[2]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[4]);
		tempFace.sortNodes();
        /*      index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.faceNumber=5;
		//face5
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[2]);
		tempFace.nodeList[1]=(elementList[i].nodeList[0]);
		//edge2
		tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge3
		tempFace.nodeList[3]=(elementList[i].nodeList[5]);
		tempFace.sortNodes();
              /*  index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);


}
//**************************************************************
void femodel::getPenta15Faces(long elementListNumber)
{
        //        int vorhanden=0;
          //      long index=0;
		long i=elementListNumber;

		tempFace.outside=true;
        tempFace.parentElementDimension=3;
		//
		tempFace.parentElement=elementList[i].label;
		tempFace.faceNumber=1;
		tempFace.type=6;
		tempFace.centroidValue=0;
		tempFace.centroidColor=0;
		//face1
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[8]);
		tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[7]);
		tempFace.nodeList[4]=(elementList[i].nodeList[1]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[6]);
		tempFace.sortNodes();
          /*      index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************/
		tempFace.faceNumber=2;
		//face2
		///edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[3]);
		tempFace.nodeList[1]=(elementList[i].nodeList[12]);
		tempFace.nodeList[2]=(elementList[i].nodeList[4]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[13]);
		tempFace.nodeList[4]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[14]);
		tempFace.sortNodes();
               /* index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.type=8;
		tempFace.faceNumber=3;
		//face3
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[6]);
		tempFace.nodeList[2]=(elementList[i].nodeList[1]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[10]);
		tempFace.nodeList[4]=(elementList[i].nodeList[4]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[12]);
		tempFace.nodeList[6]=(elementList[i].nodeList[3]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[9]);
		tempFace.sortNodes();
		
            /*    index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.faceNumber=4;
		//face4
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[1]);
		tempFace.nodeList[1]=(elementList[i].nodeList[7]);
		tempFace.nodeList[2]=(elementList[i].nodeList[2]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[11]);
		tempFace.nodeList[4]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[13]);
		tempFace.nodeList[6]=(elementList[i].nodeList[4]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[10]);
		tempFace.sortNodes();
		
             /*   index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
                //
        faceList.append(tempFace);

                tempFace.outside=true;
		//**********************************
		tempFace.faceNumber=5;
		//face5
		//edge1
		tempFace.nodeList[0]=(elementList[i].nodeList[0]);
		tempFace.nodeList[1]=(elementList[i].nodeList[9]);
		tempFace.nodeList[2]=(elementList[i].nodeList[3]);
		//edge2
		tempFace.nodeList[3]=(elementList[i].nodeList[14]);
		tempFace.nodeList[4]=(elementList[i].nodeList[5]);
		//edge3
		tempFace.nodeList[5]=(elementList[i].nodeList[11]);
		tempFace.nodeList[6]=(elementList[i].nodeList[2]);
		//edge4
		tempFace.nodeList[7]=(elementList[i].nodeList[8]);
		tempFace.sortNodes();
		
             /*   index=getFaceIndex(tempFace, &vorhanden);
                if (vorhanden==0)
                {
                    faceList.insert(index,tempFace);
                }
                else
                {
                    faceList[index].outside=false;
                    tempFace.outside=false;
                    faceList.insert(index,tempFace);

                }*/
        faceList.append(tempFace);

}
//***************************************************************+
/*!Get all outside edges*/
void femodel::getOutsideEdges()
{
        long index;
        edgeList.clear();
        int vorhanden;
	short exists=0;
	for (long f=0;f < faceList.size();f++)
		{
			if (faceList[f].outside==true)
			{
				switch (faceList[f].type)
				{
				case 3:
					tempEdge.type=2;
					tempEdge.faces[0]=f;
					tempEdge.faces[1]=-1;
				//first edge
					exists=0;
					tempEdge.edgeNumber[0]=1;
					tempEdge.nodeList[0]=(faceList[f].nodeList[0]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[1]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                           edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				
				////////////////////////////////////////////////////////
				//second edge
					exists=0;
					tempEdge.edgeNumber[0]=2;
					tempEdge.nodeList[0]=(faceList[f].nodeList[1]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[2]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                           edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				/////////////////////////////////////////////////////////////////////
				//third edgee
					exists=0;
					tempEdge.edgeNumber[0]=3;
					tempEdge.nodeList[0]=(faceList[f].nodeList[2]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[0]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                            edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				break;
				case 4:
					tempEdge.type=2;
					tempEdge.faces[0]=f;
					tempEdge.faces[1]=-1;
				//first edge
					exists=0;
					tempEdge.edgeNumber[0]=1;
					tempEdge.nodeList[0]=(faceList[f].nodeList[0]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[1]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                           edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				
				////////////////////////////////////////////////////////
				//second edge
					exists=0;
					tempEdge.edgeNumber[0]=2;
					tempEdge.nodeList[0]=(faceList[f].nodeList[1]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[2]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                            edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
					
				/////////////////////////////////////////////////////////////////////
				//third edgee
					exists=0;
					tempEdge.edgeNumber[0]=3;
					tempEdge.nodeList[0]=(faceList[f].nodeList[2]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[3]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                           edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				
				///////////////////////////////////////////////////////////////////////////
				//fourth edge
					exists=0;
					tempEdge.edgeNumber[0]=4;
					tempEdge.nodeList[0]=(faceList[f].nodeList[3]);
					tempEdge.nodeList[1]=(faceList[f].nodeList[0]);
					tempEdge.sortNodes();
                                        index=getEdgeIndex(tempEdge, &vorhanden);
                                        if (vorhanden==0)
                                        {
                                            edgeList.insert(index,tempEdge);
                                        }
                                        else
                                        {
                                           edgeList[index].faces[1]=tempEdge.faces[0];
                                        }
				break;
				
				case 8:
				tempEdge.type=3;
				tempEdge.faces[0]=f;
				tempEdge.faces[1]=-1;
				//first edge
				exists=0;
				tempEdge.edgeNumber[0]=1;
				tempEdge.nodeList[0]=(faceList[f].nodeList[0]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[1]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[2]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];
                                }
				
				////////////////////////////////////////////////////////
				//second edge
				exists=0;
				tempEdge.edgeNumber[0]=2;
				tempEdge.nodeList[0]=(faceList[f].nodeList[2]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[3]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[4]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];

                                }
					
				/////////////////////////////////////////////////////////////////////
				//third edgee
				exists=0;
				tempEdge.edgeNumber[0]=3;
				tempEdge.nodeList[0]=(faceList[f].nodeList[4]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[5]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[6]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];

                                }
				
				///////////////////////////////////////////////////////////////////////////
				//fourth edge
				exists=0;
				tempEdge.edgeNumber[0]=4;
				tempEdge.nodeList[0]=(faceList[f].nodeList[6]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[7]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[0]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                   edgeList[index].faces[1]=tempEdge.faces[0];

                                }
				
				break;
				//Extracting edges of tri6 faces
				case 6:
				tempEdge.type=3;
				tempEdge.faces[0]=f;
				tempEdge.faces[1]=-1;
				//first edge
				exists=0;
				tempEdge.edgeNumber[0]=1;
				tempEdge.nodeList[0]=(faceList[f].nodeList[0]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[1]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[2]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];

                                }
				
				////////////////////////////////////////////////////////
				//second edge
				exists=0;
				tempEdge.edgeNumber[0]=2;
				tempEdge.nodeList[0]=(faceList[f].nodeList[2]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[3]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[4]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];

                                }
				
				/////////////////////////////////////////////////////////////////////
				//third edgee
				exists=0;
				tempEdge.edgeNumber[0]=3;
				tempEdge.nodeList[0]=(faceList[f].nodeList[4]);
				tempEdge.nodeList[1]=(faceList[f].nodeList[5]);
				tempEdge.nodeList[2]=(faceList[f].nodeList[0]);
				tempEdge.sortNodes();
                                index=getEdgeIndex(tempEdge, &vorhanden);
                                if (vorhanden==0)
                                {
                                    edgeList.insert(index,tempEdge);
                                }
                                else
                                {
                                    edgeList[index].faces[1]=tempEdge.faces[0];
                                }
		
				break;
				}
			}

		
		}
		
        /*sortEdgeList(0,edgeList.size()-1);
		
		long edgecount=edgeList.size();
		for (long z=0; z<edgecount-1;z++)
	{
		
		if (edgeList[z]==edgeList[z+1])
			{
				edgeList[z].faces[1]=edgeList[z+1].faces[0];
				//edgeList.remove(z+1);
				//edgecount--;
			}
			
	}
                */
calcEdgeAngles();
}
//********************************************************************
/*! Calculates the normals and centers of the faces */

void femodel::calcFaceNormals()
{
        int numberNodes;
        double area2; //Area of the triangle multplied by 2
        double sumArea;
        vector3d normal;
        vector3d sumNormal;
        vector3d sumWeightedCenter;
        vector3d tC2; //Coordinates of the triangle center multiplies by 3
        vector3d preliminaryCenter;
        vector3d v1;
        vector3d v2;
	
	for (long f=0; f<faceList.size(); f++)
	{
            //Check wether the face is a triangle. If so the normal is calculated directly
                if (faceList[f].type==3)
		{
                    v1=getNodeCoords(faceList[f].nodeList[1])-getNodeCoords(faceList[f].nodeList[0]);
                    v2=getNodeCoords(faceList[f].nodeList[2])-getNodeCoords(faceList[f].nodeList[0]);
                    //Area=(v1 cross v2) *0.5!!
                    faceList[f].normal= cross(v1,v2)*0.5;
                }
                else
                {
                    sumNormal.zero();
                    sumWeightedCenter.zero();
                    sumArea=0;
                    //Get the face center;
                     preliminaryCenter=faceList[f].centroid;
                     numberNodes=faceList[f].type;
                     /*for (int n=0;n<numberNodes;n++)
                     {
                         preliminaryCenter= preliminaryCenter+getNodeCoords(faceList[f].nodeList[n]);
                     }
                     preliminaryCenter=preliminaryCenter/numberNodes;*/
                     //Triangulate the area with the preliminary centroid, calculate normal and area for each triangle
                     //and the normal of the face by the weighted average
                     for (int k=0;k<numberNodes-1;k++)
                     {
                        //tC2=preliminaryCenter+getNodeCoords(faceList[f].nodeList[k])+getNodeCoords(faceList[f].nodeList[k+1]);
                        normal=cross((getNodeCoords(faceList[f].nodeList[k+1])-getNodeCoords(faceList[f].nodeList[k])),
                                     (preliminaryCenter-getNodeCoords(faceList[f].nodeList[k])));
                        area2=normal.norm();

                        sumNormal= sumNormal+normal;
                        sumArea+=area2;
                        //sumWeightedCenter= sumWeightedCenter+tC2*area2;
                    }
                    faceList[f].normal=0.5*sumNormal;

                }
		
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Calculates the coordinates of the centroids of the faces with the according shape functions  */

void femodel::calcFaceCentroids()
{
	
	for (long f=0; f<faceList.size(); f++)
	{
	
	double xi=0.0;
	double eta=0.0;
    //double area=0.0;
	vector3d p[8];
	switch (faceList[f].type)
	{
        case 4:
        //quad4 face

                /*faceList[f].centroid=getNodeCoords(faceList[f].nodeList[0])*(1.0-xi)*(1.0-eta)/4.0 +
                                                    getNodeCoords(faceList[f].nodeList[1])*(xi+1.0)*(1.0-eta)/4.0 +
                                                    getNodeCoords(faceList[f].nodeList[2])*(xi+1.0)*(eta+1.0)/4.0 +
                                                    getNodeCoords(faceList[f].nodeList[3])*(1.0-xi)*(eta+1.0)/4.0;*/
                //This can be reduced to:
                  faceList[f].centroid=(getNodeCoords(faceList[f].nodeList[0])+
                                                    getNodeCoords(faceList[f].nodeList[1]) +
                                                    getNodeCoords(faceList[f].nodeList[2]) +
                                                    getNodeCoords(faceList[f].nodeList[3]))/4.0;

        break;
	case 8:
	//quad8 face
		
		faceList[f].centroid=getNodeCoords(faceList[f].nodeList[0])*(1.0-xi)*(1.0-eta)*(-(xi)-eta-1.0)/4.0 + 
							 getNodeCoords(faceList[f].nodeList[2])*(xi+1.0)*(1.0-eta)*(xi-eta-1.0)/4.0 +
							 getNodeCoords(faceList[f].nodeList[4])*(xi+1.0)*(eta+1.0)*(xi+eta-1.0)/4.0 +
							 getNodeCoords(faceList[f].nodeList[6])*(1.0-xi)*(eta+1.0)*(-(xi)+eta-1.0)/4.0 +
							 getNodeCoords(faceList[f].nodeList[1])*(1.0-xi*xi)*(1.0-eta)/2.0 +
							 getNodeCoords(faceList[f].nodeList[3])*(1.0-eta*eta)*(xi+1.0)/2.0 +
							 getNodeCoords(faceList[f].nodeList[5])*(1.0-xi*xi)*(eta+1.0)/2.0 +
							 getNodeCoords(faceList[f].nodeList[7])*(1.0-eta*eta)*(1.0-xi)/2.0;
	break;
	case 6:
	//tria6 face
	//Formfunktion nach R.Gallagher "Finite Elemente Analysis" ISBN 3-540-07740-5
	//Koordinaten der Eckpunkte ermitteln
	p[0]=getNodeCoords(faceList[f].nodeList[0]); 
	p[1]=getNodeCoords(faceList[f].nodeList[1]);
	p[2]=getNodeCoords(faceList[f].nodeList[2]);
	p[3]=getNodeCoords(faceList[f].nodeList[3]);
	p[4]=getNodeCoords(faceList[f].nodeList[4]);
	p[5]=getNodeCoords(faceList[f].nodeList[5]);
	
	
        //Flaeche der Surface berechnen
	
    //area=calcTriArea(p[0],p[1],p[5])+calcTriArea(p[1],p[3],p[5])+calcTriArea(p[1],p[2],p[3])+calcTriArea(p[3],p[4],p[5]);
	
	//Berechnen der Dreieckskoordinaten des Mittelpunkts
	//L1=L2=L3=1/3
	
	double L1=1.0/3.0;
	double L2=1.0/3.0;
	double L3=1.0/3.0;
    //faceList[f].area=area;
	//Berechnen der N-Werte
	
	//Eckknoten 1
	float N200=L1*(2.0*L1-1.0);
	//Eckknoten 2
	float N020=L2*(2.0*L2-1.0);
	//Eckknoten 3
	float N002=L3*(2.0*L3-1.0);
	//Mittelknoten 12
	float N110=4.0*L1*L2;
	//Mittelknoten 23
	float N011=4.0*L2*L3;
	//Mitelknoten 31
	float N101=4.0*L3*L1;
	
	//Berechnen der Koordinaten
	faceList[f].centroid=N200*p[0]+(N020*p[2])+(N002*p[4])+(N110*p[1])+(N011*p[3])+(N101*p[5]);			 
	
	break;
	//
	}
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Calculates the displacements of the centroids of the faces with the according shape functions  */

void femodel::calcFaceCentroidDisps()
{
	
	for (long f=0; f<faceList.size(); f++)
	{
	
	switch (faceList[f].type)
	{
	case 8:
	//quad8 face
		double xi=0.0;
		double eta=0.0;
		faceList[f].centroidDisp=getDisplacements(faceList[f].nodeList[0])*(1-xi)*(1-eta)*(-(xi)-eta-1)/4 + 
							 getDisplacements(faceList[f].nodeList[2])*(xi+1)*(1-eta)*(xi-eta-1)/4 +
							 getDisplacements(faceList[f].nodeList[4])*(xi+1)*(eta+1)*(xi+eta-1)/4 +
							 getDisplacements(faceList[f].nodeList[6])*(1-xi)*(eta+1)*(-(xi)+eta-1)/4 +
							 getDisplacements(faceList[f].nodeList[1])*(1-xi*xi)*(1-eta)/2 +
							 getDisplacements(faceList[f].nodeList[3])*(1-eta*eta)*(xi+1)/2 +
							 getDisplacements(faceList[f].nodeList[5])*(1-xi*xi)*(eta+1)/2 +
							 getDisplacements(faceList[f].nodeList[7])*(1-eta*eta)*(1-xi)/2;
							 
	break;
	}
	}


}
//****************************************************************************************************
/*! Calculates the result of the centroids of the faces with the according shape functions  */
void femodel::calcFaceCentroidResults(int r, int c)
{
	float normResult=0;
	for (long f=0; f<faceList.size(); f++)
	{
	
	switch (faceList[f].type)
	{
	case 8:
	//quad8 face
		double xi=0.0;
		double eta=0.0;
		faceList[f].centroidValue=getNodeResults(faceList[f].nodeList[0])*(1-xi)*(1-eta)*(-(xi)-eta-1)/4 + 
							 getNodeResults(faceList[f].nodeList[2])*(xi+1)*(1-eta)*(xi-eta-1)/4 +
							 getNodeResults(faceList[f].nodeList[4])*(xi+1)*(eta+1)*(xi+eta-1)/4 +
							 getNodeResults(faceList[f].nodeList[6])*(1-xi)*(eta+1)*(-(xi)+eta-1)/4 +
							 getNodeResults(faceList[f].nodeList[1])*(1-xi*xi)*(1-eta)/2 +
							 getNodeResults(faceList[f].nodeList[3])*(1-eta*eta)*(xi+1)/2 +
							 getNodeResults(faceList[f].nodeList[5])*(1-xi*xi)*(eta+1)/2 +
							 getNodeResults(faceList[f].nodeList[7])*(1-eta*eta)*(1-xi)/2;
							 
	break;
	}
	normResult=((faceList[f].centroidValue-currentMin)/(currentMax-currentMin));
	faceList[f].centroidColor=normResult;
	//QString count=QString::number(normResult);
	//emit sendMessage(count);
	}


}
//***********************************************************************
//Calclulates the angle of two faces joining an edge
 void femodel::calcEdgeAngles()
 {
 //cos A=a*b/(|a|*|b|)
 for (long e=0; e < edgeList.size(); e++)
	{
		if (edgeList[e].faces[1]!=-1)
		{
			edgeList[e].angle=vector3d::vecAngle(faceList[edgeList[e].faces[0]].normal,faceList[edgeList[e].faces[1]].normal);
		}
		else
		{
			edgeList[e].angle=0;
            borderEdges.append(e);
		}
	}
 
 }
 //****************************************************
 /*! Sets the result values of the nodes*/
 void femodel::setNodeValues( int r, unsigned int c)
 {
	long index;
	float normResult;
	if (r > resultDataList.size())
	{
		return;
	}
	if (c > resultDataList[r-1].numberComponents)
	{
		return;
	}
	currentMax=resultDataList[r-1].maxValues[c-1];
	currentMin=resultDataList[r-1].minValues[c-1];
	currentTitle=resultDataList[r-1].resultLabel.remove(" ") + " - "  + resultDataList[r-1].componentLabels[c-1];
	
	for (int i=0; i< resultDataList[r-1].resultValueList.size(); i++)
	{
		index=getNodeIndex(resultDataList[r-1].resultValueList[i].nodeLabel);
		nodeList[index].setResultValue(resultDataList[r-1].resultValueList[i].resultValues[c-1]);
		
		normResult=(((nodeList[index].getResultValue())-currentMin)/(currentMax-currentMin));
		nodeList[index].setNormalizedResultValue(normResult);
				
		
	}
	//Pass the information of min/max values  and result title to the gl-widget
	emit sendFringeAutoValues(currentMin,currentMax);
	emit sendMinMaxScale(currentMin,currentMax);
	emit sendResultTitle(currentTitle);
    //Update the Display List for the faces
    emit newResultValues(-1);
	}
//******************************************************************
 void femodel::clearResultValues()
 {
	for (long i=0; i< nodeList.size(); i++)
	{
		nodeList[i].clearResultValue();
		nodeList[i].clearNormalizedResultValue();
	}
	for (long i=0; i< faceList.size(); i++)
	{
		faceList[i].centroidValue=0;
		faceList[i].centroidColor=0;
	}
}
//****************************************************************
void femodel::createSurfaceNodeSet()
{
/*
	int k=0;
	tempNSet.name="outside";
	tempNSet.nodeLabels.clear();
	nSetList.append(tempNSet);
	int size=nSetList.size();
	for (long n=0; n<faceList.size();n++)
	{
		if (faceList[n].outside==true)
		{
			for (k=0; k<faceList[n].type; k++)
			{
				
				
			
			}
		}
	}
	//emit sendMessage("Set outside created");
	QString count=QString::number(nSetList[size-1].nodeLabels.size());
	//emit sendMessage(count);
	tempNSet.nodeLabels.clear();
*/
}
//***************************************************************************************************
void femodel::clearDisp()
{
for (long i=0; i< nodeList.size(); i++)
	{
		nodeList[i].clearDisp();
	}
	calcFaceCentroidDisps();
	dispAvailable=0;
}
//***********************************************************
/*! Sorting a facelist*/
void femodel::sortFaceList(long links,long rechts)
{
        /* Betrachtung des (Teil-)Arrays,
   * das mindestens zwei Elemente umfasst,
   * left und right sind die Grenzen des (Teil-)Arrays. */
//    emit sendMessage(QString("%1 %2").arg(links).arg(rechts));
     if (links < rechts)
    {
    face pivot = faceList[rechts]; // das rechte Grenzelement= Pivotelement!
    long l = links;
    long r = rechts;

    do {
      // Den linken Index so weit nach rechts ruecken (erhoehen),
      // solange das l-Element kleiner ist als das Pivotelement.
      while (faceList[l] < pivot)
            l++;

      // Den rechten Index so weit nach links ruecken (verkleinern),
      // solange das r-Element groesser ist als das Pivotelement.
      while (faceList[r] > pivot) r--;

      // Vertausche die aktuellen r- und l-Elemente und
      // erhoehe l und verringere r jeweils um 1,
      // falls l noch links von r (rechts) ist!
      if (l <= r) {
          face swap = faceList[l];
          faceList[l] = faceList[r];
          faceList[r] = swap;
          l++;
          r--;
      }
    } while (l <= r); // bis sich l und r irgendwo in der Mitte treffen.
  //  emit sendMessage(QString("sortiert: %1 %2").arg(l).arg(r));
    if (r<links)
        r=links;
    sortFaceList(links, r);  // sortiere die linke Teilliste.
    sortFaceList( l, rechts); // sortiere die rechte Teilliste.

    }

}
//***********************************************************
//***********************************************************************
void femodel::sortEdgeList(long links,long rechts)
{
	/* Betrachtung des (Teil-)Arrays,
   * das mindestens zwei Elemente umfasst,
   * left und right sind die Grenzen des (Teil-)Arrays. */
  if (links < rechts) {

    edge pivot = edgeList[rechts]; // das rechte Grenzelement, Pivotelement!
    long l = links;
    long r = rechts;

    do {
      /* Den linken Index so weit nach rechts r�cken (erh�hen),
       * solange das l-Element kleiner ist als das Pivotelement. */
      while (edgeList[l] < pivot) l++;

      /* Den rechten Index so weit nach links r�cken (verkleinern),
       * solange das r-Element gr��er ist als das Pivotelement. */
      while (edgeList[r] > pivot) r--;

      /* Vertausche die aktuellen r- und l-Elemente und
       * erh�he l und verringere r jeweils um 1,
       * falls l noch links von r (rechts) ist! */
      if (l <= r) {
          edge swap = edgeList[l];
          edgeList[l] = edgeList[r];
          edgeList[r] = swap;
          l++;
          r--;
      }
    } while (l <= r); // bis sich l und r irgendwo in der Mitte treffen.

    sortEdgeList(links, r);  // sortiere die linke Teilliste.
    sortEdgeList( l, rechts); // sortiere die rechte Teilliste.
  }
}
//***************************************************************************************+
/*! Getting all outside nodes of the fe-moidel*/
void femodel::getOutsideNodes()
{
    tempSet.qCommandFlag=false;
    long index=-1;
    //Check wether the outside set already exists
    //Create it when it does not exist
    //The outside set always exists and is the first set -> index 0
    if (setList.size()>0)
    {
        if(setList[0].name!="outside")
        {
                    emit sendMessage("Creating set outside");
                   //Create the outside nSet
                      tempSet.name="outside";
                      tempSet.nodeList.clear();
                      setList.prepend(tempSet);
        }
    }
    else
    {
        emit sendMessage("Creating set outside");
       //Create the outside nSet
          tempSet.name="outside";
          tempSet.nodeList.clear();
          setList.prepend(tempSet);
    }
    //If no faces exist, all nodes are outside nodes
	if (faceList.size()==0)
	{
		for (int k=0;k<nodeList.size();k++)
		{
			nodeList[k].setOutside(true);
        }
	}
    //************************************************
    //Check the facelist for outside nodes
	for (long f=0; f<faceList.size();f++)
	{
		if (faceList[f].outside==true)
		{
			for (int n=0;n<faceList[f].type;n++)
			{
				index=getNodeIndex(faceList[f].nodeList[n]);
				if (index>=0)
				{
					nodeList[index].setOutside(true);
                }
			}
		}
	}
    //Put all Nodes with the outside flag in the outside set
    for (long k=0;k<nodeList.size();k++)
    {
            if (nodeList[k].isOutside())
            {
                 setList[0].nodeList.append(nodeList[k].getLabel());
            }
    }

}
//***************************************************************************************+
void femodel::getOutsideElements()
{
    int setIndex=-1;
    //Check wether the outside set already exists
        for (int ff=0;ff<setList.size();ff++)
        {
            if (setList[ff].name=="outside")
            {
                setIndex=ff;
            }
        }
        if (setIndex==-1)
        {
            //Create the outside set if it does not exist
            tempSet.name="outside";
            setList.prepend(tempSet);
            setIndex=0;
        }
        setList[setIndex].elementList.clear();

        for (long f=0; f<faceList.size();f++)
        {
                if (faceList[f].outside==true)
                {
                            elementList[getElementIndex(faceList[f].parentElement)].outside=true;

                }
        }

        for (long e=0;e<elementList.size();e++)
        {
            if (elementList[e].outside==true)
                setList[setIndex].elementList.append(elementList[e].label);
        }
}
//**********************************************************************************************
void femodel::writeOutsideFaces(const QString &fileName)
{
	

			//File Output			
	QString fout;
	QFile MyFile( fileName );
	if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text)) 
	{
		QTextStream stream( &MyFile );
		
		stream << frdHeader(fileName);
		stream << "    2C                                                                   1\n";
		for ( long i = 0; i < faceList.size(); i++)
		{
			if (faceList[i].outside==true)
			{
				stream << " -1 ";
				stream.setFieldWidth(9);
				stream << QString::number(faceList[i].parentElement*10+faceList[i].faceNumber);  
				stream.setFieldWidth(12);
				stream << QString::number(faceList[i].centroid.x); 
				stream << QString::number(faceList[i].centroid.y);
				stream << QString::number(faceList[i].centroid.z);
				stream.setFieldWidth(0);
				stream << "\n";
			}
		}
		stream << " -3\n";
		stream << " 9999\n";
	MyFile.close();
	}
	emit sendMessage("Outside faces sored in file " + fileName);
}
/*!Write the outside surfaces in ABQ-inp-format*/
void femodel::writeOutsideFacesABQ(const QString &fileName)
{
	//File Output			
	QFile MyFile( fileName );
	if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text)) 
	{
		QTextStream stream( &MyFile );
		stream << "*SURFACE,TYPE=ELEMENT,NAME=OUTSIDE\n";
		for ( long i = 0; i < faceList.size(); i++)
		{
			if (faceList[i].outside==true)
			{
				stream << QString("%1,").arg((faceList[i].parentElement),12);  
				stream << QString("S%1,\n").arg((faceList[i].faceNumber),4);  
			}
		}
	MyFile.close();
	}
	emit sendMessage("Outside faces stored in file " + fileName);
}
/////////////////////////////////////////////////////////////////////////////////////
//!Berechnen der Dreeicksflaeche nach Bronstein
double femodel::calcTriArea(vector3d p1,vector3d p2, vector3d p3)
{
        //Dreiecksflaeche ist halbe Parellelogramflaeche
	vector3d a=p2-p1;
	vector3d b=p3-p1;
	vector3d c;
	
	c=cross(a,b);
	
	
	return(c.norm()*0.5);
	
}


/*Change hex 8 elements to hex20 elements*/
void femodel::changeHex8toHex20()
{
        emit sendMessage("Changing HEX8 into HEX20");
	vector3d vertices[20];
	int v;
	long newNodes=0;
	if (nodeList.size()<1)
		return;
		
        long lastnode=nodeList[nodeList.size()-1].getLabel();
	
	for (int e=0;e<elementList.size();e++)
	{
		if (elementList[e].type==1)
		{
                        elementList[e].type=4;
			for (v=0;v<8;v++)
			{
                                vertices[v]=getNodeCoords(elementList[e].nodeList[v]);
			}
			//first new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[0]+(vertices[1]-vertices[0])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//second new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[1]+(vertices[2]-vertices[1])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//third new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[2]+(vertices[3]-vertices[2])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//fourth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[3]+(vertices[0]-vertices[3])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//fifth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[0]+(vertices[4]-vertices[0])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//sixth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[1]+(vertices[5]-vertices[1])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//seventh new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[2]+(vertices[6]-vertices[2])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//eighth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[3]+(vertices[7]-vertices[3])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//nineth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[4]+(vertices[5]-vertices[4])/2);
			tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//tenth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[5]+(vertices[6]-vertices[5])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//eleventh new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[6]+(vertices[7]-vertices[6])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
			//twelveth new node
                        tempNode.setLabel(lastnode+newNodes+1);
			elementList[e].nodeList.append(lastnode+newNodes+1);
			newNodes++;
                        tempNode.setCoordinates(vertices[7]+(vertices[4]-vertices[7])/2);
                        tempNode.adjacentElements.append(e);
                        nodeList.append(tempNode);
			tempNode.adjacentElements.clear();
		}
               // emit sendMessage(QString("element %1, node 1 %2, node 20 %3").arg(elementList[e].label).arg(elementList[e].nodeList[0]).arg(elementList[e].nodeList[19]));
	}
        mergeNodes(0.00001,0);

        getFaces();
        getOutsideEdges();
        getOutsideNodes();
        emit modelUpdate();
        //emit newFileLoaded();
}
//*****************************************************************************+
/*Merge close nodes*/
void femodel::mergeNodes(double tolerance,int id_mode)
{
    long n=0;
    long k=1;
    long elindex;
    long replaceNodeIndex;
    double tol=tolerance;
    vector3d firstNodeCoords;
    long nodesMerged=0;
    long firstNodeLabel;
    long secondNodeLabel;
    int i=0;
    emit setCursor(1);

    while (n<(nodeList.size()-1))
    {

        firstNodeCoords=nodeList[n].getCoordinates();
        firstNodeLabel=nodeList[n].getLabel();
        k=n+1;
        while (k<nodeList.size())
        {
            if ((firstNodeCoords-nodeList[k].getCoordinates()).norm()<tol)
            {
                //emit sendMessage(QString("merging %1 %2").arg(n).arg(k));
                secondNodeLabel=nodeList[k].getLabel();

                //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                if (id_mode==0)
                {
                    for (i=0;i<nodeList[k].adjacentElements.size();i++)
                    {
                      //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                      elindex=getElementIndex(nodeList[k].adjacentElements[i]);
                      replaceNodeIndex=elementList[elindex].nodeList.indexOf( secondNodeLabel);
                      elementList[elindex].nodeList.replace(replaceNodeIndex,firstNodeLabel);
                      //Hier sollte noch überprüft werden, ob das Element schon in der Liste ist!!!
                      nodeList[n].adjacentElements.append(nodeList[k].adjacentElements[i]);
                      }
                     nodeList.remove(k);

                 nodesMerged++;
                }
                else
                {
                    for (i=0;i<nodeList[n].adjacentElements.size();i++)
                    {
                      //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                      elindex=getElementIndex(nodeList[n].adjacentElements[i]);
                      replaceNodeIndex=elementList[elindex].nodeList.indexOf( firstNodeLabel);
                      elementList[elindex].nodeList.replace(replaceNodeIndex,secondNodeLabel);
                      //Hier sollte noch überprüft werden, ob das Element schon in der Liste ist!!!
                      nodeList[k].adjacentElements.append(nodeList[n].adjacentElements[i]);
                      }
                     nodeList.remove(n);
                 firstNodeLabel=nodeList[n].getLabel();
                 nodesMerged++;
                 k=n+1;
                }

            }
            else
            {
                k++;
            }

        }
        n++;
    }
    updateFacesEdges();
    emit sendMessage(QString("%1 Nodes merged").arg(nodesMerged));
    emit modelUpdate();
    emit setCursor(0);
}
//*****************************************************************************+
/*Merge close nodes of a specified set*/
void femodel::mergeNodesSet(double tolerance,int id_mode,int set)
{
    long n=0;
    long k=1;
    long elindex;
    long replaceNodeIndex;
    double tol=tolerance;
    vector3d firstNodeCoords;
    long nodesMerged=0;
    long firstNodeLabel;
    long secondNodeLabel;
    int i=0;
    emit setCursor(1);

    while (n<(setList[set].nodeList.size()-1))
    {

        firstNodeCoords=getNodeCoords(setList[set].nodeList[n]);
        firstNodeLabel=setList[set].nodeList[n];
        k=n+1;
        while (k<setList[set].nodeList.size())
        {
            if ((firstNodeCoords-getNodeCoords(setList[set].nodeList[k])).norm()<tol)
            {
                //emit sendMessage(QString("merging %1 %2").arg(n).arg(k));
                secondNodeLabel=setList[set].nodeList[k];

                //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                if (id_mode==0)
                {
                    for (i=0;i<nodeList[getNodeIndex(setList[set].nodeList[k])].adjacentElements.size();i++)
                    {
                      //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                      elindex=getElementIndex(nodeList[getNodeIndex(setList[set].nodeList[k])].adjacentElements[i]);
                      replaceNodeIndex=elementList[elindex].nodeList.indexOf( secondNodeLabel);
                      elementList[elindex].nodeList.replace(replaceNodeIndex,firstNodeLabel);
                      //Hier sollte noch überprüft werden, ob das Element schon in der Liste ist!!!
                      nodeList[getNodeIndex(setList[set].nodeList[n])].adjacentElements.append(nodeList[getNodeIndex(setList[set].nodeList[k])].adjacentElements[i]);
                      }
                     nodeList.remove(getNodeIndex(setList[set].nodeList[k]));
                    setList[set].nodeList.removeAt(k);
                    nodesMerged++;
                }

                else
                {
                    for (i=0;i<nodeList[getNodeIndex(setList[set].nodeList[n])].adjacentElements.size();i++)
                    {
                      //emit sendMessage(QString("elements %1 ").arg(nodeList[k].adjacentElements.size()));
                      elindex=getElementIndex(nodeList[getNodeIndex(setList[set].nodeList[n])].adjacentElements[i]);
                      replaceNodeIndex=elementList[elindex].nodeList.indexOf( firstNodeLabel);
                      elementList[elindex].nodeList.replace(replaceNodeIndex,secondNodeLabel);
                      //Hier sollte noch überprüft werden, ob das Element schon in der Liste ist!!!
                      nodeList[getNodeIndex(setList[set].nodeList[k])].adjacentElements.append(nodeList[getNodeIndex(setList[set].nodeList[n])].adjacentElements[i]);
                      }
                     nodeList.remove(getNodeIndex(setList[set].nodeList[n]));
                      setList[set].nodeList.removeAt(n);
                     firstNodeLabel=setList[set].nodeList[n];
                    nodesMerged++;
                    k=n+1;
                }

            }
            else
            {
                k++;
            }

        }
        n++;
    }
    updateFacesEdges();
    emit sendMessage(QString("%1 Nodes merged").arg(nodesMerged));
    emit modelUpdate();
    emit setCursor(0);
}
//*******************************************************************
/*! Scales the node coordinates by factor*/
void femodel::scaleModel(double scaleFactor)
{
    vector3d coords;
    for (long n=0;n<nodeList.size();n++)
    {
        coords=nodeList[n].getCoordinates();
        nodeList[n].setCoordinates(coords*scaleFactor);
    }
    emit modelScaled(scaleFactor);
     // emit modelUpdate();
}
//************************************************************
/*! Update the faces and edges*/
  void femodel::updateFacesEdges()
  {
      getFaces();
      getOutsideEdges();
  }
//***************************************************************
 /*!Creates an new set with the given name*/
  void femodel::createNewSet(const QString &name)
  {
      bool exists=false;
      for (int s=0;s<setList.size();s++)
      {
        if (setList[s].name==name)
          {
            exists=true;
        }
      }
      if (exists==false)
      {
        tempSet.name=name;
        setList.append(tempSet);
      }
//****************************************************************************************
//****************************************************************************************
      //Ab hier alter Code!
      exists=false;
      //Da ist Mist!!
      //KAnn alles mit einem Ste erledigt werden
      //Create the face Set
     /* for (int i=0;i<faceSetList.size();i++)
      {
        if (faceSetList[i].name==name)
          {
            exists=true;
        }
      }
      if (exists==false)
      {
        tempFaceSet.name=name;
        tempFaceSet.faceIndex.clear();
        faceSetList.append(tempFaceSet);
      }*/
        //Create the node Set
      exists=false;

            for (int i=0;i<setList.size();i++)
            {
              if (setList[i].name==name)
                {
                  exists=true;
                  setList[i].qCommandFlag=true;
                  currentNsetIndex=i;
              }
            }
            if (exists==false)
            {
                tempSet.name=name;
                tempSet.nodeList.clear();
                setList.append(tempSet);
                setList[setList.size()-1].qCommandFlag=true;
                currentNsetIndex=setList.size()-1;
            }
      //Create the element Set
      /*exists=false;
            for (int i=0;i<elSetList.size();i++)
            {
              if (elSetList[i].name==name)
                {
                  exists=true;
              }
            }
            if (exists==false)
            {
      tempElSet.name=name;
      tempElSet.elLabels.clear();
      elSetList.append(tempElSet);
    }*/
      emit update();
  }
//**********************************************************
/*!Adds a node to the current set*/
  void femodel::addNodeToSet(long label)
  {
    emit sendMessage(QString("Open Set %1").arg(currentSet));
    int vorhanden=0;
    long index=0;
      for (int i=0;i<setList.size();i++)
      {
        if (setList[i].name==currentSet)
          {
             emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                index = setList[i].getNodeIndex(label,&vorhanden);
                if(vorhanden==0)
                {
                    setList[i].nodeList.insert(index,label);
                    //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
                }
          }
      }
      emit update();
  }
  //**********************************************************
  /*! Adds an element to the current set*/
    void femodel::addElementToSet(long label)
    {
      emit sendMessage(QString("Open Set %1").arg(currentSet));
      int vorhanden=0;
      long index=0;
        for (int i=0;i<setList.size();i++)
        {
          if (setList[i].name==currentSet)
            {
               emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                  index = setList[i].getElementIndex(label,&vorhanden);
                  if(vorhanden==0)
                  {
                      setList[i].elementList.insert(index,label);
                      //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
                  }
            }
        }
        emit update();
    }
  //***********************************************************
  //**********************************************************
  /*!Adds a face to the current set*/
    void femodel::addFaceToSet(long label)
    {
      emit sendMessage(QString("Open Set %1").arg(currentSet));
      int vorhanden=0;
      long index=0;
        for (int i=0;i<setList.size();i++)
        {
          if (setList[i].name==currentSet)
            {
               emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                  index = setList[i].getFaceIndex(label,&vorhanden);
                  if(vorhanden==0)
                  {
                      setList[i].faceIndexList.insert(index,label);
                      //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
                  }
            }
        }
        emit update();
    }
    //***********************************************************
  /*! Adding a list of nodes to a set*/
  void femodel::addNodesToSet(QList<long> labelList)
  {
       emit sendMessage(QString("Open Set %1").arg(currentSet));
       int vorhanden=0;
       long index=0;
         for (int i=0;i<setList.size();i++)
         {
           if (setList[i].name==currentSet)
             {
       //insert the new nodes

                for (long m=0;m<labelList.size();m++)
                {
                index = setList[i].getNodeIndex(labelList[m],&vorhanden);
                if(vorhanden==0)
                {
                    setList[i].nodeList.insert(index,labelList[m]);
                    //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
                }
                }
               }
        }
       //call update
         emit update();
  }
  //***********************************************************
/*! Adding a list of elements to a set*/
void femodel::addElementsToSet(QList<long> labelList)
{
     emit sendMessage(QString("Open Set %1").arg(currentSet));
     int vorhanden=0;
     long index=0;
       for (int i=0;i<setList.size();i++)
       {
         if (setList[i].name==currentSet)
           {
     //insert the new nodes

              for (long m=0;m<labelList.size();m++)
              {
              index = setList[i].getElementIndex(labelList[m],&vorhanden);
              if(vorhanden==0)
              {
                  setList[i].elementList.insert(index,labelList[m]);
                  //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
              }
              }
             }
      }
       emit update();
}
  //***********************************************************
/*! Adding a list of faces to a set*/
void femodel::addFacesToSet(QList<long> labelList)
{
     emit sendMessage(QString("Open Set %1").arg(currentSet));
     int vorhanden=0;
     long index=0;
       for (int i=0;i<setList.size();i++)
       {
         if (setList[i].name==currentSet)
           {
     //insert the new nodes

              for (long m=0;m<labelList.size();m++)
              {
              index = setList[i].getFaceIndex(labelList[m],&vorhanden);
              if(vorhanden==0)
              {
                  setList[i].faceIndexList.insert(index,labelList[m]);
                  //emit sendMessage(QString("Node %1 added to set %2").arg(label).arg(currentSet));
              }
              }
             }
      }
     //call update
       emit update();
}
  //**********************************************************
  /*!Removes a node from the current set*/
    void femodel::removeNodeFromSet(long label)
    {
      emit sendMessage(QString("Open Set %1").arg(currentSet));
      int vorhanden=0;
      long index=0;
        for (int i=0;i<setList.size();i++)
        {
          if (setList[i].name==currentSet)
            {
               emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                  index = setList[i].getNodeIndex(label,&vorhanden);
                  if(vorhanden==1)
                  {
                      setList[i].nodeList.removeAt(index);
                      //emit sendMessage(QString("Node %1 removed from set %2").arg(label).arg(currentSet));
                  }
            }
        }
        emit update();
    }
    //**********************************************************
    /*!Removes an element from the current set*/
      void femodel::removeElementFromSet(long label)
      {
        emit sendMessage(QString("Open Set %1").arg(currentSet));
        int vorhanden=0;
        long index=0;
          for (int i=0;i<setList.size();i++)
          {
            if (setList[i].name==currentSet)
              {
                 emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                    index = setList[i].getElementIndex(label,&vorhanden);
                    if(vorhanden==1)
                    {
                        setList[i].elementList.removeAt(index);
                        emit sendMessage(QString("Element %1 removed from set %2").arg(label).arg(currentSet));
                    }
              }
          }
          emit update();
      }
    //**********************************************************
    /*!Removes a node from the current set*/
      void femodel::removeFaceFromSet(long label)
      {
        emit sendMessage(QString("Open Set %1").arg(currentSet));
        int vorhanden=0;
        long index=0;
          for (int i=0;i<setList.size();i++)
          {
            if (setList[i].name==currentSet)
              {
                 emit sendMessage(QString("Open Set %1 found ").arg(currentSet));
                    index = setList[i].getFaceIndex(label,&vorhanden);
                    if(vorhanden==1)
                    {
                        setList[i].faceIndexList.removeAt(index);
                        //emit sendMessage(QString("Node %1 removed from set %2").arg(label).arg(currentSet));
                    }
              }
          }
          emit update();
      }

//**********************************
/*! Write a set to a file in abq format*/
void femodel::writeSetAbq(const QString &file, int setIndex, long offset)
{

}
//**********************************
//PUBLIC SLOT
/*! Write the labels of a set to a file in abq format*/
void femodel::writeSetAbqLabelsOnly( int setIndex, long offset)
{
    //fix directory for testing
    QFile MyFile( openFile.append("-" + setList[setIndex].name+".set" ));
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {


            QTextStream stream( &MyFile );
            //Writing the header
            stream << QString("** Nodeset created from nodeset %1 \n").arg(setList[setIndex].name);
            //Writing nodelabels
            stream << QString("*NSET,NSET=N%1 \n").arg(setList[setIndex].name);
            for ( long i = 0; i < setList[setIndex].nodeList.size(); i++)
            {
                            //stream.setFieldWidth(12);
                            stream << QString::number(setList[setIndex].nodeList[i]+offset);
                            stream << ", \n";

            }
            stream << QString("*ELSET,ELSET=E%1 \n").arg(setList[setIndex].name);
            /*for ( long i = 0; i < nSetList[setIndex].nodeLabels.size(); i++)
            {
                            //stream.setFieldWidth(12);
                            stream << QString::number(nSetList[setIndex].nodeLabels[i]+offset);

            }*/
            emit sendMessage(QString("Labels for Set %1 exported").arg(setList[setIndex].name));
        }
    return;

}
//**********************************
/*! Set the plot flag for a node set*/
void femodel::setNodeSetPlotFlag(int index,int colorIndex)
{
   // setList[index].plot=TRUE;
    setList[index].nodePlotColor=colorIndex;
    emit sendMessage(QString("Plotting nodeset %1").arg(index));
    emit update();
}
//**********************************
/*! Unset the plot flag for a node set*/
void femodel::unsetNodeSetPlotFlag(int index)
{
    setList[index].plot=FALSE;
    emit update();
}
//******************************************
/*! Set the plot flag for a node set and unsets the plot flag for all other sets*/
void femodel::setSingleNodeSetPlotFlag(int index,int colorIndex)
{
    for (int n=0;n<setList.size();n++)
    {
        setList[n].plot=FALSE;
    }
  //  setList[index].plot=TRUE;
    setList[index].nodePlotColor=colorIndex;
    emit sendMessage(QString("Plotting nodeset %1").arg(index));
    emit update();
}
//******************************************
/*! Set the plot flag for a face set and unsets the plot flag for all other sets*/
void femodel::setSingleFaceSetPlotFlag(int index,int colorIndex)
{
    for (int n=0;n<setList.size();n++)
    {
        setList[n].plotFace=FALSE;
    }
  //  setList[index].plot=TRUE;
    setList[index].facePlotColor=colorIndex;
    emit sendMessage(QString("Plotting nodeset %1").arg(index));
    emit update();
}
//***************************************************
/*! Calculate the expansion ratio of the cells*/
//Only tri3 supported
//Check wether the face belongs to a 2d or 3d cell.
//ACcording to this the  expansion ratio must be set to the face center or the volume center
void femodel::calcExpRatio()
{
    double ratio=1;
    //long owner=0;
    //long neighbour=0;
    double maxRatio=1;

    //File Output
 /*   QFile MyFile(openFile+".expRatio");
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &MyFile );
        stream << "**Test for expansion ratio\n";
    */
    //loop over all edges
    for (long e=0;e<edgeList.size();e++)
    {
        //The expansion ratio can only be calculated between two faces for 3d elements
        if (elementList[faceList[edgeList[e].faces[0]].parentElement].type < 7)
        {
            ratio=0;
        }
        else
        {
            ratio=(faceList[edgeList[e].faces[1]].normal.norm())/(faceList[edgeList[e].faces[0]].normal.norm());

            if (ratio<1)
            {
                ratio=1/ratio;

            }
            if (ratio> maxRatio)
            {
                maxRatio=ratio;
            }
        }
        if (ratio> faceList[edgeList[e].faces[0]].centroidValue)
        {
            faceList[edgeList[e].faces[0]].centroidValue=ratio;
        }
        if (ratio> faceList[edgeList[e].faces[1]].centroidValue)
        {
            faceList[edgeList[e].faces[1]].centroidValue=ratio;
        }
    /*    owner=faceList[edgeList[e].faces[0]].parentElement;
        neighbour=faceList[edgeList[e].faces[1]].parentElement;
        stream << QString("edge %1").arg(e) ;
        stream << QString(" parent %1").arg(owner);
        stream << QString(" neighbour %1").arg(neighbour);
        stream << QString(" expRatio %1 \n").arg(ratio);
    */
    }
    emit sendMessage(QString("Max Exp Ratio: %1").arg(maxRatio));
    emit fringePlot(3);
    emit sendFringeAutoValues(1.0,maxRatio);
    emit newResultValues(-1);
    //MyFile.close();

}
//*************************************************************************
/*! Move nodes radially by a given amount*/
void femodel::moveNodesRadially(double dr)
{
    double phi=0.0;
    vector3d tempCoords;
    double dy=0,dz=0;

    for (long n=0;n<nodeList.size();n++)
    {
        tempCoords=nodeList[n].getCoordinates();
        phi=atan2(tempCoords.z,tempCoords.y);
        dy=dr*cos(phi);
        dz=dr*sin(phi);
        tempCoords.y+=dy;
        tempCoords.z+=dz;
        nodeList[n].setCoordinates(tempCoords);
    }
    emit modelUpdate();
    emit sendMessage(QString(" Moved nodes radially by %1 ").arg(dr));
}
//******************************************************
/*! Creates a boundary condition*/
void femodel::createBC(const QString &name, const QString &set, int fdof, int ldof, double value)
{
    boundarycon tempBC;

    tempBC.setValues(name,set,fdof,ldof,value);
    bcList.append(tempBC);
    emit update();
}
//******************************************************
/*! Deletes a boundary condition*/
void femodel::deleteBC(int index)
{

    bcList.removeAt(index);
    emit update();
}
//******************************************************
/*! Deletes a load*/
void femodel::deleteLoads(int index)
{

    loadList.removeAt(index);
    emit update();
}
//********************************************************
/*! Delete all mesh entities*/
void femodel::deleteAllMesh()
{
    nodeList.clear();
    elementList.clear();
    faceList.clear();
    setList.clear();
    emit modelUpdate();
}
//********************************************************
/*! Delete all mesh entities*/
void femodel::createMaterial(QString name,double ymodulus,double poisson,double density)
{
    material tempMat;

    tempMat.setName(name);
    tempMat.addElastic(ymodulus,poisson,293);
    tempMat.addDensity(density,293);

    materialList.append(tempMat);
    emit sendMessage(QString("Material %1 created.").arg(name));
    emit update();
}
//******************************************************
/*! Deletes a material*/
void femodel::deleteMaterial(int index)
{

    materialList.removeAt(index);
    emit update();
}
//********************************************************
/*! Creates a fe section*/
void femodel::createFeSection(QString n, QString elset,QString material, int type, double thickness)
{
    feSection tempSect;

    tempSect.name=n;
    tempSect.set=elset;
    tempSect.material=material;

    feSectionList.append(tempSect);
    emit sendMessage(QString("Section %1 created.").arg(n));
    emit update();
}
//******************************************************
/*! Deletes a fe section */
void femodel::deleteFeSection(int index)
{

    feSectionList.removeAt(index);
    emit update();
}
//******************************************************
/*! Selects all nodes until a feature edge is encounterd */
//void femodel::autoSelect(long node, double featureAngle)
void femodel::autoSelectNodes(const QString set, double featureAngle )
{
    int setIndex=-1;
    //check wether the set exists
    for (int w=0;w<setList.size();w++)
    {
        if(setList[w].name==set)
        {
            setIndex=w;
        }
    }
    if (setIndex==-1)
    {
        emit sendMessage(QString("Set %1 does not exist. Aborting!").arg(set));
        return;
    }
    //Check wether exactly one node is in the set.
    if (setList[setIndex].nodeList.size()!=1)
    {
        emit sendMessage(QString("The target set must contain exactly one node. Aborting!") );
        return;
    }

    QList<long> rootElements;
    QList<long> selectedFaces;
    long elIndex=0;
    rootElements.clear();
    selectedFaces.clear();
    emit sendMessage("Starting auto extension");
    //check wether the node is outside
    long nodeIndex=getNodeIndex(setList[setIndex].nodeList[0]);
    if (nodeList[nodeIndex].isOutside()==false)
    {
        emit sendMessage(QString("Node %1 is not an outside node. Aborting!").arg(setList[setIndex].nodeList[0]));
        return;
    }
    emit sendMessage(QString("Getting elements for node %1").arg(setList[setIndex].nodeList[0]));
    //Get the outside elements belonging to the node
    for (int k=0;k<nodeList[nodeIndex].adjacentElements.size();k++)
    {
        elIndex=getElementIndex(nodeList[nodeIndex].adjacentElements[k]);
        if (elementList[elIndex].outside==true)
        {
            emit sendMessage(QString("Adding element to root elements %1").arg(nodeList[nodeIndex].adjacentElements[k]));
            rootElements.append(nodeList[nodeIndex].adjacentElements[k]);
        }
    }
    //Get the outside faces belonging to the root elements and put the indices in the selected Faces list
    for (int t=0;t<rootElements.size();t++)
    {
        for (long f=0;f<faceList.size();f++)
        {
            if ((faceList[f].parentElement==rootElements[t]) && (faceList[f].outside==true))
             {
                //Check wether the node is on the face
                for (int s=0;s<faceList[f].type;s++)
                {
                    if (faceList[f].nodeList[s]==setList[setIndex].nodeList[0])
                    {
                        emit sendMessage(QString("Adding face %1 to root selection ").arg(f));
                        selectedFaces.append(f);
                    }
                }
            }

        }
    }
    //***************************************************************
    //Extend the faces until a feuture edge is encountered
    long g=0;
    long e=0;
    long newFaceIndex=0;
    while (g < selectedFaces.size())
    {
        for (e=0;e<edgeList.size();e++)
        {
            if (selectedFaces[g]==edgeList[e].faces[0] || selectedFaces[g]==edgeList[e].faces[1])
            {
            //Check wether the angle is samller than the feature angle
                if (edgeList[e].angle < featureAngle)
                {
                    selectedFaces[g]==edgeList[e].faces[0] ? newFaceIndex=edgeList[e].faces[1] : newFaceIndex=edgeList[e].faces[0];
                    //Check wether the face already exists in the list
                    if (! selectedFaces.contains(newFaceIndex))
                    {
                        emit sendMessage(QString("Adding face %1 to selection ").arg(newFaceIndex));
                        selectedFaces.append(newFaceIndex);
                    }
               }
            }
        }
        g++;
    }
     emit sendMessage(QString("Total of %1 faces selected ").arg(selectedFaces.size()));

//Get the nodes belonging to the faces
    QList<long> autoSelectedNodes;
    long fIndex;
    autoSelectedNodes.clear();
    for (long d=0;d<selectedFaces.size();d++)
    {
        fIndex=selectedFaces[d];
        for (long z=0;z<faceList[fIndex].type;z++)
        {
            autoSelectedNodes.append(faceList[fIndex].nodeList[z]);
        }
    }
    //sort the LIst
    qSort(autoSelectedNodes);
    //eliminate duplicate nodes
    long j=0;
    while (j<autoSelectedNodes.size()-1)
    {
        if (autoSelectedNodes[j]==autoSelectedNodes[j+1])
        {
            autoSelectedNodes.removeAt(j+1);
        }
        else
        {
            j++;
        }
    }
    emit sendMessage("Auto Extension complete!");
    for (j=0;j<autoSelectedNodes.size();j++)
    {
        emit sendMessage(QString("Selected Node %1").arg(autoSelectedNodes[j]));
    }
    //*******************************
       //insert the new nodes
                int vorhanden;
                long index;
                for (j=0;j<autoSelectedNodes.size();j++)
                {
                    index = setList[setIndex].getNodeIndex(autoSelectedNodes[j],&vorhanden);
                    if(vorhanden==0)
                    {
                        setList[setIndex].nodeList.insert(index,autoSelectedNodes[j]);

                    }
                }

       //call update
         emit update();
}
//********************************************************
/*! Creates a load*/
void femodel::createLoad(QString n, int type,QString set, double value, int direction)
{
    load tempLoad;

    tempLoad.name=n;
    tempLoad.targetSet=set;
    tempLoad.type=type;
    tempLoad.value=value;
    tempLoad.direction=direction;

    loadList.append(tempLoad);
    emit sendMessage(QString("Load %1 created.").arg(n));
    emit update();
}
//********************************************************
/*! Creates a load*/
void femodel::createCentrifugalLoad(QString n,QString set, QString point1,QString point2, double rpm)
{
    vector3d p1,p2;
    load tempLoad;

    p1.x=point1.section(",",0,0).toDouble();
    p1.y=point1.section(",",1,1).toDouble();
    p1.z=point1.section(",",2,2).toDouble();

    p2.x=point2.section(",",0,0).toDouble();
    p2.y=point2.section(",",1,1).toDouble();
    p2.z=point2.section(",",2,2).toDouble();



    tempLoad.name=n;
    tempLoad.targetSet=set;
    tempLoad.type=2;
    tempLoad.value=rpm;
    tempLoad.point1=p1;
    tempLoad.point2=p2;

    loadList.append(tempLoad);
    emit sendMessage(QString("Load %1 created.").arg(n));
    emit update();
}
//******************************************************
/*! Selects all nodes until a feature edge is encounterd */
//void femodel::autoSelect(long node, double featureAngle)
void femodel::autoSelectFaces(const QString set, double featureAngle )
{
    int setIndex=-1;
    //check wether the set exists
    for (int w=0;w<setList.size();w++)
    {
        if(setList[w].name==set)
        {
            setIndex=w;
        }
    }
    if (setIndex==-1)
    {
        emit sendMessage(QString("Set %1 does not exist. Aborting!").arg(set));
        return;
    }
    //Check wether exactly one node is in the set.
    //if (setList[setIndex].faceIndexList.size()!=1)
    //{
    //    emit sendMessage(QString("The target set must contain exactly one face. Aborting!") );
    //    return;
    //}


    QList<long> selectedFaces;
    //
    selectedFaces.clear();
    selectedFaces=setList[setIndex].faceIndexList;
    //
    //***************************************************************
    //Extend the faces until a feuture edge is encountered
    long g=0;
    long e=0;
    long newFaceIndex=0;
    while (g < selectedFaces.size())
    {
        for (e=0;e<edgeList.size();e++)
        {
            if (selectedFaces[g]==edgeList[e].faces[0] || selectedFaces[g]==edgeList[e].faces[1])
            {
            //Check wether the angle is samller than the feature angle
                if (edgeList[e].angle < featureAngle)
                {
                    selectedFaces[g]==edgeList[e].faces[0] ? newFaceIndex=edgeList[e].faces[1] : newFaceIndex=edgeList[e].faces[0];
                    //Check wether the face already exists in the list
                    if (! selectedFaces.contains(newFaceIndex))
                    {
                        emit sendMessage(QString("Adding face %1 to selection ").arg(newFaceIndex));
                        selectedFaces.append(newFaceIndex);
                    }
               }
            }
        }
        g++;
    }
     emit sendMessage(QString("Total of %1 faces selected ").arg(selectedFaces.size()));

    //sort the LIst
    qSort(selectedFaces);
    emit sendMessage("Auto Extension complete!");
    setList[setIndex].faceIndexList.clear();
    setList[setIndex].faceIndexList=selectedFaces;

       //call update
         emit update();
}
//********************************************************
void femodel::sendM(const QString &text)
{
    emit sendMessage(text);
}
//********************************************************
/*! Creates a time animation*/
void femodel::createTimeAnimation()//(QString rLabel,QString rType)
{
    QString currentLabel="NDTEMP";
    emit sendMessage("Starting time animation");
    //Clear the current data
    timeAnimation->resultList.clear();
    timeAnimation->timepoints.clear();
    timeAnimation->nTimesteps=0;

    //load every result and check wether the result type is presented
    //If it is present store the results in the timeAnimationResults List

    switch (fileflag)
    {
        //frd file
    case 1:

    for (int k=0;k<resultList.size();k++)
    {
        emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
        if (resultList[k].resultType==0)
        {
            timeAnimation->nTimesteps++;
            timeAnimation->timepoints.append(resultList[k].frqtime);
            //load the result
            emit stepSelected(k);
            //create a display list for the result step
            emit createTimeFrame(k);
        }
    }
    break;
    }
    emit sendMessage(QString("Result size %1").arg(timeAnimation->resultList.size()));
    emit startTimeAnim(timeAnimation);

}


