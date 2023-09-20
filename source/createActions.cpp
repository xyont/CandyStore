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
/**
 * \file createActions.cpp
 * \brief A brief file description
 **/
    #include <QtGui>

    #include "mainWin.h"

	#define __appName "CandyStore"
	
void MainWindow::createActions()
    {
        newAct = new QAction(QIcon(":/filenew.png"), tr("&New File"), this);
        newAct->setShortcut(tr("Ctrl+N"));
        newAct->setStatusTip(tr("Create a new file"));
        connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

        openCSDAct = new QAction(QIcon(":/fileopen.png"), tr("Open CSD..."), this);
        //openCSDAct->setShortcut(tr("Ctrl+O"));
        openCSDAct->setStatusTip(tr("Open an existing CSD file"));
        connect(openCSDAct, SIGNAL(triggered()), this, SLOT(readCSD()));

        openAct = new QAction(QIcon(":/fileopen.png"), tr("&Open FRD"), this);
        openAct->setShortcut(tr("Ctrl+O"));
        openAct->setStatusTip(tr("Open an existing file"));
        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

        importAct = new QAction(QIcon(":/fileopen.png"), tr("Import File..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        importAct->setStatusTip(tr("Import an existing file"));
        connect(importAct, SIGNAL(triggered()), centrWin, SLOT(showFileImport()));

		
        importInpAct = new QAction(QIcon(":/fileopen.png"), tr("Import Inp..."), this);
        importInpAct->setShortcut(tr("Ctrl+I"));
        importInpAct->setStatusTip(tr("Open an existing file"));
        connect(importInpAct, SIGNAL(triggered()), this, SLOT(importInp()));

        importCDBAct = new QAction(QIcon(":/fileopen.png"), tr("Import CDB..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        importCDBAct->setStatusTip(tr("Open an Ansys CDB file"));
        connect(importCDBAct, SIGNAL(triggered()), this, SLOT(importCDB()));
		
        importVTKAct = new QAction(QIcon(":/fileopen.png"), tr("Import VTK..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        importVTKAct->setStatusTip(tr("Import an VTK file"));
        connect(importVTKAct, SIGNAL(triggered()), this, SLOT(importVtk()));

        importSTLAct = new QAction(QIcon(":/fileopen.png"), tr("Import STL..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        importSTLAct->setStatusTip(tr("Import a STL file"));
        connect(importSTLAct, SIGNAL(triggered()), this, SLOT(importStl()));

        runFBDAct = new QAction(QIcon(":/fileopen.png"), tr("Run FBD..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        runFBDAct->setStatusTip(tr("Run a fbd file"));
        connect(runFBDAct, SIGNAL(triggered()), this, SLOT(runFbd()));

        exportVTKAct = new QAction(QIcon(":/filesave.png"), tr("Export VTK..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        exportVTKAct->setStatusTip(tr("Export an VTK file"));
        connect(exportVTKAct, SIGNAL(triggered()), this, SLOT(exportVTKclicked()));

        exportOpenFoamAct = new QAction(QIcon(":/filesave.png"), tr("Export OpenFOAM..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        exportOpenFoamAct->setStatusTip(tr("Export an OpenFOAM file"));
        connect(exportOpenFoamAct, SIGNAL(triggered()), this, SLOT(exportOpenFoamClicked()));

        exportFrdAct  = new QAction(QIcon(":/filesave.png"), tr("Export Frd..."), this);
        //importInpAct->setShortcut(tr("Ctrl+I"));
        exportFrdAct->setStatusTip(tr("Export an Frd file"));
        connect(exportFrdAct, SIGNAL(triggered()), this, SLOT(exportFrdClicked()));

        saveAct = new QAction(QIcon(":/filesave.png"), tr("&Save"), this);
        saveAct->setShortcut(tr("Ctrl+S"));
        saveAct->setStatusTip(tr("Save the document to disk"));
        connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

        saveAsAct = new QAction(QIcon(":/filesaveas.png"),tr("Save &As..."), this);
        saveAsAct->setStatusTip(tr("Save the document under a new name"));
        connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

        exitAct = new QAction(QIcon(":/fileclose.png"),tr("E&xit"), this);
        exitAct->setShortcut(tr("Ctrl+Q"));
        exitAct->setStatusTip(tr("Exit the application"));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
//*************************************************************************************
//recentFiles	
	for (int i = 0; i < MaxRecentFiles; ++i) 
		{
            recentFileActs[i] = new QAction(this);
            recentFileActs[i]->setVisible(false);
            connect(recentFileActs[i], SIGNAL(triggered()),
                    this, SLOT(openRecentFile()));
		}
//******************************************************************************

	//Viewing Actions
        viewWireframeAct = new QAction(QIcon(":/wireframe.png"), tr("Wireframe"), this);
        //viewWireFrameAct->setShortcut(tr("Ctrl+X"));
        viewWireframeAct->setStatusTip(tr("View model in wireframe mode"));
        connect(viewWireframeAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setWireframeMode()));

        viewShadedAct = new QAction(QIcon(":/solid.png"), tr("Shaded"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
        viewShadedAct->setStatusTip(tr("view model in shaded mode"));
        connect(viewShadedAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setShadedMode()));

	setRotCenterAct = new QAction(QIcon(":/rotcenter.png"), tr("Rotation Center"), this);
    //viewShadedAct->setShortcut(tr("Ctrl+C"));
    setRotCenterAct->setStatusTip(tr("Set new rotation center"));
	connect(setRotCenterAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setSelectRotFlag()));

	toggleNodesAct = new QAction(QIcon(":/toggleNodes.png"), tr("Toggle Nodes"), this);
       //viewShadedAct->setShortcut(tr("Ctrl+C"));
    toggleNodesAct->setStatusTip(tr("Show/Hide Nodes"));
    connect(toggleNodesAct, SIGNAL(triggered()), this, SLOT(toggleNodes()));

    toggleShowPointsAct = new QAction(QIcon(":/togglePoints.png"), tr("Toggle Points"), this);
       //viewShadedAct->setShortcut(tr("Ctrl+C"));
    toggleShowPointsAct->setStatusTip(tr("Show/Hide Points"));
    connect(toggleShowPointsAct, SIGNAL(triggered()), this, SLOT(togglePoints()));
		
	setBackgroundColorAct = new QAction(tr("Background Color"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
    setBackgroundColorAct->setStatusTip(tr("Select Background Color"));
    connect(setBackgroundColorAct, SIGNAL(triggered()), this, SLOT(setBgColor()));

        graphicOptionsAct=new QAction(tr("Graphic Options "), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
    graphicOptionsAct->setStatusTip(tr("Set Graphics Options"));
        connect(graphicOptionsAct, SIGNAL(triggered()), centrWin, SLOT(showGraphicOptions()));
	
	zoomOutAct = new QAction(QIcon(":/viewmag-.png"),tr("Zoom Out"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
    zoomOutAct->setStatusTip(tr("Zoom Out"));
    connect(zoomOutAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(zoomMinus()));
	
	zoomInAct = new QAction(QIcon(":/viewmag+.png"),tr("Zoom In"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
    zoomInAct->setStatusTip(tr("Zoom In"));
    connect(zoomInAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(zoomPlus()));
	
	setBackgroundColorAct = new QAction(tr("Background Color"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+C"));
    setBackgroundColorAct->setStatusTip(tr("Select Background Color"));
    connect(setBackgroundColorAct, SIGNAL(triggered()), this, SLOT(setBgColor()));
	
	startAnimAct = new QAction(QIcon(":/player_play.png"),tr("Start Animation"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    startAnimAct->setStatusTip(tr("Start a scale factor animation"));
    connect(startAnimAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(startAnimation()));
	
    startTimeAnimAct = new QAction(QIcon(":/player_play_time.png"),tr("Start Time Animation"), this);
    //viewShadedAct->setShortcut(tr("Ctrl+A"));
    startTimeAnimAct->setStatusTip(tr("Start a time animation"));
    connect(startTimeAnimAct, SIGNAL(triggered()), modelDatabase, SLOT(createTimeAnimation()));

	stopAnimAct = new QAction(QIcon(":/player_stop.png"),tr("Stop Animation"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    stopAnimAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(stopAnimAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(stopAnimation()));
    connect(stopAnimAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(stopTimeAnimation()));


    viewXYAct = new QAction(QIcon(":/view-xy.png"),tr("Change to xy view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewXYAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewXYAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewXY()));

    viewYXAct = new QAction(QIcon(":/view-yx.png"),tr("Change to yx view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewYXAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewYXAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewYX()));

    viewXZAct = new QAction(QIcon(":/view-xz.png"),tr("Change to xz view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewXZAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewXZAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewXZ()));

    viewYZAct = new QAction(QIcon(":/view-yz.png"),tr("Change to yz view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewYZAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewYZAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewYZ()));

    viewZXAct = new QAction(QIcon(":/view-zx.png"),tr("Change to xz view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewZXAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewZXAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewZX()));

    viewZYAct = new QAction(QIcon(":/view-zy.png"),tr("Change to yz view"), this);
        //viewShadedAct->setShortcut(tr("Ctrl+A"));
    viewZYAct->setStatusTip(tr("Stop the current scale factor animation"));
    connect(viewZYAct, SIGNAL(triggered()), centrWin->viewingArea, SLOT(setViewZY()));
	
//**********************************************************************************************************
//Result Actions
	resultOptionAct = new QAction(QIcon(":/main.png"), tr("Result Options"), this);
    //viewShadedAct->setShortcut(tr("Ctrl+C"));
    resultOptionAct->setStatusTip(tr("Set Result Options"));
	connect(resultOptionAct, SIGNAL(triggered()), centrWin, SLOT(showResultOptions()));
//**********************************************************************************************************
//Help Actions
        aboutAct = new QAction(QIcon(":/main.png"),tr("&About CandyStore"), this);
        aboutAct->setStatusTip(tr("Show the application's About box"));
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
		//connect(aboutAct, SIGNAL(triggered()), centrWin, SLOT(commandReceived()));

        aboutQtAct = new QAction(QIcon(":/aboutqt.png"),tr("About &Qt"), this);
        aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
        connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

		aboutMouseBindingsAct = new QAction(QIcon(":/main.png"),tr("Help"), this);
        aboutMouseBindingsAct->setStatusTip(tr("Show the help window"));
        connect(aboutMouseBindingsAct, SIGNAL(triggered()), this, SLOT(help()));
		//connect(aboutAct, SIGNAL(triggered()), centrWin, SLOT(commandReceived()));

        setHelpFileAct = new QAction(QIcon(":/main.png"),tr("Set Help File"), this);
        setHelpFileAct->setStatusTip(tr("Set the help file to be used"));
        connect(setHelpFileAct, SIGNAL(triggered()), this, SLOT(setHelpFile()));

//**************************************************************************************************************
//Print Actions
		screenshotAct = new QAction(QIcon(":/screenshot.png"),tr("Save Snapshot"), this);
        screenshotAct->setStatusTip(tr("Save a snapshot shot of the viewing area"));
        connect(screenshotAct, SIGNAL(triggered()), this, SLOT(takeScreenshot()));
		//connect(aboutAct, SIGNAL(triggered()), centrWin, SLOT(commandReceived()));

    
//****************************************************************************************************************
//Special Actions
	writeOutsideFacesAct = new QAction(QIcon(":/screenshot.png"),tr("Write Outside Faces"), this);
	writeOutsideFacesAct->setStatusTip(tr("Write all outside faces into a file"));
    connect(writeOutsideFacesAct, SIGNAL(triggered()), this, SLOT(writeOutFaces()));

	interpolateResultsAct = new QAction(QIcon(":/screenshot.png"),tr("Interpolate Results"), this);
	interpolateResultsAct->setStatusTip(tr("Interpolate results from a results file"));
    connect(interpolateResultsAct, SIGNAL(triggered()), centrWin, SLOT(showInterpolateOptions()));

        hex8toHex20Act = new QAction(QIcon(":/screenshot.png"),tr("Hex8 to Hex20"), this);
        hex8toHex20Act->setStatusTip(tr("Change Hex 8 elments into Hex 20 elements and create midside nodes"));
    connect(hex8toHex20Act, SIGNAL(triggered()), this, SLOT(changeHex8()));

    createDiscAct = new QAction(QIcon(":/screenshot.png"),tr("Disc Creator"), this);
    createDiscAct->setStatusTip(tr("Create a turbo engine disc"));
    connect(createDiscAct, SIGNAL(triggered()), centrWin, SLOT(showCreateDisc()));
 //**************************************************************************************************
 //FE Actions
 //Node Actions
    mergeNodesAct = new QAction(QIcon(":/mergeNodes.png"),tr("Merge Nodes"), this);
    mergeNodesAct->setStatusTip(tr("Merge close nodes "));
    connect(mergeNodesAct, SIGNAL(triggered()), centrWin, SLOT(showMergeNodes()));

    createBCAct = new QAction(QIcon(":/boundarycon.png"),tr("Create BC"), this);
    createBCAct->setStatusTip(tr("Create boundary conditions "));
    connect(createBCAct, SIGNAL(triggered()), centrWin, SLOT(showBC()));

    createMaterialAct = new QAction(QIcon(":/materialData.png"),tr("Create Material"), this);
    createMaterialAct->setStatusTip(tr("Create Material "));
    connect(createMaterialAct, SIGNAL(triggered()), centrWin, SLOT(showMat()));

    createFeSectionAct = new QAction(QIcon(":/section.png"),tr("Create FE Section"), this);
    createFeSectionAct->setStatusTip(tr("Create a FE Section "));
    connect(createFeSectionAct, SIGNAL(triggered()), centrWin, SLOT(showFeSect()));

    createLoadAct = new QAction(QIcon(":/loads.png"),tr("Create Load"), this);
    createLoadAct->setStatusTip(tr("Create a load "));
    connect(createLoadAct, SIGNAL(triggered()), centrWin, SLOT(showLoadWidget()));

    //**************************************************************************************************
//Geometry Actions
    meshStlAct = new QAction(QIcon(":/meshgeo.png"),tr("Mesh Geometry"), this);
    meshStlAct->setStatusTip(tr("Mesh the current Geometry"));
    connect(meshStlAct, SIGNAL(triggered()), centrWin, SLOT(showMeshGeo()));
//COS Actions
	createCOSAct = new QAction(QIcon(":/addCOS.png"),tr("Create a new coordinate system "), this);
	createCOSAct->setStatusTip(tr("Opens the coordinate system creation dialog"));
        connect(createCOSAct, SIGNAL(triggered()), centrWin, SLOT(showCreateCOS()));

        deleteCOSAct = new QAction(QIcon(":/deleteCOS.png"),tr("Delete a coordinate system "), this);
        deleteCOSAct->setStatusTip(tr("Opens the delete coordinate system dialog"));
        connect(deleteCOSAct, SIGNAL(triggered()), centrWin, SLOT(showDeleteCOS()));
//PointActions
	createPointXYZAct = new QAction(QIcon(":/addPoint.png"),tr("Create a new point "), this);
	createPointXYZAct->setStatusTip(tr("Opens the point creation dialog"));
    connect(createPointXYZAct, SIGNAL(triggered()), centrWin, SLOT(showCreatePoint()));
//CurveActions
	createCurveAct = new QAction(QIcon(":/addCurve.png"),tr("Create a new curve "), this);
	createCurveAct->setStatusTip(tr("Opens the curve creation dialog"));
    connect(createCurveAct, SIGNAL(triggered()), centrWin, SLOT(showCreateCurve()));
//Plane Actions
	planeOffsetAct = new QAction(QIcon(":/screenshot.png"),tr("Create an offset plane "), this);
	planeOffsetAct->setStatusTip(tr("Creates a new plane with a given offset"));
    connect(planeOffsetAct, SIGNAL(triggered()), this, SLOT(createOffsetPlaneClicked()));
	
	planeDeleteAct = new QAction(QIcon(":/screenshot.png"),tr("Delete a plane "), this);
	planeDeleteAct->setStatusTip(tr("Deletes the selected plane"));
    //connect(planeDeleteAct, SIGNAL(triggered()), this->geoDatabase, SLOT(deletePlane()));
//**************************************************************************************************
//Analysis Actions
	abqInputDeckAct = new QAction(QIcon(":/screenshot.png"),tr("Export Abaqus/CCX Input Deck"), this);
	abqInputDeckAct->setStatusTip(tr("Creates an ABAQUS/CCX Input File from the current model "));
    connect(abqInputDeckAct, SIGNAL(triggered()), this, SLOT(abqInputDeckClicked()));

    setCcxLocationAct = new QAction(QIcon(":/main.png"),tr("Set CCX Location"), this);
    setCcxLocationAct->setStatusTip(tr("Set the location of the CCX binary."));
    connect(setCcxLocationAct, SIGNAL(triggered()), this, SLOT(setCcxLocation()));
//**************************************************************************************************
//BC ACtions
    deleteBCAct = new QAction(QIcon(":/delete.png"),tr("Delete BC"), this);
    deleteBCAct->setStatusTip(tr("Delete a Boundary Condition"));
    connect(deleteBCAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(deleteBC()));
    //**************************************************************************************************
    //Material ACtions
        deleteMaterialAct = new QAction(QIcon(":/delete.png"),tr("Delete Material"), this);
        deleteMaterialAct->setStatusTip(tr("Delete a Material"));
        connect(deleteMaterialAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(deleteMat()));
        //**************************************************************************************************
        //FE SEction ACtions
            deleteFeSectionAct = new QAction(QIcon(":/delete.png"),tr("Delete FE Section"), this);
            deleteFeSectionAct->setStatusTip(tr("Delete a FE Section"));
            connect(deleteFeSectionAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(deleteFeSec()));
            //Load ACtions
                deleteLoadAct = new QAction(QIcon(":/delete.png"),tr("Delete Load"), this);
                deleteLoadAct->setStatusTip(tr("Delete a Load"));
                connect(deleteLoadAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(deleteLoad()));
//**************************************************************************************************
//Element Actions
    verifyElementsAct = new QAction(QIcon(":/check.png"),tr("Check Elements"), this);
    verifyElementsAct->setStatusTip(tr("Check Element Quality"));
    connect(verifyElementsAct, SIGNAL(triggered()), centrWin, SLOT(showVerifyElements()));
    //**************************************************************************************************
//NSet Actions
    showNSetAct = new QAction(QIcon(":/show.png"),tr("Show Node Set"), this);
    showNSetAct->setStatusTip(tr("Show Node Set"));
    connect(showNSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(showNSet()));

    hideNSetAct = new QAction(QIcon(":/hide.png"),tr("Hide Node Set"), this);
    hideNSetAct->setStatusTip(tr("Hide Node Set"));
    connect(hideNSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(hideNSet()));
//Face Set Actions
    showFaceSetAct = new QAction(QIcon(":/show.png"),tr("Show Face Set"), this);
    showFaceSetAct->setStatusTip(tr("Show Face Set"));
    connect(showFaceSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(showFaceSet()));

    hideFaceSetAct = new QAction(QIcon(":/hide.png"),tr("Hide Face Set"), this);
    hideFaceSetAct->setStatusTip(tr("Hide Face Set"));
    connect(hideFaceSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(hideFaceSet()));
    //Element Set Actions
        showElSetAct = new QAction(QIcon(":/show.png"),tr("Show Element Set"), this);
        showElSetAct->setStatusTip(tr("Show Element Set"));
        connect(showElSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(showElSet()));

        hideElSetAct = new QAction(QIcon(":/hide.png"),tr("Hide Element Set"), this);
        hideElSetAct->setStatusTip(tr("Hide Element Set"));
        connect(hideElSetAct, SIGNAL(triggered()), centrWin->modelTree, SLOT(hideElSet()));
}
