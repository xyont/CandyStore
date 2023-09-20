/***************************************************************************
 *   Copyright (C) 2010 by Patrick Prokopczuk                               *
 *   prokopczuk@gmx.de                                                      *
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
#include "femodel.h"
#include "openfoamface.h"
//************************************
/*! Generates a openfoam project from the current model*/
void femodel::exportMeshOpenFoam(const QString &dir)
{
    int precision=12;
    writePointsOpenFoam(dir,precision);
    emit sendMessage(QString("Points exported in directory %1").arg(dir));
    writeFacesOpenFoam(dir);
    emit sendMessage(QString("Faces exported in directory %1").arg(dir));
    writeResultsOpenFoam(dir);
}

//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam header*/
QString femodel::openFoamHeader()
{
    QString header;
    header="/*--------------------------------*- C++ -*----------------------------------*\\\n";
    header+="| =========                 |                                                 |\n";
    header+="| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |\n";
    header+="|  \\    /   O peration     | Version:  1.7.x                                 |\n";
    header+="|   \\  /    A nd           | Web:      www.OpenFOAM.com                      |\n";
    header+="|    \\/     M anipulation  |                                                 |\n";
    header+="\\*---------------------------------------------------------------------------*/\n";
    return(header);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam point header*/
QString femodel::openFoamPointHeader()
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="\n";
    header+="root \"..\";\n";
    header+="case \".\";\n";
    header+="instance \"constant\";\n";
    header+="local \"polyMesh\";\n";
    header+="\n";
    header+="class vectorField;\n";
    header+="object points;\n";
    header+="}\n";

    return(header);
}
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam faces header*/
QString femodel::openFoamFaceHeader()
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="\n";
    header+="root \"..\";\n";
    header+="case \".\";\n";
    header+="instance \"constant\";\n";
    header+="local \"polyMesh\";\n";
    header+="\n";
    header+="class faceList;\n";
    header+="object faces;\n";
    header+="}\n";

    return(header);
}
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam owner header*/
QString femodel::openFoamOwnerHeader(long nif, long nof)
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="class labelList;\n";
    header+="note"";\n";
    header+="location"";\n";
    header+="object owner"";\n";
    header+="}\n";

    return(header);
}
//**************************************************************
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam neighbour header*/
QString femodel::openFoamNeighbourHeader(long nif, long nof)
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="class labelList;\n";
    header+="note"";\n";
    header+="location"";\n";
    header+="object neighbour;\n";
    header+="}\n";

    return(header);
}
//**************************************************************
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam neighbour header*/
QString femodel::openFoamBoundaryHeader(long nif, long nof)
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="class polyBoundaryMesh;\n";
    header+="note"";\n";
    header+="location"";\n";
    header+="object boundary;\n";
    header+="}\n";

    return(header);
}
//**************************************************************
//**************************************************************
//**************************************************************
/*! Writes the nodes in OpenFoam format*/
void femodel::writePointsOpenFoam(const QString &dir, int precision)
{
    //File Output
QString fout;
QFile MyFile( dir + "/points" );
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {
QTextStream stream( &MyFile );
//Writing the header
    stream << openFoamHeader();
    stream << openFoamPointHeader();
//Writing nodes
    stream << QString::number(nodeList.size());
    stream << "\n";
    stream << "(\n";
for ( long i = 0; i < nodeList.size(); i++)
{
    stream << "(";
    stream.setFieldWidth(precision+6);
    stream << QString::number(nodeList[i].getCoordinate1(),'E',precision);
    stream.setFieldWidth(0);
    stream << " ";
    stream.setFieldWidth(precision+6);
    stream << QString::number(nodeList[i].getCoordinate2(),'E',precision);
    stream.setFieldWidth(0);
    stream << " ";
    stream.setFieldWidth(precision+6);
    stream << QString::number(nodeList[i].getCoordinate3(),'E',precision);
    stream.setFieldWidth(0);
    stream << ")\n";
}
    stream << ")\n";
}
    MyFile.close();
}
//**************************************************************
/*! Writes the faces in OpenFoam format*/
void femodel::writeFacesOpenFoam(const QString &dir)
{
    long totalFaces;
    //The faces must be sorted according to openfoam specifications
    ////////////////
    //A special face structure is needed:

    openfoamface tempOFFace;

    //Creating one list for outside (boundary faces) and one for internal faces
    QList<openfoamface> outerFaces;
    QList<openfoamface> innerFaces;

    //Fill the two lists
    for (long f=0;f<faceList.size();f++)
    {
        //copy the faces from the facelist to the OFfacelists and eliminate duplicate faces
        tempOFFace.ownerElement=faceList[f].parentElement;
        tempOFFace.ownerFace=faceList[f].faceNumber;
        tempOFFace.type=faceList[f].type;
        tempOFFace.neighbourElement=-1;
        tempOFFace.neighbourFace=-1;
        for (long k=0;k<faceList[f].type;k++)
        {
            tempOFFace.vertices[k]=faceList[f].nodeList[k];
        }
        //If the face is an outside face simply add it to the outside list
        if (faceList[f].outside == true)
        {
            outerFaces.append(tempOFFace);
        }
        //Otherwise the face is an internal face and should exist twice.
        //Check this and if O.K. add the parent element of the second face as neighbour
        else
        {
            if (faceList[f]==faceList[f+1])
            {
                tempOFFace.neighbourElement=faceList[f+1].parentElement;
                tempOFFace.neighbourFace=faceList[f+1].faceNumber;
                if (faceList[f].parentElement < faceList[f+1].parentElement)
                {
                    innerFaces.append(tempOFFace);
                }
                else
                {
                    tempOFFace.ownerElement=faceList[f+1].parentElement;
                    tempOFFace.ownerFace=faceList[f+1].faceNumber;
                    tempOFFace.neighbourElement=faceList[f].parentElement;
                    tempOFFace.neighbourFace=faceList[f].faceNumber;
                    for (long k=0;k<faceList[f+1].type;k++)
                    {
                        tempOFFace.vertices[k]=faceList[f+1].nodeList[k];
                    }
                    innerFaces.append(tempOFFace);
                }
                f++;

            }

        }


    }
    totalFaces=innerFaces.size()+outerFaces.size();
    //Outer face list is O.K.
    //***************************************
    //Inner face list must be sorted
    qSort(innerFaces);
    //*************************************************
    //File Output
    QString fout;
    QString ownerFileName=dir + "/owner";
    QString neighbourFileName=dir + "/neighbour";
    QString boundaryFileName=dir + "/boundary";
    QFile OwnerFile( ownerFileName );
    QFile NeighbourFile( neighbourFileName );
    QFile MyFile( dir + "/faces" );
    QFile BoundaryFile(boundaryFileName);


    if ((MyFile.open (QIODevice::WriteOnly | QIODevice::Text)) && (OwnerFile.open (QIODevice::WriteOnly | QIODevice::Text)) &&
            (NeighbourFile.open (QIODevice::WriteOnly | QIODevice::Text)) && (BoundaryFile.open (QIODevice::WriteOnly | QIODevice::Text)))
    {
        QTextStream stream( &MyFile );
        QTextStream ownerStream (&OwnerFile);
        QTextStream neighbourStream(&NeighbourFile);
        QTextStream boundaryStream(&BoundaryFile);
        //Writing the header
        stream << openFoamHeader();
        stream << openFoamFaceHeader();
    //Writing the owner header
        ownerStream << openFoamHeader();
        ownerStream << openFoamOwnerHeader(0,0);
        ownerStream << QString::number(totalFaces);
        ownerStream << "\n";
        ownerStream << "(\n";
    //Writing the neighbour header
        neighbourStream << openFoamHeader();
        neighbourStream << openFoamNeighbourHeader(0,0);
        neighbourStream << QString::number(innerFaces.size());
        neighbourStream << "\n";
        neighbourStream << "(\n";

    //Writing the bounray header
            boundaryStream << openFoamHeader();
            boundaryStream << openFoamBoundaryHeader(0,0);
            boundaryStream << QString::number(1);
            boundaryStream << "\n";
            boundaryStream << "(\n";
    //Writing faces
        stream << QString::number(totalFaces);
        stream << "\n";
        stream << "(\n";
        for (long inf=0; inf<innerFaces.size();inf++)
        {
            stream << QString::number(innerFaces[inf].type) << "(";
            for (int t=0;t<innerFaces[inf].type;t++)
            {
                stream<< QString::number(getNodeIndex((innerFaces[inf].vertices[t]))) << " ";
            }
            stream << ")\n";// <<  QString::number((innerFaces[inf].ownerElement)-1) << " " << QString::number((innerFaces[inf].neighbourElement)-1) << "\n";

            ownerStream <<  QString::number(getElementIndex((innerFaces[inf].ownerElement))) <<"\n";
            neighbourStream << QString::number(getElementIndex(innerFaces[inf].neighbourElement)) << "\n";
        }
        for (long of=0; of<outerFaces.size();of++)
        {
            stream << QString::number(outerFaces[of].type) << "(";
            for (int t=0;t<outerFaces[of].type;t++)
            {
                stream<< QString::number(getNodeIndex(outerFaces[of].vertices[t])) << " ";
            }
            stream << ")\n";
             ownerStream <<  QString::number(getElementIndex(outerFaces[of].ownerElement)) <<"\n";
        }

        //Writing the boundary patches
        boundaryStream << "all\n";
        boundaryStream << "{\n";
        boundaryStream << "type wall;\n";
        boundaryStream << "nFaces " <<  QString::number(outerFaces.size()) << " ;\n";
        boundaryStream << "startFace " <<  QString::number(innerFaces.size()) << " ;\n";
        boundaryStream << "}\n";
    //**************
        stream << ")\n";
        ownerStream << ")\n";
        neighbourStream << ")\n";
        boundaryStream << ")\n";


    }
    else
    {
        emit sendMessage("Could not open files for writing");
    }
}
//**************************************************************
//**************************************************************
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam neighbour header*/
QString femodel::openFoamScalarResultHeader(const QString &object)
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="class volScalarField;\n";
    header+="note"";\n";
    header+="location"";\n";
    header+="object " + object + ";\n";
    header+="}\n";

    return(header);
}
//**************************************************************
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with an openfoam neighbour header*/
QString femodel::openFoamVectorResultHeader(const QString &object)
{
        QString header;

    header="FoamFile\n";
    header+="{\n";
    header+="version 2.0;\n";
    header+="format ascii;\n";
    header+="class volVectorField;\n";
    header+="note"";\n";
    header+="location"";\n";
    header+="object " + object + ";\n";
    header+="}\n";

    return(header);
}
//******************************************************************
/*! Writes the results in OpenFoam format*/
/*!Node results are vonverted to cell results*/
void femodel::writeResultsOpenFoam(const QString &dir)
{
    long index;
    double value=0;
    QString resultsFileName;
    //QTextStream* stream = new QTextStream();
    emit sendMessage(QString("no. results: %1").arg(resultDataList.size()));
    for (long r=0; r<resultDataList.size(); r++)
    {

        resultsFileName=dir + "/" + resultDataList[r].resultLabel;
        QFile resultsFile( resultsFileName );
        resultsFile.open (QIODevice::WriteOnly | QIODevice::Text);
        QTextStream stream(&resultsFile);
        stream << openFoamHeader();
        resultDataList[r].numberComponents > 1 ? stream << openFoamVectorResultHeader(resultDataList[r].resultLabel) :
                                                 stream << openFoamScalarResultHeader(resultDataList[r].resultLabel);

        stream << "// * * * * * * * * * * * * *//\n";
        stream << "\n";
        stream << "dimensions          [0 0 0 0 0 0 0];\n";
        stream << "\n";
        stream << "internalField    nonuniform List<";
        resultDataList[r].numberComponents > 1 ? stream << "vector>\n" : stream << "scalar>\n";
        stream << QString("%1\n").arg(elementList.size());
        stream << "(\n";
        for (long e=0;e<elementList.size();e++)
        {
            value=0;
            if (resultDataList[r].numberComponents > 1)
            {
                stream << "(";
            }
            for (int c=0;c<resultDataList[r].numberComponents;c++)
            {
                value=0;
                for (int n=0; n <elementList[e].nodeList.size();n++)
                {
                     index=getNodeResultListIndex(elementList[e].nodeList[n]);
                     //emit sendMessage(QString(" index %1").arg(index));
                     value+=resultDataList[r].resultValueList[index].resultValues[c];
                     // emit sendMessage(QString(" result %1").arg(resultDataList[r].resultValueList[index].resultValues[c]));
                }
                value/=static_cast<double>(elementList[e].nodeList.size());
                stream << QString("%1 ").arg(value);
            }
            if (resultDataList[r].numberComponents > 1)
            {
                stream <<")";
            }
            stream << "\n";
        }
        stream << ")\n";
        stream << ";\n";
        stream << " \n";
        stream << "boundaryField\n";
        stream << "{\n";
        stream << "all\n";
        stream << "{\n";
        stream << "     type          empty;\n";
        stream << "}\n";
        stream << "}\n";

        resultsFile.close();
    }

}
