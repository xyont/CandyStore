/***************************************************************************
 *   Copyright (C) 2011 by Patrick Prokopczuk   *
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
namespace nglib {
#include "./Netgen/libsrc/interface/nglib.h"
}

//**********************************************************
/*! Creates a 2D tria mesh from the existing stl triangles*/
/*! Netgen functions are used*/
void femodel::createMeshFromStl(geomodel *geoData,double maxh,int secondOrder, int flag3d)
{
    using namespace nglib;
    vector3d p1;
    vector3d p2;
    vector3d p3;
    double v1[3],v2[3],v3[3];
        emit setCursor(1);

    // Define pointer to a new Netgen Mesh
       Ng_Mesh *mesh;

       // Define pointer to STL Geometry
       Ng_STL_Geometry *stl_geom;

       // Result of Netgen Operations
       Ng_Result ng_res;

        Ng_Init();

       // Actually create the mesh structure
       mesh = Ng_NewMesh();

       //Testing local mesh size
       /*double pointOben[3]={72.1983 , -0.533736 , -83.7576};
       double pointUnten[3]={ 30.0236 , -0.581676 , -83.8729};
       double pointrestricted[3]={52.3472,-0.614266,-83.888};
       Ng_RestrictMeshSizePoint(mesh,pointrestricted,0.1);
       Ng_RestrictMeshSizePoint(mesh,pointUnten,0.005);
       Ng_RestrictMeshSizePoint(mesh,pointOben,0.005);
       Ng_RestrictMeshSizeBox(mesh,pointUnten,pointOben,0.005);*/
       //Create the stl structure
       stl_geom = Ng_STL_NewGeometry();
       //Add triangles

      // emit sendMessage(QString ("Number of trias %1").arg(geoData->stlTriaList.size()));

       for (long s=0;s<geoData->stlTriaList.size();s++)
       {
           p1=geoData->getPointCoords(geoData->stlTriaList[s].pointList[0]);
           v1[0]=p1.x;
           v1[1]=p1.y;
           v1[2]=p1.z;
           //emit sendMessage(QString ("p1 %1").arg(v1[0]));
           p2=geoData->getPointCoords(geoData->stlTriaList[s].pointList[1]);
           v2[0]=p2.x;
           v2[1]=p2.y;
           v2[2]=p2.z;
           p3=geoData->getPointCoords(geoData->stlTriaList[s].pointList[2]);
           v3[0]=p3.x;
           v3[1]=p3.y;
           v3[2]=p3.z;
           Ng_STL_AddTriangle(stl_geom,v1,v2,v3);

        }

        emit sendMessage("Initialise the STL Geometry structure....");

         ng_res = Ng_STL_InitSTLGeometry(stl_geom);
         if(ng_res != NG_OK)
         {
            emit sendMessage("Error Initialising the STL Geometry....Aborting!!");
             return;
         }



         // Set the Meshing Parameters to be used
           Ng_Meshing_Parameters mp;
           mp.maxh = maxh;
           //mp.minh=0.01;
           mp.grading=0.0;
           //mp.elementspercurve = 2.0;
           //mp.elementsperedge = 2.0;
           //mp.fineness = 1;
          mp.second_order = secondOrder;
          mp.uselocalh=1;



           emit sendMessage(QString("secondOrder %1").arg(secondOrder));

         emit sendMessage("Start Edge Meshing....");
         ng_res = Ng_STL_MakeEdges(stl_geom, mesh, &mp);
         if(ng_res != NG_OK)
         {
            emit sendMessage("Error in Edge Meshing....Aborting!!");
             return;
         }

         emit sendMessage("Start Surface Meshing....");
         ng_res = Ng_STL_GenerateSurfaceMesh(stl_geom, mesh, &mp);
         if(ng_res != NG_OK)
         {
            emit sendMessage("Error in Surface Meshing....Aborting!!");
             return;
         }
         /*
        // Ng_STL_Generate_SecondOrder(stl_geom,mesh);
        if (flag3d)
         {
         cout << "Start Volume Meshing...." << endl;
         ng_res = Ng_GenerateVolumeMesh (mesh, &mp);
         if(ng_res != NG_OK)
         {
            cout << "Error in Volume Meshing....Aborting!!" << endl;
            return;
         }

       }

         emit sendMessage("Meshing successfully completed....!!");
         long nNodes=Ng_GetNP (mesh);
         long nSurf=Ng_GetNSE (mesh);
         long nEle=Ng_GetNE (mesh);
         long nSeg =Ng_GetNSeg_2D(mesh);
         emit sendMessage(QString("%1 Nodes created.").arg(nNodes));
         emit sendMessage(QString("%1 Surface Elements created.").arg(nSurf));
         emit sendMessage(QString("%1 Volume Elements created.").arg(nEle));
         emit sendMessage(QString("%1 Segments created.").arg(nSeg));
         if (secondOrder==1)
         {
             emit sendMessage("Generating second order");
             //Ng_STL_Generate_SecondOrder (stl_geom,  mesh);
         }
         nNodes=Ng_GetNP (mesh);
         nSurf=Ng_GetNSE (mesh);
         nEle=Ng_GetNE (mesh);
         nSeg =Ng_GetNSeg_2D(mesh);
         emit sendMessage(QString("%1 Nodes created.").arg(nNodes));
         emit sendMessage(QString("%1 Surface Elements created.").arg(nSurf));
         emit sendMessage(QString("%1 Volume Elements created.").arg(nEle));
         emit sendMessage(QString("%1 Segments created.").arg(nSeg));
         //************************************
         //Put the nodes in the nodelist
         long offset=0,eOffset=0;
         long nodeind;
         int numNodes;
         double ncoord[3];
         Ng_Surface_Element_Type type;
         Ng_Volume_Element_Type typeVolu;
         int sele[10];
         tempElement.outside=0;
         if (!nodeList.empty())
         {
             offset+=nodeList.last().getLabel();
         }
         if (!elementList.empty())
         {
             eOffset=elementList.last().getLabel();
         }
         //***************************************************
         //Moving nodes from the ngmesh to the femodel
         for (long k=1;k<=nNodes;k++)
         {
             Ng_GetPoint(mesh,k,ncoord);
             //emit sendMessage(QString("Node %1").arg(k+offset).arg(ncoord[0]));
            tempNode.setLabel(k+offset);
            tempNode.setCoordinates(ncoord[0],ncoord[1],ncoord[2]);
            nodeList.append(tempNode);
         }
         //********************************************
         //Moving surface elements from the ngmesh to the femodel
         if (flag3d==0)
         {
         tempElement.nodeList.clear();
         for (long e=1;e<=nSurf;e++)
         {
             type = Ng_GetSurfaceElement(mesh,e,sele);
             //emit sendMessage(QString("element %1 %2").arg(e).arg(type));
            tempElement.label=e+eOffset;
            switch (type)
            {
            case 1:
                tempElement.type=7;
                numNodes=3;
                 //emit sendMessage(QString("tria"));
                for (long nn=0;nn<numNodes;nn++)
                {
                    tempElement.nodeList.append(sele[nn]);
                    //Add the elements to the nodes element list.
                    nodeind=getNodeIndex(sele[nn]);
                    nodeList[nodeind].adjacentElements.append(tempElement.label);
                }

                break;
            case 3:
                tempElement.type=8;
                numNodes=6;

                 //emit sendMessage(QString("tria"));
                tempElement.nodeList.append(sele[0]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[0]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[1]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[1]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[2]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[2]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[5]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[5]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[3]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[3]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[4]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[4]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

            break;
            default:
                tempElement.type=-1;
            break;
            }
            if (tempElement.type>0)
            {

            elementList.append(tempElement);
            }
            tempElement.nodeList.clear();
         }
        }
         //********************************************
         //Moving volume elements from the ngmesh to the femodel
         //
         if (!elementList.empty())
         {
             eOffset=elementList.last().getLabel();
         }
         tempElement.nodeList.clear();
        // emit sendMessage(QString("Transfering volu elements %1").arg(nEle));
         for (long e=1;e<=nEle;e++)
         {
             typeVolu = Ng_GetVolumeElement(mesh,e,sele);
             //emit sendMessage(QString("element %1 %2").arg(e).arg(type));
            tempElement.label=e+eOffset;
            switch (typeVolu)
            {
            case 1:
                tempElement.type=3;
                numNodes=4;
                //NETGEN has other node ordering scheme than ccx!
                tempElement.nodeList.append(sele[0]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[0]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[2]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[2]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[1]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[1]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);

                tempElement.nodeList.append(sele[3]);
                //Add the elements to the nodes element list.
                nodeind=getNodeIndex(sele[3]);
                nodeList[nodeind].adjacentElements.append(tempElement.label);
                 //emit sendMessage(QString("tria"));
            break;
            //TET10 Element
            case 4:
            tempElement.type=6;
            numNodes=10;
            //NETGEN has other node ordering scheme than ccx!
            tempElement.nodeList.append(sele[0]);
            //Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[0]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[2]);
            //Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[2]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[1]);
            //Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[1]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[3]);
            //Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[3]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);
             //emit sendMessage(QString("tria"));
  //6 4 5 9 8 7 -
  //9 7 8 6 4 5
  // 9 4 5 6 7 8
            tempElement.nodeList.append(sele[5]);
            //5Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[5]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[7]);
            //6Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[7]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[4]);
            //7Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[4]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[6]);
            //4Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[6]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);
             //emit sendMessage(QString("tria"));

            tempElement.nodeList.append(sele[9]);
            //5Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[9]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);

            tempElement.nodeList.append(sele[8]);
            //6Add the elements to the nodes element list.
            nodeind=getNodeIndex(sele[8]);
            nodeList[nodeind].adjacentElements.append(tempElement.label);
             //emit sendMessage(QString("tria"));



            break;
            default:
                tempElement.type=-1;
            break;
            }
            if (tempElement.type>0)
            {
            //*for (long nn=0;nn<numNodes;nn++)
            //{

            //}
            elementList.append(tempElement);
            }
            tempElement.nodeList.clear();
         }
         //
          Ng_Exit();
         //******************************************************
         //Update the mesh structure
         maxCoords=getMaxModelCoords();
         setModelCenter();
         getFaces();
         getOutsideEdges();
         getOutsideNodes();
         calcFaceCentroids();
         getOutsideElements();

        emit modelUpdate();*/
        emit setCursor(0);
}
//**********************************************************
/*! Creates a 3D tet mesh from the existing 2d mesh*/
/*! Netgen functions are used*/
void femodel::create3DMeshFrom2D(double maxh,int secondOrder, int flag3d)
{
    using namespace nglib;
    vector3d p1;
    vector3d p2;
    vector3d p3;
    //double v1[3],v2[3],v3[3];
    //int type=7;
    double point[3];

    // Define pointer to a new Netgen Mesh
       Ng_Mesh *mesh;

       // Result of Netgen Operations
       Ng_Result ng_res;

        Ng_Init();

       // Actually create the mesh structure
       mesh = Ng_NewMesh();

        //Putting points in the mesh
        for (long i = 0; i <= nodeList.size(); i++)
           {
             p1 = nodeList[i].getCoordinates();
             point[0]=p1.x;
             point[1]=p1.y;
             point[2]=p1.z;
             Ng_AddPoint (mesh, point);
           }
   }
