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
#include "femodel.h"
#include <math.h>
#include "node.h"
#include "pvdhandler.h"
#include "gauss_solver.h"

/*!Interpolates results from an frd results file on all nodes */
void femodel::interpolateFrdResults(const QString &file)
{
    bool xml=false;
    femodel *resultmodel=new femodel();
//read the mesh of the results file
        setCursor(1);
    resultmodel->readfrd(file);
//////////////////////////////
    //Check for existing results
    if (resultmodel->resultList.size()==0)
    {
        emit sendMessage(QString("No results found in file %1").arg(file));
        emit sendMessage("Calculating parameters only");
    }
    else
    {
        emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
        emit sendMessage("Starting Interpolation");
    }
        //**********
        //create the Interpolators
        createInterpolators(resultmodel,false);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,0);
        //**********
    emit sendMessage(QString("Interpolation finished"));
    delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an frd results file on outside faces */
/*Create an abq input file from the interpolated result values*/
void femodel::interpolateFrdResultsFace(const QString &file)
{
    bool xml=false;
    femodel *resultmodel=new femodel();
//read the mesh of the results file
    setCursor(1);
    resultmodel->readfrd(file);
//////////////////////////////
    //Check for existing results
    if (resultmodel->resultList.size()==0)
    {
        emit sendMessage(QString("No results found in file %1").arg(file));
        emit sendMessage("Calculating parameters only");
    }
    else
    {
        emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
        emit sendMessage("Starting Interpolation");
    }
///////////////////////////////
        //***********
        createInterpolatorsFace(resultmodel,false);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,0);
        //**********
    emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //*********
    QString parameterFile=openFile.remove(".frd");
    parameterFile.append("-interpol.xml");
    //emit sendMessage(parameterFile);
        emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        //Cleaning up
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an unstructured vtk results file */
void femodel::interpolateVtkMultiblockResults(const QString &file)
{
bool xml=false;
//read the mesh of the results file
setCursor(1);
femodel *resultmodel=new femodel();
        resultmodel->importVtkMultiblock(file);
//////////////////////////////
        //Check for existing results
        if (resultmodel->resultList.size()==0)
        {
                emit sendMessage(QString("No results found in file %1").arg(file));
                emit sendMessage("Calculating parameters only");
        }
        else
        {
                emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
                emit sendMessage("Starting Interpolation");
        }
///////////////////////////////
        //*************
        createInterpolators(resultmodel,false);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,2);
        //**********
        emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an unstructured vtk results file */
void femodel::interpolateVtkPvdResults(const QString &file)
{


}
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an unstructured vtk results file */
void femodel::interpolateVtkUnstructuredResults(const QString &file)
{
bool xml=false;
interpolatorList.clear();
//read the mesh of the results file
setCursor(1);
femodel *resultmodel=new femodel();
        resultmodel->importVtkUnstructured(file);
//////////////////////////////
        //Check for existing results
        if (resultmodel->resultList.size()==0)
        {
                emit sendMessage(QString("No results found in file %1").arg(file));
                emit sendMessage("Calculating parameters only");
        }
        else
        {
                emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
                emit sendMessage("Starting Interpolation");
        }
///////////////////////////////
        createInterpolators(resultmodel,false);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,1);
        //**********
        emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an frd results file on outside faces */
/*Create an abq input file from the interpolated result values*/
void femodel::interpolateVtkUnstructuredResultsFace(const QString &file)
{
bool xml=false;
//read the mesh of the results file
setCursor(1);
femodel *resultmodel=new femodel();
        resultmodel->importVtkUnstructured(file);
//////////////////////////////
        //Check for existing results
        if (resultmodel->resultList.size()==0)
        {
                emit sendMessage(QString("No results found in file %1").arg(file));
                emit sendMessage("Calculating parameters only");
        }
        else
        {
                emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
                emit sendMessage("Starting Interpolation");
        }
///////////////////////////////
        createInterpolatorsFace(resultmodel,false);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,1);
        //**********
        emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an vtk multiblock results file on outside faces */
/*Create an abq input file from the interpolated result values*/
void femodel::interpolateVtkMultiblockResultsFace(const QString &file,bool outsideResult)
{
bool xml=false;
interpolatorList.clear();
//read the mesh of the results file
setCursor(1);
femodel *resultmodel=new femodel();
        resultmodel->importVtkMultiblock(file);
//////////////////////////////
        //Check for existing results
        if (resultmodel->resultList.size()==0)
        {
                emit sendMessage(QString("No results found in file %1").arg(file));
                emit sendMessage("Calculating parameters only");
        }
        else
        {
                emit sendMessage(QString("Found %1 results in file %2").arg(resultmodel->resultList.size()).arg(file));
                emit sendMessage("Starting Interpolation");
        }
///////////////////////////////
        createInterpolatorsFace(resultmodel,outsideResult);
        //**********
        //interpolate results
        interpolateAndWriteResults(resultmodel,2);
        //**********
        emit sendMessage(QString("Interpolation finished"));
        delete resultmodel;
        //**********
        if (xml==true)
        {
        //Write an xml file with the interpolator data
            QString parameterFile=openFile.remove(".frd");
            parameterFile.append("-interpol.xml");
            writeInterpolatorData(parameterFile,file);
            //************
        }
        interpolatorList.clear();
        setCursor(0);
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/*!Interpolates results from an vtk multiblock results file on outside faces */
/*Create an abq input file from the interpolated result values*/
void femodel::interpolateVtkPvdResultsFace(const QString &file)
{
    QStringList resultFiles;
    resultFiles.clear();
///************************************************
//Parsing the pvd file
    QFile *pvdFile =new QFile();
    pvdFile->setFileName(file);

    QXmlSimpleReader xmlReader;
    QXmlInputSource *source = new QXmlInputSource(pvdFile);

    pvdHandler *handler = new pvdHandler();
    xmlReader.setContentHandler(handler);
    xmlReader.setErrorHandler(handler);

    bool ok = xmlReader.parse(source);

    if (!ok)
    {
        std::cout << "Parsing failed." << std::endl;
        return;
    }
//************************************************

}
//////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
void femodel::findParaTet4(femodel *resultmodel,long resultElIndex, vector3d newCoords,double *para)
{
    vector3d resultNodeCoords[4];
    //long label=resultmodel->elementList[resultElIndex].getLabel();

    //double xo,eo,zo;
    double f[4];
    double Matrix[16];
    double dx[4];
    double d[4];
    int ip[4];
    int* marke;
     //Ermitteln der Eckpunktkoordinaten
     for (int n=0;n<4;n++)
     {
            resultNodeCoords[n]=resultmodel->getNodeCoords(resultmodel->elementList[resultElIndex].nodeList[n]);
     }
//    Direkte Ermittlung der Parameter xi[4]
//     mit dem Gaussschen Eleiminationsverfahren fuer lineare Gleichungssysteme
//     Matrix ermitteln = Matrix des Gleichungssystemes
           Matrix[0]=resultNodeCoords[0].x;
           Matrix[1]=resultNodeCoords[1].x;
           Matrix[2]=resultNodeCoords[2].x;
           Matrix[3]=resultNodeCoords[3].x;
           Matrix[4]=resultNodeCoords[0].y;
           Matrix[5]=resultNodeCoords[1].y;
           Matrix[6]=resultNodeCoords[2].y;
           Matrix[7]=resultNodeCoords[3].y;
           Matrix[8]=resultNodeCoords[0].z;
           Matrix[9]=resultNodeCoords[1].z;
           Matrix[10]=resultNodeCoords[2].z;
           Matrix[11]=resultNodeCoords[3].z;
           Matrix[12]=1;
           Matrix[13]=1;
           Matrix[14]=1;
           Matrix[15]=1;
          // Aufstellen der rechten Seite des Gleichungssystemes
           f[0]=newCoords.x;
           f[1]=newCoords.y;
           f[2]=newCoords.z;
           f[3]=1.;
    //Solve the system
          gauss_solver::FGAUSS(4,Matrix,4,f,dx,marke,d,ip);
   //     Neuer Parametersatz
         para[0]=dx[0];
         para[1]=dx[1];
         para[2]=dx[2];
         para[3]=dx[3];
         return;
}
//*******************************************************************************+
void femodel::findParaHe8(femodel *resultmodel,long resultElIndex, vector3d newCoords,double *para)
{
    vector3d resultNodeCoords[8];
    //long label=resultmodel->elementList[resultElIndex].getLabel();
    double l1h8,l2h8,l3h8,l4h8,l5h8,l6h8,l7h8,l8h8;
    double xo,eo,zo;
    double a[9];
    double f[3];
    double dx[3];
    dx[0]=0;
    dx[1]=0;
    dx[2]=0;
    //emit sendMessage(QString("Parameters for He8 element: %1").arg(label));

     //Ermitteln der Eckpunktkoordinaten
     for (int n=0;n<8;n++)
     {
        resultNodeCoords[n]=resultmodel->getNodeCoords(resultmodel->elementList[resultElIndex].nodeList[n]);
     }
     //*****************
    //Startloesung
     double xi=0.;
     double et=0.;
     double ze=0.;
    //Start der Iteration
     for (int i=1;i<=10;i++)
     {
        xo=xi;
        eo=et;
        zo=ze;
    //*******************
    //Bestimmen der Werte der Ansatzfunktionen f�r aktuellen Parametersatz
        l1h8=(1.-xi)*(1.-et)*(1.-ze)/8.;
        l2h8=(1.+xi)*(1.-et)*(1.-ze)/8.;
        l3h8=(1.+xi)*(1.+et)*(1.-ze)/8.;
        l4h8=(1.-xi)*(1.+et)*(1.-ze)/8.;
        l5h8=(1.-xi)*(1.-et)*(1.+ze)/8.;
        l6h8=(1.+xi)*(1.-et)*(1.+ze)/8.;
        l7h8=(1.+xi)*(1.+et)*(1.+ze)/8.;
        l8h8=(1.-xi)*(1.+et)*(1.+ze)/8.;
     //************************
     //Funktionalmatrix der Koordinatenfunktionen aufstellen
     //(Ableitung jeder Funktion nach jeder Variablen)
         a[0]=(-(1-et)*(1-ze)*resultNodeCoords[0].x+(1-et)*(1-ze)*resultNodeCoords[1].x
                +(1+et)*(1-ze)*resultNodeCoords[2].x-(1+et)*(1-ze)*resultNodeCoords[3].x
                -(1-et)*(1+ze)*resultNodeCoords[4].x+(1-et)*(1+ze)*resultNodeCoords[5].x
                +(1+et)*(1+ze)*resultNodeCoords[6].x-(1+et)*(1+ze)*resultNodeCoords[7].x)/8.;

     a[1]=(-(1-xi)*(1-ze)*resultNodeCoords[0].x-(1+xi)*(1-ze)*resultNodeCoords[1].x
                +(1+xi)*(1-ze)*resultNodeCoords[2].x+(1-xi)*(1-ze)*resultNodeCoords[3].x
                -(1-xi)*(1+ze)*resultNodeCoords[4].x-(1+xi)*(1+ze)*resultNodeCoords[5].x
                +(1+xi)*(1+ze)*resultNodeCoords[6].x+(1-xi)*(1+ze)*resultNodeCoords[7].x)/8.;

     a[2]=(-(1-xi)*(1-et)*resultNodeCoords[0].x-(1+xi)*(1-et)*resultNodeCoords[1].x
                -(1+xi)*(1+et)*resultNodeCoords[2].x-(1-xi)*(1+et)*resultNodeCoords[3].x
                +(1-xi)*(1-et)*resultNodeCoords[4].x+(1+xi)*(1-et)*resultNodeCoords[5].x
                +(1+xi)*(1+et)*resultNodeCoords[6].x+(1-xi)*(1+et)*resultNodeCoords[7].x)/8.;

     a[3]=(-(1-et)*(1-ze)*resultNodeCoords[0].y+(1-et)*(1-ze)*resultNodeCoords[1].y
                +(1+et)*(1-ze)*resultNodeCoords[2].y-(1+et)*(1-ze)*resultNodeCoords[3].y
                -(1-et)*(1+ze)*resultNodeCoords[4].y+(1-et)*(1+ze)*resultNodeCoords[5].y
                +(1+et)*(1+ze)*resultNodeCoords[6].y-(1+et)*(1+ze)*resultNodeCoords[7].y)/8.
                ;
     a[4]=(-(1-xi)*(1-ze)*resultNodeCoords[0].y-(1+xi)*(1-ze)*resultNodeCoords[1].y
                +(1+xi)*(1-ze)*resultNodeCoords[2].y+(1-xi)*(1-ze)*resultNodeCoords[3].y
                -(1-xi)*(1+ze)*resultNodeCoords[4].y-(1+xi)*(1+ze)*resultNodeCoords[5].y
                +(1+xi)*(1+ze)*resultNodeCoords[6].y+(1-xi)*(1+ze)*resultNodeCoords[7].y)/8.;

     a[5]=(-(1-xi)*(1-et)*resultNodeCoords[0].y-(1+xi)*(1-et)*resultNodeCoords[1].y
                -(1+xi)*(1+et)*resultNodeCoords[2].y-(1-xi)*(1+et)*resultNodeCoords[3].y
                +(1-xi)*(1-et)*resultNodeCoords[4].y+(1+xi)*(1-et)*resultNodeCoords[5].y
                +(1+xi)*(1+et)*resultNodeCoords[6].y+(1-xi)*(1+et)*resultNodeCoords[7].y)/8.;

     a[6]=(-(1-et)*(1-ze)*resultNodeCoords[0].z+(1-et)*(1-ze)*resultNodeCoords[1].z
                +(1+et)*(1-ze)*resultNodeCoords[2].z-(1+et)*(1-ze)*resultNodeCoords[3].z
                -(1-et)*(1+ze)*resultNodeCoords[4].z+(1-et)*(1+ze)*resultNodeCoords[5].z
                +(1+et)*(1+ze)*resultNodeCoords[6].z-(1+et)*(1+ze)*resultNodeCoords[7].z)/8.;

     a[7]=(-(1-xi)*(1-ze)*resultNodeCoords[0].z-(1+xi)*(1-ze)*resultNodeCoords[1].z
                +(1+xi)*(1-ze)*resultNodeCoords[2].z+(1-xi)*(1-ze)*resultNodeCoords[3].z
                -(1-xi)*(1+ze)*resultNodeCoords[4].z-(1+xi)*(1+ze)*resultNodeCoords[5].z
                +(1+xi)*(1+ze)*resultNodeCoords[6].z+(1-xi)*(1+ze)*resultNodeCoords[7].z)/8.;

     a[8]=(-(1-xi)*(1-et)*resultNodeCoords[0].z-(1+xi)*(1-et)*resultNodeCoords[1].z
                -(1+xi)*(1+et)*resultNodeCoords[2].z-(1-xi)*(1+et)*resultNodeCoords[3].z
                +(1-xi)*(1-et)*resultNodeCoords[4].z+(1+xi)*(1-et)*resultNodeCoords[5].z
                +(1+xi)*(1+et)*resultNodeCoords[6].z+(1-xi)*(1+et)*resultNodeCoords[7].z)/8.;
     //*******************************************************
     //Funktionen fuer die 3 Koordinaten = rechte Seite des Systems
     f[0]=-(l1h8*resultNodeCoords[0].x+l2h8*resultNodeCoords[1].x+l3h8*resultNodeCoords[2].x+
            l4h8*resultNodeCoords[3].x+l5h8*resultNodeCoords[4].x+l6h8*resultNodeCoords[5].x+
            l7h8*resultNodeCoords[6].x+l8h8*resultNodeCoords[7].x-newCoords.x);
     f[1]=-(l1h8*resultNodeCoords[0].y+l2h8*resultNodeCoords[1].y+l3h8*resultNodeCoords[2].y+
            l4h8*resultNodeCoords[3].y+l5h8*resultNodeCoords[4].y+l6h8*resultNodeCoords[5].y+
            l7h8*resultNodeCoords[6].y+l8h8*resultNodeCoords[7].y-newCoords.y);
     f[2]=-(l1h8*resultNodeCoords[0].z+l2h8*resultNodeCoords[1].z+l3h8*resultNodeCoords[2].z+
            l4h8*resultNodeCoords[3].z+l5h8*resultNodeCoords[4].z+l6h8*resultNodeCoords[5].z+
            l7h8*resultNodeCoords[6].z+l8h8*resultNodeCoords[7].z-newCoords.z);

     //*********************************************************************
     //Loesen des Linearen Gleichungssystemes
     solveLinGl3(a,f,dx);
     //   Neuer Parametersatz
     xi=xo+dx[0];
     et=eo+dx[1];
     ze=zo+dx[2];
    //    checken ob alle Aenderungen im Parametersatz <10e-5 sind
         if( ((fabs(xi-xo) < 1E-5) && (fabs(et-eo) < 1E-5))
         &&(fabs(ze-zo) < 1E-5) )
         {
         //Verlassen der Iteration wenn Aenderungen klein genug
         break;
         }
    }
    para[0]=xi;
    para[1]=et;
    para[2]=ze;
    //emit sendMessage(QString("Parameter: %1 ; %2 ; %3 ;").arg(xi).arg(et).arg(ze));

}
//***********************************************
/*!L�sen eines linearen Gleichungssystems der Groesse 3*/
int femodel::solveLinGl3(double *a,double *f,double *x)
{
//    a=Matrix, f=rechte Seite,x= Loesung
//     Determinante und entwickelte Determinanten
      double detA, detR1, detR2, detR3;
//-----------------------------------------------------------------------
      int flag=1;
//     Determinante der Funktionalmatrix (entwickelt nach der ersten Spalte)
      detA=a[0]*(a[4]*a[8]-a[5]*a[7])
          -a[3]*(a[1]*a[8]-a[7]*a[2])
          +a[6]*(a[1]*a[5]-a[4]*a[2]);
//     Flag fuer Singularitaet setzen, wenn detA sehr klein ist,
//     die Detrminante also quasi singulaer ist
      if (fabs(detA) < 1E-15)
      {
         flag=0;
                 //emit sendMessage("singulaer");
         return(flag);
      }
//    Determinanten (entwickelt nach der esten Spalte)
      detR1=f[0]*(a[4]*a[8]-a[5]*a[7])
              -f[1]*(a[1]*a[8]-a[7]*a[2])
              +f[2]*(a[1]*a[5]-a[4]*a[2]);
      detR2=a[0]*(f[1]*a[8]-a[5]*f[2])
              -a[3]*(f[0]*a[8]-f[2]*a[2])
              +a[6]*(f[0]*a[5]-f[1]*a[2]);
      detR3=a[0]*(a[4]*f[2]-f[1]*a[7])
              -a[3]*(a[1]*f[2]-a[7]*f[0])
              +a[6]*(a[1]*f[1]-a[4]*f[0]);
//     Loesungsvektor
      x[0]=detR1/detA;
      x[1]=detR2/detA;
      x[2]=detR3/detA;
    return(flag);
}
//****************************************************************
/*! Create the interpolator object*/
void femodel::createInterpolators(femodel *resultmodel,bool outsideResults)
{
    double distance=1E+16;
    double paraIndex1=1E+16;
    double paraIndex=1E+16;
    double para[4],bestPara[4];
    //double n1length,n2length,n3length,d1,d2,d3;
    double dist;
    long index;
    long k;
    int e;
    QList<long> closestNodes;
    long elIndex,bestElIndex;
    vector3d origin,n1,n2,n3;
    //*********
    //Clear the interpolator list
    interpolatorList.clear();
    //*********
    //Loop over all new nodes
    for (long n=0; n< nodeList.size(); n++)
    {
            index=0;
            distance=1E+16;
            /////////////////////////////////////
            //Find the closest result node
            for (k=0; k< resultmodel->nodeList.size();k++)
            {
                    dist=nodeList[n].distanceToPoint(resultmodel->nodeList[k]);
                    if (dist<distance)
                    {
                            distance=dist;
                            index=k;
                    }
            }
            ////////////////////////////////////
            //Store the index of the closest result nodes in a list;
            closestNodes.append(index);
            //Get the coordinates of the result node
            origin=resultmodel->nodeList[index].getCoordinates();
            //emit sendMessage(QString("Closest result node for new node %1: %2").arg(nodeList[n].getLabel()).arg(resultmodel->nodeList[index].getLabel()));
            ////////////////////////////////////////////////
            //Get the element in which the new node lies (or the closest)
            //quadratic elements are treated like linear elements
            bestElIndex=resultmodel->getElementIndex(resultmodel->nodeList[index].adjacentElements[0]);
            for (e=0; e < resultmodel->nodeList[index].adjacentElements.size(); e++)
            {
                    elIndex=resultmodel->getElementIndex(resultmodel->nodeList[index].adjacentElements[e]);
                    //emit sendMessage(QString("Elements of result node %1: %2").arg(nodeList[index].getLabel()).arg(resultmodel->elementList[elIndex].getLabel()));
                    switch (resultmodel->elementList[elIndex].type)
                    {
                        //Hex8
                    case 1:
                            findParaHe8(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                            para[3]=0;
                    break;
                        //tet4
                    case 3:
                        findParaTet4(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                    break;
                        //Hex20 treated like Hex8 (midside nodes ignored)
                    case 4:
                            findParaHe8(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                            para[3]=0;
                    break;
                    case 6:
                        findParaTet4(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                    break;
                    }
                    //checken, ob Knoten im Element liegt, also ob alle Parameter
                    //vom Betrag kleiner als 1 sind.
                    //Wenn ja wird nicht weitergesucht.
                    if ((fabs(para[0]) <= 1) && (fabs(para[1]) <= 1) && (fabs(para[2]) <= 1) && (fabs(para[3]) <= 1))
                    {
                            bestElIndex=elIndex;
                            //emit sendMessage("inside");
                            bestPara[0]=para[0];
                            bestPara[1]=para[1];
                            bestPara[2]=para[2];
                            bestPara[3]=para[3];
                            break;
                    }
                    paraIndex1=fabs(para[0])+fabs(para[1])+fabs(para[2])+fabs(para[3]);
                    if (paraIndex1 < paraIndex)
                    {
                            paraIndex=paraIndex1;
                            bestElIndex=elIndex;
                            bestPara[0]=para[0];
                            bestPara[1]=para[1];
                            bestPara[2]=para[2];
                            bestPara[3]=para[3];
                    }
            }
            //Parameter auf Betraege von 1 begrenzen, um unsinnige Extrapolationen
            //zu verhindern. Damit wird quasi der Oberflaechenwert verwendet.
            if (bestPara[0] > 1) bestPara[0]=1.;
            if (bestPara[0] < -1) bestPara[0]=-1.;
            if (bestPara[1] > 1) bestPara[1]=1.;
            if (bestPara[1] < -1) bestPara[1]=-1.;
            if (bestPara[2] > 1) bestPara[2]=1.;
            if (bestPara[2] < -1) bestPara[2]=-1.;
            if (bestPara[3] > 1) bestPara[3]=1.;
            if (bestPara[3] < -1) bestPara[3]=-1.;
            //Ausgabe derWerte zu testzwecken
            //emit sendMessage(QString("Best element index for new node index %1: %2").arg(n).arg(bestElIndex));
            //emit sendMessage(QString("Parameters: %1 ; %2 ; %3 ; %4").arg(bestPara[0]).arg(bestPara[1]).arg(bestPara[2]).arg(bestPara[3]));
            tempInterpol.setData(n,bestElIndex,1,bestPara);
            interpolatorList.append(tempInterpol);
            ///////////////////////////////
    }
}
//*******************************************************
//****************************************************************
/*!Create the interpolator object*/
void femodel::createInterpolatorsFace(femodel *resultmodel, bool outsideResults)
{
    int resultType;
    double distance=1E+16;
    double paraIndex1=1E+16;
    double paraIndex=1E+16;
    double para[4],bestPara[4];
    //double n1length,n2length,n3length,d1,d2,d3;
    double dist;
    long index;
    long k;
    int e;
    QList<long> closestNodes;
    long elIndex,bestElIndex;
    vector3d origin,n1,n2,n3;
    //*********
    //Clear the interpolator list
    interpolatorList.clear();
    //*********
    //loop over all faces in the new mesh
    for (long n=0; n< faceList.size(); n++)
    {
    if (faceList[n].outside==true)
    {
            index=0;
            distance=1E+16;
            /////////////////////////////////////
            //Find the closest result node
            if (outsideResults==false)
            {
            for (k=0; k< resultmodel->nodeList.size();k++)
            {

                    dist=nodeList[n].distanceToPoint(resultmodel->nodeList[k]);
                    if (dist<distance)
                    {
                            distance=dist;
                            index=k;
                    }
            }
            }
            else
            {
                for (k=0; k< resultmodel->nodeList.size();k++)
                {
                    if (resultmodel->nodeList[k].isOutside()==true)
                    {
                        dist=nodeList[n].distanceToPoint(resultmodel->nodeList[k]);
                        if (dist<distance)
                        {
                                distance=dist;
                                index=k;
                        }
                    }
                }
            }
            ////////////////////////////////////
            //Store the index of the closest result nodes in a list;
            closestNodes.append(index);
            //Get the coordinates of the result node
            origin=resultmodel->nodeList[index].getCoordinates();
            //emit sendMessage(QString("Closest result node for new node %1: %2").arg(nodeList[n].getLabel()).arg(resultmodel->nodeList[index].getLabel()));
            ////////////////////////////////////////////////
            //Get the element in which the new node lies (or the closest)
            //quadratic elements are treated like linear elements
            bestElIndex=resultmodel->getElementIndex(resultmodel->nodeList[index].adjacentElements[0]);
            for (e=0; e < resultmodel->nodeList[index].adjacentElements.size(); e++)
            {
                    elIndex=resultmodel->getElementIndex(resultmodel->nodeList[index].adjacentElements[e]);
                    //emit sendMessage(QString("Elements of result node %1: %2").arg(nodeList[index].getLabel()).arg(resultmodel->elementList[elIndex].getLabel()));
                    switch (resultmodel->elementList[elIndex].type)
                    {
                        //Hex8
                    case 1:
                            findParaHe8(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                            para[3]=0;
                            resultType=1;
                    break;
                        //tet4
                    case 3:
                    break;
                        //Hex20 treated like Hex8 (midside nodes ignored)
                    case 4:
                            findParaHe8(resultmodel,elIndex,nodeList[n].getCoordinates(),para);
                            para[3]=0;
                            resultType=1;
                    break;
                    case 6:
                        //Tet10 treated like tet4 (midside nodes ignored)
                    break;
                    }
                    //checken, ob Knoten im Element liegt, also ob alle Parameter
                    //vom Betrag kleiner als 1 sind.
                    //Wenn ja wird nicht weitergesucht.
                    if ((fabs(para[0]) <= 1) && (fabs(para[1]) <= 1) && (fabs(para[2]) <= 1) && (fabs(para[3]) <= 1))
                    {
                            bestElIndex=elIndex;
                            //emit sendMessage("inside");
                            bestPara[0]=para[0];
                            bestPara[1]=para[1];
                            bestPara[2]=para[2];
                            bestPara[3]=para[3];
                            break;
                    }
                    paraIndex1=fabs(para[0])+fabs(para[1])+fabs(para[2])+fabs(para[3]);
                    if (paraIndex1 < paraIndex)
                    {
                            paraIndex=paraIndex1;
                            bestElIndex=elIndex;
                            bestPara[0]=para[0];
                            bestPara[1]=para[1];
                            bestPara[2]=para[2];
                            bestPara[3]=para[3];
                    }
            }
            //Parameter auf Betraege von 1 begrenzen, um unsinnige Extrapolationen
            //zu verhindern. Damit wird quasi der Oberflaechenwert verwendet.
            if (bestPara[0] > 1) bestPara[0]=1.;
            if (bestPara[0] < -1) bestPara[0]=-1.;
            if (bestPara[1] > 1) bestPara[1]=1.;
            if (bestPara[1] < -1) bestPara[1]=-1.;
            if (bestPara[2] > 1) bestPara[2]=1.;
            if (bestPara[2] < -1) bestPara[2]=-1.;
            if (bestPara[3] > 1) bestPara[3]=1.;
            if (bestPara[3] < -1) bestPara[3]=-1.;
            //Ausgabe derWerte zu testzwecken
            //emit sendMessage(QString("Best element index for new node index %1: %2").arg(n).arg(bestElIndex));
            //emit sendMessage(QString("Parameters: %1 ; %2 ; %3 ; %4").arg(bestPara[0]).arg(bestPara[1]).arg(bestPara[2]).arg(bestPara[3]));
            tempInterpol.setData(n,bestElIndex,resultType,bestPara);
            interpolatorList.append(tempInterpol);
            ///////////////////////////////
    }
    }
}
//*******************************************************
void femodel::interpolateAndWriteResults(femodel *resultmodel, int resulttype)
{
    long r,h,s;
    int interpolComponents;
    double value=0,et,xi=0,ze;
    double resultvalue[27];
    long end=0;
    //vector3d newvector3d;

    QString interpolFile=openFile;
    int index=openFile.lastIndexOf(".");

    if (index>0)
    {
        interpolFile.remove(index,(openFile.length()-index)*2);
    }
    interpolFile.append("-interpol.frd");
    writeMeshFrd(interpolFile,0);
    QFile MyFile(interpolFile);
    if (!MyFile.open (QIODevice::Append | QIODevice::Text))
    {
            return;
    }
    QTextStream stream( &MyFile );
    switch (resulttype)
    {
        //frd
    case 0:
        end=resultmodel->resultList.size();
    break;
        //vtu
    case 1:
        end=1;
    break;
        //vtmb
    case 2:
        end=1;
    break;
    }
    for (long w=0; w<end; w++)
    {
            //emit sendMessage(QString("Result %1: %2").arg(resultmodel->resultList[w].resultStep).arg(resultmodel->resultList[w].frqtime));
            switch (resulttype)
            {
                //frd
            case 0:
                resultmodel->readfrdresults(resultmodel->resultList[w].resultStep);
            break;
                //vtu
            case 1:
                resultmodel->readVtkResults(resultmodel->resultList[0].resultStep);
            break;
                //vtmb
            case 2:
                resultmodel->readVtkResults(resultmodel->resultList[0].resultStep);
            break;
            }
            for (r=0; r<resultmodel->resultDataList.size(); r++)
            {
                    //emit sendMessage(QString("number results:%1").arg(resultmodel->resultDataList.size()));
                    //emit sendMessage(QString("Interpolating %1 results").arg(resultmodel->resultDataList[r].resultLabel));
                    if (resultmodel->resultDataList[r].resultLabel.contains("DISP"))
                    {
                            interpolComponents=3;
                    }
                    else
                    {
                            if (resultmodel->resultDataList[r].resultLabel.contains("STRESS"))
                                    interpolComponents=6;
                            else
                                    interpolComponents=resultmodel->resultDataList[r].numberComponents;
                    }
                    //emit sendMessage(QString("Interpolating %1 components for %2 nodes").arg(interpolComponents).arg(interpolatorList.size()));
                    stream << resultHeader(w+1,resultmodel->resultList[w].frqtime,resultmodel->resultDataList[r].resultLabel,interpolComponents,resultmodel->resultDataList[r].componentLabels);
                    for (h=0 ; h<interpolatorList.size(); h++)
                    {
                            stream << QString(" -1%1").arg(nodeList[interpolatorList[h].newNode].getLabel(),10);
                            for (s=0;s<interpolComponents;s++)
                            {
                                switch (interpolatorList[h].resultElType)
                                {
                                    //hex8
                                case 1:
                                // emit sendMessage(QString("%1 %2").arg(r).arg(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[0]));
                                resultvalue[0]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[0])].resultValues[s];
                                resultvalue[1]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[1])].resultValues[s];
                                resultvalue[2]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[2])].resultValues[s];
                                resultvalue[3]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[3])].resultValues[s];
                                resultvalue[4]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[4])].resultValues[s];
                                resultvalue[5]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[5])].resultValues[s];
                                resultvalue[6]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[6])].resultValues[s];
                                resultvalue[7]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[7])].resultValues[s];
                                xi=interpolatorList[h].parameter[0];
                                et=interpolatorList[h].parameter[1];
                                ze=interpolatorList[h].parameter[2];
                                value=((1.-xi)*(1.-et)*(1.-ze)/8.)*resultvalue[0]+
                                ((1.+xi)*(1.-et)*(1.-ze)/8.)*resultvalue[1]+
                                ((1.+xi)*(1.+et)*(1.-ze)/8.)*resultvalue[2]+
                                ((1.-xi)*(1.+et)*(1.-ze)/8.)*resultvalue[3]+
                                ((1.-xi)*(1.-et)*(1.+ze)/8.)*resultvalue[4]+
                                ((1.+xi)*(1.-et)*(1.+ze)/8.)*resultvalue[5]+
                                ((1.+xi)*(1.+et)*(1.+ze)/8.)*resultvalue[6]+
                                ((1.-xi)*(1.+et)*(1.+ze)/8.)*resultvalue[7];
                                break;
                                 //tet4
                                case 3:
                                    resultvalue[0]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[0])].resultValues[s];
                                    resultvalue[1]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[1])].resultValues[s];
                                    resultvalue[2]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[2])].resultValues[s];
                                    resultvalue[3]=resultmodel->resultDataList[r].resultValueList[resultmodel->getNodeIndex(resultmodel->elementList[interpolatorList[h].resultElIndex].nodeList[3])].resultValues[s];
                                    value=interpolatorList[h].parameter[0]*resultvalue[0]+
                                          interpolatorList[h].parameter[1]*resultvalue[1]+
                                          interpolatorList[h].parameter[2]*resultvalue[2]+
                                          interpolatorList[h].parameter[3]*resultvalue[3];

                                break;
                                }
                                stream << QString("%1,").arg(value,12);
                            }
                            emit sendMessage(QString("Value: %1").arg(xi));
                            stream << "\n";
                    }
                    stream << " -3\n";
            }
            stream << " 9999\n";
    }
}
//****************************
/*!Write the interpolator-list data to an xml file*/
void femodel::writeInterpolatorData(const QString &parameterFile, const QString &oldfile)
{
    //emit sendMessage(parameterFile);
    QFile interpolfile(parameterFile);
    if (!interpolfile.open(QFile::WriteOnly | QFile::Text))
    {
        return;
    }

    QXmlStreamWriter* writer =new QXmlStreamWriter(&interpolfile);
    writer->setAutoFormatting(TRUE);
     writer->writeStartDocument();
    writer->writeDTD("<!DOCTYPE csc>");
    writer->writeStartElement("csdata");
     writer->writeStartElement("interpolation");
     writer->writeTextElement("newfile",openFile);
     writer->writeTextElement("oldfile",oldfile);
     for (long g=0 ; g<interpolatorList.size(); g++)
     {
            writer->writeStartElement("interpolator");
            writer->writeTextElement("newNodeIndex",QString::number(interpolatorList[g].newNode));
            writer->writeTextElement("bestResultElIndex",QString::number(interpolatorList[g].resultElIndex));
            writer->writeTextElement("bestResultElType",QString::number(interpolatorList[g].resultElType));
            //writer->writeTextElement("ResultNodes",QString::number(interpolatorList[g].resultElType));
            writer->writeTextElement("Parameter",QString::number(interpolatorList[g].parameter[0])+";"+QString::number(interpolatorList[g].parameter[1])+";"+QString::number(interpolatorList[g].parameter[2])+";"+QString::number(interpolatorList[g].parameter[3]));
            writer->writeEndElement();
     }
     writer->writeEndElement();
     writer->writeEndDocument();
}
