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
//TODO Display list for displaced edges
//TODO Display list for animation
/**
 * \file GLwin.cpp
 * \brief A brief file description
 **/
//#include <QtGui>
#include <math.h>
#include "glWin2.h"
#define TEX_PIXELS 							512
#define GL_CLAMP_TO_EDGE                  0x812F
//sensitivity 0=nozoom 0.99=monster zoom
#define __ZOOM_SENSITIVITY                0.02
GLViewer2::GLViewer2(femodel *feDatabase, geomodel *geoDatabase)
{

    glInit();
    readFileFlag=0;
    octreeFlag=0;
    timeAnimFlag=0;
    currentTimeAnimFrame=7210;
        testorigin.x=0;
        testorigin.y=0;
        testorigin.z=0;
        testnormal.x=0;
        testnormal.y=1;
        testnormal.z=0;
        nSetPlottingStack.clear();
        elSetPlottingStack.clear();
        setSelectionMode(0);
        selectionHeight=6;
        selectionWidth=6;
        rflag=0;
        qaddFlag=0;
        rFlag=0;
        setMouseTracking(true);
	numberAnimationSteps=9;
	currentAnimScaleFactor=1;
	animationTimer = new QTimer(this);
        timeAnimationTimer= new QTimer(this);
	pickedName=-1;
	tball = new trackball(this);
	feData=feDatabase;
	geoData=geoDatabase;
	nodeStatus=0;
    pointStatus=0;
	faceStatus=0;
    edgeStatus=0;
	resultPlotStatus=0;
	//setOrthoProjection();
    dispScale=1;
	resultColors.clear();
	numberResultColors=16;
	setScaleLabels(0,0);
	scaleTitle="test";
	selectRotFlag=0;
	showUndef=true;
	translations[0]=0;
	translations[1]=0;
	translations[2]=0;
    initializeGL();
	verticalClipping=1;
	horizontalClipping=1;
	zClipping=1;
	rotquat.zero();
	rotCenter.zero();
	build_rotmatrix(rotMatrix,rotquat);
	m_copy(&rMem[0][0],&rotMatrix[0][0]);
	vmem[0]=0;
	vmem[1]=0;
	vmem[2]=0;
	vmem[3]=0;

	//****************
	connect(this, SIGNAL(rotCenterSelected(long)), this, SLOT(setRotCenter(long)));
        connect(this, SIGNAL(rotCenterSelectedPoint(long)), this, SLOT(setRotCenterPoint(long)));
        connect(animationTimer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
        connect(timeAnimationTimer, SIGNAL(timeout()), this, SLOT(updateTimeAnimation()));
}
//*************************************************
/*! Resets the GLWidget*/
void GLViewer2::resetData()
{
     nSetPlottingStack.clear();
}

//**********************************************************
void GLViewer2::resizeGL(int w, int h)
 {
     //int side = qMin(w, h);
     glViewport(0, 0, w, h);
	 
	 aspectRatio=static_cast<double>(w)/static_cast<double>(h);
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
     glMatrixMode(GL_MODELVIEW);
 }
//************************************************************
void GLViewer2::paintGL()
 
{
    //Perform the painting only, if no file reading/writing process is working
    if (readFileFlag==0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //
        renderText(10,height()-5,QString("Current set: %1").arg(feData->currentSet));
        if (qaddFlag!=0)
        {
            drawSelectionBox();
            selectionMode == 0 ? renderText(10,height()-15,"Selection Mode: Individual") : renderText(10,height()-15,"Selection Mode: All");
        }
        moveModel();
        glLineWidth(2.0);
        //Draw the coordinate systems
        drawCoords();
        //**********
        if (octreeFlag==1)
        {
            drawOctree();
        }
        //drawCubeWireframe(feData->modelCenter,feData->modelRadius*2.1);
        //************
        glEnable(GL_DEPTH_TEST);
        //drawLight(GL_LIGHT0);
        glPointSize(5.0);
        glColor3f(0.,1.,1.);
        //*************
        //glEnable(GL_BLEND);
        //glDisable(GL_BLEND);

        glBegin(GL_POINTS);
        glVertex3d(0,0,0);
        glEnd();
        //plot points
        if (pointStatus==1)
        {
            glCallList(8001);
        }
        //**************************
        glEnable(GL_BLEND);
         glDepthMask(GL_FALSE);
        //draw stl trias
        glCallList(8000);
         glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        //************************************************
        //drawPlane(testorigin,testnormal);
        drawCurves();
        drawSurfaces();
        //************************************
        //Draw Border eges
        drawBorderEdges();
        //***********************************
        glDisable(GL_BLEND);
        if (pickedName>-1 && qaddFlag==0)
        {
            drawSelectedNode(pickedName);
        }
        //************************************
        //check wether a displacement exists -> deformed mesh
        if (feData->dispAvailable==true)
        {
            switch (nodeStatus)
            {
                case 0:
			//hide nodes
                break;
                /*case 1:
                //show outside nodes
				glCallList(5011);
                        break;*/
                case 2:
                //show all nodes
				glCallList(5010);
                break;
            }
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1.0,1.0);
            switch (faceStatus)
            {
                case 0:
                    if (timeAnimFlag==0)
                    {
                        glCallList(7010);
                    }
                    else
                    {
                        glCallList(currentTimeAnimFrame);
                    }
                        //drawDispFaces();
                break;
                case 1:
                ;
                break;
                case 2:
                    //drawAllDispEdges();
                break;
            }
            glDisable(GL_POLYGON_OFFSET_FILL);
            glLineWidth(1.0);
            if (edgeStatus==0)
            {
                    drawDispEdges();
            }
            glLineWidth(3.0);
            drawDispFeatureEdges();
            glEnable(GL_BLEND);
            glDepthMask(GL_FALSE);
            if (showUndef==true)
            {
                glEnable(GL_POLYGON_OFFSET_FILL);
                glPolygonOffset(1.0,1.0);
                glCallList(7000);
                glDisable(GL_POLYGON_OFFSET_FILL);
                glLineWidth(1.0);
                drawFeatureEdges();
            }
            glDepthMask(GL_TRUE);
            drawPlanes();
            glDisable(GL_BLEND);
            glLineWidth(1.0);
            //showNumericResults();
        }
        //*******************************************************************
	//no displacements in model -> original geometry only
        else
        {
            //plot entities when a qcommand is active
            plotQNodes();
            plotQFaces();
            //*******
            //Plot all selected Node sets jk

            plotNodeSets();
            //Plot all selected Face sets
            plotFaceSets();
            //Plot all selected Element Sets
            plotElementSets();
            //**********************
		switch (nodeStatus)
		{
			case 0:
			//hide nodes
			break;
            /*case 1:
			//show outside nodes
				glCallList(5001);
            break;*/
			case 2:
			//show all nodes
				glCallList(5000);
			break;
		}

                //***********************
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1.0,1.0);
		switch (faceStatus)
		{
			case 0:
                //drawDispFaces();
                //glCallList(7000);
                //Draw the faces with result values
                glCallList(7010);
			break;
			case 1:
			;
			break;
			case 2:
                glLineWidth(1.0);
                drawAllEdges();
			break;
		}
		
		glDisable(GL_POLYGON_OFFSET_FILL);
                if (edgeStatus==1)
                {
                    glLineWidth(1.0);
                    //drawEdges();
                    glCallList(6000);
                }
                glLineWidth(2.0);
                drawFeatureEdges();
		glLineWidth(1.0);
		glEnable(GL_BLEND);
		drawPlanes();
		glDisable(GL_BLEND);
	}
	//glEnable(GL_BLEND);
	glLineWidth(1.0);
	glDisable(GL_DEPTH_TEST);
	drawRefCoordSystem();
	drawResultFileInfo();
        if (timeAnimFlag==1)
        {
            drawTimeInformation();
        }
	glEnable(GL_DEPTH_TEST);
    }
}
//******************************************************
void GLViewer2::updateView()
{
	updateGL();
	//setViewingCenterRadius();
}
//******************************************************
/*!Apply all translations and rotations*/
void GLViewer2::moveModel()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	v[0]=rotCenter[0];
	v[1]=rotCenter[1];
	v[2]=rotCenter[2];
	v[3]=1;
	m_sub(&dr[0][0],&rotMatrix[0][0],&rMem[0][0]);
	v_multmat(v,&dr[0][0]);
	glTranslated(-v[0]-vmem[0],-v[1]-vmem[1],-v[2]-vmem[2]);
	glTranslated(translations.x,translations.y,translations.z);
	glMultMatrixd(&rotMatrix[0][0]);
}
//***********************************************************//
/*! Everything that has to be done if a new file is loaded*/
void GLViewer2::newFileActions()
{
    currentAnimScaleFactor=1;
	pickedName=-1;
    faceStatus=0;
	setResultPlotStatus(0);
	setViewingCenterRadius();
	createFaceDisplayList();
    createDisplacedFaceDisplayList(-1);
    createEdgeDisplayList();
    createOutsideNodesDisplayList();
	createNodesDisplayList();
    createPointsDisplayList();
	createDisplacedOutsideNodesDisplayList();
	createDisplacedNodesDisplayList();
	setViewingCenterRadius();
	//showEntireScene();
	translations.zero();
	rotquat.zero();
	rotCenter.zero();
	build_rotmatrix(rotMatrix,rotquat);
	m_copy(&rMem[0][0],&rotMatrix[0][0]);
	vmem[0]=0;
	vmem[1]=0;
	vmem[2]=0;
	vmem[3]=0;
	v[0]=0;
	v[1]=0;
	v[2]=0;
	v[3]=0;
	setRotCenter(feData->modelCenter);
	translations=-1*feData->modelCenter;
	updateGL();
	paintGL();

}
//*********************************************************
void GLViewer2::initializeGL()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0);
	GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	createResultColorTexture();
	//createResultIsoTexture();
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
  // Opens help window
  //help();
    setOrthoProjection();
}
//**************************************************************************
 void GLViewer2::mousePressEvent(QMouseEvent *event)
 {
    long pointSelected=-1;
    long nodeSelected=-1;
    lastPos = event->pos();
	if ((event->buttons() & Qt::LeftButton) && event->modifiers() & Qt::ShiftModifier)
		select(event);
	
	else if ((event->button() == Qt::LeftButton) && (selectRotFlag == 1)) 
	{
        nodeSelected=select(event);
        //
        pointSelected=selectPoints(event);
        //
        if (nodeSelected+pointSelected==-2)
        {
            emit sendMessage(QString("No entity selected. Rotation center not changed"));
        }
        else
        {
            if (pointSelected>0)
            {
                emit sendMessage(QString("Rotation Center set to point %1 ").arg(pointSelected));
            }
            if (nodeSelected>0)
            {
                emit sendMessage(QString("Rotation Center set to node %1 ").arg(nodeSelected));
            }
        }

		selectRotFlag=0;
	}
         this->setFocus();
 }
 //***************************************************************************
  void GLViewer2::mouseDoubleClickEvent(QMouseEvent *event)
 {
	if (event->buttons() & Qt::RightButton) 
	 {
		centerModel();
	 } 
	 else if (event->buttons() & Qt::LeftButton)
	{
		zoomPlus();
	}
	 updateGL();
     lastPos = event->pos();

 }
//****************************************************************************
  /*!Evets when the mouse is moved*/
 void GLViewer2::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) 
	 {
			
            //rotquat=rotquat+tball->calcQuat(event->pos(),lastPos,width(),height());
            rotquat=rotquat*tball->calcQuat(event->pos(),lastPos,width(),height());
			rotquat.normalize();
			build_rotmatrix(rotMatrix,rotquat);
	 
	 } 
    else if (event->buttons() & Qt::RightButton)
	 {
         translations.x += 2*aspectRatio/static_cast<double>(width())*dx*horizontalClipping;
         translations.y -= 2/static_cast<double>(height())*dy*verticalClipping;
     }
     else if (event->buttons() & Qt::MidButton)
	 {
         if (!(dy>0 && horizontalClipping<0.000000001))
             {
                if (dy>0)
                {	horizontalClipping *= (1+__ZOOM_SENSITIVITY);
                        verticalClipping *=(1+__ZOOM_SENSITIVITY);
			resizeGL(width(),height());
		}
                else
                {
                    horizontalClipping *= (1-__ZOOM_SENSITIVITY);
                    verticalClipping *= (1-__ZOOM_SENSITIVITY);
                    resizeGL(width(),height());
                }
            }
        }
     lastPos = event->pos();
	 updateGL();
         this->setFocus();

}
//*************************************************************************************
void GLViewer2::zoomPlus()
{
	
		/*horizontalClipping*=(1-(i/39*0.2));
		verticalClipping*=(1-(i/39*0.2));
		resizeGL(width(),height());
		paintGL();*/
		horizontalClipping*=0.8;
		verticalClipping*=0.8;
		resizeGL(width(),height());
		updateGL();
}
//*************************************************************************************
void GLViewer2::zoomMinus()
{
	horizontalClipping*=1.2;
	verticalClipping*=1.2;
	resizeGL(width(),height());
	updateGL();
}
//*************************************************************************************
void GLViewer2::setViewingCenterRadius()
{

		//qGLViewer2::Vec mc;
		//mc[0]=feData->modelCenter[0];
		//mc[1]=feData->modelCenter[1];
		//mc[2]=feData->modelCenter[2];
		//setSceneCenter(mc);
		if (feData->modelRadius > geoData->modelRadius)
                {
                    horizontalClipping=feData->modelRadius*1.8;
                    verticalClipping=feData->modelRadius*1.8;
                }
                else
                {
                    horizontalClipping=geoData->modelRadius*1.3;
                    verticalClipping=geoData->modelRadius*1.3;
                }
		//

		if (feData->maxCoords.norm()*1.5 > geoData->modelRadius) 
			zClipping=feData->maxCoords.norm()*1.5;
		else
			zClipping=geoData->modelRadius;
			
		resizeGL(width(),height());
		updateGL();
		paintGL();
}
//*************************************************************************************
void GLViewer2::centerModel()
{
	translations.x=0;
	translations.y=0;
	translations.z=0;
}
//***************************************************************************************
void GLViewer2::setOrthoProjection()
{
	
}
//***************************************************************************************
//****************************************************
void GLViewer2::drawFaces()
{

}
//***************************************************
void GLViewer2::drawNodes()
{

}
//***************************************************
void GLViewer2::drawCurves()
{
	if (geoData->curveList.size()>0)
	{
		glColor3f(0.,0.,0.);
		for (long c=0;c<geoData->curveList.size();c++)
		{
			glBegin(GL_LINES);
			glVertex3dv(geoData->getPointCoords(geoData->curveList[c].getStartPoint()));
			glVertex3dv(geoData->getPointCoords(geoData->curveList[c].getEndPoint()));
			//emit sendMessage("Curve!");
			glEnd();
		}
		
	}
}
//****************************************************
void GLViewer2::drawDispNodes()
{

}
//******************************************************
void GLViewer2::drawDispFaces()
{
	int test=0;
	long index;
	switch (resultPlotStatus)
	{
	case 0:
		glColor3d(0.5,0.5,0.5);
	break;
	case 1:
		glColor3d(1,1,1);
		glEnable(GL_TEXTURE_1D);
	break;
	case 2:
		glColor3d(1,1,1);
		glEnable(GL_TEXTURE_1D);
	break;
	}
	for (long i=0; i<feData->faceList.size() ; i++)
	{
		if (feData->faceList[i].outside==true)
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (test==0)
		{
		switch (feData->faceList[i].type)
		{
                case 3:
                    glBegin(GL_TRIANGLES);
                  {
                          index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());


                          index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                          index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                      }
                    glEnd();
                 break;
                  case 4:
		  glBegin(GL_TRIANGLES);
		{
			index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
		}
		glEnd();	
		break;
		case 8:
		glBegin(GL_TRIANGLE_FAN);
		{
			glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->faceList[i].centroidValue-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->faceList[i].centroid+currentAnimScaleFactor*dispScale*feData->faceList[i].centroidDisp);
			
			for (int n=0; n<feData->faceList[i].type; n++)
				
			{
				index=feData->getNodeIndex(feData->faceList[i].nodeList[n]);
				glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
				//glTexCoord1d(feData->nodeList[index].resultColor);
				glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			}
			index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
				glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
				glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
		}
		glEnd();
		break;
		case 6:
		glBegin(GL_TRIANGLES);
		{
			index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
		
			index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[4]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
			
			index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
			glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
			glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
		
		}
		glEnd();
		break;
		}
		}
		}
		
		
	}
	if (resultPlotStatus==1)
	{
		glDisable(GL_TEXTURE_1D);
	}
	}
//********************************************************************************
void GLViewer2::drawEdges()
{
	glColor4f(0, 0 , 0,1);
	for (long i=0; i<feData->faceList.size() ; i++)
	{
		if (feData->faceList[i].outside==true)
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		{
			for (int n=0; n<feData->faceList[i].type; n++)
			{
				glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[n]));
			}
		}
		glEnd();
		}
	}
}
//********************************************************************************
void GLViewer2::createEdgeDisplayList()
{
    glNewList(6000,GL_COMPILE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor4f(0, 0 , 0,1);
    for (long i=0; i<feData->faceList.size() ; i++)
    {
        if (feData->faceList[i].outside==true)
        {

        glBegin(GL_POLYGON);
        {
            for (int n=0; n<feData->faceList[i].type; n++)
            {
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[n]));
            }
        }
        glEnd();
        }
    }
    glEndList();
}
//*****************************************************************************************************************
void GLViewer2::drawAllEdges()
{
	glColor4f(0, 0 , 0,1);
	for (long i=0; i<feData->faceList.size() ; i++)
	{
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		{
			for (int n=0; n<feData->faceList[i].type; n++)
			{
				glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[n]));
			}
		}
		glEnd();
		
	}
}
//*******************************************************************************
void GLViewer2::drawDispEdges()
{
long nodeIndex;
	glColor4f(0, 0 , 0,1);
	for (long i=0; i<feData->faceList.size() ; i++)
	{
		if (feData->faceList[i].outside==true)
		{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		{
			for (int n=0; n<feData->faceList[i].type; n++)
			{
				nodeIndex=feData->getNodeIndex(feData->faceList[i].nodeList[n]);
				glVertex3dv(feData->nodeList[nodeIndex].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[nodeIndex].getDisplacements());
			}
		}
		glEnd();
		}
	}
}
//*****************************************************************
void GLViewer2::drawFeatureEdges()
{
long nodeIndex;
	glColor4f(0, 0 , 0,1);
	for (long i=0; i<feData->edgeList.size() ; i++)
	{
		if (feData->edgeList[i].angle > featureEdgeAngle)
		{
		glBegin(GL_LINE_STRIP);
		{
			for (int n=0; n<feData->edgeList[i].type; n++)
			{
				nodeIndex=feData->getNodeIndex(feData->edgeList[i].nodeList[n]);
				glVertex3dv(feData->nodeList[nodeIndex].getCoordinates());
			}
		}
		glEnd();
		}
	}
}
//*****************************************************************************
void GLViewer2::drawDispFeatureEdges()
{
	long nodeIndex;
	for (long i=0; i<feData->edgeList.size() ; i++)

	{
		if (feData->edgeList[i].angle > 45)
		{
		glBegin(GL_LINE_STRIP);
		{
			glColor4f(0, 0 , 0,1);
			for (int n=0; n<feData->edgeList[i].type; n++)
			{
				nodeIndex=feData->getNodeIndex(feData->edgeList[i].nodeList[n]);
				glVertex3dv(feData->nodeList[nodeIndex].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[nodeIndex].getDisplacements());
			}
		}
		glEnd();
		}
	}
}
//*****************************************************************************
/*! draw the border edges (all edges) that have only one face)*/
void GLViewer2::drawBorderEdges()
{
    long nodeIndex;
    glLineWidth(2.0);
    for (long i=0; i<feData->borderEdges.size() ; i++)

    {
        glBegin(GL_LINE_STRIP);
        {
            glColor4f(0, 0 , 0,1);
            for (int n=0; n<feData->edgeList[feData->borderEdges[i]].type; n++)
            {
                nodeIndex=feData->getNodeIndex(feData->edgeList[feData->borderEdges[i]].nodeList[n]);
                glVertex3dv(feData->nodeList[nodeIndex].getCoordinates());
            }
        }
        glEnd();

    }
}
//***************************************************************
void GLViewer2::setNodeStatus(short status)
{
	nodeStatus=status;
	//updateGL();
}
/*! Toggles the node status (show outside/show all/hide)*/
void GLViewer2::toggleNodeStatus()
{
	if (nodeStatus==0)
		{
                        nodeStatus=2;
		}
	else
	{
            nodeStatus=0;
        }
        /*	if (nodeStatus==1)
		{
			nodeStatus=2;
		}
		else
		{
			nodeStatus=0;
		}
        }*/
	updateGL();
	paintGL();
}
/*! Toggles the point status */
void GLViewer2::togglePointStatus()
{
    pointStatus==0 ? pointStatus=1 : pointStatus=0;
    emit sendMessage(QString("Point Status %1").arg(pointStatus));
    updateGL();
    paintGL();
}
//********************************************************************************************
/*! Calculates and sets the initial deformation scale factor*/
void GLViewer2::setInitScaleFactor()
{
	if (feData->dispRatio >1)
	{
		dispScale=0.2/feData->dispRatio;
	}
	else
	{
		dispScale=1;
	}
	createDisplacedOutsideNodesDisplayList();
	createDisplacedNodesDisplayList();
	//updateGL();
	paintGL();
}
//****************************************************************************************************
void GLViewer2::setDispScale(float d)
{
	dispScale=d;
	createDisplacedNodesDisplayList();
	createDisplacedOutsideNodesDisplayList();
        createDisplacedFaceDisplayList(-1);
	updateGL();
	paintGL();
}
//**********************************************************************************************
/*!Draws the reference coord system */

void GLViewer2::drawRefCoordSystem()
{
    vector3d orig;
    vector3d v1;
    vector3d v2;

    orig.x=0;
    orig.y=0;
    orig.z=0;

    v1.x=1;
    v1.y=0;
    v1.z=0;

    v2.x=0;
    v2.y=0;
    v2.z=1;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glOrtho(-10*aspectRatio,10*aspectRatio,-10,10,-5.0,5.0);


    glMatrixMode(GL_MODELVIEW);
    glTranslated(-9*aspectRatio,-8.0,0);
    glMultMatrixd(&rotMatrix[0][0]);
    vector3d vectory=-1*cross(v1,v2);
    glColor3f(1,0,0);
    drawArrowFrom(orig,v1,1.5);
    glColor3f(0,1,0);
    drawArrowFrom(orig,vectory,1.5);
    glColor3f(0,0,1);
    drawArrowFrom(orig,v2,1.5);


    if (resultPlotStatus==1 || resultPlotStatus==3)
	{
		
		drawResultColorBox();
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	
}
//***************************************************************************************
/*! Draws a coord system*/
void GLViewer2::drawCoordSystem(vector3d origin, vector3d vector1, vector3d vector2)
{
//double refx=0;//-8.5*aspectRatio;
//double refy=0;//-8.5;
double cutplane=horizontalClipping;//8.5;
double length=cutplane/5;
/*vector3d start;
start.zero();
vector3d xVec,yVec,zVec,newVec;
xVec.zero();
xVec.x=1;  
yVec.zero();
yVec.y=1; 
zVec.zero();
zVec.z=1;
*/

        vector3d vectory=-1*cross(vector1,vector2);
	glColor3f(1,0,0);
        drawArrowFrom(origin,vector1,length);
	glColor3f(0,1,0);
        drawArrowFrom(origin,vectory,length);
	glColor3f(0,0,1);
        drawArrowFrom(origin,vector2,length);
}
//******************************************************************************************
/*!                                         */
void GLViewer2::drawCoords()
{
    for (int k=0; k<geoData->cosList.size();k++)
    {
        if (geoData->cosList[k].type==0)
        {
            drawCoordSystem(geoData->cosList[k].origin,geoData->cosList[k].vector1,geoData->cosList[k].vector2);
        }
    }

}


//***********************************************************************************
/*! Draws the fringe value box*/
void GLViewer2::drawResultColorBox()
{
glDisable(GL_TEXTURE_1D);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
glColor3d(1,1,1);
startScreenCoords();
glBegin(GL_POLYGON);
        glVertex2d(10,10);
        glVertex2d(110,10);
        glVertex2d(110,280);
        glVertex2d(10,280);
glEnd();
//glColor3d(0,0,0);
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    glVertex2d(9,9);
//    glVertex2d(101,9);
//    glVertex2d(101,151);
//    glVertex2d(9,151);

glColor3d(1,1,1);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_1D);
	for (int i=1; i<=numberResultColors; i++)
	{
		glBegin(GL_POLYGON);
		
			glTexCoord1d(((numberResultColors/TEX_PIXELS)/numberResultColors)*(i-1));
						
                                glVertex2d(15,(height()/2+25)-(+(i+0.75)*(height()*0.45/numberResultColors)));
                                glVertex2d(45,(height()/2+25)-(+(i+0.75)*(height()*0.45/numberResultColors)));
                                glVertex2d(45,(height()/2+25)-(+(i+0.75)*(height()*0.45/numberResultColors))+10);
                                glVertex2d(15,(height()/2+25)-(+(i+0.75)*(height()*0.45/numberResultColors))+10);
		glEnd();
	}	
	glDisable(GL_TEXTURE_1D);
	glColor3d(0,0,0);
endScreenCoords();
	for (int i=0; i<=numberResultColors; i++)
	{
		renderText(50,(height()/2+25)-(+(i+0.75)*(height()*0.45/numberResultColors)),scaleLabels[i]);
	}
		
	renderText(20,20,scaleTitle);
	

}

//*******************************************************************
/**Draws some information about the current file to the bottom of the widget
(File name, current displacement scale factor)
*/
void GLViewer2::drawResultFileInfo()
{
	int offset=feData->openFile.size();
	renderText(width()/2-offset*6/2,height()-20,"Result File: " + feData->openFile);
	renderText(width()/2-offset*6/2,height()-8,QString("Displacement Scale Factor:  %1").arg(dispScale));

}
//*********************************************************************
/*! Show the current time if a time step is selected*/
void GLViewer2::drawTimeInformation()
{
    int offset=feData->openFile.size();
    renderText(width()/2-offset*6/2,height()-32,QString("Time:  %1").arg(currentTime));
}

//*******************************************************************
/*! Builds a rotational matrix for openGL from a quaternion*/

void GLViewer2::build_rotmatrix(double m[4][4], quaternion q)
{
/*    m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
    m[0][1] = 2.0 * (q[0] * q[1] - q[2] * q[3]);
    m[0][2] = 2.0 * (q[2] * q[0] + q[1] * q[3]);
    m[0][3] = 0.0;

    m[1][0] = 2.0 * (q[0] * q[1] + q[2] * q[3]);
    m[1][1]= 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
    m[1][2] = 2.0 * (q[1] * q[2] - q[0] * q[3]);
    m[1][3] = 0.0;

    m[2][0] = 2.0 * (q[2] * q[0] - q[1] * q[3]);
    m[2][1] = 2.0 * (q[1] * q[2] + q[0] * q[3]);
    m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
    m[2][3] = 0.0;

    m[3][0] = 0.0;
    m[3][1] = 0.0;
    m[3][2] = 0.0;
    m[3][3] = 1.0;
    */
    m[0][0] = 1.0 - 2.0 * (q[2] * q[2] + q[3] * q[3]);
       m[0][1] = 2.0 * (q[1] * q[2] - q[3] * q[0]);
       m[0][2] = 2.0 * (q[1] * q[3] + q[2] * q[0]);
       m[0][3] = 0.0;

       m[1][0] = 2.0 * (q[1] * q[2] + q[3] * q[0]);
       m[1][1]= 1.0 - 2.0 * (q[3] * q[3] + q[1] * q[1]);
       m[1][2] = 2.0 * (q[2] * q[3] - q[1] * q[0]);
       m[1][3] = 0.0;

       m[2][0] = 2.0 * (q[3] * q[1] - q[2] * q[0]);
       m[2][1] = 2.0 * (q[2] * q[3] + q[1] * q[0]);
       m[2][2] = 1.0 - 2.0 * (q[2] * q[2] + q[1] * q[1]);
       m[2][3] = 0.0;

       m[3][0] = 0.0;
       m[3][1] = 0.0;
       m[3][2] = 0.0;
       m[3][3] = 1.0;
}
//
//*************************************************************
/*! CAlculate a quaternion out of a rotation matrix*/
quaternion GLViewer2::MatrixToQuat(double a[4][4] )
{
    quaternion q;
    emit sendMessage("CAlc quat");
  float trace = a[0][0] + a[1][1] + a[2][2]; // I removed + 1.0f; see discussion with Ethan
  if( trace > 0 ) {// I changed M_EPSILON to 0
     float s = 0.5f / sqrtf(trace+ 1.0f);
     q.w = 0.25f / s;
     q.x = ( a[2][1] - a[1][2] ) * s;
     q.y = ( a[0][2] - a[2][0] ) * s;
     q.z = ( a[1][0] - a[0][1] ) * s;
   } else {
     if ( a[0][0] > a[1][1] && a[0][0] > a[2][2] ) {
       float s = 2.0f * sqrtf( 1.0f + a[0][0] - a[1][1] - a[2][2]);
       q.w = (a[2][1] - a[1][2] ) / s;
       q.x = 0.25f * s;
       q.y = (a[0][1] + a[1][0] ) / s;
       q.z = (a[0][2] + a[2][0] ) / s;
     } else if (a[1][1] > a[2][2]) {
       float s = 2.0f * sqrtf( 1.0f + a[1][1] - a[0][0] - a[2][2]);
       q.w = (a[0][2] - a[2][0] ) / s;
       q.x = (a[0][1] + a[1][0] ) / s;
       q.y = 0.25f * s;
       q.z = (a[1][2] + a[2][1] ) / s;
     } else {
       float s = 2.0f * sqrtf( 1.0f + a[2][2] - a[0][0] - a[1][1] );
       q.w = (a[1][0] - a[0][1] ) / s;
       q.x = (a[0][2] + a[2][0] ) / s;
       q.y = (a[1][2] + a[2][1] ) / s;
       q.z = 0.25f * s;
     }
   }
  return(q);
}
//*******************************************************************************
void GLViewer2::showNumericResults()
{
	vector3d coord;
	for (long n=0; n< feData->nodeList.size(); n++)
	{
		coord=feData->nodeList[n].getCoordinates();
		//renderText(coord[0],coord[1],coord[2],QString::number(feData->nodeList[n].getResultValue()));
	}

}
//********************************************************************
void GLViewer2::showNodeLabels()
{
	vector3d coord;
	for (long n=0; n< feData->nodeList.size(); n++)
	{
		coord=feData->nodeList[n].getCoordinates();
		//renderText(coord[0],coord[1],coord[2],QString::number(feData->nodeList[n].getLabel()));
	}
}
//***************************************************************************************************
void GLViewer2::keyPressEvent(QKeyEvent *e)
{
	e->ignore();
}

//******************************************************************************************************
void GLViewer2::createResultColorTexture()
{
  int   i, n;
  float r=0, g=0, b=0;
  GLfloat   contur_tex[TEX_PIXELS*3];
  GLuint tex_id;
  
  //if(steps>TEX_PIXELS) steps=TEX_PIXELS;

  n=0;
  for (i=0; i<numberResultColors; i++)
  {
    define_rgb( (float)i/(numberResultColors-1.), &r,&g,&b);
    contur_tex[n]  =r;
    contur_tex[n+1]=g;
    contur_tex[n+2]=b;
    n+=3;
  }
  for (; i<TEX_PIXELS; i++)
  {
    contur_tex[n]  =r;
    contur_tex[n+1]=g;
    contur_tex[n+2]=b;
    n+=3;
  }


  /*
  n=0;
  for (i=0; i<TEX_PIXELS; i++)
  {    printf("%d %d %lf %lf %lf\n",steps, i, contur_tex[n], contur_tex[n+1], contur_tex[n+2]); n+=3; }
  */
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_1D, tex_id);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage1D(GL_TEXTURE_1D, 0, 3, TEX_PIXELS, 0, GL_RGB, GL_FLOAT, contur_tex);
}
//********************************************************************************************
void GLViewer2::createResultIsoTexture()
{
//texture kreieren: Alles weiss, ausser an Gesamtzahl/ANzahlisolinien 

  int   i, n;
 // float r, g, b;
  GLfloat   contur_iso_tex[TEX_PIXELS*3];
  GLuint tex_iso_id;
  
  //if(steps>TEX_PIXELS) steps=TEX_PIXELS;

  n=0;
 /*for (i=0; i<numberResultColors; i++)
  {
    define_rgb( (float)i/(numberResultColors-1.), &r,&g,&b);
    contur_tex[n]  =r;
    contur_tex[n+1]=g;
    contur_tex[n+2]=b;
    n+=3;
  }
*/  for (i=0; i<TEX_PIXELS; i++)
  {
    contur_iso_tex[n]  =1.;
    contur_iso_tex[n+1]=1.;
    contur_iso_tex[n+2]=1.;
    n+=3;
  }
	contur_iso_tex[300]=0;
    contur_iso_tex[301]=0;
    contur_iso_tex[302]=0;
  
  

  /*
  n=0;
  for (i=0; i<TEX_PIXELS; i++)
  {    printf("%d %d %lf %lf %lf\n",steps, i, contur_tex[n], contur_tex[n+1], contur_tex[n+2]); n+=3; }
  */
  glGenTextures(2, &tex_iso_id);
  glBindTexture(GL_TEXTURE_1D, tex_iso_id);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage1D(GL_TEXTURE_1D, 0, 3, TEX_PIXELS, 0, GL_RGB, GL_FLOAT, contur_iso_tex);
}
//********************************************************************************************************************

void GLViewer2::define_rgb(float v, float *r, float *g, float *b)
{
   /* v,r,g,b == 0->1 */

#if MY_COLORS
        *r = v;
        *g = cos( (v-0.5) * 3.1415 );
        *b = 1. - v;
#else
        if ( v < 0.1 )       *r = .5 - v*1./0.2;
        else if ( v < 0.5 )  *r = 0. ;
        else if ( v < 0.9 )  *r=  (v-.5)*1./0.4 ;
        else                 *r= 1. ;

        if ( v < 0.1 )       *r = .5 - v*1./0.2;
        else if ( v < 0.5 )  *r = 0. ;
        else if ( v < 0.8 )  *r=  (v-.5)*1./0.3 ;
        else                 *r= 1. - (v-.8)*1./0.4 ;

        if ( v < 0.1 )       *r = .5 - v*1./0.2;
        else if ( v < 0.5 )  *r = 0. ;
        else if ( v < 0.8 )  *r=  (v-.5)*1./0.3 ;
        else if ( v < 0.90 )  *r=  1. ;
        else                 *r=  1. - (v-0.90)*1./0.4 ;


        if ( v < 0.1 )
          *g = 0.;
        else if ( v < 0.3 )  *g = (v-.1) * 1./0.2;
        else if ( v < 0.5 )  *g = 1. - (v-.3) * 1./0.4;
        else if ( v < 0.9 )  *g = .5 + (v-.5) * 1./0.8;
        else                 *g = 1. - (v-.9) * 1./0.1;

        if ( v < 0.1 )
          *g = 0.;
        else if ( v < 0.3 )  *g = (v-.1) * 1./0.2;
        else if ( v < 0.5 )  *g = 1. - (v-.3) * 1./0.4;
        else if ( v < 0.8 )  *g = .5 + (v-.5) * 1./0.6;
        else if ( v < 0.9 )  *g = 1. - (v-.8) * 1./0.1;
        else                 *g = 0.;

        if ( v < 0.1 )
          *g = 0.;
        else if ( v < 0.3 )  *g = (v-.1) * 1./0.2;
        else if ( v < 0.5 )  *g = 1. - (v-.3) * 1./0.4;
        else if ( v < 0.8 )  *g = .5 + (v-.5) * 1./0.6;
        else   *g = 1. - (v-.8) * 1./0.2;


        if ( v < 0.5 )  *b = 1. - (v) * 1./0.5;
        else if ( v < 0.9 )  *b = 0.;
        else *b = (v-.9)*1./0.1;

        if ( v < 0.5 )  *b = 1. - (v) * 1./0.5;
        else *b = 0;
#endif
}
//*****************************************************************************************************
void GLViewer2::setScaleLabels(double min,double max)
{
	double scaleLabel=0.0;
	scaleLabels.clear();
	
	
	for (int i=0; i<=numberResultColors; i++)
	{
		scaleLabel=min+(i/numberResultColors)*(max-min);
		scaleLabels.append(QString("%1").arg(scaleLabel, 0, 'E', 3));
	}
}
//*****************************************************************************************************
/*!Sets the automtic calculated min/max fringe values
*/
void GLViewer2::setFringeAutoValues(double min,double max)
{
	fringeMinAuto=min;
	fringeMaxAuto=max;
	setFringeMax(max);
	setFringeMin(min);
    createDisplacedFaceDisplayList(-1);
    updateGL();
    paintGL();
}
//**********************************************************
/*!Sets the maximum value of the fringe plot */

void GLViewer2::setFringeMax(double max)
{
    if (max <= fringeMin)
    {
        emit sendMessage("Max fringe value must be gerater than minimum value!");
        return;
    }
	fringeMax=max;
	setScaleLabels(fringeMin,fringeMax);
	emit sendMessage(QString("Maximum fringe value set to %1").arg(max));
    createDisplacedFaceDisplayList(-1);
    updateGL();
	paintGL();
}
//**********************************************************
/*! Sets the minimum value of the fringe plot */
void GLViewer2::setFringeMin(double min)
{
    if (min >= fringeMax)
    {
        emit sendMessage("Min fringe value must be smaller than maximum value!");
        return;
    }
    fringeMin=min;
    setScaleLabels(fringeMin,fringeMax);
    emit sendMessage(QString("Minimum fringe value set to %1").arg(min));
    createDisplacedFaceDisplayList(-1);
    updateGL();
    paintGL();
}
//**********************************************************
/*! Sets the maximum value of the fringe plot to the ato calculated value */
void GLViewer2::setFringeMaxAuto()
{
	fringeMax=fringeMaxAuto;
	setScaleLabels(fringeMin,fringeMax);
	emit sendMessage(QString("Maximum fringe value set automaticaly to %1").arg(fringeMax));
	//updateGL();
	paintGL();
}
//**********************************************************
/*! Sets the minimum value of the fringe plot  automaticaly*/
void GLViewer2::setFringeMinAuto()
{
	fringeMin=fringeMinAuto;
	setScaleLabels(fringeMin,fringeMax);
	emit sendMessage(QString("Minimum fringe value set automaticaly to %1").arg(fringeMin));
	//updateGL();
	paintGL();
}
//**********************************************************

void GLViewer2::setResultPlotStatus(short status)
{
	resultPlotStatus=status;
    updateGL();
	paintGL();
}
//**********************************************************
void GLViewer2::setScaleTitle(const QString &title)
{
	scaleTitle=title;
}
//**********************************************************
void GLViewer2::setShadedMode()
{
	faceStatus=0;
        if (edgeStatus==0)
        edgeStatus=1;
        else
        edgeStatus=0;
        updateGL();
	paintGL();

}
//**********************************************************
void GLViewer2::setWireframeMode()
{
	if (faceStatus==1)
	faceStatus=2;
	else
	faceStatus=1;
	
	updateGL();
	paintGL();
}
//**********************************************************
/*! Sets the rotation center*/
void GLViewer2::setRotCenter(vector3d center)
{
	rotCenter=center;
	m_copy(&rMem[0][0],&rotMatrix[0][0]);
	vmem[0]+=v[0];
	vmem[1]+=v[1];
	vmem[2]+=v[2];
	vmem[3]+=v[3];
	pickedName=-1;
}
//*************************************************************
/*! Sets the rotation center*/
void GLViewer2::setRotCenter(double x,double y,double z)
{
	rotCenter.x=x;
	rotCenter.x=y;
	rotCenter.x=z;
	m_copy(&rMem[0][0],&rotMatrix[0][0]);
	vmem[0]+=v[0];
	vmem[1]+=v[1];
	vmem[2]+=v[2];
	vmem[3]+=v[3];
	pickedName=-1;
}
//**********************************************************
/*! Sets the rotation center */
void GLViewer2::setRotCenter(long index)
{
	rotCenter=feData->getNodeCoords(index);
	m_copy(&rMem[0][0],&rotMatrix[0][0]);
	vmem[0]+=v[0];
	vmem[1]+=v[1];
	vmem[2]+=v[2];
	vmem[3]+=v[3];
	pickedName=-1;
}
//**********************************************************
/*! Sets the rotation center */
void GLViewer2::setRotCenterPoint(long index)
{
    rotCenter=geoData->getPointCoords(index);
    m_copy(&rMem[0][0],&rotMatrix[0][0]);
    vmem[0]+=v[0];
    vmem[1]+=v[1];
    vmem[2]+=v[2];
    vmem[3]+=v[3];
    pickedName=-1;
}
//**********************************************************
/*! Starts a scale factor animation*/

void GLViewer2::startAnimation()
{
int i;
int k=0;
animCount=0;
	for (i=0; i<=numberAnimationSteps;i++)
	{
		animationScaleFactors[i]=sin((static_cast<double>(i)/static_cast<double>(numberAnimationSteps))*(3.1415/2.0));
	}
	for (i=numberAnimationSteps; i<=2*numberAnimationSteps;i++)
	{
		animationScaleFactors[i]=animationScaleFactors[i-k];
		k++;
		k++;
	}
	for (i=2*numberAnimationSteps; i<=3*numberAnimationSteps;i++)
	{
		animationScaleFactors[i]=-1*animationScaleFactors[i-2*numberAnimationSteps];
	}
	k=0;
	for (i=3*numberAnimationSteps; i<4*numberAnimationSteps;i++)
	{
		animationScaleFactors[i]=animationScaleFactors[i-k];
		k++;
		k++;
	}
	
	animationTimer->start(100);
	emit sendMessage("Animation started");
}
//**********************************************************
/*! Starts a scale factor animation*/
void GLViewer2::stopAnimation()
{
	
	animationTimer->stop();
	emit sendMessage("Animation stopped");
	currentAnimScaleFactor=1;
	updateGL();
}
//**********************************************************
/*! Creates a display list for stl trias */
void GLViewer2::createSTLTriaDisplayList()
{
    glNewList(8000,GL_COMPILE);
    glLineWidth(1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glColor4f(0.5, 0.5 , 0.5,0.2);
    glBegin(GL_TRIANGLES);
    for (long h=0;h<geoData->stlTriaList.size();h++)
    {

            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[0]));
            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[1]));
            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[2]));
    }
    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    glColor4f(0.0, 0.0 , 0.0,0.2);
    glBegin(GL_TRIANGLES);
    for (long h=0;h<geoData->stlTriaList.size();h++)
    {

            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[0]));
            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[1]));
            glVertex3dv(geoData->getPointCoords(geoData->stlTriaList[h].pointList[2]));
    }
    glEnd();
    glEndList();
}

//**********************************************************
/*! Creates a display list for all all outside faces*/
void GLViewer2::createFaceDisplayList()
{
glNewList(7000,GL_COMPILE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	glColor4f(0.5, 0.5 , 0.5,0.2);	
	for (long int i=0; i<feData->faceList.size() ; i++)
	{
		if (feData->faceList[i].outside==true)
		{
		
		switch(feData->faceList[i].type)
		{
                case 3:
                glBegin(GL_TRIANGLES);
                {
                        //glColor4f(0.5, 0.5 , 0.5,0.2);
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
                }
                glEnd();
                break;
            case 4:
            glBegin(GL_TRIANGLES);
            {
                //glColor4f(0.5, 0.5 , 0.5,0.2);
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
		
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
            }
            glEnd();
            break;
        case 6:
        glBegin(GL_TRIANGLES);
        {
            //glColor4f(0.5, 0.5 , 0.5,0.2);
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));

            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));

            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));

            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[4]));
            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));
        }
        glEnd();
        break;
            case 8:
            glBegin(GL_TRIANGLE_FAN);
            {
			//glColor4f(0.5, 0.5 , 0.5,0.2);
						
				glVertex3dv(feData->faceList[i].centroid);
                for (int n=0; n<feData->faceList[i].type; n++)
                {
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[n]));
                }
                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
			
            }
            glEnd();
            break;

        }
		}
	}
	glEndList();
}
//**********************************************************
/*! Creates a display list for all displaced outside faces, and the colors for the result values*/
void GLViewer2::createDisplacedFaceDisplayList(int frame)
{
   // glDeleteLists(7010,1);
    if (frame==-1)
    {
        glNewList(7010,GL_COMPILE);
    }
    else
    {
        glNewList((7200+frame),GL_COMPILE);
    }
    {
    int test=0;
    long index;
    switch (resultPlotStatus)
    {
    case 0:
        glColor3d(0.5,0.5,0.5);
         glDisable(GL_TEXTURE_1D);
    break;
    case 1:
        glColor3d(1,1,1);
        glEnable(GL_TEXTURE_1D);
    break;
    case 2:
        glColor3d(1,1,1);
        glEnable(GL_TEXTURE_1D);
    break;
    default:
        glColor3d(1,1,1);
        glEnable(GL_TEXTURE_1D);
    }
    for (long i=0; i<feData->faceList.size() ; i++)
    {
        if (feData->faceList[i].outside==true)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            if (test==0)
            {
                switch (feData->faceList[i].type)
                {
                //tria3 face
                case 3:
                    glBegin(GL_TRIANGLES);
                    {

                        for (int j=0;j<=2;j++)
                        {
                          index=feData->getNodeIndex(feData->faceList[i].nodeList[j]);
                          resultPlotStatus<3 ?
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin))):
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->faceList[i].centroidValue-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                        }

                        /*
                          index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                          index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
                          glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                          glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                        */
                    }
                    glEnd();
                break;
                //quad4 face
                case 4:
                    glBegin(GL_TRIANGLES);
                    {
                        index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                    }
                    glEnd();
                break;
                //quad 8 face
                case 8:
                    glBegin(GL_TRIANGLE_FAN);
                    {
                        glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->faceList[i].centroidValue-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->faceList[i].centroid+currentAnimScaleFactor*dispScale*feData->faceList[i].centroidDisp);

                        for (int n=0; n<feData->faceList[i].type; n++)
                        {
                            index=feData->getNodeIndex(feData->faceList[i].nodeList[n]);
                            glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                            //glTexCoord1d(feData->nodeList[index].resultColor);
                            glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                        }
                        index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
                        glTexCoord1d(fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS)*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                    }
                    glEnd();
                break;
                //tria6 face
                case 6:
                    glBegin(GL_TRIANGLES);
                    {
                        index=feData->getNodeIndex(feData->faceList[i].nodeList[0]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[1]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[2]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[3]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[4]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());

                        index=feData->getNodeIndex(feData->faceList[i].nodeList[5]);
                        glTexCoord1d((fabs(currentAnimScaleFactor)*(numberResultColors/TEX_PIXELS))*((feData->nodeList[index].getResultValue()-fringeMin)/(fringeMax-fringeMin)));
                        glVertex3dv(feData->nodeList[index].getCoordinates()+currentAnimScaleFactor*dispScale*feData->nodeList[index].getDisplacements());
                    }
                    glEnd();
                break;
                }
            }
        }
        }
        if (resultPlotStatus==1)
        {
            glDisable(GL_TEXTURE_1D);
        }
    }
	glEndList();
    updateGL();
    paintGL();
}
//**********************************************************
/*! Create display lists for animation*/
void GLViewer2::createScaleFactorAnimationFaceLists()
{
//double animationScaleFactor=0;
int numberFrames=10;
	for (int n=0; n < numberFrames;n++)
	{
		glNewList(7100+n,GL_COMPILE);
		
		glEndList();
	}
}
//**********************************************************
/*!Creates the display list for all Nodes */

void GLViewer2::createNodesDisplayList()
{
	
	glNewList(5000,GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(0.,0.,1.);
	for (long n=0;n<feData->nodeList.size();n++)
	{
			glVertex3dv(feData->nodeList[n].getCoordinates());
	}
	glEnd();
	glEndList();
}
//**********************************************************
/*!Creates the display list for Outside Nodes */

void GLViewer2::createOutsideNodesDisplayList()
{
	
	glNewList(5001,GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(0.,0.,1.);
	for (long n=0;n<feData->nodeList.size();n++)
	{
		if (feData->nodeList[n].isOutside())
		{
			glVertex3dv(feData->nodeList[n].getCoordinates());
		}
	}
	glEnd();
	glEndList();
}
//**********************************************************
/*!Creates the display list for Displaced Nodes */

void GLViewer2::createDisplacedNodesDisplayList()
{
	glNewList(5010,GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(0,0,1);
	
	for (long i=0; i< feData->nodeList.size(); ++i)
	{
      glVertex3dv(feData->nodeList[i].getCoordinates()+dispScale*feData->nodeList[i].getDisplacements());
    }
	glEnd();
	glEndList();
}
//**********************************************************
/*!Creates the display list for Displaced Outside Nodes */

void GLViewer2::createDisplacedOutsideNodesDisplayList()
{
	glNewList(5011,GL_COMPILE);
	glBegin(GL_POINTS);
	glColor3f(0,0,1);
	
	for (long i=0; i< feData->nodeList.size(); ++i)
	{
		if (feData->nodeList[i].isOutside())
		{
			glVertex3dv(feData->nodeList[i].getCoordinates()+dispScale*feData->nodeList[i].getDisplacements());
		}
	}
	glEnd();
	glEndList();
}
//*****************************************************************

//void GLViewer2::mousePressEvent(QMouseEvent* e)
//{
/*	if ((e->button() == Qt::LeftButton) && (selectRotFlag == 1))
    {
	
	//myMouseBehavior = true;
	if (camera()->setRevolveAroundPointFromPixel(e->pos()))
	{
	  setVisualHintsMask(1);
	  //updateGL();
	}
	selectRotFlag=0;
	emit sendMessage("New Rotation Center set."); 
	}
   else
     QGLViewer2::mousePressEvent(e);

*/
//}

//**********************************************************
/*! Sets the select rotation center flag                */
void GLViewer2::setSelectRotFlag()
{
	selectRotFlag=1;
	emit sendMessage("Select new rotation center!"); 
}

//**********************************************************
/*! Check wether a entity is under the mouse pointer and give information about the entity*/
long GLViewer2::select(const QMouseEvent* event)
{
  long selection=-1;
  QPoint pickedPoint=event->pos();
  GLdouble x=pickedPoint.x();
  GLdouble y=height()-pickedPoint.y();

  GLuint selectBuf[512];
  GLint hits;
  GLint viewport[4];
  
  //prepare select mode
  glGetIntegerv (GL_VIEWPORT, viewport);
  glSelectBuffer(512, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  // Loads the matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  
  gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);
  
  glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
  glMatrixMode(GL_MODELVIEW);
  moveModel();
  //***********************************************
  /*Check for nodes*/
  drawWithNames();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();
  hits = glRenderMode (GL_RENDER);

  if (hits>0)
  {
    selection=processHits(hits,selectBuf);
  }
  else
    {
        pickedName=-1;
        /*if (selectRotFlag==1)
		{
			emit sendMessage("No node selected - Rotation Center not changed!");
        }*/
    }
   //************************************************
  /*Check for points*/
  emit sendMessage("Checking for point hits");
  //prepare select mode
  glGetIntegerv (GL_VIEWPORT, viewport);
  glSelectBuffer(512, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  // Loads the matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);

  glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
  glMatrixMode(GL_MODELVIEW);
  moveModel();
  drawPointsWithNames();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();
  hits = glRenderMode (GL_RENDER);

  if (hits>0)
  {
    emit sendMessage("Point hit");
    selection=processPointHits(hits,selectBuf);
  }
  else
    {
        pickedName=-1;
        /*
        if (selectRotFlag==1)
        {
            emit sendMessage("No Point selected - Rotation Center not changed!");
        }*/
    }


  //*************************************************
    updateGL();
    return(selection);
}
//**********************************************************

long GLViewer2::selectPoints(const QMouseEvent* event)
{
  long selection=-1;
  QPoint pickedPoint=event->pos();
  GLdouble x=pickedPoint.x();
  GLdouble y=height()-pickedPoint.y();

 /* QString number1="x " + QString::number(x);//feData->nodeList[selectedName()].getLabel()) + " selected";
    emit sendMessage(number1);
    number1="y " + QString::number(y);//feData->nodeList[selectedName()].getLabel()) + " selected";
    emit sendMessage(number1);
  */
  //setSelectionWidth(10);
  //setSelectionHeight(10);

  GLuint selectBuf[512];
  GLint hits;
  GLint viewport[4];

  //prepare select mode
  glGetIntegerv (GL_VIEWPORT, viewport);
  glSelectBuffer(512, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  // Loads the matrices
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);

  glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
  glMatrixMode(GL_MODELVIEW);
  moveModel();
  drawPointsWithNames();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glFlush();
  hits = glRenderMode (GL_RENDER);

  if (hits>0)
  {
    selection=processPointHits(hits,selectBuf);
  }
  else
    {
        pickedName=-1;
        /*
        if (selectRotFlag==1)
        {
            emit sendMessage("No Point selected - Rotation Center not changed!");
        }*/
    }

    updateGL();
    return(selection);
}
//**********************************************************
long GLViewer2::processHits(GLint hits, GLuint buffer[])
{
    long frontNode=-1;
    float frontZ=9999;
    float zValue=0;
        unsigned int j;
        GLint i;
    long selectedNode=-1;
 //       long selectedIndex=-1;
	GLuint names, *ptr;
	QString number3="Number of hits " + QString::number(hits);
	//emit sendMessage(number3); 
	ptr= (GLuint *) buffer;
	for (i=0;i<hits;i++)
	{
		names=*ptr;
                // number3="Number of names " + QString::number(names);
		//emit sendMessage(number3); 
		ptr++;
        zValue=(float) *ptr/0x7ffffff;
                //number3="z1: " + QString::number((float) *ptr/0x7ffffff);
		//emit sendMessage(number3);
		ptr++;
                //number3="z2: " + QString::number((float) *ptr/0x7ffffff);
		//emit sendMessage(number3);
		ptr++;
                //number3="Name: "  ;
		//emit sendMessage(number3);
		for (j=0; j<names;j++)
		{

                        //number3=QString::number((long) *ptr);
			//emit sendMessage(number3);
            selectedNode=(long) *ptr;
            if (zValue < frontZ)
            {
                frontZ=zValue;
                frontNode=selectedNode;
            }
			ptr++;
			
		}
	}

    emit sendMessage(QString("Selected Node: %1").arg(frontNode));
	
	if (selectRotFlag==0)
	{
        emit nodeSelected(frontNode);
	}
	else
	{
        emit rotCenterSelected(frontNode);
        //emit sendMessage(QString("New Rotation Center selected: Node %1").arg(frontNode));
	}
    return(frontNode);
}
//**********************************************************
long GLViewer2::processPointHits(GLint hits, GLuint buffer[])
{

    long frontPoint=-1;
    float frontZ=9999;
    float zValue=0;
        unsigned int j;
        GLint i;
    long selectedNode=-1;
 //       long selectedIndex=-1;
    GLuint names, *ptr;
    QString number3="Number of hits " + QString::number(hits);
    //emit sendMessage(number3);
    ptr= (GLuint *) buffer;
    for (i=0;i<hits;i++)
    {
        names=*ptr;
                // number3="Number of names " + QString::number(names);
        //emit sendMessage(number3);
        ptr++;
        zValue=(float) *ptr/0x7ffffff;
                //number3="z1: " + QString::number((float) *ptr/0x7ffffff);
        //emit sendMessage(number3);
        ptr++;
                //number3="z2: " + QString::number((float) *ptr/0x7ffffff);
        //emit sendMessage(number3);
        ptr++;
                //number3="Name: "  ;
        //emit sendMessage(number3);
        for (j=0; j<names;j++)
        {
                        //number3=QString::number((long) *ptr);
            //emit sendMessage(number3);
            selectedNode=(long) *ptr;
            if (zValue < frontZ)
            {
                frontZ=zValue;
                frontPoint=selectedNode;
            }
            ptr++;

        }
    }

    //selectedNode=feData->nodeList[selectedIndex].getLabel();
    emit sendMessage(QString("Selected POint: %1").arg(frontPoint));

    if (selectRotFlag==0)
    {
        emit sendMessage("Point selected");
        emit pointSelected(frontPoint);

    }
    else
    {
        emit rotCenterSelectedPoint(frontPoint);
        //emit sendMessage("New Rotation Center selected: Point " + QString::number(frontPoint));
    }
    return(frontPoint);
}
//****************************************************************************
void GLViewer2::drawWithNames()
{

	if (feData->dispAvailable==true)
	{
   
		for (long i=0; i<feData->nodeList.size(); ++i)
		{
			if (feData->nodeList[i].isOutside())
			{
			 // Draw nodes, loading a name (id) for each of them
                glLoadName(feData->nodeList[i].getLabel());
				glBegin(GL_POINTS);
				glVertex3dv(feData->nodeList[i].getCoordinates()+dispScale*feData->nodeList[i].getDisplacements());
				glEnd();
			}
		}
   }
   else
   {
        if (nodeStatus==2)
       {
            for (long k=0;k<feData->nodeList.size();k++)
            {
                    glLoadName(feData->nodeList[k].getLabel() );
                    glBegin(GL_POINTS);
                    glVertex3dv(feData->nodeList[k].getCoordinates());
                    glEnd();

            }
       }
       else
       {
           for (long i=0; i<nSetPlottingStack.size(); ++i)
            {
                        for (long k=0;k<feData->setList[nSetPlottingStack[i]].nodeList.size();k++)
                        {
                                glLoadName(feData->setList[nSetPlottingStack[i]].nodeList[k] );
                                glBegin(GL_POINTS);
                                glVertex3dv(feData->getNodeCoords(feData->setList[nSetPlottingStack[i]].nodeList[k]));
                                glEnd();

                        }
            }
         }
    }
}
//****************************************************************************
//****************************************************************************
void GLViewer2::drawFacesWithNames()
{

    if (feData->dispAvailable==false)
    {

        for (long i=0; i<feData->faceList.size(); ++i)
        {
            if (feData->faceList[i].outside==true)
            {
             // Draw nodes, loading a name (id) for each of them
                glLoadName(i);
                switch(feData->faceList[i].type)
                {
                        case 3:
                        glBegin(GL_TRIANGLES);
                        {
                                //glColor4f(0.5, 0.5 , 0.5,0.2);
                                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                                glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
                        }
                        glEnd();
                        break;
                    case 4:
                    glBegin(GL_TRIANGLES);
                    {
                        //glColor4f(0.5, 0.5 , 0.5,0.2);
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));

                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
                    }
                    glEnd();
                    break;
                case 6:
                glBegin(GL_TRIANGLES);
                {
                    //glColor4f(0.5, 0.5 , 0.5,0.2);
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));

                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));

                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[1]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[2]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));

                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[3]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[4]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[5]));
                }
                glEnd();
                break;
                    case 8:
                    glBegin(GL_TRIANGLE_FAN);
                    {
                    //glColor4f(0.5, 0.5 , 0.5,0.2);

                        glVertex3dv(feData->faceList[i].centroid);
                        for (int n=0; n<feData->faceList[i].type; n++)
                        {
                            glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[n]));
                        }
                        glVertex3dv(feData->getNodeCoords(feData->faceList[i].nodeList[0]));

                    }
                    glEnd();
                    break;

                }

            }
        }
   }
   else
   {

    }
}
//****************************************************************************
//draws elements for picking
void GLViewer2::drawElementsWithNames()
{
    float color[4]={1,0,0,1};
    if (feData->dispAvailable==false)
    {

        for (long i=0; i<feData->elementList.size(); ++i)
        {
            //if (feData->faceList[i].outside==true)
            //{
             // Draw elements, loading a name (id) for each of them
                glLoadName(feData->elementList[i].getLabel());
                plotElement(feData->elementList[i].getLabel(),color);

        }
        //}
   }
   else
   {

   }
}
//****************************************************************************
/*! Draws points with names in selction mode */
void GLViewer2::drawPointsWithNames()
{

        if (pointStatus==1)
       {
            for (long k=0;k<geoData->pointList.size();k++)
            {
                    glLoadName(geoData->pointList[k].getLabel() );
                    glBegin(GL_POINTS);
                    glVertex3dv(geoData->pointList[k].getCoordinates());
                    glEnd();

            }
       }
       /*else
       {
           for (long i=0; i<nSetPlottingStack.size(); ++i)
            {
                        for (long k=0;k<feData->setList[nSetPlottingStack[i]].nodeList.size();k++)
                        {
                                glLoadName(feData->setList[nSetPlottingStack[i]].nodeList[k] );
                                glBegin(GL_POINTS);
                                glVertex3dv(feData->getNodeCoords(feData->setList[nSetPlottingStack[i]].nodeList[k]));
                                glEnd();

                        }
            }
         }
    }*/

}
//**********************************************************
/*!Draw the selected node bigger and in special color (at the moment fixed to red)*/

void GLViewer2::drawSelectedNode(long label)
{
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
        long index=feData->getNodeIndex(label);
	if (feData->dispAvailable!=true)
	{
      glVertex3dv(feData->nodeList[index].getCoordinates());
	}
	else
	{
      glVertex3dv(feData->nodeList[index].getCoordinates()+dispScale*feData->nodeList[index].getDisplacements());
	}
	  glEnd();
	glPointSize(5);
}
//*****************************************************************************************
/*!Draw the plane with id label*/
void GLViewer2::drawPlanes()
{

	vector3d cornerPoint[4];
	vector3d normal,origin;
        double edgeLength(horizontalClipping/6);
//	GLdouble orthoW=horizontalClipping,orthoH=verticalClipping;
	
	//camera()->getOrthoWidthHeight(orthoW,orthoH);
	for (long p=0;p<geoData->planeList.size();p++)
	{
		if (geoData->planeList[p].isDrawn())
		{
	
	//draw point at the origin
	//helpPoint[0]=geoData->planeList[index].getOrigin();
	normal=geoData->planeList[p].getNormal();
	origin=geoData->planeList[p].getOrigin();
	
	//draw arrow from origin in normal direction
	drawArrowFrom(origin,normal,horizontalClipping/5);
	//calculate corner points for plane visualization
	vector3d ort=normal.orthoVec();
	vector3d help=cross(normal,ort);
	help.normalize();
	cornerPoint[0]=origin+normal.orthoVec()*edgeLength+help*edgeLength;
	cornerPoint[1]=origin-normal.orthoVec()*edgeLength+help*edgeLength;
	cornerPoint[2]=origin-normal.orthoVec()*edgeLength-help*edgeLength;
	cornerPoint[3]=origin+normal.orthoVec()*edgeLength-help*edgeLength;
	
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
		{
		for (int i=0; i<4;i++)
		
		{
			glColor4f(0, 0 , 0.4,0.2);
			glVertex3dv(cornerPoint[i]);
		}
		}
		glEnd();
		glLineWidth(2.0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBegin(GL_POLYGON);
		{
		for (int i=0; i<4;i++)
		
		{
			glColor4f(0, 0 , 0.4,0.7);			
			glVertex3dv(cornerPoint[i]);
		}
		}
        glEnd();
	}
	//draw first corner point == control point
	/*glBegin(GL_POINTS);
	{
		glColor4f(0, 0 , 1,1);			
		glVertex3dv(cornerPoints[0]);
	}
	glEnd();*/
	}
	glLineWidth(1.0);
}
//***************************************************************************************************
/*!Adds plane p to the cutting plane list*/
void GLViewer2::addCuttingPlane(long p)
{

}
//******************************************************************************************************
/*!Removes plane c from the cutting plane list*/
void GLViewer2::removeCuttingPlane(long c)
{

}
//******************************************************************************************************
/*!Toggles the undeformed mesh on/off*/
void GLViewer2::toggleShowUndef(bool check)
{
	showUndef=check;
	if (check==true)
		emit sendMessage("Undeformed Mesh on");
	else
		emit sendMessage("Undeformed Mesh off");
	updateGL();
}
//************************************************************************************************
void GLViewer2::v_multmat(double *v, double *m)
{
double b[4];


b[0]=v[0]*m[0]+v[1]*m[4]+v[2]*m[8]+v[3]*m[12];
b[1]=v[0]*m[1]+v[1]*m[5]+v[2]*m[9]+v[3]*m[13];
b[2]=v[0]*m[2]+v[1]*m[6]+v[2]*m[10]+v[3]*m[14];
b[3]=v[0]*m[3]+v[1]*m[7]+v[2]*m[11]+v[3]*m[15];

v[0]=b[0];
v[1]=b[1];
v[2]=b[2];
v[3]=b[3];
}
//*****************************************************************************************************
void  GLViewer2::m_sub(double *ms, double *m, double *s)
/* ********************************************************* */
/*  ms(4,4) = m(4,4) - s(4,4)  def. aus OpenGL               */
/*               m( spalte,reihe)                            */
/* ********************************************************* */
{
  int j;
 
  for (j=0; j<16;j++ )
    ms[j]= m[j]-s[j];
}
//**************************************************************************************************
void  GLViewer2::m_copy(double *s, double *m)
/* ********************************************************* */
/*  s(4,4) = m(4,4)   def. aus OpenGL                        */
/*               m( spalte,reihe)                            */
/* ********************************************************* */
{
  int j;

  for (j=0; j<16;j++ )
    s[j]= m[j];
}
//*********************************************************************************************************
/*!Sets the width of the selection area*/
void GLViewer2::setSelectionWidth(GLdouble w)
{

}
//*******************************************************************************************************
/*!Sets the height of the selection area*/
void GLViewer2::setSelectionHeight(GLdouble h)
{

}
//********************************************************************************************************
/*! Updates the animation*/
void GLViewer2::updateAnimation()
{
	//emit sendMessage(QString("update %1 %2 ").arg(animCount).arg(animationScaleFactors[animCount]));
	currentAnimScaleFactor=animationScaleFactors[animCount];
	animCount++;
	if (animCount >= 4*numberAnimationSteps)
		animCount=0;
	updateGL();
}

//***********************************************************************
/*! Draws an arrow from coordinates start to coordinates end*/
void GLViewer2::drawArrow(vector3d start,vector3d end)
{

}
//***********************************************************************
/*! Draws an arrow with length l and direction d from coordinates start*/
void GLViewer2::drawArrowFrom(vector3d start,vector3d d,double l)
{

vector3d dir=d;
dir.normalize();
vector3d end=start+l*dir;
vector3d rotVec=dir.orthoVec(); 
vector3d newVec=dir.rotate(rotVec,20);
newVec.normalize();
glBegin(GL_LINES);

	//glColor3f(1,0,0);
	glVertex3dv(start);
	glVertex3dv(end);
	for (int h=0;h<12;h++)
	{
		glVertex3dv(end);
		glVertex3dv(end+newVec*-0.2*l);
		newVec=newVec.rotate(dir,30);
	}
	
glEnd();
}
//***********************************************************************
/*! Draws an arrow with length l and direction d to coordinates end*/
void GLViewer2::drawArrowTo(vector3d end,vector3d d,double l)
{

}
//*************************************************************************
/*! Recalculates the cutting planes after a scale operation*/
void GLViewer2::modelScaled(double)
{
    feData->modelRadius=feData->modelRadius*1000;
    //feData->maxCoordsfeData-max;
    setViewingCenterRadius();
}
//*************************************************************************
/*!Starts drawing in screen coords with 0,0 at upper left*/
void GLViewer2::startScreenCoords()
{
    int w=width();
    int h=height();
    glPushAttrib(GL_TRANSFORM_BIT | GL_VIEWPORT_BIT);

    /* Setup projection parameters. */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDepthRange(0, 0);
    glViewport(0,0,w,h);

}
//************************************************************
/*!Ends drawing in screen coords*/
void GLViewer2::endScreenCoords()
{
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}
//**************************************************************
/*!Draws a selection box*/
void GLViewer2::drawSelectionBox()
{
    startScreenCoords();
    glPointSize(5.0);
    glColor3f(0.,0.,0.);
    glBegin(GL_LINE_LOOP);
    glVertex2d(lastPos.x()+selectionWidth/2,lastPos.y()-selectionHeight/2);
    glVertex2d(lastPos.x()+selectionWidth/2,lastPos.y()+selectionHeight/2);
    glVertex2d(lastPos.x()-selectionWidth/2,lastPos.y()+selectionHeight/2);
    glVertex2d(lastPos.x()-selectionWidth/2,lastPos.y()-selectionHeight/2);
    glEnd();
    endScreenCoords();
}
//***************************************************************
void GLViewer2::selectNode()
{

    QPoint pickedPoint=lastPos;
    GLdouble x=pickedPoint.x();
    GLdouble y=height()-pickedPoint.y();



    //MAximumselection. 4 values are neede for ech pick
    GLuint selectBuf[20480];
    GLint hits;
    GLint viewport[4];

    //prepare select mode
    glGetIntegerv (GL_VIEWPORT, viewport);
    glSelectBuffer(20480, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    // Loads the matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);

    glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
    glMatrixMode(GL_MODELVIEW);
    moveModel();
    drawWithNames();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    hits = glRenderMode (GL_RENDER);

    if (hits>0)
    {
          processHitsNodeSelect(hits,selectBuf);
    }
    else
    {
                  pickedName=-1;
                  emit sendMessage("No node selected");
    }
    updateGL();
}
//***************************************************************
void GLViewer2::selectFace()
{

    QPoint pickedPoint=lastPos;
    GLdouble x=pickedPoint.x();
    GLdouble y=height()-pickedPoint.y();



    //MAximum selection. 4 values are needed for each pick
    GLuint selectBuf[20480];
    GLint hits;
    GLint viewport[4];

    //prepare select mode
    glGetIntegerv (GL_VIEWPORT, viewport);
    glSelectBuffer(20480, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    // Loads the matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);

    glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
    glMatrixMode(GL_MODELVIEW);
    moveModel();
    drawFacesWithNames();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    hits = glRenderMode (GL_RENDER);

    if (hits>0)
    {
        emit sendMessage("Face selected");
        processHitsFaceSelect(hits,selectBuf);
    }
    else
    {
                  pickedName=-1;
                  emit sendMessage("No face selected");
    }
    updateGL();
}
//***************************************************************
void GLViewer2::selectElement()
{

    QPoint pickedPoint=lastPos;
    GLdouble x=pickedPoint.x();
    GLdouble y=height()-pickedPoint.y();



    //MAximum selection. 4 values are needed for each pick
    GLuint selectBuf[20480];
    GLint hits;
    GLint viewport[4];

    //prepare select mode
    glGetIntegerv (GL_VIEWPORT, viewport);
    glSelectBuffer(20480, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);

    // Loads the matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(x, y, selectionWidth, selectionHeight, viewport);

    glOrtho(-1*aspectRatio*horizontalClipping, +aspectRatio*horizontalClipping, -1*verticalClipping, verticalClipping, -1*zClipping, zClipping);
    glMatrixMode(GL_MODELVIEW);
    moveModel();
    drawElementsWithNames();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glFlush();
    hits = glRenderMode (GL_RENDER);

    if (hits>0)
    {
        emit sendMessage("Element selected");
        processHitsElementSelect(hits,selectBuf);
    }
    else
    {
                  pickedName=-1;
                  emit sendMessage("No element selected");
    }
    updateGL();
}
//**********************************************************
void GLViewer2::processHitsNodeSelect(GLint hits, GLuint buffer[])
{
    long frontNode=-1;
    float frontZ=9999;
    float zValue=0;
    QList<long> selectedNodes;
            selectedNodes.clear();
        unsigned int j;
        GLint i;
        long selectedNode=-1;
 //       long selectedIndex=-1;
        GLuint names, *ptr;
        ptr= (GLuint *) buffer;
        emit sendMessage(QString("number of hits %1").arg(hits));
        for (i=0;i<hits;i++)
        {
                names=*ptr;
                // emit sendMessage(QString("number of names %1").arg(names));
                ptr++;
                zValue= ((float) *ptr/0x7ffffff);
                //emit sendMessage(QString("zValue %1").arg(zValue));
                ptr++;
                ptr++;
                for (j=0; j<names;j++)
                {
                        selectedNode=(long) *ptr;
                        if (zValue < frontZ)
                        {
                            frontZ=zValue;
                            frontNode=selectedNode;
                        }
                        if (selectionMode==1)
                        {
                            //selectedIndex=feData->getNodeIndex(selectedNode);
                            //pickedName=selectedIndex;
                            emit sendMessage(QString("Adding Node %1 to set").arg(selectedNode));
                            if (qaddFlag>0)
                            {
                            if (qaddFlag==1)
                            {
                               // emit addNodeToSet(selectedNode);
                                selectedNodes.append(selectedNode);
                            }
                            else
                            {
                                emit removeNodeFromSet(selectedNode);
                            }
                            pickedName=-1;
                            }
                        }
                        ptr++;
                }

        }
        if (selectionMode==0)
        {
            //selectedIndex=feData->getNodeIndex(selectedNode);
            //pickedName=selectedIndex;

            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                //emit sendMessage(QString("Front Node: %1 selected Node: %2 ").arg(frontNode).arg(selectedNode));
                emit addNodeToSet(frontNode);
            }
            else
            {
                emit removeNodeFromSet(frontNode);
            }
            pickedName=-1;
            }
        }
        else
        {
            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                emit addNodesToSet(selectedNodes);
            }
            pickedName=-1;
            }
        }
         updateNSetDisplayLists();
         updateGL();
}
//****************************************************************************
//**********************************************************
void GLViewer2::processHitsFaceSelect(GLint hits, GLuint buffer[])
{

    float frontZ=9999;
    float zValue=0;
    long frontFace=-1;
    QList<long> selectedFaces;
            selectedFaces.clear();
        unsigned int j;
        GLint i;
        long selectedFace=-1;
 //       long selectedIndex=-1;
        GLuint names, *ptr;
        ptr= (GLuint *) buffer;
        emit sendMessage(QString("Number of hits %1").arg(hits));
        for (i=0;i<hits;i++)
        {
                names=*ptr;
                ptr++;
                zValue= ((float) *ptr/0x7ffffff);
                ptr++;
                ptr++;
                for (j=0; j<names;j++)
                {
                        selectedFace=(long) *ptr;
                        if (zValue < frontZ)
                        {
                            frontZ=zValue;
                            frontFace=selectedFace;
                        }
                        if (selectionMode==1)
                        {
                            //selectedIndex=feData->getNodeIndex(selectedNode);
                            //pickedName=selectedIndex;
                            emit sendMessage(QString("Adding Face %1 to set").arg(selectedFace));
                            if (qaddFlag>0)
                            {
                            if (qaddFlag==1)
                            {
                               // emit addNodeToSet(selectedNode);
                                selectedFaces.append(selectedFace);
                            }
                            else
                            {
                                //emit removeFaceFromSet(selectedFace);
                            }
                            pickedName=-1;
                            }
                        }
                        ptr++;
                }

        }
        if (selectionMode==0)
        {
            //selectedIndex=feData->getNodeIndex(selectedNode);
            //pickedName=selectedIndex;

            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                emit sendMessage(QString("Adding Face %1 to set").arg(selectedFace));
                emit addFaceToSet(frontFace);
            }
            else
            {
                emit removeFaceFromSet(frontFace);
            }
            pickedName=-1;
            }
        }
        else
        {
            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                emit addFacesToSet(selectedFaces);
            }
            pickedName=-1;
            }
        }
         //updateFaceSetDisplayLists();
         updateGL();
}
//**********************************************************
void GLViewer2::processHitsElementSelect(GLint hits, GLuint buffer[])
{

    float frontZ=9999;
    float zValue=0;
    long frontElement=-1;
    QList<long> selectedElements;
    selectedElements.clear();
        unsigned int j;
        GLint i;
        long selectedElement=-1;

        GLuint names, *ptr;
        ptr= (GLuint *) buffer;
        emit sendMessage(QString("Number of hits %1").arg(hits));
        for (i=0;i<hits;i++)
        {
                names=*ptr;
                ptr++;
                zValue= ((float) *ptr/0x7ffffff);
                ptr++;
                ptr++;
                for (j=0; j<names;j++)
                {
                        selectedElement=(long) *ptr;
                        if (zValue < frontZ)
                        {
                            frontZ=zValue;
                            frontElement=selectedElement;
                        }
                        if (selectionMode==1)
                        {

                            //emit sendMessage(QString("Adding Element %1 to set").arg(selectedElement));
                            if (qaddFlag>0)
                            {
                            if (qaddFlag==1)
                            {
                                selectedElements.append(selectedElement);
                            }
                            else
                            {
                                emit removeElementFromSet(selectedElement);
                            }
                            pickedName=-1;
                            }
                        }
                        ptr++;
                }

        }
        if (selectionMode==0)
        {
            //selectedIndex=feData->getNodeIndex(selectedNode);
            //pickedName=selectedIndex;

            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                emit sendMessage(QString("Adding Element %1 to set").arg(frontElement));
                emit addElementToSet(frontElement);
            }
            else
            {
                emit sendMessage(QString("Removing Element %1 to set").arg(frontElement));
                emit removeElementFromSet(frontElement);
            }
            pickedName=-1;
            }
        }
        else
        {
            if (qaddFlag>0)
            {
            if (qaddFlag==1)
            {
                emit addElementsToSet(selectedElements);
            }
            pickedName=-1;
            }
        }

         updateGL();
}
//****************************************************************************
/*!Sets the selection mode*/
void GLViewer2::setSelectionMode(int mode)
{
    selectionMode=mode;
}
//******************************************************************************
/*!Evaluates the "r" clicks and changes the selection box size*/
void GLViewer2::RPressed()
{
    if (rflag==0)
    {
        firstRPoint=lastPos;
        rflag=1;
    }
    else
    {
        selectionWidth=abs(lastPos.x()-firstRPoint.x());
        if (selectionWidth<6)
        {
            selectionWidth=6;
        }
        selectionHeight=abs(lastPos.y()-firstRPoint.y());
        if (selectionHeight<6)
        {
            selectionHeight=6;
        }
        rflag=0;
    }
    emit sendMessage(QString("Selection box changed to %1 %2").arg(selectionWidth).arg(selectionHeight));
     updateGL();
    paintGL();
}
//*************************************************************************************
/*!Plot the selected node sets*/
  void GLViewer2::plotNodeSets()
  {
      //Plot the sets from the stack in reverse order
          for (int n=nSetPlottingStack.size()-1;n>=0;n--)
          {
             // emit sendMessage(QString("Plotting set %1").arg(nSetPlottingStack[n]));
                  plotNSet(nSetPlottingStack[n]);
          }
  }
  //*************************************************************************************
  //*************************************************************************************
  /*!Plot the selected node sets*/
    void GLViewer2::plotFaceSets()
    {
        //Plot the sets from the stack in reverse order
            for (int n=faceSetPlottingStack.size()-1;n>=0;n--)
            {
               // emit sendMessage(QString("Plotting set %1").arg(nSetPlottingStack[n]));
                    plotFaceSet(faceSetPlottingStack[n]);
            }
    }
    //*************************************************************************************
    //*************************************************************************************
    /*!Plot the selected node sets*/
      void GLViewer2::plotElementSets()
      {
          //Plot the sets from the stack in reverse order
              for (int n=elSetPlottingStack.size()-1;n>=0;n--)
              {
                 // emit sendMessage(QString("Plotting set %1").arg(nSetPlottingStack[n]));
                      plotElSet(elSetPlottingStack[n]);
              }
      }
      //*************************************************************************************
  /*!Plot nodes during a q command*/
    void GLViewer2::plotQNodes()
    {
      if (feData->setList.size()>0)
          {
              for (int n=0;n<feData->setList.size();n++)
              {
                  if (feData->setList[n].qCommandFlag==true)
                  {
                      glBegin(GL_POINTS);
                      glColor3f(1.,0.,0.);
                      for (long k=0;k<feData->setList[n].nodeList.size();k++)
                      {
                          glVertex3dv(feData->getNodeCoords(feData->setList[n].nodeList[k]));
                      }
                      glEnd();
                  }
              }
          }
    }
//***********************************************************************
//*************************************************************************************
  /*!Plot faces during a q command*/
    void GLViewer2::plotQFaces()
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      long index=-1;
      if (feData->setList.size()>0)
          {
              for (int n=0;n<feData->setList.size();n++)
              {
                  if (feData->setList[n].qCommandFlag==true)
                  {

                      glColor3f(1.,0.,0.);
                      for (long k=0;k<feData->setList[n].faceIndexList.size();k++)
                      {
                          index=feData->setList[n].faceIndexList[k];
                          switch(feData->faceList[index].type)
                          {
                                  case 3:
                                  glBegin(GL_TRIANGLES);
                                  {
                                          //glColor4f(0.5, 0.5 , 0.5,0.2);
                                          glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[0]));
                                          glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[1]));
                                          glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[2]));
                                  }
                                  glEnd();
                                  break;
                              case 4:
                              glBegin(GL_TRIANGLES);
                              {
                                  //glColor4f(0.5, 0.5 , 0.5,0.2);
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[0]));
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[1]));
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[2]));

                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[0]));
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[2]));
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[3]));
                              }
                              glEnd();
                              break;
                          case 6:
                          glBegin(GL_TRIANGLES);
                          {
                              //glColor4f(0.5, 0.5 , 0.5,0.2);
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[0]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[1]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[5]));

                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[1]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[3]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[5]));

                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[1]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[2]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[3]));

                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[3]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[4]));
                              glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[5]));
                          }
                          glEnd();
                          break;
                              case 8:
                              glBegin(GL_TRIANGLE_FAN);
                              {
                              //glColor4f(0.5, 0.5 , 0.5,0.2);

                                  glVertex3dv(feData->faceList[index].centroid);
                                  for (int n=0; n<feData->faceList[index].type; n++)
                                  {
                                      glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[n]));
                                  }
                                  glVertex3dv(feData->getNodeCoords(feData->faceList[index].nodeList[0]));

                              }
                              glEnd();
                              break;

                          }
                      }

                  }
              }
          }
    }
//***********************************************************************
    void GLViewer2::updateNSetDisplayLists()
    {
    /*    if (feData->setList.size()>1)
        {
            for (int n=1;n<feData->setList.size();n++)
            {
                glNewList(5001+n,GL_COMPILE);
                glBegin(GL_POINTS);
                glColor3f(1.,0.,0.);
                for (long k=0;k<feData->setList[n].nodeList.size();k++)
                {
                        glVertex3dv(feData->getNodeCoords(feData->setList[n].nodeList[k]));
                }
            glEnd();
            glEndList();
            }
        }*/
    }
//
    //***********************************************************************
        void GLViewer2::plotNSet(int index)
        {
             switch (feData->setList[index].nodePlotColor)
             {
             case 0:
                 glColor3f(1.,0.,0.);
             break;
             case 1:
                glColor3f(0.,1.,0.);
             break;
             case 2:
                glColor3f(0.,0.,1.);
             break;
             default:
                glColor3f(1.,0.,0.);
             break;
             }
               glBegin(GL_POINTS);

                    for (long k=0;k<feData->setList[index].nodeList.size();k++)
                    {
                       // emit sendMessage(QString("%1").arg(feData->setList[index].nodeList[k]));
                            glVertex3dv(feData->getNodeCoords(feData->setList[index].nodeList[k]));
                    }
                glEnd();
        }
    //
        //***********************************************************************
            void GLViewer2::plotFaceSet(int index)
            {
                 switch (feData->setList[index].facePlotColor)
                 {
                 case 0:
                     glColor3f(1.,0.,0.);
                 break;
                 case 1:
                    glColor3f(0.,1.,0.);
                 break;
                 case 2:
                    glColor3f(0.,0.,1.);
                 break;
                 default:
                    glColor3f(1.,0.,0.);
                 break;
                 }
                 long faceIndex=-1;
                 if (feData->dispAvailable==false)
                 {
                     //emit sendMessage(QString("Set: %1  NumberOfFaces: %2").arg(feData->setList[index].name).arg(feData->setList[index].faceIndexList.size()));

                     for (long i=0; i<feData->setList[index].faceIndexList.size(); ++i)
                     {
                         faceIndex=feData->setList[index].faceIndexList[i];
                         if (faceIndex>-1 && faceIndex <   feData->faceList.size()-1)
                         {
                             switch(feData->faceList[faceIndex].type)
                             {
                                     case 3:
                                     glBegin(GL_TRIANGLES);
                                     {
                                             //glColor4f(0.5, 0.5 , 0.5,0.2);
                                             glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                                             glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                                             glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                                     }
                                     glEnd();
                                     break;
                                 case 4:
                                 glBegin(GL_TRIANGLES);
                                 {
                                     //glColor4f(0.5, 0.5 , 0.5,0.2);
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));

                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                                 }
                                 glEnd();
                                 break;
                             case 6:
                             glBegin(GL_TRIANGLES);
                             {
                                 //glColor4f(0.5, 0.5 , 0.5,0.2);
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));

                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));

                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));

                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[4]));
                                 glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));
                             }
                             glEnd();
                             break;
                                 case 8:
                                 glBegin(GL_TRIANGLE_FAN);
                                 {
                                 //glColor4f(0.5, 0.5 , 0.5,0.2);

                                     glVertex3dv(feData->faceList[faceIndex].centroid);
                                     for (int n=0; n<feData->faceList[faceIndex].type; n++)
                                     {
                                         glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[n]));
                                     }
                                     glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));

                                 }
                                 glEnd();
                                 break;

                             }

                         }
                     }
                     }

                else
                {

                 }
            }
        //
//*******************************************************
        void GLViewer2::addNsetToPlottingStack(int index)
        {
            nSetPlottingStack.removeAll(index);
            nSetPlottingStack.append(index);
            updateGL();
        }
///********************************************************
        void GLViewer2::removeNsetFromPlottingStack(int index)
        {
             nSetPlottingStack.removeAll(index);
             updateGL();
        }
//********************************************************
        void GLViewer2::setSingleNSetToPlottingStack(int index)
        {
             nSetPlottingStack.clear();
             nSetPlottingStack.append(index);
             updateGL();
         }
//*******************************************************************
        void GLViewer2::setSingleFaceSetToPlottingStack(int index)
        {
             faceSetPlottingStack.clear();
             faceSetPlottingStack.append(index);
             updateGL();
         }
        //*******************************************************
        void GLViewer2::setSingleElSetToPlottingStack(int index)
        {
             elSetPlottingStack.clear();
             elSetPlottingStack.append(index);
             updateGL();
         }
        //*******************************************************
                void GLViewer2::addFaceSetToPlottingStack(int index)
                {
                    faceSetPlottingStack.removeAll(index);
                    faceSetPlottingStack.append(index);
                    updateGL();
                }
                ///********************************************************
                        void GLViewer2::removeFaceSetFromPlottingStack(int index)
                        {
                             faceSetPlottingStack.removeAll(index);
                             updateGL();
                        }
                        //***************
                        void GLViewer2::addElSetToPlottingStack(int index)
                        {

                            elSetPlottingStack.removeAll(index);
                            elSetPlottingStack.append(index);
                            updateGL();
                        }
                ///********************************************************
                void GLViewer2::removeElSetFromPlottingStack(int index)
                {
                    elSetPlottingStack.removeAll(index);
                    updateGL();
                }
//************************************************************
        void GLViewer2::setFeatureEdgeAngle(int angle)
        {
            featureEdgeAngle=angle;
            updateGL();
        }
//*************************************************************
        void GLViewer2::drawPlane(vector3d origin, vector3d normal)
        {
            vector3d cornerPoint[4];

            double edgeLength(horizontalClipping/6);
    //	GLdouble orthoW=horizontalClipping,orthoH=verticalClipping;

            //camera()->getOrthoWidthHeight(orthoW,orthoH);
            //draw point at the origin
            //helpPoint[0]=geoData->planeList[index].getOrigin();
            //draw arrow from origin in normal direction
            drawArrowFrom(origin,normal,horizontalClipping/5);
            //calculate corner points for plane visualization
            vector3d ort=normal.orthoVec();
            vector3d help=cross(normal,ort);
            help.normalize();
            cornerPoint[0]=origin+normal.orthoVec()*edgeLength+help*edgeLength;
            cornerPoint[1]=origin-normal.orthoVec()*edgeLength+help*edgeLength;
            cornerPoint[2]=origin-normal.orthoVec()*edgeLength-help*edgeLength;
            cornerPoint[3]=origin+normal.orthoVec()*edgeLength-help*edgeLength;


            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glBegin(GL_POLYGON);
                    {
                    for (int i=0; i<4;i++)

                    {
                            glColor4f(0, 0 , 0.4,0.2);
                            glVertex3dv(cornerPoint[i]);
                    }
                    }
                    glEnd();
                    glLineWidth(2.0);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    glBegin(GL_POLYGON);
                    {
                    for (int i=0; i<4;i++)

                    {
                            glColor4f(0, 0 , 0.4,0.7);
                            glVertex3dv(cornerPoint[i]);
                    }
                    }
            glEnd();

            //draw first corner point == control point
            /*glBegin(GL_POINTS);
            {
                    glColor4f(0, 0 , 1,1);
                    glVertex3dv(cornerPoints[0]);
            }
            glEnd();*/

            glLineWidth(1.0);
        }
//****************************************************
/*! Activate the reading flag*/
        void GLViewer2::activateReadFlag()
        {
            readFileFlag=1;
        }
//****************************************************
/*! Activate the reading flag*/
        void GLViewer2::deactivateReadFlag()
        {
            readFileFlag=0;
            updateGL();

        }
//****************************************************
/*! Create a dispay list for all geometric points*/
void GLViewer2::createPointsDisplayList()
{
    glNewList(8001,GL_COMPILE);

    glPointSize(5);
    glColor3f(0.,1.,1.);
    //plot points
    glBegin(GL_POINTS);


    for (long n=0;n<geoData->pointList.size();n++)
    {
        glVertex3dv(geoData->pointList[n].getCoordinates());
    }

    glEnd();
    glEndList();
}
//******************************************************
/*! Plot an element*/
void GLViewer2::plotElement(long elementLabel, float *color)
{
    QList<long> faceIndices;
    faceIndices.clear();
    //go through the facelist and get the indices of all faces of the element
    for (long f=0;f<feData->faceList.size();f++)
    {
        if (feData->faceList[f].parentElement==elementLabel)
        {
            faceIndices.append(f);
        }
    }
    long faceIndex;
    //emit sendMessage(QString("Number of faces %1").arg(faceIndices.size()));
    //***********
    //plot the faces
    for (long k=0;k<faceIndices.size();k++)
    {
        faceIndex=faceIndices[k];
        switch(feData->faceList[faceIndex].type)
            {
                    case 3:
                    glColor4fv(color);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glBegin(GL_TRIANGLES);
                    {
                            //glColor4f(0.5, 0.5 , 0.5,0.2);
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                    }
                    glEnd();
                    glColor4f(0,0,0,1);
                    glBegin(GL_LINES);
                    {
                            //glColor4f(0.5, 0.5 , 0.5,0.2);
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                            glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));

                    }
                    glEnd();
                    break;
                case 4:
                    glColor4fv(color);
                    glBegin(GL_TRIANGLES);
                    {
                    //glColor4f(0.5, 0.5 , 0.5,0.2);
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));

                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                    }
                    glEnd();
                    //
                    glColor4f(0,0,0,1);
                    glBegin(GL_LINES);
                    {
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                    }

                break;
                case 6:
                    glColor4fv(color);
                    glBegin(GL_TRIANGLES);
                    {

                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));

                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));

                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));

                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[4]));
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));
                    }
                    glEnd();
                    glColor4f(0,0,0,1);

                    glBegin(GL_LINES);
                    {
                //glColor4f(0.5, 0.5 , 0.5,0.2);
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[1]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[2]));

                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[3]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[4]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[5]));
                glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));

            }
            glEnd();
            break;
                case 8:
                    glColor4fv(color);

                glBegin(GL_TRIANGLE_FAN);
                {
                //glColor4f(0.5, 0.5 , 0.5,0.2);

                    glVertex3dv(feData->faceList[faceIndex].centroid);
                    for (int n=0; n<feData->faceList[faceIndex].type; n++)
                    {
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[n]));
                    }
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));

                }
                glEnd();
                glColor4f(0,0,0,1);
                glBegin(GL_LINES);
                {
                    for (int n=0; n<feData->faceList[faceIndex].type; n++)
                    {
                        glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[n]));
                    }
                    glVertex3dv(feData->getNodeCoords(feData->faceList[faceIndex].nodeList[0]));

                }
                break;

            }

    }

}
//***********************************************************************
    void GLViewer2::plotElSet(int index)
    {
        float color[4];
         switch (feData->setList[index].elementPlotColor)
         {
         case 0:
             //glColor3f(1.,0.,0.);
             color[0]=1;
             color[1]=0;
             color[2]=0;
             color[3]=1;
         break;
         case 1:
            glColor3f(0.,1.,0.);
         break;
         case 2:
            glColor3f(0.,0.,1.);
         break;
         default:
            glColor3f(1.,0.,0.);
         break;
         }
         if (feData->dispAvailable==false)
         {
             for (long i=0; i<feData->setList[index].elementList.size(); ++i)
             {
                plotElement(feData->setList[index].elementList[i],color);
             }
         }

}
//*********************
void GLViewer2::drawOctree()
{

       drawOcNode(feData->nodeOctree->rootNode);

}
//*********************
void GLViewer2::drawOcNode(octNode *oc)
{
    //emit sendMessage("Drawing Node");
    drawCubeWireframe(oc->getCenter(),oc->edgeLength);
    if (oc->hasChildren==false)
    {
        return;
    }
    for (int k=0;k<8;k++)
    {
        //if (oc->children[k].hasItems==true)
        //{
         //   emit sendMessage("Drawing child");
        drawOcNode(&(oc->children[k]));
        //}
    }

}

//*********************
void GLViewer2::drawCubeWireframe(vector3d center,double length)
{
    //emit sendMessage(QString("length %1").arg(length));
    glColor4f(0,0,0,0);
    glLineWidth(1.0);
    glBegin(GL_LINE_STRIP);
        //draw the edges
        glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z+0.5*length);
        glVertex3d(center.x-0.5*length,center.y+0.5*length,center.z+0.5*length);
        glVertex3d(center.x-0.5*length,center.y-0.5*length,center.z+0.5*length);
        glVertex3d(center.x+0.5*length,center.y-0.5*length,center.z+0.5*length);
        glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z+0.5*length);
    glEnd();
    glBegin(GL_LINE_STRIP);
        //draw the edges
        glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z-0.5*length);
        glVertex3d(center.x-0.5*length,center.y+0.5*length,center.z-0.5*length);
        glVertex3d(center.x-0.5*length,center.y-0.5*length,center.z-0.5*length);
        glVertex3d(center.x+0.5*length,center.y-0.5*length,center.z-0.5*length);
        glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z-0.5*length);
    glEnd();
    glBegin(GL_LINES);
    glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z+0.5*length);
    glVertex3d(center.x+0.5*length,center.y+0.5*length,center.z-0.5*length);
    glVertex3d(center.x-0.5*length,center.y+0.5*length,center.z+0.5*length);
    glVertex3d(center.x-0.5*length,center.y+0.5*length,center.z-0.5*length);
    glVertex3d(center.x-0.5*length,center.y-0.5*length,center.z+0.5*length);
    glVertex3d(center.x-0.5*length,center.y-0.5*length,center.z-0.5*length);
    glVertex3d(center.x+0.5*length,center.y-0.5*length,center.z+0.5*length);
    glVertex3d(center.x+0.5*length,center.y-0.5*length,center.z-0.5*length);
    glEnd();

}
//*****************************************
/*! Sets the view to xy*/
void GLViewer2::setViewXY()
        //OPengl x ->right
        //      y -> down !!
{
   rotMatrix[0][0]=1;
   rotMatrix[0][1]=0;
   rotMatrix[0][2]=0;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=0;
   rotMatrix[1][1]=1;
   rotMatrix[1][2]=0;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=0;
   rotMatrix[2][1]=0;
   rotMatrix[2][2]=1;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   rotquat.normalize();
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
//*****************************************
/*! Sets the view to xy*/
void GLViewer2::setViewYX()
{
   rotMatrix[0][0]=0;
   rotMatrix[0][1]=1;
   rotMatrix[0][2]=0;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=1;
   rotMatrix[1][1]=0;
   rotMatrix[1][2]=0;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=0;
   rotMatrix[2][1]=0;
   rotMatrix[2][2]=-1;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
//*****************************************
//*****************************************
/*! Sets the view to yz*/
void GLViewer2::setViewYZ()
{
   rotMatrix[0][0]=0;
   rotMatrix[0][1]=0;
   rotMatrix[0][2]=1;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=1;
   rotMatrix[1][1]=0;
   rotMatrix[1][2]=0;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=0;
   rotMatrix[2][1]=1;
   rotMatrix[2][2]=0;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
//*****************************************s
/*! Sets the view to xz*/
void GLViewer2::setViewXZ()
{
   rotMatrix[0][0]=1;
   rotMatrix[0][1]=0;
   rotMatrix[0][2]=0;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=0;
   rotMatrix[1][1]=0;
   rotMatrix[1][2]=-1;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=0;
   rotMatrix[2][1]=1;
   rotMatrix[2][2]=0;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
//*****************************************
/*! Sets the view to zx*/
void GLViewer2::setViewZX()
{
   rotMatrix[0][0]=0;
   rotMatrix[0][1]=1;
   rotMatrix[0][2]=0;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=0;
   rotMatrix[1][1]=0;
   rotMatrix[1][2]=1;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=1;
   rotMatrix[2][1]=0;
   rotMatrix[2][2]=0;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
//*****************************************
/*! Sets the view to zy*/
void GLViewer2::setViewZY()
{
   rotMatrix[0][0]=0;
   rotMatrix[0][1]=0;
   rotMatrix[0][2]=-1;
   rotMatrix[0][3]=0;

   rotMatrix[1][0]=0;
   rotMatrix[1][1]=1;
   rotMatrix[1][2]=0;
   rotMatrix[1][3]=0;

   rotMatrix[2][0]=1;
   rotMatrix[2][1]=0;
   rotMatrix[2][2]=0;
   rotMatrix[2][3]=0;

   rotMatrix[3][0]=0;
   rotMatrix[3][1]=0;
   rotMatrix[3][2]=0;
   rotMatrix[3][3]=1;

   emit sendMessage(QString("rotquat before %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));
   rotquat=MatrixToQuat(rotMatrix);
   emit sendMessage(QString("rotquat after %0 %1 %2 %3").arg(rotquat.w).arg(rotquat.x).arg(rotquat.y).arg(rotquat.z));

    rotquat.normalize();
    //build_rotmatrix(rotMatrix, rotquat);
    updateGL();
}
void GLViewer2::toggleOctreeFlag()
{
    if (octreeFlag==0)
        octreeFlag=1;
    else
        octreeFlag=0;

    updateGL();
    paintGL();

}
//***********************************************
/*! Start a time animation*/
void GLViewer2::startTimeAnimation(timeAnimationClass *timeAnim)
{
    timeAnimFlag=1;
    timeAnimationTimer->start(100);
    emit sendMessage("Time Animation started");
    updateGL();
}
//***********************************************
/*! Stop a time animation*/
void GLViewer2::stopTimeAnimation()
{
    timeAnimFlag=0;
    timeAnimationTimer->stop();
    emit sendMessage("Time Animation stopped");
    updateGL();
}
//********************************************************************************************************
/*! Updates the time animation*/
void GLViewer2::updateTimeAnimation()
{
        int nSteps=feData->timeAnimation->nTimesteps;
        //emit sendMessage(QString("update %1 %2 ").arg(animCount).arg(animationScaleFactors[animCount]));
        currentTimeAnimFrame++;

        if (currentTimeAnimFrame> nSteps+7200)
        {
            currentTimeAnimFrame=7200;
        }
        currentTime=feData->timeAnimation->timepoints[currentTimeAnimFrame-7200];
        updateGL();
}
//*************************************************************************************************
/*! Draw geometric surfaces*/
void GLViewer2::drawSurfaces()
{


    for (int s=0;s<geoData->surfaceList.size();s++)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBegin(GL_POLYGON);
                {
                for (int i=0; i<geoData->surfaceList[s].points.size();i++)

                {
                        glColor4f(0, 0 , 0.4,0.2);
                        glVertex3dv(geoData->getPointCoords(geoData->surfaceList[s].points[i]));
                }
                }
                glEnd();
                /*glLineWidth(2.0);
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glBegin(GL_POLYGON);
                {
                for (int i=0; i<4;i++)

                {
                        glColor4f(0, 0 , 0.4,0.7);
                        glVertex3dv(cornerPoint[i]);
                }
                }
        glEnd();*/

    }

}
