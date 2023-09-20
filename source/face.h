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
#ifndef FACE_H
 #define FACE_H

 #include "node.h"
 
//!   Face data 
/*! 
	The face class holds all relevant attributes
	for the face. 
 */
    class face 
    {

    public:
           face();
           ~face();
	   long parentElement;
       int parentElementDimension;
	   int type;
	   int faceNumber;
	   bool outside;
       long nodeList[10];
	   long orderedNodeList[10];
	   vector3d normal;
       bool operator==(const face &f1) const;
       bool operator<(const face &f1) const;
       bool operator>(const face &f1) const;
       bool operator>=(const face &f1) const;
	   vector3d centroid;
	   vector3d centroidDisp;
	   double centroidValue;
	   float centroidColor;
	   void sortNodes();
	
	
	private:
		void calcArea();
		
		
    };
 
#endif
