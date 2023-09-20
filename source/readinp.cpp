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
 * \file readinp.cpp
 * \brief Reads CCX or ABAQUS data into the femodel
 **/
#include <QtGui>
#include <QFile>
#include "femodel.h"
#include "element.h"
#include "node.h"
//*********************************************************************************************
/*! Importing a model from an Input file */
int femodel::readinp(const QString &fileName)
{

// preparing the database for import
//	int currentResultStep=-1;
//	short nodelines;
//	short n[3];
//	int flag;
//	int numberNodesInLine=0;
	QString line;
	
//////////////////////////////////////////////////////////
	clearDatabase();
///////////////////////////////////////////////////
	tempNode.setOutside(false);
///////////////////////////////////////////////////////////
	readinpdata(fileName);
return(1);
}
//************************************************************************************************
//Reading the  file data 
int femodel::readinpdata(const QString &fileName)
{
//int numberStrings=0;
QStringList keywordList;
QString keyword;
QString line;
QString includeFile,absolutePath;
QDir dir,parentDir;
int readingflag=0;
int optionFlag=0;
int numberStrings=0;
int numberValues=0,nodecount;
int elType=0;
QString elProp;
QStringList optionList;
//Setting Up an Input-Filestream
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            emit sendMessage(QString("Input File %1 not found ").arg(fileName));
            return(0);
        }
        parentDir=QFileInfo(fileName).canonicalPath();
	QTextStream in(&file);
        keywordList.clear();
/////////////////////////////////////////////////////////////

        do
        {
            line = in.readLine();
            if (readCCXisComment(line))
            {
                // if line is a keyword read the next line
                line = in.readLine();
            }
            numberStrings=line.count(",")+1;
            // Check for a keyword
            if (line[0]=='*')
            {
                //reset the reading Flag
                readingflag=0;
                //
                keyword=line.section(",",0,0);
                optionList.clear();
                if (numberStrings > 1)
                {
                    optionFlag=1;

                    for (int o=1;o<numberStrings;o++)
                    {
                        optionList.append(line.section(",",o,o));
                    }
                }
                else
                {
                    optionFlag=0;
                }
                keyword.remove("*");
                keyword.toUpper();
                //read data from an include File
                if (keyword=="INCLUDE")
                {
                        includeFile=line.section("=",1,1);
                        dir=includeFile;
                        if (dir.isAbsolute())
                        {
                            //emit sendMessage("Absolute Path");
                            emit sendMessage(QString("Include File: %1").arg(includeFile));
                            readinpdata(includeFile);
                        }
                        else
                        {

                            //emit sendMessage("Relative Path");
                            absolutePath = parentDir.absoluteFilePath(includeFile);
                             emit sendMessage(QString("Include File: %1").arg(absolutePath));
                            readinpdata(absolutePath);
                        }

                 }

                 if (keyword=="NODE")
                 {
                     emit sendMessage(QString("*NODE found"));
                    readingflag=1;
                 }
                 if (keyword=="ELEMENT")
                 {
                     emit sendMessage(line);
                     readingflag=2;
                     elType=-1;
                     if (optionFlag==0)
                     {
                         emit sendMessage("No element type specified, keyword will be skipped");
                         readingflag=0;
                     }
                     else
                     {
                         for (int z=0;z<optionList.size();z++)

                         {
                             emit sendMessage(optionList[z]);
                             if (optionList[z].section("=",0,0).remove(" ").toUpper()=="TYPE")
                             {
                                elProp= optionList[z].section("=",1,1).toUpper();
                                elType=getIntElType(elProp);
                             }
                             else
                             {
                                 emit sendMessage(QString("Unknown Option %1 will be ignored!").arg(optionList[z].section("=",0,0).toUpper()));
                             }
                         }
                     }
                     if (elType <=0)
                     {
                             readingflag=0;
                             emit sendMessage(QString(tr("Unknown element type: %1. Element Block will be skipped! ").arg(elType)));
                     }
                     emit sendMessage(QString("element type: %1").arg(elType));
                   }
              }
            //Line is not a keyword!
              else
              {
                  switch (readingflag)
                  {
                      //reading a node line
                  case 1:
                      tempNode.clearCoordinates();
                      tempNode.setLabel(line.section(",",0,0).toLong());
                      //emit sendMessage("Node");
                      if (tempNode.getLabel() > 0)
                      {
                              for (int k=1;k<numberStrings; k++)
                              {
                                      tempNode.setCoordinates(k,line.section(",",k,k).toDouble());
                              }
                              nodeList.append(tempNode);
                      }
                  break;
                      //reading an element
                  case 2:
 //                     emit sendMessage("Reading elements");
                      numberStrings=line.count(",")+1;
                      //Check wether the last string really exists
                      if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
                      {
                          numberStrings=numberStrings-1;
                      }
                      switch (elType)
                      //how many values nmust be read?
                      {
                      case 1:
                              //HEX8 element
                              //element ID + 8 nodes
                              numberValues=9;
                      break;
                      case 2:
                              //PENTA6 element
                              //element ID + 6 nodes
                              numberValues=7;
                      break;
                      case 3:
                              //TET4 element
                              //element ID + 4 nodes
                              numberValues=5;
                      break;
                      case 4:
                              //Hex 20 element
                              //element ID + 20 nodes
                              numberValues=21;
                      break;
                      case 5:
                              //Penta 15 element
                              //element ID + 15 nodes
                              numberValues=16;
                      break;
                      case 6:
                              //Tet10 Element
                              //element ID + 10 nodes
                              numberValues=11;
                      break;
                      default:
                              numberValues=0;
                      break;
                  }
                      tempElement.label=line.section(",",0,0).toLong();
   //                   emit sendMessage(QString("Reading element %1").arg(tempElement.label));
                      if (tempElement.label > 0)
                      {
                      tempElement.nodeList.clear();
                      tempElement.type=elType;
                      tempElement.prop=elProp;

                      if (numberStrings <numberValues)
                      {
                          if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
                          //Element description continues on the next line
                          {
                                  //emit sendMessage("multiline");
                                  nodecount=numberStrings-1;
                                  readingflag=3;
                                  for (int n=1;n<numberStrings-1;n++)
                                  {
                                          //emit sendMessage(line.section(",",n,n));
                                          tempElement.nodeList.append(line.section(",",n,n).toLong());
                                  }
                                  numberValues=numberValues-numberStrings;
                           }
                      }
                       else
                      {
                            if(numberStrings == numberValues)
                           {
                               for (int n=1;n<numberStrings;n++)
                               {
                                       //emit sendMessage(line.section(",",n,n));
                                       tempElement.nodeList.append(line.section(",",n,n).toLong());
                               }
                               //emit sendMessage("Appending element");
                               elementList.append(tempElement);

                           }
                        }
                    }
                      /*
                           else
                           {
                            emit sendMessage("Not enough nodes. Element will be skipped");
                           }


                      }


                      else
                      {
                          emit sendMessage("Too many Nodes .Element will be skipped!");
                      }
                      }*/
			
                      break;
                  case 3:
                      numberStrings=line.count(",")+1;
                      //Check wether the last string really exists
                      if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
                      {
                          numberStrings=numberStrings-1;
                      }
                      if(numberStrings == numberValues)
                     {
                         for (int n=1;n<numberStrings;n++)
                         {
                                 //emit sendMessage(line.section(",",n,n));
                                 tempElement.nodeList.append(line.section(",",n,n).toLong());
                         }
                         //emit sendMessage("Appending element");
                         elementList.append(tempElement);
                        readingflag=2;
                     }


                       break;
                  }

            }
          }
            while (! in.atEnd());
//Finishing import
	openFile=fileName;
	file.close();
	maxCoords=getMaxModelCoords();	
	setModelCenter();
	getFaces();
	getOutsideEdges();
	getOutsideNodes();
	calcFaceCentroids();
return(1);

    }
//*********************************************************************
/*!Reads nodes from an abaqus/ccx inp-file and returns the next keyword*/
QStringList femodel::readCCXNodes(QTextStream &in)
{
emit sendMessage("Reading Nodes");
/*
QString line, keyword;
QStringList keywordlist;
keywordlist.clear();
int numberStrings;
long k;
while (!in.atEnd())
	{
		line = in.readLine();
		
			numberStrings=line.count(",")+1;
			keyword=line.section(",",0,0);
			if (keyword[0]=='*' && keyword[1]!='*') //line is a keyword
			{
				for (int l=0;l< numberStrings;l++)
				{
					keywordlist.append(keyword=line.section(",",l,l).toUpper());
				}
				
				return(keywordlist);
			}
			else if (keyword[0]=='*' && keyword[1]=='*') //line is a comment
			{
			//do nothing
			}
			else
			{
				tempNode.clearCoordinates();
				tempNode.setLabel(line.section(",",0,0).toLong());
				//emit sendMessage("Node");
				if (tempNode.getLabel() > 0)
				{
					for (k=1;k<numberStrings; k++)
					{
						tempNode.setCoordinates(k,line.section(",",k,k).toDouble());
					}
					nodeList.append(tempNode);
				}
			}
		
	}
//keywordlist.append("EOF");
return(keywordlist);
*/
}
//*********************************************************************
/*!Reads elements from an abaqus/ccx inp-file and returns the next keyword*/
QStringList femodel::readCCXElements(QTextStream &in, const QStringList &options)
{

QString line,keyword,elset,elProp;
QStringList keywordlist;
keywordlist.clear();
int numberStrings,n;
long l;
int elType=-1,nextLine;
int nodecount=0;
int numberValues;
	elset="all";


// Parsing the options
        for (int g=1;g< options.size();g++)
	{
		if (options[g].section("=",0,0).remove(" ").toUpper()=="TYPE")
		{
			if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D8")
			{
				elType=1;
				elProp="C3D8";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D8R")
			{
				elType=1;
				elProp="C3D8R";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D4")
			{
				elType=3;
				elProp="C3D4";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D10")
			{
				elType=6;
				elProp="C3D10";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D10M")
			{
				elType=6;
				elProp="C3D10M";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D20")
			{
				elType=4;
				elProp="C3D20";
			}
			else if(options[g].section("=",1,1).remove(" ").toUpper()=="C3D20R")
			{
				elType=4;
				elProp="C3D20R";
			}
			else 
			{
				elType=-1;
				emit sendMessage(tr("Unknown element type %1").arg(options[g].section("=",1,1).toUpper()));
			}
		}
        else
        {
        if (options[g].section("=",0,0).toUpper()=="ELSET")
		{
			elset=options[g].section("=",1,1).toUpper();
		}
		else
		{
			emit sendMessage(tr("Unknown option %1 for keyword *ELEMENT").arg(options[g].section("=",0,0).toUpper()));
        }
    }
	}
//Reading the element information
	while (!in.atEnd())
	{
		line = in.readLine();
		
			numberStrings=line.count(",")+1;
                        //Check wether the last string really exists
                        if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
                        {
                            numberStrings=numberStrings-1;
                        }
			keyword=line.section(",",0,0);
			if (keyword[0]=='*' && keyword[1]!='*') //line is a keyword
			{
				for (l=0;l< numberStrings;l++)
				{
					keywordlist.append(keyword=line.section(",",l,l).toUpper());
				}
				
				return(keywordlist);
			}
			else if (keyword[0]=='*' && keyword[1]=='*') //line is a comment
			{
			//do nothing
			}
			else
//Reading elements starts here			
			{
				if (elType>0)
				{
					switch (elType)
					//how many values nmust be read?
					{
					case 1:
						//HEX8 element
						//element ID + 8 nodes
						numberValues=9;
					break;
					case 2:
						//PENTA6 element
						//element ID + 6 nodes
						numberValues=7;
					break;
					case 3:
						//TET4 element
						//element ID + 4 nodes
						numberValues=5;
					break;
					case 4:  
						//Hex 20 element
						//element ID + 20 nodes
						numberValues=21;
					break;
					case 5:  
						//Penta 15 element
						//element ID + 15 nodes
						numberValues=16;
					break;
					case 6:
						//Tet10 Element
						//element ID + 10 nodes
						numberValues=11;
					break;
					default:
						numberValues=0;
					break;
					}
					
					tempElement.label=line.section(",",0,0).toLong();
					if (tempElement.label > 0)
					{
					tempElement.nodeList.clear();
					tempElement.type=elType;
					tempElement.prop=elProp;
					if (numberStrings <=numberValues)
					{
						if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
						//Element description continues on the next line
						{
							//emit sendMessage("multiline");
							nodecount=numberStrings-2;
							nextLine=1;
							for (n=1;n<numberStrings-1;n++)
							{
								//emit sendMessage(line.section(",",n,n));
								tempElement.nodeList.append(line.section(",",n,n).toLong());
							}
							while (nextLine==1)
							{
								//emit sendMessage("cont");
								line = in.readLine();
								numberStrings=line.count(",")+1;
								keyword=line.section(",",0,0);
								
								if (keyword[0]=='*' && keyword[1]!='*') //line is a keyword
								{
									for (l=0;l< numberStrings;l++)
									{
									keywordlist.append(keyword=line.section(",",l,l).toUpper());
									}
								return(keywordlist);
								}
								else if (keyword[0]=='*' && keyword[1]=='*') //line is a comment
								{
								//do nothing
								}
								else
								{
									if (line.section(",",numberStrings-1,numberStrings-1).remove(" ").isEmpty())
									{
										nextLine=1;
										nodecount+=numberStrings-1;
										for (n=0;n<numberStrings-1;n++)
										{
											tempElement.nodeList.append(line.section(",",n,n).toLong());
										}
									}
									else
									{
										//emit sendMessage("nextLine");
										nextLine=0;
										nodecount+=numberStrings;
										for (n=0;n<numberStrings;n++)
										{
											tempElement.nodeList.append(line.section(",",n,n).toLong());
										}
										if (nodecount==numberValues-1)
										{
											elementList.append(tempElement);
											//emit sendMessage("el OK");
										}
										else
										{
											emit sendMessage(tr("Wrong number of Nodes for element %1. Element will be skipped").arg(tempElement.label)); 
										}
									}
								
								}
							}
						}
						else if (numberStrings < numberValues)
						{
							emit sendMessage(tr("Wrong number of Nodes for element %1. Element will be skipped").arg(tempElement.label)); 
						}
						else
						{
							
							for (n=1;n<=numberStrings;n++)
							{
								tempElement.nodeList.append(line.section(",",n,n).toLong());
							}
							tempElement.type=1;
							tempElement.prop=elProp;
							elementList.append(tempElement);
						}
					}
					else 
					{
                                                        emit sendMessage(tr("More Nodes than needed for element %1. Element will be skipped").arg(tempElement.label));
					}
					}
				}
				
				
			}
		
	}
keywordlist.prepend("EOF");
return(keywordlist);
}
//*********************************************************************
/*!Reads an unknown keyword section from an abaqus/ccx inp-file and returns the next keyword*/
QStringList femodel::readCCXUnknown(QTextStream &in)
{

QString line,keyword;
QStringList keywordlist;
keywordlist.clear();
int numberStrings;
//long k;
while (!in.atEnd())
	{
		line = in.readLine();
		
			numberStrings=line.count(",")+1;
			keyword=line.section(",",0,0);
			if (keyword[0]=='*' && keyword[1]!='*') //line is a keyword
			{
				for (int l=0;l< numberStrings;l++)
				{
					keywordlist.append(keyword=line.section(",",l,l).toUpper());
				}
				return(keywordlist);
				
			}
			else if (keyword[0]=='*' && keyword[1]=='*') //line is a comment
			{
			//do nothing
			}
			else
			{
				
			}
		
	}
keywordlist.prepend("EOF");
return(keywordlist);
}
//*************************************************************************
//!Reads an include file 
QStringList femodel::readCCXInclude(const QString &file, const QStringList &options, const QStringList &currentKeywordList)
{

return(currentKeywordList);
}
//***************************************************************
/*! Checks wether a line is a comment*/
bool femodel::readCCXisComment(const QString &line)
{
        if (line[0]=='*' && line[1]=='*') //line is a comment
		return(true);
	else
		return(false);
}
//*********************************************************************************
int femodel::getIntElType(const QString &type)
{
    int elType=-1;


    if(type=="C3D8")
    {
            elType=1;
    }
    if(type=="C3D8R")
    {
            elType=1;
    }
    if(type=="C3D4")
    {
            elType=3;
    }
    if(type=="C3D10")
    {
            elType=6;
    }
    if(type=="C3D10M")
    {
            elType=6;
    }
    if(type=="C3D20")
    {
            elType=4;
    }
    if(type=="C3D20R")
    {
            elType=4;
    }
    if (elType<0)
    {
            
            emit sendMessage(QString("Unknown element type %1").arg(type));
    }
    return(elType);
}
