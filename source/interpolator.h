/***************************************************************************
 *   Copyright (C) 2009 by Patrick Prokopczuk   *
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

# ifndef INTERPOLATOR_H
# define INTERPOLATOR_H


 #include <qgl.h>
#include <math.h>
//!   Class with interpolation information 
/*! 
	The interpolator class holds information for interpolation of results to a new node
 */
class interpolator
{
	 public:
	   interpolator();
	   long newNode;
	   long resultElIndex;
	   int resultElType;
           //QList<long> resultNodes;
	   double parameter[4];
		QString getInformation();
		void setData(long,long,int,double*);
	//	vector3d interpolate(vector3d);
};
#endif
