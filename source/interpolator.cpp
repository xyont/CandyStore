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
 #include "interpolator.h"
 /*!Standrad Constructor*/
 interpolator::interpolator()
 {
 ;
 }
 
 //*****************************************************
 /*!Writes the interpolation information*/
QString interpolator::getInformation()
{
QString info;

        info=QString("%1 , %2 ,%3 , %4 , %5 , %6 ,%7").arg(newNode).arg(resultElIndex).arg(resultElType).arg(parameter[0]).arg(parameter[1]).arg(parameter[2]).arg(parameter[3]);
	return(info);
}
//*******************************************************
/*!Sets the interpolation information*/
void interpolator::setData(long newnode, long resultel,int eltype, double *para)
{
	newNode=newnode;
	resultElIndex=resultel;
	resultElType=eltype;
        parameter[0]=para[0];
	parameter[1]=para[1];
	parameter[2]=para[2];
	parameter[3]=para[3];
}
