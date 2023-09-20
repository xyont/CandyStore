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

//************************************
/*! Generates a frd msh file from the current model*/
void femodel::exportMeshFrd(const QString &setname,long offset)
{
    QString meshFile=openFile;
    meshFile.append(".frd");
    writeMeshFrd(meshFile,offset);
    emit sendMessage(QString("Mesh exported in file %1").arg(meshFile));
}
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with a frd header*/
QString femodel::frdHeader(const QString &comment)
{
    QString header;
    header="    1C "+ comment + "\n    1UUSER\n    1UDATE              ";
    header+= QDate::currentDate().toString(Qt::ISODate) + "\n";
    header+="    1UTIME              ";
    header+= QTime::currentTime().toString() + "\n";
    header+="    1UHOST\n";
    header+="    1UPGM               CandyStore\n";
    header+="    1UDIR\n";
    header+="    1UDBN\n";
    return(header);
}
//////////////////////////////////////////////////////////////////////////
/*! Returns a QString with a frd result header */
QString femodel::resultHeader(int step,double steptime,const QString &resultname,int numberComponents,QStringList &compLabels)
{
    QString header;
        QString rname=resultname.left(8);
        int resulttype=1;
        int entityType=1;
    rname.remove(" ");
    header=(QString("    1PSTEP%1\n").arg(step,26));
    header+="  100CL1";
    if (step<10)
        header+="0";

        header+=QString::number(step)+ QString("  %1%2%3%4%5\n").arg(steptime,12).arg("Comment",32).arg(3,2).arg(step,5).arg(1,12);
        header+=QString(" -4  %1%2%3\n").arg(rname,-8).arg(numberComponents,5).arg(resulttype,5);
    for (int c=0;c<numberComponents;c++)
    {
                header+=QString(" -5  %1    1%2\n").arg(compLabels[c],-8).arg(entityType,5);
    }
    return(header);
}
//////////////////////////////////////////////////////////////////////////////

/*********************************************************************/
/*! Writes the current mesh into an frd file*/
void femodel::writeMeshFrd(const QString &fileName,long offset=0)
{
                //File Output
    QString fout;
    QFile MyFile( fileName );
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &MyFile );
        //Writing the header
        stream << frdHeader(fileName);
        //Writing nodes
        stream << "    2C                                                                   1\n";
        for ( long i = 0; i < nodeList.size(); i++)
        {
                stream << " -1";
                stream.setFieldWidth(10);
                stream << QString::number(nodeList[i].getLabel()+offset);
                stream.setFieldWidth(12);
                stream << QString::number(nodeList[i].getCoordinate1());
                stream << QString::number(nodeList[i].getCoordinate2());
                stream << QString::number(nodeList[i].getCoordinate3());
                stream.setFieldWidth(0);
                stream << "\n";
        }
        stream << " -3 \n";
        //Writing elements
        stream << "    3C                                                                   1\n";
        for ( long k = 0; k < elementList.size(); k++)
        {
            stream << " -1";
            stream.setFieldWidth(10);
            stream << QString::number(elementList[k].getLabel()+offset);
            stream.setFieldWidth(5);
            stream << QString::number(elementList[k].type);
            stream.setFieldWidth(0);
            stream << "\n";
            switch (elementList[k].type)
            {
                //Hex8 elements
                case 1:
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int n=0;n<8;n++)
                    {
                        stream << QString::number(elementList[k].nodeList[n]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                break;
                //Tet4 elements
                case 3:
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int n=0;n<4;n++)
                    {
                        stream << QString::number(elementList[k].nodeList[n]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                break;
            //Hex20elements
                case 4:
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int n=0;n<10;n++)
                    {
                           stream << QString::number(elementList[k].nodeList[n]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int m=10;m<20;m++)
                    {
                        stream << QString::number(elementList[k].nodeList[m]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                break;
                //Tet10elements
                case 6:
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int n=0;n<10;n++)
                    {
                        stream << QString::number(elementList[k].nodeList[n]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                break;
                //Tri3 Element
                case 7:
                    stream << " -2";
                    stream.setFieldWidth(10);
                    for (int n=0;n<3;n++)
                    {
                        stream << QString::number(elementList[k].nodeList[n]+offset);
                    }
                    stream.setFieldWidth(0);
                    stream << "\n";
                break;
                }
            }
            stream << " -3 \n";

        }
        MyFile.close();
        //Write results if any exist
        writeFRDresults(fileName,0,offset);
}
//************************************************************************
/*! Writes the current results into an frd file*/
/* The mode flag indicates wether the results will be appended to the, or the file be overwritten*/
void femodel::writeFRDresults(const QString &fileName,int mode,long offset=0)
{

   // QString fileName1="./resulttest.frd";
    emit sendMessage(QString("Fileflag: %1").arg(fileflag));
   // emit sendMessage(openFile);

    QFile MyFile( fileName );
    if (MyFile.open (QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream( &MyFile );
        //Writing results
        switch (fileflag)
        {
            //frd file
        case 1:

        for (int k=0;k<resultList.size();k++)
        {
            emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
            readfrdresults(resultList[k].resultStep);
            emit sendMessage(QString("Result Datalist %1").arg(resultDataList.size()));
            for (long r=0; r<resultDataList.size(); r++)
            {
                        stream << resultHeader(resultList[k].resultStep,resultList[k].frqtime,resultDataList[r].resultLabel,resultDataList[r].numberComponents,resultDataList[r].componentLabels);
                        for (long nn=0; nn<resultDataList[r].resultValueList.size();nn++)
                        {
                            stream << QString(" -1%1").arg(resultDataList[r].resultValueList[nn].nodeLabel+offset,10);
                            for (unsigned int c=0;c<resultDataList[r].numberComponents;c++)
                            {
                                stream << QString("%1").arg(resultDataList[r].resultValueList[nn].resultValues[c],12);
                            }
                            stream << "\n";
                        }
                        stream << " -3 \n";

            }
        }
        break;
        //vtu file
        case 2:
        ;
        //No break because vtu and vtmb files are treated the same way
        //vtmb file
        case 3:

            //emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
            readVtkResults(1);
            emit sendMessage(QString("Result Datalist %1").arg(resultDataList.size()));
            for (long r=0; r<resultDataList.size(); r++)
            {
                        stream << resultHeader(1,1,resultDataList[r].resultLabel,resultDataList[r].numberComponents,resultDataList[r].componentLabels);
                        for (long nn=0; nn<resultDataList[r].resultValueList.size();nn++)
                        {
                            stream << QString(" -1%1").arg(resultDataList[r].resultValueList[nn].nodeLabel+offset,10);
                            for (unsigned int c=0;c<resultDataList[r].numberComponents;c++)
                            {
                                stream << QString("%1").arg(resultDataList[r].resultValueList[nn].resultValues[c],12);
                            }
                            stream << "\n";
                        }
                        stream << " -3 \n";

            }

        break;

        }
         stream << "9999\n";
    }

}

//**********************************
/*! Write a set to a file in frd format*/
void femodel::writeSetFrd(int setIndex, long offset)
{
    long index=-1;
    QString directory=QFileInfo(openFile).absolutePath();
    QString fileName=directory +"/"+ setList[setIndex].name + ".frd";
    emit sendMessage(fileName);
    QFile MyFile( fileName );
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {


            QTextStream stream( &MyFile );
            //Writing the header
            stream << frdHeader(fileName);
            //Writing nodes
            stream << "    2C                                                                   1\n";
            for ( long i = 0; i < setList[setIndex].nodeList.size(); i++)
            {
                            stream << " -1";
                            stream.setFieldWidth(10);
                            stream << QString::number(setList[setIndex].nodeList[i]+offset);
                            stream.setFieldWidth(12);
                            stream << QString::number(getNodeCoords(setList[setIndex].nodeList[i]).x);
                            stream << QString::number(getNodeCoords(setList[setIndex].nodeList[i]).y);
                            stream << QString::number(getNodeCoords(setList[setIndex].nodeList[i]).z);
                            stream.setFieldWidth(0);
                            stream << "\n";
            }
            stream << " -3 \n";
            //Writing results
            switch (fileflag)
            {
                //frd file
            case 1:

            for (int k=0;k<resultList.size();k++)
            {
                emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
                readfrdresults(resultList[k].resultStep);
                emit sendMessage(QString("Result Datalist %1").arg(resultDataList.size()));
                for (long r=0; r<resultDataList.size(); r++)
                {
                        stream << resultHeader(resultList[k].resultStep,resultList[k].frqtime,resultDataList[r].resultLabel,resultDataList[r].numberComponents,resultDataList[r].componentLabels);
                        //get the index of the nodes of the set in the result data list
                        for ( long j = 0; j < setList[setIndex].nodeList.size(); j++)
                        {
                             index=resultDataList[r].getIndex(setList[setIndex].nodeList[j]);
                            //When the node is in the result data list write it to the file
                            if (index> -1)
                            {
                                 stream << QString(" -1%1").arg(resultDataList[r].resultValueList[index].nodeLabel+offset,10);
                                 for (unsigned int a=0;a<resultDataList[r].numberComponents;a++)
                                 {
                                     stream << QString("%1").arg(resultDataList[r].resultValueList[index].resultValues[a],12);
                                 }
                                 stream << "\n";

                            }
                        }
                        stream << " -3 \n";

                }
            }
            break;
            //vtu file
            case 2:
            ;
            //No break because vtu and vtmb files are treated the same way
            //vtmb file
            case 3:

                //emit sendMessage(QString("Reading result step %1").arg(resultList[k].resultStep));
                readVtkResults(1);
                emit sendMessage(QString("Result Datalist %1").arg(resultDataList.size()));
                for (long r=0; r<resultDataList.size(); r++)
                {
                        stream << resultHeader(1,1,resultDataList[r].resultLabel,resultDataList[r].numberComponents,resultDataList[r].componentLabels);
                        //get the index of the nodes of the set in the result data list
                        for ( long j = 0; j < setList[setIndex].nodeList.size(); j++)
                        {
                             index=resultDataList[r].getIndex(setList[setIndex].nodeList[j]);
                            //When the node is in the result data list write it to the file
                            if (index> -1)
                            {
                                 stream << QString(" -1%1").arg(resultDataList[r].resultValueList[index].nodeLabel+offset,10);
                                 for (unsigned int a=0;a<resultDataList[r].numberComponents;a++)
                                 {
                                     stream << QString("%1").arg(resultDataList[r].resultValueList[index].resultValues[a],12);
                                 }
                                 stream << "\n";

                            }
                        }
                        stream << " -3 \n";
                }
            break;
            }
             stream << "9999\n";

            emit sendMessage("file exported");
        }
    return;
}
