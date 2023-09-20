
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
 * \file readfrdresults.cpp
 * \brief A brief file description
 **/
#include <QtGui>
#include <math.h>
#include "femodel.h"

//**Help functions for reading an frd file
/*! Returns the position of a resultstep in an frd file*/
qint64 femodel::getFilePosition(int resultstep)
{
    qint64 tempResultPos;
    for (int k=0;k<resultList.size();k++)
    {
        //sendMessage(QString::number(resultList[k].resultStep));
        if (resultstep==resultList[k].resultStep)
        {
                    tempResultPos=resultList[k].resultPosition;
                    if (tempResultPos>20000)
                    {
                        if(tempResultPos-20000 < tempResultPos*0.9)
                        {
                            return(tempResultPos-20000);
                        }
                        else
                        {
                            return(tempResultPos*0.9);
                        }
                    }
                    else
                    {
                        return(0);
                    }
                    //sendMessage(QString::number(resultPos));
        }
    }
    return(0);
}
//*************************************************************************************
/*! Reading the results from step <resultstep>*/
int femodel::readfrdresults(int resultstep)
{
	qint64 resultPos=0;
	
	int flag,formatFlag,nodeLabelLength,resultSpacing;
	int currentstep;
	int numberComponents=0;
	int numberResults=0;
	bool resultIsDisp=false;
	QString line;
	QString currentResult;
	QString resultComponent;
	long nodeLabel;
	long nodeIndex;
//	double resultComp;
	double dValue;
	dispAvailable=false;
	dispRatio=0;
	ResultTitles.clear();
	ResultComponents.clear();
	resultDataList.clear();
	resultData.clearResultData();
	
//	resultListNodalDisp.clear();
	//sendMessage(QString::number(resultPos));
    sendMessage(QString::number(resultstep));
	//sendMessage(QString::number(resultList.size()));

    //Get the position of the result to be read
    resultPos=getFilePosition(resultstep);

	QFile file(openFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit sendMessage("Could not open file for reading");
        return(-1);
    }
    //file.seek(resultPos);
    QTextStream in(&file);
    in.seek(resultPos);
    line = in.readLine();
	flag=line.left(5).toInt();
	while(!in.atEnd())
		{
            //emit sendMessage(line);
		
			if (flag==100)
			{
                formatFlag=line.mid(73,2).toInt();
                emit sendMessage(QString("Format Flag %1").arg(formatFlag));
				if (formatFlag==0)
				{
					nodeLabelLength=4;
					resultSpacing=12;
				}
				else
				{	
					nodeLabelLength=9;
					resultSpacing=12;
				}
				resultData.clearResultData();
				//sendMessage(line);
				numberComponents=0;
                currentstep=line.mid(58,5).toInt();
                if (currentstep>resultstep)
                {
                    emit sendMessage("c > r");
                    break;
                }
				if (resultstep==currentstep)
				{
					numberResults++;
					resultDataList.append(resultData);
					while(flag!=-3 )
					{
					
					if (flag==-4)
					{
						currentResult=line.mid(5,8);
						//ResultTitles.append(currentResult);
						resultDataList[numberResults-1].resultLabel=currentResult;
						
						if (currentResult.contains("DISP"))
						{
							dispAvailable=true;
							resultIsDisp=true;
						}
								while(flag!=-3 )
								{
		
								
								if (flag==-5)
								{	
									
									if (line.mid(33,5).toInt()!=1)
									{	
										numberComponents++;
										resultDataList[numberResults-1].addComponent(line.mid(5,8));
									}		
									
								}
								
								
								if (flag==-1) 
								{
									
									nodeLabel=line.mid(4,nodeLabelLength).toLong();
									nodeIndex=getNodeIndex(nodeLabel);
									if(resultIsDisp==true)
									{
										nodeList[nodeIndex].clearDisp();	
									}
									tempNodeResult.nodeLabel=nodeLabel;
									tempNodeResult.resultValues.clear();
									for (int c=0;c < numberComponents; c++)
									{
										dValue=line.mid(4+nodeLabelLength+12*c,12).toDouble();
										if(resultIsDisp==true)
										{
											nodeList[nodeIndex].setDisplacements(c+1,dValue);
										}
										tempNodeResult.resultValues.append(dValue);
										//////////////////////////////////////////////////////////////////////////////////
                                                                                //Wenn noch kein Ergebnis vorhanden, sind min und max werte = erstes Ergebnis
                                                                                if (resultDataList[numberResults-1].resultValueList.size()==0)
                                                                                {
                                                                                    resultDataList[numberResults-1].minValues[c]=dValue;
                                                                                    resultDataList[numberResults-1].maxValues[c]=dValue;
                                                                                }
                                                                                else
                                                                                {
                                                                                if (dValue< resultDataList[numberResults-1].minValues[c])
										{
											resultDataList[numberResults-1].minValues[c]=dValue;
										}
										if (dValue> resultDataList[numberResults-1].maxValues[c])
										{
											resultDataList[numberResults-1].maxValues[c]=dValue;
										}
                                                                                }
										/////////////////////////////////////////////////////////////////////////////////////	
									
									}
									if(resultIsDisp==true)
									{
										if (nodeList[nodeIndex].getDisplacements().norm()/modelRadius > dispRatio)
										{
											dispRatio=nodeList[nodeIndex].getDisplacements().norm()/modelRadius;
										}
									}
									resultDataList[numberResults-1].resultValueList.append(tempNodeResult);
								}
								
								line = in.readLine(200);
								flag=line.left(3).toInt();
								}
																			
					}
					
					if (flag==-3) break;
					line = in.readLine(200);
					flag=line.left(3).toInt();
					}
					
					resultIsDisp=false;
				}
			}
			line = in.readLine(200);
			flag=line.left(5).toInt();
		}
	//setNodalDisplacements(numberComponents);
	file.close();
	resultData.clearResultData();
	//**********************************************************************************
	//Calculate the additional result values //
	for (int a=0; a<resultDataList.size(); a++)
	{
		if (resultDataList[a].resultLabel.contains("DISP"))
		{
			resultDataList[a].calcDispMagnitude();
			calcFaceCentroidDisps();
		}
		if (resultDataList[a].resultLabel.contains("STRESS"))
		{
			resultDataList[a].calcMisesStress();
			resultDataList[a].calcPrincStress();
		}
	}
	//************************************************************************************
	
return(1);
}
