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
#include "vtkreader.h"

vtkreader::vtkreader()
{
//verbose = 2;

//fieldName = "Pressure";
// noArray = 0;
//fileName = "";
//solutionFileName = "";

}

// vtkXMLMultiBlockDataReader

void vtkreader::readXMLUnstructuredData(){
	//get all data from the file
    vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();
	
      /*  reader->SetFileName(fileName);
	reader->Update();
	
	vtkUnstructuredGrid* unstructGrid = reader->GetOutput();
	
	vtkIdType numPoints = unstructGrid->GetNumberOfPoints();
	vtkIdType numCells  = unstructGrid->GetNumberOfCells();
	
	//read in all of the points
	double* point;
	for(long i = 0; i < numPoints; i++)
	{
		point = unstructGrid->GetPoint(i);
        //	tempNode.setCoordinates(point);
        //	nodeList.append(tempNode);
	}
	
	for(long c = 0; c < numCells; c++)
	{
                vtkCell* cell = unstructGrid->GetCell(c);
                int numberOfCellIds = cell->GetNumberOfIds();
                vtkIdList pointIds = cell->GetPointIds();
		int cellType = cell->GetCellType();
		
        //	tempElement.setlabel(c+1);
		//If abfrage für element type
        //	tempElement.setType(1);
		
//		for (long p =0;p<numberOfCellIds;p++)
//		{
        //		tempElement.nodeList.append(pointIds[p]);
//		}
        //	elementList.append(tempElement);
	}
        */

}

/*
void vtkreader::readPLOT3D(){
	vtkPLOT3DReader* pl3d = vtkPLOT3DReader::New();
	pl3d->BinaryFileOff();
// 	pl3d->MultiGridOn();
	pl3d->SetByteOrderToLittleEndian();
	pl3d->SetXYZFileName(fileName.c_str());
	pl3d->SetFunctionFileName(solutionFileName.c_str());
	pl3d->Update();

	vtkStructuredGrid* structGrid = pl3d->GetOutput();

// anzahlZellen = structGrid->GetNumberOfCells()
// anzahlPunkte = structGrid->GetNumberOfPoints()
	int* dimensions = structGrid->GetDimensions();

	if( verbose > 1 ) cout << dimensions[0] << "\t" <<  dimensions[1] << "\t" << dimensions[2] << "\n";

//	punkte = structGrid.GetPoints()

	vtkPointData* pointData = structGrid->GetPointData();
	int noArray = pointData->GetNumberOfArrays();

	// Liest nur das 1. Array ein!
	vtkDataArray* array = pointData->GetArray(0);
// 	if( solutionFileName != "" ) array->SetName(fieldName.c_str());

	// Initialisierung des vector Containers "coordinatesXYZ" (4D)
// 	coordinatesXYZ.resize ( 3, vector< vector< vector<double> > > ( dimensions[0], vector< vector<double> > ( dimensions[1] , vector<double> ( dimensions[2] ) ) ) );
// 	coordinatesXYZ.resize ( 3, v3Double ( dimensions[0], v2Double ( dimensions[1] , v1Double ( dimensions[2] ) ) ) );
// 	functionData.resize ( anzahlArrays, v3Double ( dimensions[0], v2Double ( dimensions[1] , v1Double ( dimensions[2] ) ) ) );

// 	Initialisierung des valarray Containers "coordinatesXYZ" (4D)
	coordinatesXYZ.resize( 3, v3Double ( v2Double (  v1Double ( dimensions[2] ) , dimensions[1] ) , dimensions[0] ) );
	functionData.resize( noArray , v3Double ( v2Double (  v1Double ( dimensions[2] ) , dimensions[1] ) , dimensions[0] ) );

	int n;
	for ( int jj=0;jj< noArray;jj++ )
	{
		array = pointData->GetArray(jj);
		n=0;
		for ( int k=0; k< dimensions[2]; k++ )
		{
			for ( int j=0;j< dimensions[1];j++ )
			{
				for ( int i=0;i< dimensions[0];i++ )
				{
					functionData[jj][i][j][k] = array->GetTuple1(n);
					n += 1;
				}
			}
		}
	}

	n=0;
	double* point;
	for ( int k=0; k< dimensions[2]; k++ )
	{
		for ( int j=0;j< dimensions[1];j++ )
		{
			for ( int i=0;i< dimensions[0];i++ )
			{
				point = structGrid->GetPoint(n);
				if( verbose > 2 ) cout << point[0] << "\t" <<  point[1] << "\t" << point[2] << "\n";

				for ( int kk=0; kk< 3; kk++ )
				{
					coordinatesXYZ[kk][i][j][k] = point[kk];
				}
				n += 1;
			}
		}
	}
}
*/
/*
void vtkreader::readMultiBlockPLOT3D(){
	cout << "Not yet implemented!"<< endl;

}
*/
/*
void vtkreader::readXMLPolyData(){
	//get all data from the file
	vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
	reader->SetFileName(fileName.c_str());
	reader->Update();
	vtkPolyData* polydata = reader->GetOutput();

	//get the number of points the file contains
	vtkIdType NumPoints = polydata->GetNumberOfPoints();

	//if there are no points, quit
	if(!(NumPoints > 0) )
	{
		exit(-1);
	}

	//read in all of the points
	std::vector<Point> Points;
	double point[3];
	for(vtkIdType i = 0; i < NumPoints; i++)
	{
		polydata->GetPoint(i, point);
		Points.push_back(Point(point[0], point[1], point[2]));
	}

	//get the triangles (if there are any)
	std::vector<std::vector<int> > VertexLists;
	vtkIdType NumPolys = polydata->GetNumberOfPolys();
	if(NumPolys > 0)
	{
		vtkCellArray* TriangleCells = polydata->GetPolys();
		vtkIdType npts;
		vtkIdType *pts;

		while(TriangleCells->GetNextCell(npts, pts))
		{
			std::vector<int> List(3);
			List[0] = pts[0];
			List[1] = pts[1];
			List[2] = pts[2];

			VertexLists.push_back(List);
		}
	}

	std::cout << "Points: " << Points.size() << std::endl;
	for(unsigned int i = 0; i < Points.size(); i++)
	{
		std::cout << Points[i].x << " " << Points[i].y << " " << Points[i].z << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Triangles: " << VertexLists.size() << std::endl;
	for(unsigned int i = 0; i < VertexLists.size(); i++)
	{
		std::cout << VertexLists[i][0] << " " << VertexLists[i][1] << " " << VertexLists[i][2] << std::endl;
	}

}
*/

vtkreader::~vtkreader()
{
}


