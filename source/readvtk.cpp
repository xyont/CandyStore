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
 * \file readvtk.cpp
 **/
#include <QtGui>
#include <QFile>
#include "femodel.h"
#include "element.h"
#include "node.h"
//******************************************************************************************
/*!Import an unstructured vtk file */

void femodel::importVtkMultiblock(const QString &fileName)
{
    //long numberBlocks,numberBlocks2;
    emit sendMessage(QString("Importing Vtk Multiblock grid data from %1").arg(fileName));
    vtkAppendFilter* append = vtkAppendFilter::New();
    clearDatabase();
    vtkstd::string vtkfile = fileName.toStdString();

    //get all data from the file
    vtkXMLMultiBlockDataReader* reader = vtkXMLMultiBlockDataReader::New();

    reader->SetFileName(vtkfile.c_str());
    reader->Update();
    vtkDataObject* input = vtkDataObject::New();
    input=reader->GetOutput();
    if (input->IsA("vtkMultiBlockDataSet"))
    {
        vtkMultiBlockDataSet* mbinput=vtkMultiBlockDataSet::New();
        mbinput=vtkMultiBlockDataSet::SafeDownCast(input);

        getMultiblockData(append,mbinput);

        append->Update();
    }
    else
    {
        emit sendMessage(QString(tr("File does not contain a vtk multiblock. Import canceled!")));
        return;
    }

        vtkUnstructuredGrid* unstructGrid = append->GetOutput();

      	readVtkMeshFromUnstructuredGrid(unstructGrid);

    emit sendMessage(QString(" Vtk Multiblock Mesh imoprted from %1").arg(fileName));
    openFile=fileName;
    fileflag=3;
}
//******************************************************************************************
/*!Import an unstructured vtk file */

void femodel::importVtkUnstructured(const QString &fileName)
{
    emit sendMessage(QString("Importing Vtk  unstructured grid data from %1").arg(fileName));

    clearDatabase();
    vtkstd::string vtkfile = fileName.toStdString();

    //get all data from the file
    vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();

    reader->SetFileName(vtkfile.c_str());
    reader->Update();

    vtkUnstructuredGrid* unstructGrid = reader->GetOutput();

    readVtkMeshFromUnstructuredGrid(unstructGrid);

    emit sendMessage(QString("Vtk Mesh imoprted from %1").arg(fileName));
    openFile=fileName;
    fileflag=2;
}
//****************************************************************
/*!Reads vtk results*/
int femodel::readVtkResults(int resultstep)
{
        //long numberBlocks,numberBlocks2;
    vtkstd::string vtkfile = openFile.toStdString();

    dispAvailable=false;
    dispRatio=0;
    ResultTitles.clear();
    ResultComponents.clear();
    resultDataList.clear();
    resultData.clearResultData();
    int numberResults=0;
    vtkUnstructuredGrid* unstructGrid = vtkUnstructuredGrid::New();
    if (fileflag==2)
    {
        vtkXMLUnstructuredGridReader* reader = vtkXMLUnstructuredGridReader::New();

        reader->SetFileName(vtkfile.c_str());
        reader->Update();
        unstructGrid = reader->GetOutput();
    }
    else
    {
        vtkAppendFilter* append = vtkAppendFilter::New();

        //get all data from the file
        vtkXMLMultiBlockDataReader* reader = vtkXMLMultiBlockDataReader::New();

        reader->SetFileName(vtkfile.c_str());
        reader->Update();
        vtkDataObject* input = vtkDataObject::New();
        input=reader->GetOutput();

        vtkMultiBlockDataSet* mbinput=vtkMultiBlockDataSet::New();
        mbinput=vtkMultiBlockDataSet::SafeDownCast(input);

        getMultiblockData(append,mbinput);

        append->Update();

        unstructGrid = append->GetOutput();
    }


vtkPointData* pointData = unstructGrid->GetPointData();
int noArray = pointData->GetNumberOfArrays();
// emit sendMessage(QString("%1 results available").arg(noArray));

resultData.clearResultData();
tempResultIndex.frqtime=1;
tempResultIndex.resultStep = 1;
resultList.append(tempResultIndex);
double *nodeResults;
double  nodeResult;

for ( int jj=0;jj< noArray;jj++ )
{
        vtkDataArray* array = pointData->GetArray(jj);
        const char* arrayName = array->GetName();

        resultDataList.append(resultData);
        numberResults++;
        resultDataList[numberResults-1].resultLabel=QString(arrayName);

        int noComponents = array->GetNumberOfComponents();
        for (int k=1;k<=noComponents;k++)
        {
            resultDataList[numberResults-1].addComponent(QString("Comp %1").arg(k));
        }
        //emit sendMessage(QString("%1 results available with %2 components").arg(resultDataList[numberResults-1].resultLabel).arg(noComponents));

        if (nodeList.size()>0)
        {
            switch ( noComponents)
             {
             case 1:
             nodeResult= array->GetTuple1(0);

             resultDataList[numberResults-1].maxValues[0]=nodeResult;
             resultDataList[numberResults-1].minValues[0]=nodeResult;
             break;
             case 3:
              nodeResults= array->GetTuple3(0);
              for (int kk=0;kk<3;kk++)
              {
                resultDataList[numberResults-1].maxValues[kk]=nodeResults[kk];
                resultDataList[numberResults-1].minValues[kk]=nodeResults[kk];
                }
             break;
             case 4:
              nodeResults= array->GetTuple4(0);
              for (int kk=0;kk<4;kk++)
              {
                resultDataList[numberResults-1].maxValues[kk]=nodeResults[kk];
                resultDataList[numberResults-1].minValues[kk]=nodeResults[kk];
                }
             break;
             }

        }
        for ( long pp=0;pp< nodeList.size();pp++ )
        {
            tempNodeResult.nodeLabel=pp+1;
            tempNodeResult.resultValues.clear();
            switch ( noComponents)
             {
            case 1:
             nodeResult= array->GetTuple1(pp);
             tempNodeResult.resultValues.append(nodeResult);

             if (nodeResult< resultDataList[numberResults-1].minValues[0])
             {
                     resultDataList[numberResults-1].minValues[0]=nodeResult;
             }
             if (nodeResult> resultDataList[numberResults-1].maxValues[0])
             {
                     resultDataList[numberResults-1].maxValues[0]=nodeResult;
             }
            break;
             case 3:
             nodeResults= array->GetTuple3(pp);
             for (int cc=0;cc<3;cc++)
             {
                tempNodeResult.resultValues.append(nodeResults[cc]);

                if (nodeResults[cc]< resultDataList[numberResults-1].minValues[cc])
                {
                     resultDataList[numberResults-1].minValues[cc]=nodeResults[cc];
                }
                if (nodeResults[cc]> resultDataList[numberResults-1].maxValues[cc])
                {
                     resultDataList[numberResults-1].maxValues[cc]=nodeResults[cc];
                }
             }
              break;
             case 4:
             nodeResults= array->GetTuple4(pp);
             for (int cc=0;cc<4;cc++)
             {
                tempNodeResult.resultValues.append(nodeResults[cc]);

                if (nodeResults[cc]< resultDataList[numberResults-1].minValues[cc])
                {
                     resultDataList[numberResults-1].minValues[cc]=nodeResults[cc];
                }
                if (nodeResults[cc]> resultDataList[numberResults-1].maxValues[cc])
                {
                     resultDataList[numberResults-1].maxValues[cc]=nodeResults[cc];
                }
             }
              break;
            case 9:
             nodeResults= array->GetTuple9(pp);
            break;
            }
            resultDataList[numberResults-1].resultValueList.append(tempNodeResult);
        }
    }
    return(1);
}
///***********************************************
void femodel::readVtkMeshFromUnstructuredGrid(vtkUnstructuredGrid *unstructGrid)
{
    vtkIdType numPoints = unstructGrid->GetNumberOfPoints();
    vtkIdType numCells  = unstructGrid->GetNumberOfCells();
	emit sendMessage(QString("%1 grid points ").arg(numPoints));
	emit sendMessage(QString("%1 cells").arg(numCells));
	
    //read in all of the points
    int numberNodes=0;
    double* point;
    for(long i = 0; i < numPoints; i++)
    {
           point = unstructGrid->GetPoint(i);
           tempNode.setLabel(i+1);
           tempNode.setCoordinates(point[0],point[1],point[2]);
           nodeList.append(tempNode);
    }
    emit sendMessage(QString("%1 grid points imported").arg(nodeList.size()));

    int cellType=0;
    int p=0;
    long tempid;

    for(long c = 0; c <numCells; c++)
    {
            vtkCell* cell = unstructGrid->GetCell(c);

            //int numberOfNodes = cell->GetNumberOfPoints();
           // emit sendMessage(QString(" Points: %1").arg(numberOfNodes));
            cellType= cell->GetCellType();
            vtkIdList* pointIds = cell->GetPointIds();

            tempElement.label=(c+1);

            //Check the element type
            //emit sendMessage(QString("el Type : %1").arg(cellType));
            switch (cellType)
            {
             //tria3
            case 5:
               tempElement.type=7;
               numberNodes=3;
             break;
             //Tet4
             case 10:
                tempElement.type=3;
                numberNodes=4;
             break;
             //VTK_VOXEL -> Hex8 element
            case 11:
                tempElement.type=1;
                numberNodes=8;
             break;
            //VTK_HEXAHEDRON -> Hex8 element
            case 12:
                tempElement.type=1;
                numberNodes=8;
             break;
             //VTK_WEDGE -> penta6 element
            case 13:
                tempElement.type=2;
                numberNodes=6;
            break;
            //Tet10 element
            case 24:
                tempElement.type=6;
                numberNodes=10;
             break;
             //VTK_QUADRATIC_HEXAHEDRON -> hex20 element
            case 25:
                tempElement.type=4;
                numberNodes=20;
             break;
             //Unknown element
            default:
                tempElement.type=-1;
            break;
            }

            //Add the nodelabels to the element-nodelist
            //Fortunately the node ordering is identical in vtk and frd
            if (tempElement.type>0)
            {
            for (p=0;p<numberNodes;p++)
            {
                   //Add 1 because lowest node id is 0 in vtk , 1 in frd
                tempid=(pointIds->GetId(p))+1;
                tempElement.nodeList.append(tempid);
                nodeList[tempid-1].adjacentElements.append(tempElement.label);
                //emit sendMessage(QString(" size %1 ").arg(nodeList[tempid-1].adjacentElements.size()));
            }
            elementList.append(tempElement);
            tempElement.nodeList.clear();
            }
   }


    if (nodeList.size()>0)
    {
            maxCoords=getMaxModelCoords();
            setModelCenter();
    }

    if (elementList.size()>0)
    {
        getFaces();
        getOutsideEdges();
        getOutsideNodes();
        getOutsideElements();
        //calcFaceCentroids();
    }
//check for vtk results
    vtkPointData* pointData = unstructGrid->GetPointData();
    int noArray = pointData->GetNumberOfArrays();
    emit sendMessage(QString("%1 results available").arg(noArray));
    if (noArray>0)
    {
        resultData.clearResultData();
        tempResultIndex.frqtime=1;
        tempResultIndex.resultStep = 1;
        resultList.append(tempResultIndex);
    }
    
}
//***********************************************************
/*!Gets data from an multiblock dataset*/

 void femodel::getMultiblockData(vtkAppendFilter *append,vtkMultiBlockDataSet *mbdata)
  {
     long numberBlocks;
     numberBlocks=mbdata->GetNumberOfBlocks();

     for (int k=0;k<numberBlocks;k++)
     {
         //Ueberpruefen ob Block ein Grid ist oder wieder Multiblock. Dann rekursiv weitermachen
         if (mbdata->GetBlock(k)->IsA("vtkMultiBlockDataSet"))
         {
             getMultiblockData(append,vtkMultiBlockDataSet::SafeDownCast(mbdata->GetBlock(k)));
         }
         else
         {
             append->AddInput(mbdata->GetBlock(k));
         }
     }
  }
