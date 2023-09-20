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

/**
 * \file readfrd.cpp
 **/
#include <QtGui>
#include <QFile>
#include <QThread>
#include "femodel.h"
#include "element.h"
#include "node.h"
#include "readfrdthread.h"


/*!Reading frd data from a file*/
int femodel::readfrd(const QString &fileName)
{

    long resultsInBlock=0;
	int currentResultStep=-1;
	float currentResultTime=-1;
	short nodelines;
	short n[3];
	int flag,n1hex20,n2hex20;
	int formatFlag,coordSpacing,nodeLabelLength;
	int elLabelLength,nodeSpacing,elTypeLength;
	int n1penta15,n2penta15;
//	int numberNodesInLine=0;
	QString line;
	
//////////////////////////////////////////////////////////
	clearDatabase();
///////////////////////////////////////////////////
	tempNode.setOutside(false);
///////////////////////////////////////////////////////////
//Setting Up an Input-Filestream
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return(0);

//STarting the timer for time evaluation
    QTime t;
    t.start();

	QTextStream in(&file);
/////////////////////////////////////////////////////////////
//////////Reading the file
	line = in.readLine();
	flag=line.left(5).toInt();
	while (!in.atEnd())
	{
	if(flag == 1)
   		{
      			;	/* do nothing but accept */
		}
		else if(flag == 2)
		{
                        formatFlag=line.mid(73,2).toInt();
			//emit sendMessage(QString("Format Flag %1").arg(formatFlag));
			if (formatFlag==0)
			{
                                nodeLabelLength=5;
				coordSpacing=12;
			}
			else
			{	
                                nodeLabelLength=10;
				coordSpacing=12;
			}
			sendMessage("Reading Nodes");

                        line = in.readLine();
                        flag=line.left(3).toInt();

                        while (flag !=-3)
			{
                                tempNode.setLabel(line.mid(3,nodeLabelLength).toLong());
                                tempNode.setCoordinates(1,line.mid(3+nodeLabelLength,coordSpacing).toDouble());
                                tempNode.setCoordinates(2,line.mid(3+nodeLabelLength+coordSpacing,coordSpacing).toDouble());
                                tempNode.setCoordinates(3,line.mid(3+nodeLabelLength+2*coordSpacing,coordSpacing).toDouble());
				//nSetList[0].nodeLabels.append(tempNode.getLabel());
                                nodeList.append(tempNode);
                                line = in.readLine();
                                flag=line.left(3).toInt();
				//
                        };
		}
		//****Reading elements
		else if(flag == 3)
		{
                        formatFlag=line.mid(73,2).toInt();
			//emit sendMessage(QString("Format Flag %1").arg(formatFlag));
			if (formatFlag==0)
			{
                                elLabelLength=5;
				elTypeLength=5;
				nodeSpacing=5;
				n1hex20=15;
				n2hex20=5;
				n1penta15=15;
				n2penta15=0;
			}
			else
			{	
                                elLabelLength=10;
				elTypeLength=5;
				nodeSpacing=10;
				n1hex20=10;
				n2hex20=10;
				n1penta15=10;
				n2penta15=5;
			}
			sendMessage("Reading Elements");
                        tempElement.outside=0;

                        line = in.readLine();
                        flag=line.left(3).toInt();

                        while (flag !=-3)
			{
                                tempElement.label = line.mid(3,elLabelLength).toLong();
                                tempElement.type = line.mid(3+elLabelLength,elTypeLength).toInt();
				switch (tempElement.type)
				{
				case 1:
				//Hex 8 Element
					nodelines=1;
					n[0]=8;
					n[1]=0;
					n[2]=0;
					break;
				case 2:
				//Penta 6 Element
					nodelines=1;
                    n[0]=6;
					n[1]=0;
					n[2]=0;
					break;
				case 3:
				//Tet 4 Element
					nodelines=1;
					n[0]=4;
					n[1]=0;
					n[2]=0;
                break;
				case 4:  
				//Hex 20 element
					nodelines=2;
					n[0]=n1hex20;
					n[1]=n2hex20;
					n[2]=0;
					break;
				case 5:  
				//Penta 15 element
					nodelines=2;
					n[0]=n1penta15;
					n[1]=n2penta15;
					n[2]=0;
					break;
				case 6:
				//Tet10 Element
					nodelines=1;
					n[0]=10;
					n[1]=0;
					n[2]=0;
                break;
                case 7:
                //Tria3 Element
                    nodelines=1;
                    n[0]=3;
                    n[1]=0;
                    n[2]=0;
                break;
                case 10:
                //Quad8 Element
                    nodelines=1;
                    n[0]=8;
                    n[1]=0;
                    n[2]=0;
                break;

				default:
				//Bar Element is chosen for all unknown elements
					nodelines=1;
					n[0]=2;
					n[1]=0;
					n[2]=0;
					break;
				}
				long nodelab;
				long nodeind;
				for (int i=0; i<=nodelines-1; i++)
				{
					line = in.readLine();
					flag=line.left(3).toInt();
					//n=line.
					for (int k=1; k<=n[i]; k++)
					{
                                                nodelab=line.mid(3+(k-1)*nodeSpacing,nodeSpacing).toLong();
                                                tempElement.nodeList.append(nodelab);
                                              //Add the elements to the nodes element list.
                                                nodeind=getNodeIndex(nodelab);
                                                nodeList[nodeind].adjacentElements.append(tempElement.label);
					}
				}
				
				elementList.append(tempElement);
				//Knotenliste für nächstes Element leeren
				tempElement.nodeList.clear();

                                line = in.readLine();
                                flag=line.left(3).toInt();
                            };
		}
//--------------------------------------------------------------------------------------------------------------
//Reading the ResultsInformation (only information no values)

		else if(flag == 100)
		{
                emit sendMessage("Reading results");
                        formatFlag=line.mid(73,2).toInt();
                        tempResultIndex.frqtime=line.mid(12,12).toFloat();
                        tempResultIndex.resultStep = line.mid(58,5).toInt();
                        resultsInBlock=line.mid(24,12).toLong();
                        if (formatFlag==0)
			{
				//tempResultIndex.resultInc=line.mid(11,15);
				//emit sendMessage(line.mid(11,15));
			}
                        tempResultIndex.resultPosition=file.pos();
                        sendMessage(QString::number(file.pos()));
                        //Check wether the result is a time result or a frequency result
                        if (line.contains("M"))
				{
                                //Frequency result
				tempResultIndex.resultType=1;
				}
			else
				{
                                //Time result
				tempResultIndex.resultType=0;
				}
				
			if (currentResultStep!=tempResultIndex.resultStep || currentResultTime!=tempResultIndex.frqtime)
			//if (currentResultStep!=tempResultIndex.resultStep)
			{
					currentResultTime=tempResultIndex.frqtime;
					currentResultStep=tempResultIndex.resultStep;
                                        resultData.clearResultData();
                                        resultList.append(tempResultIndex);
				}

			do
			{
                                line = in.readLine();
                                flag=line.left(5).toInt();
                        }while (flag!=-3 || in.atEnd());

		}	
	line = in.readLine();
        flag=line.left(5).toInt();

	}
emit sendMessage("End of results");
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////	
openFile=fileName;
fileflag=1;
file.close();
sendMessage("File loaded!");
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
//Check the timer
double time =t.elapsed()/1000.;
QString timestring=QString::number(time);
emit sendMessage(QString(tr("File loaded in %0 seconds").arg(timestring)));
//delete the time
nodeOctree->initialise();
return(1);
}
//***********************************************************************************
/////////////////////////////////////////////////////////////////////////////////////
/*!Reading frd result info from a file*/
int femodel::readfrdResultInfo(const QString &fileName)
{
    long resultsInBlock=0;
    int currentResultStep=-1;
    float currentResultTime=-1;
    int flag;
    int formatFlag;
    QString line;
    ///////////////////////////////////////////////////////////
    //Setting Up an Input-Filestream
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
                return(0);

    //Starting the timer for time evaluation
        QTime t;
        t.start();

        QTextStream in(&file);
    /////////////////////////////////////////////////////////////
    //////////Reading the file
        line = in.readLine();
        flag=line.left(5).toInt();
        while (!in.atEnd())
        {
        if(flag == 1)
            {
                    ;	/* do nothing but accept */
            }
        //Reading the ResultsInformation (only information no values)

        else if(flag == 100)
                {
                        emit sendMessage("Reading results");
                                formatFlag=line.mid(73,2).toInt();
                                tempResultIndex.frqtime=line.mid(12,12).toFloat();
                                tempResultIndex.resultStep = line.mid(58,5).toInt();
                                resultsInBlock=line.mid(24,12).toLong();
                                if (formatFlag==0)
                    {
                        //tempResultIndex.resultInc=line.mid(11,15);
                        //emit sendMessage(line.mid(11,15));
                    }
                                tempResultIndex.resultPosition=file.pos();
                                sendMessage(QString::number(file.pos()));
                    if (line.contains("M"))
                        {
                        tempResultIndex.resultType=1;
                        }
                    else
                        {
                        tempResultIndex.resultType=0;
                        }

                    if (currentResultStep!=tempResultIndex.resultStep || currentResultTime!=tempResultIndex.frqtime)
                    //if (currentResultStep!=tempResultIndex.resultStep)
                    {
                            currentResultTime=tempResultIndex.frqtime;
                            currentResultStep=tempResultIndex.resultStep;
                                                resultData.clearResultData();
                                                resultList.append(tempResultIndex);
                        }

                    do
                    {
                                        line = in.readLine();
                                        flag=line.left(5).toInt();
                                }while (flag!=-3 || in.atEnd());

                }
            line = in.readLine();
                flag=line.left(5).toInt();

            }
        double time =t.elapsed()/1000.;
        QString timestring=QString::number(time);
        emit sendMessage(QString(tr("Result infos evaluated in %0 seconds").arg(timestring)));
        return(1);
}
