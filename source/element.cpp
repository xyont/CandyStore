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
 * \file element.cpp
 * \brief Methods for element class
 **/
#include "element.h"
//*****************************************************************************************
/*! Standard Constructor creates a dummy element*/
element::element()
{
	label=0;
        outside=0;
	nodeList.clear();
	
}
//*************************************************************************
/*! Returns the label of the element*/
long element::getLabel()
{
	return(label);
}
//*************************************************
/*! Returns the vertice number of a node. Numbering strats with 0!! Returns -1 if the node does not exost in the element*/
int element::getVerticeNumber(long nodeLabel)
{
	for (int i=0; i<nodeList.size(); i++)
	{
		if (nodeList[i]==nodeLabel)
		{
			return(i);
		}
	}
	return(-1);
}
