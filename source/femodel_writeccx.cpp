/***************************************************************************
 *   Copyright (C) 2008 by Patrick Prokopczuk                              *
 *   prokopczuk@gmx.de                                                     *
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
//*****************************************************************************
/*! Writes the current mesh into an CCX input file*/
void femodel::writeMeshAbq(const QString &fileName,int *etypes,long offset=0)
{
                //File Output
    QList<long> etype1list;
    QList<long> etype3list;
    QList<long> etype4list;
    QList<long> etype6list;
    QList<long> etype7list;


    //int currentElType;
    QString fout;
    //int counter=0;
    etype1list.clear();
    etype3list.clear();
    etype4list.clear();
    etype6list.clear();
    etype7list.clear();
    QFile MyFile( fileName );
    if (MyFile.open (QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream( &MyFile );
        //Writing the header
        //stream << frdHeader(fileName);
        //Writing nodes
        stream << "*NODE,NSET=all\n";
        for ( long i = 0; i < nodeList.size(); i++)
        {
                //stream << " -1";
                stream.setFieldWidth(10);
                                stream << QString::number(nodeList[i].getLabel()+offset) +",";
                stream.setFieldWidth(12);
                stream << QString::number(nodeList[i].getCoordinate1()) +",";
                stream << QString::number(nodeList[i].getCoordinate2()) +",";
                stream << QString::number(nodeList[i].getCoordinate3()) ;
                stream.setFieldWidth(0);
                stream << "\n";
        }
        // **************************************************************************
        // Writing the nodesets
        for ( long s = 0; s < setList.size(); s++)
        {

                stream << QString("*NSET,NSET=%1\n").arg(setList[s].name);
                for (long t=0;t<setList[s].nodeList.size();t++)
                {
                    stream << QString::number(setList[s].nodeList[t]) +",\n";
                }
        }
        //Writing elements
        //Create a list for each element type
        for ( long e = 0; e < elementList.size(); e++)
        {
            switch (elementList[e].type)
            {
                case 1:
                    etype1list.append(e);
                break;
                case 3:
                    etype3list.append(e);
                break;
                case 4:
                    etype4list.append(e);
                break;
                case 6:
                    etype6list.append(e);
                break;
                case 7:
                    etype7list.append(e);
                break;
            }
         }
//********************************
//Writing hex8 elements
                if (etype1list.size()>0)
                   {
                       etypes[1]=1;
                                        stream << "*ELEMENT,ELSET=ETYPE1,TYPE=C3D8\n";
                                        for ( long k = 0; k < etype1list.size(); k++)
                                        {
                                                stream.setFieldWidth(10);
                                                stream << QString::number(elementList[etype1list[k]].getLabel()+offset)+",";
                                                stream.setFieldWidth(10);
                                                for (int n=0;n<8;n++)
                                                {
                                                        stream << QString::number(elementList[etype1list[k]].nodeList[n]+offset)+",";
                                                }
                                                stream.setFieldWidth(0);
                                                stream << "\n";

                                        }
                   }
//********************************
//Writing hex20 elements
        if (etype4list.size()>0)
        {
            etypes[4]=1;
            stream << "*ELEMENT,ELSET=ETYPE4,TYPE=C3D20R\n";
            for ( long k = 0; k < etype4list.size(); k++)
            {
                stream.setFieldWidth(10);
                                stream << QString::number(elementList[etype4list[k]].getLabel()+offset)+",";
                stream.setFieldWidth(10);
                for (int n=0;n<10;n++)
                {
                                        stream << QString::number(elementList[etype4list[k]].nodeList[n]+offset)+",";
                }
                stream.setFieldWidth(0);
                stream << "\n";
                stream.setFieldWidth(10);
                for (int m=10;m<12;m++)
                {
                                        stream << QString::number(elementList[etype4list[k]].nodeList[m]+offset)+",";
                }
            //Node Ordering differs between frd and inp format!!
                                stream << QString::number(elementList[etype4list[k]].nodeList[16]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[17]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[18]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[19]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[12]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[13]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[14]+offset)+",";
                                stream << QString::number(elementList[etype4list[k]].nodeList[15]+offset);
                stream.setFieldWidth(0);
                stream << "\n";
            }
        }
//********************************
//********************************
        //Writing tet4 elements
                if (etype3list.size()>0)
                {
                    etypes[3]=1;
                    stream << "*ELEMENT,ELSET=ETYPE3,TYPE=C3D4\n";
                    for ( long k = 0; k < etype3list.size(); k++)
                    {
                        stream.setFieldWidth(10);
                                        stream << QString::number(elementList[etype3list[k]].getLabel()+offset)+",";
                        stream.setFieldWidth(10);
                        for (int n=0;n<4;n++)
                        {
                                                stream << QString::number(elementList[etype3list[k]].nodeList[n]+offset)+",";
                        }
                        stream.setFieldWidth(0);
                        stream << "\n";
                        stream.setFieldWidth(10);
                    }
                }
        //*************************************************
//Writing tet10 elements
        if (etype6list.size()>0)
        {
            etypes[6]=1;
            stream << "*ELEMENT,ELSET=ETYPE6,TYPE=C3D10\n";
            for ( long k = 0; k < etype6list.size(); k++)
            {
                stream.setFieldWidth(10);
                                stream << QString::number(elementList[etype6list[k]].getLabel()+offset)+",";
                stream.setFieldWidth(10);
                for (int n=0;n<10;n++)
                {
                                        stream << QString::number(elementList[etype6list[k]].nodeList[n]+offset)+",";
                }
                stream.setFieldWidth(0);
                stream << "\n";
                stream.setFieldWidth(10);
            }
        }
//*************************************************
//********************************
                //Writing tria3 elements
                        if (etype7list.size()>0)
                        {
                            etypes[7]=1;
                            //Tria 3 elements are not supported in ccx, assume a plane stress element for abaqus
                            stream << "*ELEMENT,ELSET=ETYPE7,TYPE=CPS3\n";
                            for ( long k = 0; k < etype7list.size(); k++)
                            {
                                stream.setFieldWidth(10);
                                stream << QString::number(elementList[etype7list[k]].getLabel()+offset)+",";
                                stream.setFieldWidth(10);
                                for (int n=0;n<3;n++)
                                {
                                    stream << QString::number(elementList[etype7list[k]].nodeList[n]+offset)+",";
                                }
                                stream.setFieldWidth(0);
                                stream << "\n";
                                stream.setFieldWidth(10);
                            }
                        }
                //*************************************************
    //Write the EALL ELSET
                        stream << "*****************************************\n";
                        stream << "*ELSET,ELSET=all\n";
                        for (int k=0;k<8;k++)
                        {
                            if (etypes[k]==1)
                            {
                                stream << "ETYPE"+QString::number(k) << ",\n";
                            }
                        }

        stream.setFieldWidth(0);
//Writimg the mpc's
        stream << "******************************************\n";
        stream << "**MPCs\n";
        for (long d=0; d<mpcList.size();d++)
        {
            stream << "*EQUATION\n2,\n";
            stream << QString::number(mpcList[d].dependent) + ", " + QString::number(mpcList[d].dof) + ", -1," +
                      QString::number(mpcList[d].independent) + ", " + QString::number(mpcList[d].dof) + ", 1\n" ;
        }

//********************************************************
//Writing the surfaces
        stream << "*******************************************\n";
        for (long d=0; d<setList.size();d++)
        {
            if (setList[d].faceIndexList.size()>0)
            {
            stream << "*SURFACE, NAME=";
            stream << setList[d].name << "\n";
            for (long f=0 ;f<setList[d].faceIndexList.size();f++)
            {
                stream << faceList[setList[d].faceIndexList[f]].parentElement << " ,S";
                stream << faceList[setList[d].faceIndexList[f]].faceNumber << ",\n";
            }
            }
        }
        MyFile.close();
    }
}
//************************************
/*!Generates an abq/ccx msh file from the current model*/
void femodel::exportMeshAbq(const QString &setname,long offset)
{
    int types[21];
    QString meshFile=openFile.append(".mesh.abq");
        writeMeshAbq(meshFile,types,offset);
    emit sendMessage(QString("Mesh exported in file %1 ").arg(meshFile));
}
//*************************************
/*!Generates a dummy input filefrom the current model*/
void femodel::exportDummyAbqInputData(const QString &fileName)
{
    int types[21];
    //write the mesh (nodes and elements, including sets)
    writeMeshAbq(fileName,types);

    QFile MyFile( fileName );
    if (MyFile.open (QIODevice::Append | QIODevice::Text))
    {
        QTextStream stream( &MyFile);


        //Writing Material
                stream << "******************************************\n";
                stream << "**Material\n";


                for (long b=0; b<materialList.size();b++)
                {
                    stream << "*Material,NAME=";
                    stream << materialList[b].name << "\n";
                    if (materialList[b].elasticDataList.size()>0)
                    {
                         stream << "*ELASTIC\n";
                        for (int e=0;e<materialList[b].elasticDataList.size();e++)
                         {
                        stream << QString::number(materialList[b].elasticDataList[e].yModulus) + ", "
                                + QString::number(materialList[b].elasticDataList[e].poison) + + ", "
                                + QString::number(materialList[b].elasticDataList[e].temperature) + "\n";
                        }
                        stream << "**********\n";
                    }
                    if (materialList[b].densityList.size()>0)
                    {
                         stream << "*DENSITY\n";
                        for (int e=0;e<materialList[b].densityList.size();e++)
                         {
                        stream << QString::number(materialList[b].densityList[e].value) + ", "
                                + QString::number(materialList[b].densityList[e].temperature) + "\n";
                        }
                        stream << "**********\n";
                    }
                }
        //Writing Sections
                stream << "******************************************\n";
                stream << "**FE-Sectionss\n";

                for (long b=0; b<feSectionList.size();b++)
                {
                    stream << "*SOLID SECTION,ELSET=" +
                              feSectionList[b].set + " , MATERIAL=" +
                              feSectionList[b].material + "\n";
                }
        //Writing BCs
                stream << "******************************************\n";
                stream << "**BCs\n";
                stream << "*BOUNDARY\n";
                for (long b=0; b<bcList.size();b++)
                {
                    stream << bcList[b].nodeset + " , " +QString::number(bcList[b].firstDof) + ", " + QString::number(bcList[b].lastDof) + "\n";
                }
        //************************************
        //Frequency step
        stream << "*******************************************\n";
        stream << "**Frequency step \n";
        stream << "*STEP \n";
        stream << "*FREQUENCY\n";
        stream << "5\n";
        stream << "*NODE OUTPUT\n";
        stream << "U\n";
        stream << "*ELEMENT OUTPUT\n";
        stream << "S,E\n";
        stream << "*END STEP\n";
        //STatic STep
        stream << "*******************************************\n";
        stream << "**Frequency step \n";
        stream << "*STEP \n";
        stream << "*STATIC\n";
        stream << "*******";
                stream << "**Loads\n";
                //******************
                //CLOADS
                stream << "*CLOAD\n";
                for (long b=0; b<loadList.size();b++)
                {
                    if (loadList[b].type==0)
                    {
                    stream << loadList[b].targetSet + " , " +QString::number(loadList[b].direction) + ", " + QString::number(loadList[b].value) + "\n";
                    }
                }
                //******************
                //Pressure Loads
                stream << "*DSLOAD\n";
                for (long b=0; b<loadList.size();b++)
                {
                    if (loadList[b].type==1)
                    {
                    stream << loadList[b].targetSet + " , " + QString::number(loadList[b].value) + "\n";
                    }
                }

                //*******************
                //Centrifugal Loads
                stream << "*DLOAD\n";
                for (long b=0; b<loadList.size();b++)
                {
                    if (loadList[b].type==2)
                    {
                    stream << loadList[b].targetSet + " , CENTRIF," + QString::number(pow(loadList[b].value/60*2*M_PI,2)) +"," +
                            loadList[b].getPoint1String() +","+ loadList[b].getPoint2String()+ "\n";
                    }
                }


                //***********************
        stream << "*NODE OUTPUT\n";
        stream << "U\n";
        stream << "*ELEMENT OUTPUT\n";
        stream << "S,E\n";
        stream << "*END STEP\n";
        MyFile.close();
    }
    //Create a batch file for CCX
    //ccxLocation=this->parent.ccxLocation// x"C:\\Programme\\bConverged\\Calculix\\ccx\\ccx.exe";
    //if (ccxLocation !="")
    {
        QString ccxLocationMod=ccxLocation.replace("Program Files","\"Program Files\"");
        QString batchFileName=fileName + ".bat";
        QString inputName= fileName;
        inputName.remove(".inp");
        QFile batchFile( batchFileName );
        if (batchFile.open (QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream batchstream( &batchFile);

            //Command for executing ccx
                //OPening a command shell
                    batchstream << "%windir%\\system32\\cmd.exe /t:F1 /k";
                    //execute ccx
                    batchstream << " \"" << ccxLocationMod << " "  << inputName << "\"\n" ;
        }

    }


}
//*************************************************************************
