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
/**
 * \file geomodel.cpp
 * \brief A brief file description
 **/
#include "geomodel.h"
//*****************************************************************************************
//*****************************************************************************************
/*! Standard Constructor*/
geomodel::geomodel()
{
	planeList.clear();
        cosList.clear();

	tempPlane.setLabel(1);
	tempPlane.setOrigin(-1,1,0);
	tempPlane.setNormal(1,5,8);
	tempPlane.setDrawn(true);
    //*******************************************
        /*Create the global cos*/
        tempCos.setName("global rect");
        tempCos.setOrigin(0,0,0);
        tempCos.setVector1(1,0,0);
        tempCos.setVector2(0,0,1);
        tempCos.setType(0);
        cosList.append(tempCos);
        /*Create the global cyl cos*/
        tempCos.setName("global cyl");
        tempCos.setOrigin(0,0,0);
        tempCos.setVector1(0,0,1);
        tempCos.setVector2(1,0,0);
        tempCos.setType(1);
        cosList.append(tempCos);
    //********************************************************
        //	planeList.append(tempPlane);
	pointList.clear();
	curveList.clear();
    surfaceList.clear();

        modelRadius=0.0001;
}
//******************************************************************************************
void geomodel::initialize()
{
    emit geoDatabaseModified();

}
//******************************************************************************
/*!Ceates an offset plane*/
void geomodel::createOffsetPlane(long baseplane)
{
	long baseIndex=-1;
	double dist=1;
	emit sendMessage(QString("New offset plane"));
	for (long i=0;i<planeList.size();i++)
	{
		if (baseplane== planeList[i].getLabel())
		{
			baseIndex=i;
			break;
		}
	}
	if (baseIndex==-1)
	{
		emit sendMessage("Illegal base plane - No plane created");
		return;
	}
	tempPlane.setLabel(planeList[planeList.size()-1].getLabel()+1);
	tempPlane.setOrigin(planeList[baseIndex].getOrigin()+dist*planeList[0].getNormal());
	tempPlane.setNormal(planeList[baseIndex].getNormal());
	tempPlane.setDrawn(true);
	planeList.append(tempPlane);
	emit newPlaneCreated();
	emit geoDatabaseModified();
}
//******************************************************************************************
/*!Deletes plane*/
void geomodel::deletePlane(long label)
{
	for (long i=0;i<planeList.size();i++)
	{
		if (label== planeList[i].getLabel())
		{
			planeList.removeAt(i);
		}
	}
	emit geoDatabaseModified();
}
//*******************************************
/*Creates a new point and returns the label of the newly created point*/
long geomodel::createPoint(double x, double y, double z,int mode=0)
{
	point temppoint;
	long label=1;
	if (pointList.size()>0)
		label=pointList[pointList.size()-1].getLabel()+1;
	temppoint.setLabel(label);
	temppoint.setCoordinates(x,y,z);
	pointList.append(temppoint);
	if (temppoint.getCoordinates().norm() > modelRadius)
	{
		modelRadius=temppoint.getCoordinates().norm()*1.3;
	}
    if (mode==0)
    {
        emit sendMessage(QString("Point %1 created at %2 %3 %4  ").arg(label).arg(x).arg(y).arg(z));
        emit geoDatabaseModified();
    }
    return(label);

}
//*******************************************
/*Creates a new point*/
void geomodel::createPoint(long pointIndex,long pointLabel,double x, double y, double z)
{

      point temppoint;
    long label=pointLabel;
    temppoint.setLabel(label);
    temppoint.setCoordinates(x,y,z);
    if (pointIndex==0)
    {
        pointList.append(temppoint);
    }
    else
    {
        pointList.insert(pointIndex,temppoint);
    }
    if (temppoint.getCoordinates().norm() > modelRadius)
    {
        modelRadius=temppoint.getCoordinates().norm()*1.1;
    }
    emit sendMessage(QString("Point %1 created at %2 %3 %4  ").arg(label).arg(x).arg(y).arg(z));
    emit geoDatabaseModified();
}
//***********************************************
/*Creates a new curve (straight line) between s and e and returns the label of the new line*/
long geomodel::createCurve(long s,long e)
{

	curve tempCurve;
	//check wether the points exist!
	//Otherwise don't create the line
	if (getPointIndex(s)==-1)
	{
		emit sendMessage(QString("Startpoint (Point %1) does not exist. Line not created").arg(s));
                return (-1);
	}
	if (getPointIndex(e)==-1)
	{
		emit sendMessage(QString("Endpoint (Point %1) does not exist. Line not created").arg(e));
                return (-1);
	}
	long label=1;
	if (curveList.size()>0)
		label=curveList[curveList.size()-1].getLabel()+1;
	tempCurve.setLabel(label);
	tempCurve.setPoints(s,e);
        //****************
        //Set the pointers for the points
        tempCurve.sPoint=&pointList[getPointIndex(s)];
        tempCurve.ePoint=&pointList[getPointIndex(e)];
        //****************
	curveList.append(tempCurve);
	emit sendMessage(QString("Curve %1 created between point %2 and point %3 ").arg(label).arg(s).arg(e));
	emit geoDatabaseModified();
        return(label);
}
//***********************************************
/*Creates a curve with label l (straight line) between s and e */
void geomodel::createCurve(long lineLabel,long s,long e)
{
    emit sendMessage(QString("lineLabel: %1").arg(lineLabel));
    curve tempCurve;
    long lineIndex;
    //Check wether a line with the given label already exists
    if (curveList.size()==0)
    {
        lineIndex=0;
    }
    else
    {
        lineIndex=getLineIndex(lineLabel);
        emit sendMessage(QString("lineIndex: %1").arg(lineIndex));
        if (lineIndex > -1)
        {
           emit sendMessage(QString("Line %1 already exitst! Line creation cancelled!").arg(lineLabel));
           return;
        }
    }

    //check wether the points exist!
    //Otherwise don't create the line
    if (getPointIndex(s)<0)
    {
        emit sendMessage(QString("Startpoint (Point %1) does not exist. Line not created").arg(s));
        return;
    }

    if (getPointIndex(e)<0)
    {
        emit sendMessage(QString("Endpoint (Point %1) does not exist. Line not created").arg(e));
        return;
    }

    tempCurve.setLabel(lineLabel);
    tempCurve.setPoints(s,e);
    if (curveList.size()==0)
    {
        curveList.append(tempCurve);
    }
    else
    {
        if (lineLabel > curveList.last().getLabel())
            curveList.append(tempCurve);
        else
            curveList.insert(lineIndex*-1,tempCurve);
    }

    emit sendMessage(QString("Curve %1 created between point %2 and point %3 ").arg(lineLabel).arg(s).arg(e));
    emit geoDatabaseModified();
}
//***************************************************
//***********************************************************************************************
/*! Returns the coordinates (vector3d) of the point with the given  label*/
vector3d geomodel::getPointCoords(long label)
	{
	vector3d errorCoords;
		errorCoords[0]=-99;
		errorCoords[1]=-99;
		errorCoords[2]=-99;
	
	
	
unsigned long links=1;
unsigned long rechts=pointList.size();
unsigned long mitte;
	while(1)
	{
	if (links>rechts) return (errorCoords);
	mitte=(links+rechts)/2;
	if (pointList[mitte-1].getLabel()==label) return(pointList[mitte-1].getCoordinates());
	
	if (label<pointList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//***********************************************************************************************
/*! Returns the point index in the pointlist ot the point with the given label*/
/*! Returns the negative value of the possible insert location if the point does not exist*/
long geomodel::getPointIndex(long label)
{
unsigned long links=1;
unsigned long rechts=pointList.size();
unsigned long mitte;
//return -1 if the pointlist is empty
if (pointList.size()<1)
{
    return(-1);
}
	while(1)
	{
    if (links>rechts) return (-1*rechts);
	mitte=(links+rechts)/2;
	if (pointList[mitte-1].getLabel()==label) return(mitte-1);
	
	if (label<pointList[mitte-1].getLabel())
		rechts=mitte-1;
	else
		links=mitte+1;
	}
}
//***********************************************************************************************
//***********************************************************************************************
/*! Returns the curve index in the curvelist of the curve with the given label*/
/*! Returns the negative value of the possible insert location if the curve does not exist*/
long geomodel::getLineIndex(long label)
{
unsigned long links=1;
unsigned long rechts=curveList.size();
unsigned long mitte;
    while(1)
    {
    if (links>rechts) return (-1*rechts);
    mitte=(links+rechts)/2;
    if (curveList[mitte-1].getLabel()==label) return(mitte-1);

    if (label<curveList[mitte-1].getLabel())
        rechts=mitte-1;
    else
        links=mitte+1;
    }
}
//***********************************************************************************************
//***********************************************************************************************
/*! Returns the surface index in the surfacelist of the surface with the given label*/
/*! Returns the negative value of the possible insert location if the surface does not exist*/
long geomodel::getSurfaceIndex(long label)
{
unsigned long links=1;
unsigned long rechts=surfaceList.size();
unsigned long mitte;
    while(1)
    {
    if (links>rechts) return (-1*rechts);
    mitte=(links+rechts)/2;
    if (surfaceList[mitte-1].getLabel()==label) return(mitte-1);

    if (label<surfaceList[mitte-1].getLabel())
        rechts=mitte-1;
    else
        links=mitte+1;
    }
}
//***********************************************************************************************
/*! Creates a new COS*/
void geomodel::createCos(const QString &name,vector3d origin, vector3d v1, vector3d v2, int type )
{
    coordinateSystem tempCos;

    tempCos.setName(name);
    tempCos.setOrigin(origin);
    tempCos.setVector1(v1);
    tempCos.setVector2(v2);
    cosList.append(tempCos);
    emit sendMessage(QString("COS '%1' created ").arg(name));
    emit geoDatabaseModified();
}
//***********************************************************************************************
/*! Reads a stl file*/
void geomodel::readSTL(const QString &file)
{
    stltriangle tempStlTria;
    int cellType=0;
    vtkstd::string vtkfile = file.toStdString();

    //get all data from the file
    vtkSTLReader* reader = vtkSTLReader::New();

    reader->SetFileName(vtkfile.c_str());
    reader->Update();
    emit sendMessage("file loaded!");
    vtkPolyData* polyData = reader->GetOutput();

    vtkIdType numPoints = polyData->GetNumberOfPoints();
    vtkIdType numPolys = polyData->GetNumberOfPolys();
    vtkIdType numCells = polyData->GetNumberOfCells();
    double* point;


    for (long p=0;p<numPoints;p++)
    {
        point = polyData->GetPoint(p);
        createPoint(point[0],point[1],point[2],1);
    }
    emit sendMessage(QString("Number of polys %1 %2").arg(numPolys).arg(numCells));
    for (long t=0;t<numCells;t++)
    {
        vtkCell* cell = polyData->GetCell(t);

        //int numberOfNodes = cell->GetNumberOfPoints();
       // emit sendMessage(QString(" Points: %1").arg(numberOfNodes));
        cellType= cell->GetCellType();
        vtkIdList* pointIds = cell->GetPointIds();
        for (int c=0;c<3;c++)
        {
                tempStlTria.pointList[c]=pointIds->GetId(c)+1;

        }
        stlTriaList.append(tempStlTria);
   }
    vector3d maxcoord=getMaxCoords();
    modelRadius=maxcoord.norm();
    emit geoDatabaseModified();
}
//***********************************************************************************************
/*! Creates a stl triangle file*/
void geomodel::createSTLTriangle(long p1,long p2,long p3)
{
}
//*************************************************************************************************
/*! Checks the geometry model for the max coords */
vector3d geomodel::getMaxCoords()
{
    vector3d coord;
    vector3d maxCoords;

    maxCoords[0]=1;
    maxCoords[1]=1;
    maxCoords[2]=1;

    for (long int i=0; i< pointList.size(); ++i)
    {
        coord=pointList[i].getCoordinates();

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
//***********************************************************************************
//***********************************************************************************
/*! Create a new surface*/
void geomodel::createSurface(long label, QList<long> edges)
{
    QList<long> points;
    int direction=0;
    emit sendMessage(QString("surface Label: %1").arg(label));
    surface tempSurface;
    long surfaceIndex;
    //Check wether a surface with the given label already exists
    if (surfaceList.size()==0)
    {
        surfaceIndex=0;
    }
    else
    {
        surfaceIndex=getSurfaceIndex(label);
        emit sendMessage(QString("surface Index: %1").arg(surfaceIndex));
        if (surfaceIndex > -1)
        {
           emit sendMessage(QString("Surface %1 already exitst! Surface creation cancelled!").arg(label));
           return;
        }
    }
    emit sendMessage(QString("Number of edges %1").arg(edges.size()));

    //check wether the lines exist!
    //Otherwise don't create the surface
    //If the curvelist ist empty the first edge cant exist
    if (curveList.size()< 1)
    {
        emit sendMessage(QString("Edge 1 (Line %1) does not exist. Surface not created").arg(edges[0]));
        return;
    }
    for (int g=0;g<edges.size();g++)
    {
        if (getLineIndex(edges[g])<0)
        {
            emit sendMessage(QString("Edge %1 (Line %2) does not exist. Surface not created").arg(g+1).arg(edges[g]));
            return;
        }
    }
    if (! closedLoop(edges))
    {
        emit sendMessage(QString("Edges do not form a closed loop. Surface not created"));
        return;
    }
    tempSurface.setLabel(label);
    tempSurface.setEdges(edges);
    //Create a sorted list of the corner points
    points.clear();
    points.append(curveList[getLineIndex(edges[0])].getStartPoint());
    points.append(curveList[getLineIndex(edges[0])].getEndPoint());
    //Depending on the direction of the first curve the second or last curve is next
    if (curveList[getLineIndex(edges[0])].getEndPoint()==curveList[getLineIndex(edges[1])].getEndPoint() ||
        curveList[getLineIndex(edges[0])].getEndPoint()==curveList[getLineIndex(edges[1])].getStartPoint())
    {
        direction=0;
    }
    else
    {
        direction=1;
    }
    //*******************************************************************************
    if (direction==0)
    {
        for (int p=1;p<(edges.size());p++)
        {
            if (curveList[getLineIndex(edges[p])].getStartPoint()==points.last())
            {
                points.append(curveList[getLineIndex(edges[p])].getEndPoint());
            }
            else
            {
                points.append(curveList[getLineIndex(edges[p])].getStartPoint());
            }
        }

    }
    else
    {
        for (int p=edges.size()-1;p>0;p--)
        {
            if (curveList[getLineIndex(edges[p])].getStartPoint()==points.last())
            {
                points.append(curveList[getLineIndex(edges[p])].getEndPoint());
            }
            else
            {
                points.append(curveList[getLineIndex(edges[p])].getStartPoint());
            }
        }
    }
    tempSurface.points=points;
    //*******************************************************************************
    if (surfaceList.size()==0)
    {
       surfaceList.append(tempSurface);
    }
    else
    {
        if (label > surfaceList.last().getLabel())
            surfaceList.append(tempSurface);
        else
            surfaceList.insert(surfaceIndex*-1,tempSurface);
    }

    emit sendMessage(QString("Surface %1 created ").arg(label));
    emit geoDatabaseModified();
}
///////////////////////////////////////////////////////////////////////
/*! Check wether a given line sets formsa closed loop*/
bool geomodel::closedLoop(QList<long> edges)
{
    long index1,index2;
    int nEdges=edges.size();
    // if less than 3 edges are in the list they cant form a loop
    if (nEdges<3)
        return(false);
    //The edges form a closed loop if every endpoint is used by two lines
    for (int l=0;l<(nEdges-1);l++)
    {
        if (edges[l]==edges[l+1])
            return(false);
        index1=getLineIndex(edges[l]);
        index2=getLineIndex(edges[l+1]);
        if (! curveList[index1].connectedTo(curveList[index2]))
            return(false);

    }
    //The last check is for the last edge with the first edge
    index1=getLineIndex(edges[edges.size()-1]);
    index2=getLineIndex(edges[0]);
    if (! curveList[index1].connectedTo(curveList[index2]))
        return(false);

    return(true);
}
//*******************************************************************
void geomodel::createPointArcCenter(long label,vector3d p1,vector3d p2,vector3d p3)
{
    vector3d a=p2-p1;
    vector3d b=p3-p2;

    vector3d n=cross(a,b);



}
