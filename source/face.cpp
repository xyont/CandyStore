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
 * \file face.cpp
 * \brief A brief file description
 **/
 
#include "face.h"

/*!StandardConstructor*/
face::face()
{

}
face::~face()
{

}
//****************************************************
/*!SOrting the nodes in ascending order and store them in the orderedNodeList*/
void face::sortNodes()
{
    long sortierungen,x,rechts,w;
    //Copying Nodes into the orderedNodeList
    for (int i=0;i<type;i++)
    {
		orderedNodeList[i]=nodeList[i];
    }
    //Sorting the nodes with Bubblesort
    rechts=type;
    do
    {
        rechts--;
        sortierungen=0;
        for (x=0;x<rechts;x++)
        {
            if (orderedNodeList[x]>orderedNodeList[x+1])
            {
                w=orderedNodeList[x];
                orderedNodeList[x]=orderedNodeList[x+1];
                orderedNodeList[x+1]=w;
                sortierungen++;
            }
        }
    } while(sortierungen&&(rechts>1));
}
//**************************************************************************************************************
/*! 
== Operator - 
 Two Faces are equal when the number and labels of the vertices are equal 
*/  
bool face::operator==(const face &f1) const
{	
    if (f1.type!=this->type)
    {
        return(false);
    }
    else
    {
    for (int k=0;k<type;k++)
        {
            if(orderedNodeList[k]!=f1.orderedNodeList[k])
                return(false);
        }
    }
    return(true);
}
//**************************************************************************************************************
/*! 
< Operator
A face is smaller than another when 1. its type is smiller, the nodes in the ordered node list are smaller
*/  
bool face::operator<(const face &f1) const
{		
    if (this->type < f1.type)
    {
        return(true);
    }
    else
    {
        for (int k=0;k<type;k++)
        {
            if(orderedNodeList[k]<f1.orderedNodeList[k])
                return(true);
            else
            {
                if(orderedNodeList[k]>f1.orderedNodeList[k])
                    return(false);
            }
        }
    }
    return (false);
}
//**************************************************************************************************************
/*! 
> Operator - 

*/  
bool face::operator>(const face &f1) const
{	
    if (this->type > f1.type)
    {
        return(true);
    }
    else
    {
        for (int k=0;k<type;k++)
        {
            if(orderedNodeList[k]>f1.orderedNodeList[k])
                return(true);
            else
            {
            if(orderedNodeList[k]<f1.orderedNodeList[k])
                return(false);
            }
        }
    }
    return (false);
}
//*********************************************************************************
/*!
>= Operator

*/
bool face::operator>=(const face &f1) const
{

        if (this->type > f1.type)
        {
                return(true);
        }
        else
        {
                for (int k=0;k<type;k++)
                {
                        if(orderedNodeList[k]>f1.orderedNodeList[k])
                                return(true);
                        else
                        {
                                if(orderedNodeList[k]<f1.orderedNodeList[k])
                                return(false);
                        }
                }
        }
        return (true);
}
//**************************************************************************************************************
/*! 
	Calculates the area of a face by triangulation
 */
void face::calcArea()
{
        //area calculation must be performed in femodel!! because face dooes not know the ccordinates!
}
//**************************************************************************************************************


