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
#include "edge.h"

edge::edge()
{
	faces[0]=-1;
	faces[1]=-1;
	angle=0;
}

bool edge::operator==(const edge &e1) 
{
	if (e1.type!=this->type)
	{
		return(false);
	}
	else
	{
		for (int n=0; n < e1.type; n++)
		{
			if (e1.orderedNodeList[n]!=this->orderedNodeList[n])
			{
				return(false);
			}
			
		}
	}
	return(true);
}

void edge::sortNodes()
{
for (int i=0;i<type;i++)
	{
		orderedNodeList[i]=nodeList[i];
	}
	//Sortieren der Nodes  mit Bubblesort
	unsigned long rechts=type,sortierungen,x;
	unsigned long w;
	
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


bool edge::operator<(const edge &e1) 
{
		
	if (this->type < e1.type)
	{
		return(true);
	}
	
	else
	{
		for (int k=0;k<type;k++)
		{
			if(orderedNodeList[k]<e1.orderedNodeList[k])
				return(true);
			else
			{
				if(orderedNodeList[k]>e1.orderedNodeList[k])
				return(false);
			}
			
			
			
		}
	}
	return (false);
}

bool edge::operator>(const edge &e1) 
{
		
	if (this->type > e1.type)
	{
		return(true);
	}
	
	else
	{
		for (int k=0;k<type;k++)
		{
			if(orderedNodeList[k]>e1.orderedNodeList[k])
				return(true);
			else
			{
				if(orderedNodeList[k]<e1.orderedNodeList[k])
				return(false);
			}
			
			
			
		}
	}
	return (false);
}