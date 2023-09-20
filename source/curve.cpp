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
 * \file curve.cpp
 * \brief A brief file description
 **/
#include "curve.h"
//*****************************************************************************************
/*! Standard Constructor creates a empty curve*/
curve::curve()
{
	label=0;
	startPoint=0;
	endPoint=0;
	division=1;
}
//*****************************************************************************************
/*! Constructor creates a curve with label l from point s to point e*/
curve::curve(long l,long s,long e)
{
	label=l;
	startPoint=s;
	endPoint=e;
	division=1;
}
//*****************************************************************************************
/*! Sets the label of the curve to label*/
void curve::setLabel(long l)
{
	label=l;
}
//*****************************************************************************************
/*! Returns the label of the curve */
long curve::getLabel()
{
	return(label);
}
//*****************************************************************************************
/*! Sets the division of the curve*/
void curve::setDivision(long l)
{
	label=l;
}
//*****************************************************************************************
/*! Returns the division of the curve */
long curve::getDivision()
{
	return(division);
}
//*****************************************************************************************
/*! Sets the start point of the curve*/
void curve::setStartPoint(long l)
{
	startPoint=l;
}
//*****************************************************************************************
/*! Returns the start point of the curve */
long curve::getStartPoint()
{
	return(startPoint);
}
//*****************************************************************************************
/*! Sets the end point of the curve*/
void curve::setEndPoint(long l)
{
	endPoint=l;
}
//*****************************************************************************************
/*! Returns the end point of the curve */
long curve::getEndPoint()
{
	return(endPoint);
}
//*****************************************************************************************
/*! Sets the start and end point of the curve*/
void curve::setPoints(long s,long e)
{
	startPoint=s;
	endPoint=e;
}
//*****************************************************************************************
/*! Reverses the line direction (Changes start and endpoint */
void curve::reverse()
{
    long temp=startPoint;
    startPoint=endPoint;
    endPoint=temp;
}
//*****************************************************************************************
/*! Checks wether two lines are connetced*/
bool curve::connectedTo(curve &line)
{
    //Check wether the startpoint or the endpoint is identical to the startpoint or endpoint of the other curve
    if (getStartPoint()==line.getStartPoint() || getStartPoint()==line.getEndPoint() )
        return(true);
    if (getEndPoint()==line.getStartPoint() || getEndPoint()==line.getEndPoint() )
        return(true);
    ////////////////
    return(false);

}
//********************************************************
/*! Return the length of the curve*/
double curve::length()
{
    return ((ePoint->getCoordinates()-sPoint->getCoordinates()).norm());
}
