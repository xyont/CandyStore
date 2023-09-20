/***************************************************************************
 *   Copyright (C) 2010 by Patrick Prokopczuk   *
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
 * \file writeVTK.cpp
 **/
#include <QFile>
#include <QMessageBox>
#include "femodel.h"
#include "element.h"
#include "node.h"

#include <vtkCell.h>
#include <vtkTriangle.h>
#include <vtkTetra.h>
#include <vtkHexahedron.h>
#include <vtkQuadraticTetra.h>
#include <vtkQuadraticHexahedron.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkDataArrayCollection.h>
#include <vtkDoubleArray.h>



/*!Writing model data in VTK format*/
void femodel::writeVTK(const QString &fileName)
{
    int numComp=1;
    bool nodesAvailable=false;
    bool elAvailable=false;
    //cellLocations will not be used, bt initialized for having a valid pointer address
    vtkIdTypeArray* cellLocations = vtkIdTypeArray::New();
    vtkUnsignedCharArray* cellTypes = vtkUnsignedCharArray::New();


     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text))
     {

         return;
        }
    //Check wether nodes exist
     if (nodeList.size()>0)
     {
         nodesAvailable=true;
         // Check wether elements exist
         if (elementList.size()>0)
         {
             elAvailable=true;
         }
     }

    //Initialize the vtk Point Array
    vtkPoints* points = vtkPoints::New();

    //Put all nodes in the array
        for (long n=0; n<nodeList.size();n++)
        {
            points->InsertNextPoint(nodeList[n].getCoordinate1(), nodeList[n].getCoordinate2(), nodeList[n].getCoordinate3());
        }

    //Initialize a pointer for each supported element type

    vtkHexahedron* hex  = vtkHexahedron::New();
    vtkQuadraticHexahedron* hex20  = vtkQuadraticHexahedron::New();
    vtkTetra* tet = vtkTetra::New();
    vtkQuadraticTetra* tet10 = vtkQuadraticTetra::New();
    vtkTriangle* tria3 = vtkTriangle::New();


    //Initialize the vtk Cell Array
    vtkCellArray* cellArray = vtkCellArray::New();

    // Put all supported elements in the cell array and the element type in the celltype array

    for (long e=0;e<elementList.size();e++)
    {
        switch (elementList[e].type)
        {
        //Hex8
        case 1:
            for (int k=0;k<8;k++)
            {
                hex->GetPointIds()->SetId(k,getNodeIndex(elementList[e].nodeList[k]));
            }
            cellArray->InsertNextCell(hex);
            cellTypes->InsertNextValue(12);
        break;
        //Tet4
        case 3:
            for (int k=0;k<4;k++)
            {
                tet->GetPointIds()->SetId(k,getNodeIndex(elementList[e].nodeList[k]));
            }
            cellArray->InsertNextCell(tet);
            cellTypes->InsertNextValue(10);
        break;
        //Hex20
        case 4:
            for (int k=0;k<20;k++)
            {
                hex20->GetPointIds()->SetId(k,getNodeIndex(elementList[e].nodeList[k]));
            }
            cellArray->InsertNextCell(hex20);
            cellTypes->InsertNextValue(25);
        break;
        //Tet10
        case 6:
            for (int k=0;k<10;k++)
            {
                tet10->GetPointIds()->SetId(k,getNodeIndex(elementList[e].nodeList[k]));
            }
            cellArray->InsertNextCell(tet10);
            cellTypes->InsertNextValue(24);
        break;
        //Tria3
        case 7:
        for (int k=0;k<3;k++)
            {
                tria3->GetPointIds()->SetId(k,getNodeIndex(elementList[e].nodeList[k]));
            }
            cellArray->InsertNextCell(tria3);
            cellTypes->InsertNextValue(5);
        break;
        //All unsupported types
        default:
        ;
        //do nothing
        }
    }
    //Initialize the unstructured grid object
    vtkUnstructuredGrid* unstructuredGrid = vtkUnstructuredGrid::New();

    //put points and cells to the grid (if there are any)
    if (nodesAvailable)
    {
        unstructuredGrid->SetPoints(points);
        //Getting the node result values
        for (int k=0;k<resultList.size();k++)
        {
            emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
            readfrdresults(resultList[k].resultStep);
            emit sendMessage(QString("Result Datalist %1").arg(resultDataList.size()));

            for (int r=0;r<resultDataList.size();r++)
            {
                //Get the number of components. Max 9 components are used
                numComp=resultDataList[r].numberComponents;
                if (numComp>9)
                {
                    numComp=9;
                }
                emit sendMessage(QString("Exporting %1 results").arg(resultDataList[r].resultLabel));
                emit sendMessage(QString("%1 components").arg(numComp));
                //Initialize a new data array
                vtkDoubleArray* dataArray = vtkDoubleArray::New();
                dataArray->SetName(resultDataList[r].resultLabel.toAscii());
                //Putting the result data into vtk data arrays and adding them to the point data
                switch (resultDataList[r].numberComponents)
                {
                case 1:
                    dataArray->SetNumberOfComponents(1);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple1(resultDataList[r].resultValueList[j].resultValues[0]);
                    }
                    unstructuredGrid->GetPointData()->AddArray(dataArray);

                break;
                case 2:
                    dataArray->SetNumberOfComponents(2);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple2(resultDataList[r].resultValueList[j].resultValues[0],resultDataList[r].resultValueList[j].resultValues[1]);
                    }
                    unstructuredGrid->GetPointData()->AddArray(dataArray);

                break;
                case 3:
                    dataArray->SetNumberOfComponents(3);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple3(resultDataList[r].resultValueList[j].resultValues[0],
                                                   resultDataList[r].resultValueList[j].resultValues[1],
                                                   resultDataList[r].resultValueList[j].resultValues[2]);

                    }
                     unstructuredGrid->GetPointData()->AddArray(dataArray);

                break;
                case 4:
                    //Suppose this is a vector and scalar value (magnitude)
                    dataArray->SetNumberOfComponents(3);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple3(resultDataList[r].resultValueList[j].resultValues[0],
                                                   resultDataList[r].resultValueList[j].resultValues[1],
                                                   resultDataList[r].resultValueList[j].resultValues[2]);

                    }
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                    //Create a new array for the magnitude
                    dataArray = vtkDoubleArray::New();
                    dataArray->SetNumberOfComponents(1);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple1(resultDataList[r].resultValueList[j].resultValues[3]);
                    }
                    dataArray->SetName((resultDataList[r].resultLabel + "_MAG").toAscii());
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                break;
                case 5:
                    //Suppose this is a vector and 2 scalar values
                    dataArray->SetNumberOfComponents(3);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple3(resultDataList[r].resultValueList[j].resultValues[0],
                                                   resultDataList[r].resultValueList[j].resultValues[1],
                                                   resultDataList[r].resultValueList[j].resultValues[2]);

                    }
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                    //Create a new array for the first scalar value
                    dataArray = vtkDoubleArray::New();
                    dataArray->SetNumberOfComponents(1);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple1(resultDataList[r].resultValueList[j].resultValues[3]);
                    }
                    dataArray->SetName((resultDataList[r].resultLabel + "_"+resultDataList[r].componentLabels[3] ).toAscii());
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                    //Create a new array for the second scalar value
                    dataArray = vtkDoubleArray::New();
                    dataArray->SetNumberOfComponents(1);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple1(resultDataList[r].resultValueList[j].resultValues[4]);
                    }
                    dataArray->SetName((resultDataList[r].resultLabel + "_"+resultDataList[r].componentLabels[4]).toAscii());
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                    break;
                    default:
                    //6 and more components -> suppose this is a symmetric tensor with additional scalar values
                    //In ccx the stress tensor is written as sxx,syy,szz,sxy,syz,sxz
                    dataArray->SetNumberOfComponents(9);
                    for (long j=0;j<nodeList.size();j++)
                    {
                       dataArray->InsertNextTuple9(resultDataList[r].resultValueList[j].resultValues[0],
                                                   resultDataList[r].resultValueList[j].resultValues[3],
                                                   resultDataList[r].resultValueList[j].resultValues[5],
                                                   resultDataList[r].resultValueList[j].resultValues[3],
                                                   resultDataList[r].resultValueList[j].resultValues[1],
                                                   resultDataList[r].resultValueList[j].resultValues[4],
                                                   resultDataList[r].resultValueList[j].resultValues[5],
                                                   resultDataList[r].resultValueList[j].resultValues[4],
                                                   resultDataList[r].resultValueList[j].resultValues[2]
                                                   );

                    }
                    unstructuredGrid->GetPointData()->AddArray(dataArray);
                    //Create a new array for the first scalar value
                    for (int k=6;k<numComp;k++)
                    {
                        dataArray = vtkDoubleArray::New();
                        dataArray->SetNumberOfComponents(1);
                        for (long j=0;j<nodeList.size();j++)
                        {
                           dataArray->InsertNextTuple1(resultDataList[r].resultValueList[j].resultValues[k]);
                        }
                        dataArray->SetName((resultDataList[r].resultLabel + "_"+resultDataList[r].componentLabels[k] ).toAscii());
                        unstructuredGrid->GetPointData()->AddArray(dataArray);
                    }

                }
            }
        }


        if (elAvailable)
        {
            unstructuredGrid->SetCells(cellTypes,cellLocations, cellArray);
        }

    }
 // Write file
    vtkstd::string vtkfile = fileName.toStdString();
    //vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
    vtkXMLUnstructuredGridWriter* writer = vtkXMLUnstructuredGridWriter::New();
    writer->SetFileName(vtkfile.c_str());
    writer->SetInput(unstructuredGrid);
    writer->Write();
    emit sendMessage(QString("VTK file written to %1").arg(fileName));
}
