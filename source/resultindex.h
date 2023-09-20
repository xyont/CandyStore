/***************************************************************************
 *   Copyright (C) 2006 by Patrick Prokopczuk   *
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
 * \file resultindex.h
 * \brief A brief file description
 **/

 #ifndef RESULTINDEX_H
 #define RESULTINDEX_H

 #include <QtGui>  
 //!   Result Index class  
/*! 
	The result index class is used to hold basic information about an available result.
	(step, type, time or frequenzy, position in the result file) 
 */
    class resultindex
    {

    public:
       int resultStep;
       int resultType;
       qint64 resultPosition;
       float frqtime;
    private:

    };

    #endif

