/***************************************************************************
 *   Copyright (C) 2009 by Oliver Borm   *
 *   oli.borm@web.de   *
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
#ifndef VTKREADER_H
#define VTKREADER_H


#include <QString>
//#include <vtkCellArray.h>
#include <vtkCell.h>
#include <vtkIdList.h>
//#include <vtkPoints.h>
#include <vtkPointData.h>

//#include <vtkPolyData.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
// #include <vtkStructuredGridSource.h>

//#include <vtkPLOT3DReader.h>
//#include <vtkXMLPolyDataReader.h>
#include <vtkXMLMultiBlockDataReader.h>
#include <vtkXMLStructuredDataReader.h>
#include <vtkXMLUnstructuredDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>

using namespace std;

/**
	@author Oliver Borm <oli.borm@web.de>
*/
class vtkreader{
private:

	//! verbose modus
//	int verbose;
	//! block dimensions
// 	int* dimensions;
	//! number of arrays
// 	int noArray;

public:

    vtkreader();

    void readXMLUnstructuredData();

    //void readMultiBlockPLOT3D();

    //void readPLOT3D();

// 	int* getDimensions(){return dimensions;}

// 	int getNoArray(){return noArray;}

	//string fileName;

	//string solutionFileName;

	//string fieldName;

//	v4Double	coordinatesXYZ;

//	v4Double	functionData;


    ~vtkreader();

};

#endif
