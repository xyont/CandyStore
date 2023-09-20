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
 * \file node.cpp
 * \brief A brief file description
 **/
#include "node.h"
//*****************************************************************************************
/*! Standard Constructor creates a node at the global origin*/
node::node()
	:point()
{
	clearDisp();
	clearResultValue();
	clearNormalizedResultValue();
	setOutside(false);
	adjacentElements.clear();
}
//*****************************************************************************************
/*! Sets the displacement vector */
void node::setDisplacements(vector3d disp)
{
	displacements.x=disp.x;
	displacements.y=disp.y;
	displacements.z=disp.z;
}
//*****************************************************************************************
/*! Sets the displacement vector */
void node::setDisplacements(double x, double y, double z)
{
	displacements.x=x;
	displacements.y=y;
	displacements.z=z;
}
//*****************************************************************************************
/*! Sets the displacement value of Degree of Freedem dof to value */
void node::setDisplacements(int dof, double value)
{
	switch (dof)
	{
	case 1:
		displacements.x=value;
	break;
	case 2:
		displacements.y=value;
	break;
	case 3:
		displacements.z=value;
	break;
	}
}
//********************************************************************************************
/*! Sets the displacement vector of the node to 0 */
void node::clearDisp()
{
	displacements.x=0.;
	displacements.y=0.;
	displacements.z=0.;
}
//********************************************************************************************
/*! Returns the  displacement vector of the node  */
vector3d node::getDisplacements()
{
	return(displacements);
}
//********************************************************************************************
/*! Returns the  result value of the node  */
double node::getResultValue()
{
	return(resultValue);
}
//*****************************************************************************************
/*! Sets the result value of the node to value  */
void node::setResultValue(double value)
{
	resultValue=value;
}
//*****************************************************************************************
/*! Sets the result value of the node to 0  */
void node::clearResultValue()
{
	resultValue=0.;
}
//********************************************************************************************
/*! Returns the normalized result value of the node  */
float node::getNormalizedResultValue()
{
	return(normalizedResultValue);
}
//*****************************************************************************************
/*! Sets the normalized result value of the node to value  */
void node::setNormalizedResultValue(float value)
{
	normalizedResultValue=value;
}
//*****************************************************************************************
/*! Sets the normalized result value of the node to 0  */
void node::clearNormalizedResultValue()
{
	normalizedResultValue=0.;
}
//*****************************************************************************************
/*! Sets the "outside flag" to true or false  */
void node::setOutside(bool yn)
{
	outside=yn;
}

//*****************************************************************************************
/*! Returns the value of the "outside flag" (true or false)  */
bool node::isOutside()
{
	return(outside);
}
