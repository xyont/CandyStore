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
/*! Reads data from a cdb file*/
int femodel::readcdb(const QString &fileName)
{
    //QMAP for element types
    QMap<int,int> eTypeMap;
    QString line;
    QString block,field1;
    QString::SectionFlag flag = QString::SectionSkipEmpty;
    int solidflag=0;
    long numNodes=0;
    long numEle=0;
    long numFields=0;
    long numNodesInFirstLine=0;
    long maxNodesInFirstLine=0;
    int numNodesInEle=0;
    long nodeind=-1;
    long anElType=-1;
//////////////////////////////////////////////////////////
    clearDatabase();
///////////////////////////////////////////////////
    tempNode.setOutside(false);
///////////////////////////////////////////////////////////
//Setting Up an Input-Filestream
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return(0);

    QTextStream in(&file);
/////////////////////////////////////////////////////////////
//////////Reading the file
    line = in.readLine();
     emit sendMessage(QString(tr("line: %1").arg(line)));
    block=line.section(",",0,0);
    while (!in.atEnd())
    {
        if(block=="ET")
        {
            eTypeMap.insert(line.section(",",1,1).toInt(),line.section(",",2,2).toInt());
            emit sendMessage(QString(tr("Mapped ET %1 to %2").arg(line.section(",",1,1).toInt()).arg(line.section(",",2,2).toInt())));
        }

        if(block=="NBLOCK")
        {
            emit sendMessage(QString(tr("New Block: %1").arg(block)));
            numFields=line.section(",",1,1).toLong();
            numNodes=line.section(",",4,4).toLong();
             emit sendMessage(QString(tr("Number of Nodes: %1").arg(numNodes)));

            line=in.readLine();
            for (long n=0;n<numNodes;n++)
            {
                line=in.readLine();
                tempNode.setLabel(line.mid(0,8).toLong());
                tempNode.setCoordinates(1,line.mid(24,20).toDouble());
                tempNode.setCoordinates(2,line.mid(44,20).toDouble());
                tempNode.setCoordinates(3,line.mid(64,20).toDouble());
                nodeList.append(tempNode);

            }

        }
        if(block=="EBLOCK")
        {
            emit sendMessage(QString(tr("New Block: %1").arg(block)));
            if (line.section(",",2,2).toUpper()=="SOLID")
            {
                emit sendMessage("SOLID");
                solidflag=1;
                maxNodesInFirstLine=8;
            }
            else
            {
                solidflag=0;
                maxNodesInFirstLine=10;
            }
            //numNodesInFirstLine=line.section(",",2,2).toLong();
            emit sendMessage(QString(tr("Reading Elements")));

            line=in.readLine();
            field1 = line.section(" ",0,0,flag);
            //Read elements until a -1 is encountered
            while ( field1.toLong() != -1)
            {
                tempElement.nodeList.clear();
                line=in.readLine();
                field1=line.section(" ",0,0,flag);
                if (field1.toLong() == -1)
                    break;
                switch (eTypeMap.value(line.mid(8,8).toInt()))
                {
                case 200:
                    //tet10 (only if keyopt1 =9!!
                    anElType=200;
                    tempElement.type=6;
                break;
                case 92:
                    //tet10
                    anElType=92;
                    tempElement.type=6;
                break;
                case 186:
                    anElType=186;
                    tempElement.type=4;
                break;
                default:
                    anElType=-1;

                }

                tempElement.label=line.mid(80,8).toLong();
                numNodesInEle=line.mid(64,8).toInt();
                //if (tempElement.label<100)
                //    emit sendMessage(QString(tr("eLEMENT: %1 type: %2 node: %3 ").arg(tempElement.label).arg(tempElement.type).arg(numNodesInEle)));

                if (numNodesInEle > maxNodesInFirstLine)
                {
                    //if (tempElement.label<100)
                   // emit sendMessage(QString(tr("two node lines")));
                    for (int n=0;n<maxNodesInFirstLine;n++)
                    {
                        tempElement.nodeList.append(line.mid(88+8*n,8).toLong());
                        nodeind=getNodeIndex(line.mid(88+8*n,8).toLong());
                        nodeList[nodeind].adjacentElements.append(tempElement.label);
                    }
                    line=in.readLine();
                    for (int n=0;n<(numNodesInEle-maxNodesInFirstLine);n++)
                    {
                        tempElement.nodeList.append(line.mid(8*n,8).toLong());
                        nodeind=getNodeIndex(line.mid(88+8*n,8).toLong());
                        nodeList[nodeind].adjacentElements.append(tempElement.label);

                    }



                }
                else
                {
                   // if (tempElement.label<100)
                     //   emit sendMessage("oneline");

                    for (int n=0;n<numNodesInEle;n++)
                    {
                        tempElement.nodeList.append(line.mid(88+8*n,8).toLong());
                        nodeind=getNodeIndex(line.mid(88+8*n,8).toLong());
                        nodeList[nodeind].adjacentElements.append(tempElement.label);
                    }

                }
                //if the anElType=186 check which element type is used
                if (anElType==186)
                {
                    if (tempElement.nodeList[2]==tempElement.nodeList[3])
                    {
                        //element is tet or prism
                        if ((tempElement.nodeList[4]==tempElement.nodeList[5]))
                        {
                            //element is a tet
                            tempElement.type=6;
                            //remove the double nodes
                            tempElement.nodeList.removeLast();
                            tempElement.nodeList.removeAt(15);
                            tempElement.nodeList.removeAt(14);
                            tempElement.nodeList.removeAt(13);
                            tempElement.nodeList.removeAt(12);
                            tempElement.nodeList.removeAt(10);
                            tempElement.nodeList.removeAt(7);
                            tempElement.nodeList.removeAt(6);
                            tempElement.nodeList.removeAt(5);
                            tempElement.nodeList.removeAt(3);


                        }
                        else
                        {
                            //element is a prism
                            tempElement.type=5;
                            //remove the double nodes
                            tempElement.nodeList.removeLast();
                            tempElement.nodeList.removeAt(14);
                            tempElement.nodeList.removeAt(10);
                            tempElement.nodeList.removeAt(7);
                            tempElement.nodeList.removeAt(3);
                        }
                    }
                }

            elementList.append(tempElement);
            }

        }

        line = in.readLine();
        block=line.section(",",0,0);

    }
    /////////////////////////////////////////////////////////////
    openFile=fileName;
    fileflag=9;
    file.close();

    //fileRead=1;
    /////////////////////////////////////////////////////////////
    emit sendMessage("Getting max model coords");
    if (nodeList.size()>0)
    {
        maxCoords=getMaxModelCoords();
        setModelCenter();
    }
    emit sendMessage("Getting faces");
    getFaces();
    emit sendMessage("Getting outside edges");
    getOutsideEdges();
    emit sendMessage("Getting outside nodes");
    getOutsideNodes();
    emit sendMessage("Getting outside elements");
    getOutsideElements();
    nodeOctree->initialise();
    return(1);
}
