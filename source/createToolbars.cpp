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
 * \file createToolbars.cpp
 * \brief A brief file description
 **/
 #include <QtGui>
 #include "mainWin.h"
 #include "resultselector.h"

void MainWindow::createToolBars()
{
	fileToolBar = new QToolBar(tr("File"),this);
    addToolBar(Qt::TopToolBarArea,fileToolBar);
    //fileToolBar->addAction(newAct);
	fileToolBar->addAction(openAct);
	//fileToolBar->addAction(saveAct);
	
    viewToolBar = new QToolBar(tr("View"),this);
	addToolBar(Qt::TopToolBarArea,viewToolBar);
    viewToolBar->addAction(setRotCenterAct);
	viewToolBar->addAction(viewWireframeAct);
    viewToolBar->addAction(viewShadedAct);
	viewToolBar->addAction(toggleNodesAct);
    viewToolBar->addAction(toggleShowPointsAct);
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
	viewToolBar->addAction(startAnimAct);
        viewToolBar->addAction(startTimeAnimAct);
        viewToolBar->addAction(stopAnimAct);


    viewOrientationToolBar = new QToolBar(tr("View Orientation"),this);
    addToolBar(Qt::TopToolBarArea,viewOrientationToolBar);
    //viewOrientationToolBar->addAction(viewXYAct);
    //viewOrientationToolBar->addAction(viewXZAct);
    //viewOrientationToolBar->addAction(viewYXAct);
    //viewOrientationToolBar->addAction(viewYZAct);
    //viewOrientationToolBar->addAction(viewZXAct);
    //viewOrientationToolBar->addAction(viewZYAct);

    viewOrientationToolButton = new QToolButton();
    viewOrientationToolButton->setDefaultAction(viewXYAct);
    viewOrientationToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    viewOrientationToolButton->addAction(viewXYAct);
    viewOrientationToolButton->addAction(viewXZAct);
    viewOrientationToolButton->addAction(viewYXAct);
    viewOrientationToolButton->addAction(viewYZAct);
    viewOrientationToolButton->addAction(viewZXAct);
    viewOrientationToolButton->addAction(viewZYAct);
    viewOrientationToolBar->addWidget(viewOrientationToolButton);
	
	resultToolBar = new QToolBar(tr("Results"),this);
    addToolBar(Qt::TopToolBarArea,resultToolBar);
	resultSelect = new resultSelector(this,modelDatabase);
	resultToolBar->addWidget(resultSelect);
	
	planeToolBar = new QToolBar(tr("Plane"));
	planeToolBar->addAction(planeOffsetAct);
	planeToolBar->addAction(planeDeleteAct);
	addToolBar(Qt::RightToolBarArea,planeToolBar);
	planeToolBar->hide();

        nodeToolBar = new QToolBar(tr("Node"));
        nodeToolBar->addAction(mergeNodesAct);
        //nodeToolBar->addAction(planeDeleteAct);
        addToolBar(Qt::LeftToolBarArea,nodeToolBar);
        nodeToolBar->hide();
	
        cosToolBar = new QToolBar(tr("COS"));
        cosToolBar->addAction(createCOSAct);
        cosToolBar->addAction(deleteCOSAct);

	geometryToolBar = new QToolBar(tr("Geometry"));
	geometryToolBar->addAction(createPointXYZAct);
	geometryToolBar->addAction(createCurveAct);
    geometryToolBar->addAction(meshStlAct);
	//geometryToolBar->addAction(createSurfaceAct);		
	//geometryToolBar->addAction(createBodyAct);
        addToolBar(Qt::LeftToolBarArea,cosToolBar);
        cosToolBar->hide();
	addToolBar(Qt::LeftToolBarArea,geometryToolBar);
	geometryToolBar->hide();

    feDataToolBar = new QToolBar(tr("FE-Data"));
    feDataToolBar->addAction(createBCAct);
    feDataToolBar->addAction(createLoadAct);
    feDataToolBar->addAction(createMaterialAct);
    feDataToolBar->addAction(createFeSectionAct);
    addToolBar(Qt::LeftToolBarArea,feDataToolBar);
    feDataToolBar->hide();

	printToolBar = new QToolBar(tr("Print"),this);
	addToolBar(Qt::TopToolBarArea,printToolBar);
	printToolBar->addAction(screenshotAct);

    bcToolBar = new QToolBar(tr("BC"),this);
    addToolBar(Qt::LeftToolBarArea,bcToolBar);
    bcToolBar->addAction(deleteBCAct);
    bcToolBar->hide();

    materialToolBar = new QToolBar(tr("Material"),this);
    addToolBar(Qt::LeftToolBarArea,materialToolBar);
    materialToolBar->addAction(deleteMaterialAct);
    materialToolBar->hide();

    feSectionToolBar = new QToolBar(tr("FE-Section"),this);
    addToolBar(Qt::LeftToolBarArea,feSectionToolBar);
    feSectionToolBar->addAction(deleteFeSectionAct);
    feSectionToolBar->hide();

    loadToolBar = new QToolBar(tr("Load"),this);
    addToolBar(Qt::LeftToolBarArea,loadToolBar);
    loadToolBar->addAction(deleteLoadAct);
    loadToolBar->hide();

    elementToolBar = new QToolBar(tr("Elements"),this);
    addToolBar(Qt::LeftToolBarArea,elementToolBar);
    elementToolBar->addAction(verifyElementsAct);
    elementToolBar->hide();

    stlToolBar = new QToolBar(tr("STL"),this);
    addToolBar(Qt::LeftToolBarArea,stlToolBar);
    stlToolBar->hide();

    nSetToolBar = new QToolBar(tr("Nodesets"),this);
    addToolBar(Qt::LeftToolBarArea,nSetToolBar);
    nSetToolBar->addAction(showNSetAct);
    nSetToolBar->addAction(hideNSetAct);
    nSetToolBar->hide();

    faceSetToolBar = new QToolBar(tr("Face Sets"),this);
    addToolBar(Qt::LeftToolBarArea,faceSetToolBar);
    faceSetToolBar->addAction(showFaceSetAct);
    faceSetToolBar->addAction(hideFaceSetAct);
    faceSetToolBar->hide();

    elSetToolBar = new QToolBar(tr("Element Sets"),this);
    addToolBar(Qt::LeftToolBarArea,elSetToolBar);
    elSetToolBar->addAction(showElSetAct);
    elSetToolBar->addAction(hideElSetAct);
    elSetToolBar->hide();



}
