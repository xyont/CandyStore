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
 * \file mainWin.cpp
 * \brief A brief file description
 **/
 
 #include <QtGui>
 #include "mainWin.h"
 #include "glWin2.h"
 #include "modelTreeView.h"
 #include "helpwidget.h"
 #include "helpbrowser.h"
//#include <QWebView>


 #define __appName "CandyStore"
 //********************************************************************************
 /*!Constructor fot the applications main window */
 
 MainWindow::MainWindow()
 {
     setAttribute(Qt::WA_DeleteOnClose);
	

    //
	modelDatabase = new femodel;
	geoDatabase=new geomodel;
	
    readfrd = new readFrdMeshThread();

	centrWin = new Modul3D(this, modelDatabase, geoDatabase);
    // textEdit = new QTextEdit;
     setCentralWidget(centrWin);

     createActions();
     createMenus();
	 createToolBars();
     (void)statusBar();
	 readSettings();

     setWindowTitle(tr(__appName));
	 setWindowIcon(QIcon(":/main.png"));
     //resize(400, 300);
	 nodeStatus=0;
	 connect(modelDatabase, SIGNAL(sendMessage(const QString)), centrWin, SLOT(recieveMessage(const QString)));

	 connect(geoDatabase, SIGNAL(sendMessage(const QString)), centrWin, SLOT(recieveMessage(const QString)));
	 connect(centrWin->viewingArea, SIGNAL(sendMessage(const QString)), centrWin, SLOT(recieveMessage(const QString)));
	 connect(modelDatabase,SIGNAL(sendMinMaxScale(double,double)), centrWin->viewingArea, SLOT(setScaleLabels(double, double)));
	 connect(modelDatabase,SIGNAL(sendFringeAutoValues(double,double)), centrWin->viewingArea, SLOT(setFringeAutoValues(double, double)));
	 connect(modelDatabase,SIGNAL(sendResultTitle(const QString)), centrWin->viewingArea, SLOT(setScaleTitle(const QString)));
         //Das ist Müll!! Bei newFileActions wird die View zurückgesetzt!!
         connect(modelDatabase,SIGNAL(modelUpdate()), centrWin->viewingArea, SLOT(newFileActions()));
         connect(modelDatabase,SIGNAL(modelUpdate()), centrWin->modelTree, SLOT(updateTree()));
         connect(modelDatabase,SIGNAL(update()), centrWin->modelTree, SLOT(updateTree()));
         connect(modelDatabase,SIGNAL(update()), centrWin->viewingArea, SLOT(updateView()));
         connect(modelDatabase,SIGNAL(modelScaled(double)), centrWin->viewingArea, SLOT(modelScaled(double)));
         connect(modelDatabase,SIGNAL(startTimeAnim(timeAnimationClass*)), centrWin->viewingArea, SLOT(startTimeAnimation(timeAnimationClass*)));
         connect(this->resultSelect,SIGNAL(loadStep(int)),this,SLOT(loadNewResults(int)));
         //
         connect(this,SIGNAL(updateView()), centrWin->viewingArea, SLOT(newFileActions()));
        //
          connect(this->resultSelect,SIGNAL(updateFaces(int)),centrWin->viewingArea, SLOT(createDisplacedFaceDisplayList(int)));
          connect(modelDatabase,SIGNAL(newResultValues(int)),centrWin->viewingArea, SLOT(createDisplacedFaceDisplayList(int)));
          connect(modelDatabase,SIGNAL(fringePlot(short)),centrWin->viewingArea, SLOT(setResultPlotStatus(short)));
          connect(modelDatabase,SIGNAL(createTimeFrame(int)),centrWin->viewingArea, SLOT(createDisplacedFaceDisplayList(int)));

          //
         connect(this->resultSelect,SIGNAL(fringePlotAvailable(short)),centrWin->viewingArea, SLOT(setResultPlotStatus(short)));
        connect(centrWin->viewingArea,SIGNAL(nodeSelected(long)),centrWin->infoBox, SLOT(showNodeInformation(long)));
        connect(centrWin->viewingArea,SIGNAL(pointSelected(long)),centrWin->infoBox, SLOT(showPointInformation(long)));
	connect(this->resultSelect,SIGNAL(fringePlotAvailable(short)),centrWin->infoBox, SLOT(updateInfo()));
	connect(centrWin->modelTree, SIGNAL(sendMessage(const QString)), centrWin, SLOT(recieveMessage(const QString)));
	connect(centrWin->modelTree, SIGNAL(stepSelected(int)), this->resultSelect, SLOT(newStepSelected(int)));
        connect(modelDatabase, SIGNAL(stepSelected(int)), this->resultSelect, SLOT(newStepSelected(int)));

        connect(this, SIGNAL(sendMessage(const QString)), centrWin, SLOT(recieveMessage(const QString)));
        connect(this, SIGNAL(runScript(const QString)), centrWin, SLOT(runScript(const QString)));
        connect(this, SIGNAL(toggleNodesSignal()), centrWin->viewingArea, SLOT(toggleNodeStatus()));
        connect(this, SIGNAL(togglePointsSignal()), centrWin->viewingArea, SLOT(togglePointStatus()));


        //**********
        connect(this, SIGNAL(newFileLoaded()), centrWin->viewingArea, SLOT(newFileActions()));
	connect(this, SIGNAL(newFileLoaded()), resultSelect, SLOT(newFileActions()));
	connect(this, SIGNAL(newFileLoaded()), centrWin->modelTree, SLOT(updateTree()));
    //***********
    //Actions to perform, when a new frd file is loaded
    //connect(this,SIGNAL(resetData()),centrWin->viewingArea, SLOT(reset()));
    //connect(this,SIGNAL(resetData()), centrWin->modelTree, SLOT(resetData()));
    connect(readfrd, SIGNAL(started()), centrWin->viewingArea, SLOT(activateReadFlag()));
    connect(readfrd, SIGNAL(finished()), centrWin->viewingArea, SLOT(deactivateReadFlag()));
    connect(readfrd, SIGNAL(finished()), centrWin->viewingArea, SLOT(newFileActions()));
    connect(readfrd, SIGNAL(finished()), resultSelect, SLOT(newFileActions()));
    connect(readfrd, SIGNAL(finished()), centrWin->modelTree, SLOT(updateTree()));
    connect(readfrd, SIGNAL(finished()), this, SLOT(restoreCursor()));
    //***********
    connect(this, SIGNAL(writeOFaces(QString)), modelDatabase, SLOT(writeOutsideFaces(QString)));
	connect(centrWin->modelTree, SIGNAL(planeSelected(long)), this, SLOT(showPlaneTools()));
	connect(centrWin->modelTree, SIGNAL(geometrySelected()), this, SLOT(showGeoTools()));
    connect(centrWin->modelTree, SIGNAL(geometryDeselected()), this, SLOT(hideGeoTools()));
    connect(centrWin->modelTree, SIGNAL(nodesSelected()), this, SLOT(showNodeTools()));
    connect(centrWin->modelTree, SIGNAL(nodesDeselected()), this, SLOT(hideNodeTools()));
	connect(centrWin->modelTree, SIGNAL(planeDeselected()), this, SLOT(hidePlaneTools()));
    connect(centrWin->modelTree, SIGNAL(cosRootSelected()), this, SLOT(showCosTools()));
    connect(centrWin->modelTree, SIGNAL(cosRootDeselected()), this, SLOT(hideCosTools()));
    connect(centrWin->modelTree, SIGNAL(feDataSelected()), this, SLOT(showFeDataTools()));
    connect(centrWin->modelTree, SIGNAL(feDataDeselected()), this, SLOT(hideFeDataTools()));
    connect(centrWin->modelTree, SIGNAL(bcSelected(int)), this, SLOT(showBCTools(int)));
    connect(centrWin->modelTree, SIGNAL(bcDeselected()), this, SLOT(hideBCTools()));
    connect(centrWin->modelTree, SIGNAL(loadSelected(int)), this, SLOT(showLoadTools()));
    connect(centrWin->modelTree, SIGNAL(loadDeselected()), this, SLOT(hideLoadTools()));
    //
    connect(centrWin->modelTree, SIGNAL(nodeSetSelected(int)), this, SLOT(showNodeSetTools(int)));
    connect(centrWin->modelTree, SIGNAL(nodeSetDeselected()), this, SLOT(hideNodeSetTools()));
    //
    connect(centrWin->modelTree, SIGNAL(faceSetSelected()), this, SLOT(showFaceSetTools()));
    connect(centrWin->modelTree, SIGNAL(faceSetDeselected()), this, SLOT(hideFaceSetTools()));
    //
    connect(centrWin->modelTree, SIGNAL(elSetSelected()), this, SLOT(showElSetTools()));
    connect(centrWin->modelTree, SIGNAL(elSetDeselected()), this, SLOT(hideElSetTools()));
    //
    connect(centrWin->modelTree, SIGNAL(matSelected(int)), this, SLOT(showMatTools(int)));
    connect(centrWin->modelTree, SIGNAL(matDeselected()), this, SLOT(hideMatTools()));
    connect(centrWin->modelTree, SIGNAL(feSectionSelected(int)), this, SLOT(showFeSecTools(int)));
    connect(centrWin->modelTree, SIGNAL(feSectionDeselected()), this, SLOT(hideFeSecTools()));
    connect(centrWin->modelTree, SIGNAL(elementsSelected()), this, SLOT(showElementTools()));
    connect(centrWin->modelTree, SIGNAL(elementsDeselected()), this, SLOT(hideElementTools()));
    //*****
    //centrWin.modelTree -> centrWin.modelInfo
    connect(centrWin->modelTree, SIGNAL(matSelected(int)), centrWin->infoBox, SLOT(showMaterialInformation(int)));
    connect(centrWin->modelTree, SIGNAL(feSectionSelected(int)), centrWin->infoBox, SLOT(showSectionInformation(int)));
    connect(centrWin->modelTree, SIGNAL(bcSelected(int)), centrWin->infoBox, SLOT(showBCInformation(int)));

    //centrWin -> modelDatabase
    connect(centrWin, SIGNAL(exportMeshFrd(const QString,long)), modelDatabase, SLOT(exportMeshFrd(const QString,long)));
    connect(centrWin, SIGNAL(writeSetFrd(int,long)), modelDatabase, SLOT(writeSetFrd(int,long)));
    connect(centrWin, SIGNAL(exportMeshAbq(const QString,long)), modelDatabase, SLOT(exportMeshAbq(const QString,long)));
    connect(centrWin,SIGNAL(plotSet(int,int)),modelDatabase, SLOT(setNodeSetPlotFlag(int,int)));
    connect(centrWin, SIGNAL(plotSingleNSet(int,int)),modelDatabase, SLOT(setSingleNodeSetPlotFlag(int,int)));
     connect(centrWin, SIGNAL(plotSingleFaceSet(int,int)),modelDatabase, SLOT(setSingleFaceSetPlotFlag(int,int)));

    connect(centrWin,SIGNAL(unplotSet(int)),modelDatabase, SLOT(unsetNodeSetPlotFlag(int)));
    connect(centrWin, SIGNAL(exportLabelsAbq(int,long)),modelDatabase, SLOT(writeSetAbqLabelsOnly(int,long)));
    connect(centrWin, SIGNAL(exportFoam(const QString)),modelDatabase, SLOT(exportMeshOpenFoam(const QString)));
    connect(centrWin, SIGNAL(moveRadially(double)),modelDatabase, SLOT(moveNodesRadially(double)));
    connect(centrWin, SIGNAL(meshSTL(geomodel*,double,int,int)),modelDatabase, SLOT(createMeshFromStl(geomodel*,double,int,int)));
    connect(centrWin, SIGNAL(deleteMesh()),modelDatabase, SLOT(deleteAllMesh()));
    connect(centrWin, SIGNAL(createMaterial(QString,double,double,double)),modelDatabase, SLOT(createMaterial(QString,double,double,double)));
    connect(centrWin, SIGNAL(createFeSection(QString,QString,QString,int,double)),modelDatabase, SLOT(createFeSection(QString,QString,QString,int,double)));
    connect(centrWin, SIGNAL(autoExtendNodes(QString,double)),modelDatabase, SLOT(autoSelectNodes(QString,double)));
    connect(centrWin, SIGNAL(autoExtendFaces(QString,double)),modelDatabase, SLOT(autoSelectFaces(QString,double)));


    //
    //
    connect(centrWin->modelTree, SIGNAL(deleteBouCon(int)),modelDatabase, SLOT(deleteBC(int)));
    connect(centrWin->modelTree, SIGNAL(deleteMaterial(int)),modelDatabase, SLOT(deleteMaterial(int)));
    connect(centrWin->modelTree, SIGNAL(deleteFeSection(int)),modelDatabase, SLOT(deleteFeSection(int)));
    connect(centrWin->modelTree, SIGNAL(deleteLoads(int)),modelDatabase, SLOT(deleteLoads(int)));
    //
    connect(centrWin, SIGNAL(createPointXYZ(double,double,double,int)), geoDatabase, SLOT(createPoint(double,double,double,int)));
    connect(centrWin, SIGNAL(createPointXYZIndex(long,long,double,double,double)), geoDatabase, SLOT(createPoint(long,long,double,double,double)));
    connect(centrWin, SIGNAL(createSurface(long,QList<long>)), geoDatabase, SLOT(createSurface(long,QList<long>)));
    connect(centrWin->createPoint, SIGNAL(createPointXYZ(double,double,double,int)), geoDatabase, SLOT(createPoint(double,double,double,int)));
    connect(centrWin->createCOS, SIGNAL(createCoord(const QString&,vector3d,vector3d,vector3d,int)), geoDatabase, SLOT(createCos(const QString&,vector3d,vector3d,vector3d,int)));

        //
    connect(centrWin->interpolOptions, SIGNAL(startInterpolation(const QString)), modelDatabase, SLOT(interpolateFrdResults(const QString)));
    connect(centrWin->interpolOptions, SIGNAL(startInterpolationVtkUnstructured(const QString)), modelDatabase, SLOT(interpolateVtkUnstructuredResults(const QString)));
    connect(centrWin->interpolOptions, SIGNAL(startInterpolationVtkMultiblock(const QString)), modelDatabase, SLOT(interpolateVtkMultiblockResults(const QString)));
    connect(centrWin->interpolOptions, SIGNAL(startFaceInterpolation(const QString)), modelDatabase, SLOT(interpolateFrdResultsFace(const QString)));
    connect(centrWin->interpolOptions, SIGNAL(startFaceInterpolationVtkUnstructured(const QString)), modelDatabase, SLOT(interpolateVtkUnstructuredResultsFace(const QString)));
    connect(centrWin->interpolOptions, SIGNAL(startFaceInterpolationVtkMultiblock(const QString,bool)), modelDatabase, SLOT(interpolateVtkMultiblockResultsFace(const QString,bool)));
    //
    connect(centrWin, SIGNAL(createStraightLine(long,long)), geoDatabase, SLOT(createCurve(long,long)));
    connect(centrWin, SIGNAL(createStraightLineLabel(long,long,long)), geoDatabase, SLOT(createCurve(long,long,long)));
    //
    connect(centrWin->createBC, SIGNAL(startBCCreation(QString,QString,int,int,double)), modelDatabase, SLOT(createBC(QString,QString,int,int,double)));
    //
    connect(centrWin->createMat, SIGNAL(startMaterialCreation(QString,double,double,double)), modelDatabase, SLOT(createMaterial(QString,double,double,double)));
    connect(centrWin->matWidget, SIGNAL(startMaterialCreation(QString,double,double,double)), modelDatabase, SLOT(createMaterial(QString,double,double,double)));
    //
    connect(centrWin->createFeSect, SIGNAL(startSectCreation(QString,QString,QString,int,double)), modelDatabase, SLOT(createFeSection(QString,QString,QString,int,double)));
    //
    connect(centrWin->createLoadsView, SIGNAL(startLoadCreation(QString,int,QString,double,int)), modelDatabase, SLOT(createLoad(QString,int,QString,double,int)));
    connect(centrWin->createLoadsView, SIGNAL(startCentrifugalLoadCreation(QString,QString,QString,QString,double)), modelDatabase, SLOT(createCentrifugalLoad(QString,QString,QString,QString,double)));
    //
	connect(centrWin->createCurve, SIGNAL(createStraightLine(long,long)), geoDatabase, SLOT(createCurve(long,long)));
        connect(centrWin->mergeNodes,SIGNAL(mergeNodes(double,int)),modelDatabase,SLOT(mergeNodes(double,int)));
        connect(centrWin->mergeNodes,SIGNAL(mergeNodesSet(double,int,int)),modelDatabase,SLOT(mergeNodesSet(double,int,int)));
        connect(centrWin, SIGNAL(scaleModel(double)),modelDatabase,SLOT(scaleModel(double)));
        connect(centrWin, SIGNAL(newSet(const QString&)),modelDatabase,SLOT(createNewSet(const QString&)));
        //centrWin.viewingArea->modelDatabase
        connect(centrWin->viewingArea,SIGNAL(addNodeToSet(long)),modelDatabase, SLOT(addNodeToSet(long)));
        connect(centrWin->viewingArea,SIGNAL(addNodesToSet(QList<long>)),modelDatabase, SLOT(addNodesToSet(QList<long>)));
        connect(centrWin->viewingArea,SIGNAL(addFacesToSet(QList<long>)),modelDatabase, SLOT(addFacesToSet(QList<long>)));
        connect(centrWin->viewingArea,SIGNAL(addElementsToSet(QList<long>)),modelDatabase, SLOT(addElementsToSet(QList<long>)));
        connect(centrWin->viewingArea,SIGNAL(removeNodeFromSet(long)),modelDatabase, SLOT(removeNodeFromSet(long)));
        connect(centrWin->viewingArea,SIGNAL(removeFaceFromSet(long)),modelDatabase, SLOT(removeFaceFromSet(long)));
        connect(centrWin->viewingArea,SIGNAL(removeElementFromSet(long)),modelDatabase, SLOT(removeElementFromSet(long)));
        connect(centrWin->viewingArea,SIGNAL(addFaceToSet(long)),modelDatabase, SLOT(addFaceToSet(long)));
        connect(centrWin->viewingArea,SIGNAL(addElementToSet(long)),modelDatabase, SLOT(addElementToSet(long)));
        //modelDatabase->this
        connect(modelDatabase,SIGNAL(setCursor(int)), this,SLOT(setCursor(int)));
        connect(modelDatabase,SIGNAL(newResults()), this, SLOT(updateResultSelector()));
        //modelDatabse->resultSelect
        //
        //this->modelDatabase
        connect(this, SIGNAL(interparaFrd(const QString&)), modelDatabase, SLOT(interpolateFrdResults(const QString&)));
        connect(this,SIGNAL(writeVTU(QString)),modelDatabase,SLOT(writeVTK(QString)));
        connect(this,SIGNAL(writeOpenFoam(QString)),modelDatabase,SLOT(exportMeshOpenFoam(QString)));
        connect(this,SIGNAL(writeFRD(const QString&,long)),modelDatabase,SLOT(writeMeshFrd(const QString&,long)));
        connect(this, SIGNAL(createMaterial(QString,double,double,double)),modelDatabase, SLOT(createMaterial(QString,double,double,double)));
        connect(this, SIGNAL(createFeSection(QString,QString,QString,int,double)),modelDatabase, SLOT(createFeSection(QString,QString,QString,int,double)));
        connect(this, SIGNAL(createBC(QString,QString,int,int,double)), modelDatabase, SLOT(createBC(QString,QString,int,int,double)));
        connect(this, SIGNAL(createLoad(QString,int,QString,double,int)), modelDatabase, SLOT(createLoad(QString,int,QString,double,int)));
        connect(this, SIGNAL(createCentrifugalLoad(QString,QString,QString,QString,double)), modelDatabase, SLOT(createCentrifugalLoad(QString,QString,QString,QString,double)));


        //geoDatabse->this
        connect(geoDatabase,SIGNAL(geoDatabaseModified()),centrWin->viewingArea,SLOT(createPointsDisplayList()));
        connect(geoDatabase,SIGNAL(geoDatabaseModified()),centrWin->viewingArea,SLOT(createSTLTriaDisplayList()));
        connect(geoDatabase,SIGNAL(geoDatabaseModified()),centrWin->viewingArea,SLOT(updateView()));
        connect(geoDatabase,SIGNAL(geoDatabaseModified()),centrWin->viewingArea,SLOT(setViewingCenterRadius()));
        //this->geoDatabase
        connect(this, SIGNAL(offsetPlane(long)),geoDatabase, SLOT(createOffsetPlane(long)));
        connect(this,SIGNAL(loadSTL(QString)),geoDatabase,SLOT(readSTL(QString)));
        //
        connect(centrWin,SIGNAL(plotSet(int,int)),centrWin->viewingArea, SLOT(addNsetToPlottingStack(int)));
        connect(centrWin,SIGNAL(plotFaceSet(int,int)),centrWin->viewingArea, SLOT(addFaceSetToPlottingStack(int)));
        connect(centrWin,SIGNAL(plotElSet(int,int)),centrWin->viewingArea, SLOT(addElSetToPlottingStack(int)));
        connect(centrWin, SIGNAL(plotSingleNSet(int,int)),centrWin->viewingArea, SLOT(setSingleNSetToPlottingStack(int)));
        connect(centrWin, SIGNAL(plotSingleFaceSet(int,int)),centrWin->viewingArea, SLOT(setSingleFaceSetToPlottingStack(int)));
        connect(centrWin, SIGNAL(plotSingleElSet(int,int)),centrWin->viewingArea, SLOT(setSingleElSetToPlottingStack(int)));
        connect(centrWin,SIGNAL(unplotSet(int)),centrWin->viewingArea, SLOT(removeNsetFromPlottingStack(int)));
        //
        connect(centrWin->modelTree,SIGNAL(plotNSet(int,int)),centrWin->viewingArea, SLOT(addNsetToPlottingStack(int)));
        connect(centrWin->modelTree,SIGNAL(unplotNSet(int,int)),centrWin->viewingArea, SLOT(removeNsetFromPlottingStack(int)));
        //
        connect(centrWin->modelTree,SIGNAL(plotFaceSet(int,int)),centrWin->viewingArea, SLOT(addFaceSetToPlottingStack(int)));
        connect(centrWin->modelTree,SIGNAL(unplotFaceSet(int,int)),centrWin->viewingArea, SLOT(removeFaceSetFromPlottingStack(int)));
        //
        connect(centrWin->modelTree,SIGNAL(plotElSet(int,int)),centrWin->viewingArea, SLOT(addElSetToPlottingStack(int)));
        connect(centrWin->modelTree,SIGNAL(unplotElSet(int,int)),centrWin->viewingArea, SLOT(removeElSetFromPlottingStack(int)));
        //
        connect(centrWin,SIGNAL(calcExpRatio()),modelDatabase, SLOT(calcExpRatio()));
        connect(centrWin->verifyElementView,SIGNAL(erat2D()),modelDatabase,SLOT(calcExpRatio()));
        //Initializing the geodatabase//
        geoDatabase->initialize();
        if (helpfileSet == 0)
        {
            setHelpFile();
        }

        //Initializing the help engine
         helpEngine = new QHelpEngine(helpfile, this);
         if (helpEngine->setupData())
         {
             emit sendMessage(QString("Help setup succesfully finished (%1)\n").arg(helpfile));

         }
         else
         {
             emit sendMessage("Help file could not be found\n");

         }

}
//*******************************************************************
 /*! Sets the location of the help file*/
 void MainWindow::setHelpFile()
 {
     QString tempFile;
     int fd=QMessageBox::warning(this,"CandyStore Configuration","Please select the help file that should be used!\n You can find the standard help file in the help directory \n in your installation directory");
     tempFile=QFileDialog::getOpenFileName(this, tr("Select the help file!"),"/","qhc files (*.qhc)");
     if (QFile::exists(tempFile))
     {
         helpfileSet=1;
         helpfile=tempFile;
         emit sendMessage(QString("Help file succesfully set (%1)").arg(helpfile));
     }
     else
     {
          emit sendMessage(QString("Help file not set (%1)").arg(helpfile));
     }
 }
 //*******************************************************************
  /*! Sets the location of the help file*/
  void MainWindow::setCcxLocation()
  {
      QString tempFile;
      int fd=QMessageBox::warning(this,"CandyStore Configuration","Please select your CalculiX CCX executable");
      tempFile=QFileDialog::getOpenFileName(this, tr("Select CCX executable!"),"/","exe (*.exe)");
      if (QFile::exists(tempFile))
      {

          ccxLocation=tempFile;
          modelDatabase->ccxLocation=ccxLocation;          emit sendMessage(QString("CCX executable succesfully set (%1)").arg(ccxLocation));
      }
      else
      {
           emit sendMessage(QString("CCX  not set (%1)").arg(ccxLocation));
      }
  }
//*******************************************************************************
 void MainWindow::newFile()
 {
     
 }
 //********************************************************************************
/*! Open the file open dialog with the file filters set for frd files. When an existing file is selected, it is read assuming inp format **/
 void MainWindow::open()
 {
	QString fileName;
    if (lastDirectory=="")
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","frd files (*.frd);;All files (*.*)");
     }
	else
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"frd files (*.frd);;All files (*.*)");
	}
	if (!fileName.isEmpty())
    {
		loadFile(fileName,0);
		lastDirectory=QFileInfo(fileName).absolutePath();
	}
 }
 //********************************************************************************
 /*!Open the file open dialog with the file filters set for inp files. When an existing file is selected, it is read assuming inp format */
void MainWindow::importInp()
 {
 
	QString fileName;
     if (lastDirectory=="")
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","inp files (*.inp);;All files (*.*)");
     }
	else
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"inp files (*.inp);;All files (*.*)");
	 }
	 
	 if (!fileName.isEmpty())
    {
                setCursor(1);
		loadFile(fileName,1);
		lastDirectory=QFileInfo(fileName).absolutePath();
		emit sendMessage(lastDirectory);
            }
     setCursor(0);
     
 }
//********************************************************************************
/*!Open the file open dialog with the file filters set for cdb files. When an existing file is selected, it is read assuming cdb format */
void MainWindow::importCDB()
{

   QString fileName;
    if (lastDirectory=="")
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","cdb files (*.cdb);;All files (*.*)");
    }
   else
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"cdb files (*.cdb);;All files (*.*)");
    }

    if (!fileName.isEmpty())
   {
       setCursor(1);
       loadFile(fileName,9);
       lastDirectory=QFileInfo(fileName).absolutePath();
       emit sendMessage(lastDirectory);
    }
    setCursor(0);

}
 //*********************************************************************************
//********************************************************************************
/*!Open the file open dialog with the file filters set for inp files. When an existing file is selected, it is read assuming inp format */
void MainWindow::importStl()
{
   QString fileName;
    if (lastDirectory=="")
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","STL files (*.stl);;All files (*.*)");
    }
   else
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"STL files (*.stl);;All files (*.*)");
    }

    if (!fileName.isEmpty())
   {
       setCursor(1);
       emit sendMessage(QString("Loading stl geometry from file %1").arg(fileName));
       emit loadSTL(fileName);
       lastDirectory=QFileInfo(fileName).absolutePath();
       emit sendMessage(lastDirectory);
    }
    setCursor(0);
}
//*********************************************************************************
 void MainWindow::save()
 {
     if (curFile.isEmpty())
         saveAs();
     else
         saveFile(curFile);
 }
//********************************************************************
 void MainWindow::saveAs()
 {
	QString fileName =QFileDialog::getSaveFileName(this, tr("Save CandyStore Data"),
                                          QDir::currentPath(),
                                          tr("Candy Store Data Files (*.csd)"));
     if (fileName.isEmpty())
         return;

    saveCSData(fileName);


 }
//********************************************************************
 void MainWindow::openRecentFile()
 {
     QAction *action = qobject_cast<QAction *>(sender());
     if (action)
         loadFile(action->data().toString(),0);
 }
//*******************************************************************************
 void MainWindow::about()
 {
    QMessageBox::about(this, tr("About Candy-Store"),
             tr("<b>Candy-Store </b> is a post processor for Calculix Crunchix "));
 }

//****************************************************************************************
 void MainWindow::loadFile(const QString &fileName,int type)
 {
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr(__appName),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }
     centrWin->modelTree->resetData();
	 QTime t;
	 t.start();
     emit resetData();
     setCursor(1);
	 statusBar()->showMessage(tr("Loading file"), 2000);
	 
	 //Loading the model data
     setCurrentFile(fileName,type);
	 switch (type)
	 {
		case 0:
         readfrd->setFilename(fileName);
         readfrd->setFeData(modelDatabase);
         readfrd->start();
            //modelDatabase->readfrd(fileName);
		break;
		case 1:
			modelDatabase->readinp(fileName);
            emit newFileLoaded();
            restoreCursor();
		break;
        case 9:
            modelDatabase->readcdb(fileName);
            emit newFileLoaded();
            restoreCursor();
        break;
     }
	 
	 double time =t.elapsed()/1000.;
	 QString timestring=QString::number(time);
     //modelDatabase->nodeOctree->initialise();
	 emit sendMessage(QString(tr("File loaded in %0 seconds").arg(timestring)));

	 
 }
//*****************************************************************************
 void MainWindow::saveFile(const QString &fileName)
 {
  /*   QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr(__appName),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << textEdit->toPlainText();
     QApplication::restoreOverrideCursor();

     setCurrentFile(fileName);
     statusBar()->showMessage(tr("File saved"), 2000);
  */
 }
//**********************************************************************
 void MainWindow::setCurrentFile(const QString &fileName, int type)
 {
     curFile = fileName;
     if (curFile.isEmpty())
         setWindowTitle(tr(__appName));
     else
         setWindowTitle(tr("%1 - %2").arg(strippedName(curFile))
                                     .arg(tr(__appName)));

     if (type==0)
	 {
	 QSettings settings(__appName, __appName);
     QStringList files = settings.value("recentFileList").toStringList();
     files.removeAll(fileName);
     files.prepend(fileName);
     while (files.size() > MaxRecentFiles)
         files.removeLast();

     settings.setValue("recentFileList", files);

     foreach (QWidget *widget, QApplication::topLevelWidgets()) 
	 {
         MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
         if (mainWin)
             mainWin->updateRecentFileActions();
     }
	 }
 }
//*******************************************************************
 void MainWindow::updateRecentFileActions()
 {
     QSettings settings(__appName, __appName);
     QStringList files = settings.value("recentFileList").toStringList();

     int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

     for (int i = 0; i < numRecentFiles; ++i) {
         QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
         recentFileActs[i]->setText(text);
         recentFileActs[i]->setData(files[i]);
         recentFileActs[i]->setVisible(true);
     }
     for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
         recentFileActs[j]->setVisible(false);

     separatorAct->setVisible(numRecentFiles > 0);
 }
//**************************************************************************
 QString MainWindow::strippedName(const QString &fullFileName)
 {
     return QFileInfo(fullFileName).fileName();
 } 
 //************************************************************************************
 /*!Reads the application settings */
void MainWindow::readSettings()
    {
        QSettings settings(__appName, __appName);
        QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
        QSize size = settings.value("size", QSize(400, 400)).toSize();
        lastDirectory = settings.value("lastDir", QString("")).toString();
		lastScreenshotDirectory = settings.value("lastScreenshotDir", QString("")).toString();
		resize(size);
        move(pos);
        setFeatureEdgeAngle(settings.value("featureEdgeAngle",QString("45.0")).toDouble());
        helpfileSet=settings.value("helpfileSet",QString("0")).toInt();
        helpfile=settings.value("helpfile",QString("")).toString();
        ccxLocation=settings.value("ccxLocation",QString("")).toString();
        modelDatabase->ccxLocation=ccxLocation;
		
	}
//********************************************************************************
/*! Writes the current application settings*/
void MainWindow::writeSettings()
    {
        QSettings settings(__appName, __appName);
        settings.setValue("pos", pos());
        settings.setValue("size", size());
		settings.setValue("lastDir", lastDirectory);
		settings.setValue("lastScreenshotDir",lastScreenshotDirectory);
        settings.setValue("featureEdgeAngle", getFeatureEdgeAngle());
        settings.setValue("helpfileSet",helpfileSet);
        settings.setValue("helpfile",helpfile);
        settings.setValue("ccxLocation",ccxLocation);
	}
//**********************************************************************************
void MainWindow::closeEvent(QCloseEvent *event)
    {
            writeSettings();
            event->accept();
    }
//****************************************************************************
void MainWindow::setBgColor()
	{
	QColor bgColor;
	bgColor = QColorDialog::getColor();
	if (bgColor.isValid())
		{
			centrWin->setBgColor(bgColor);
		}
	
	}
//************************************************************************************************
/*!Picks up the internal toggle nodes signal and sends a external toggle nodes signal
*/
void MainWindow::toggleNodes()
	{
		emit toggleNodesSignal();
	}
//************************************************************************************************
/*!Picks up the internal toggle nodes signal and sends a external toggle nodes signal
*/
void MainWindow::togglePoints()
    {
        emit togglePointsSignal();
    }
//************************************************************************************************
/**Loads a new result step into memory 
*/

void MainWindow::loadNewResults(int step)
{
        switch (modelDatabase->fileflag)
        {
        case 1:
            modelDatabase->readfrdresults(step);
        break;
        case 2:
             modelDatabase->readVtkResults(step);
        break;
        case 3:
            modelDatabase->readVtkResults(step);
        break;
    }
        updateResultSelector();
	centrWin->viewingArea->setInitScaleFactor();
	centrWin->viewingArea->updateGL();
}
//************************************************************************************************
/*!Opens the help dialog box
*/
void MainWindow::help()
{
    QWidget *helpWidget = new QWidget();
    QPushButton *homeButton = new QPushButton(tr("&Home"));
    QPushButton *backButton = new QPushButton(tr("&Back"));
    QPushButton *closeButton = new QPushButton(tr("Close"));
    //QVBoxLayout *vlayout = new QVBoxLayout();
    //QWebView *whBrowser = new QWebView();
       QSplitter *helpPanel = new QSplitter(Qt::Horizontal);
      HelpBrowser *helpBrowser = new HelpBrowser(helpEngine);
      helpBrowser->setSource(QUrl::fromEncoded("qthelp://net.sourceforge.candystore/candystore-20101207/index.html"));
      connect(helpEngine->contentWidget(), SIGNAL(linkActivated(const QUrl &)),
              helpBrowser, SLOT(setSource(const QUrl &)));
    //vlayout->addWidget(helpEngine->searchEngine()->queryWidget());
    //vlayout->addWidget(helpEngine->searchEngine()->resultWidget());
    //searchWidget->setLayout(vlayout);
      QHBoxLayout *buttonLayout = new QHBoxLayout;
      buttonLayout->addWidget(homeButton);
      buttonLayout->addWidget(backButton);
      buttonLayout->addStretch();
      buttonLayout->addWidget(closeButton);

      QVBoxLayout *mainLayout = new QVBoxLayout;
      mainLayout->addLayout(buttonLayout);
      mainLayout->addWidget(helpBrowser);
      helpWidget->setLayout(mainLayout);

      helpPanel->insertWidget(0, helpEngine->contentWidget());
      helpPanel->insertWidget(1, helpWidget);

      //helpPanel->insertWidget(2,whBrowser);
      //helpPanel->insertWidget(2,searchWidget);
      helpPanel->setStretchFactor(1,1);
      helpPanel->setWindowTitle("Help");

      connect(backButton, SIGNAL(clicked()),
              helpBrowser, SLOT(backward()));
       connect(closeButton, SIGNAL(clicked()), helpPanel, SLOT(close()));
       connect(homeButton, SIGNAL(clicked()), helpBrowser, SLOT(home()));

    helpPanel->show();
    helpPanel->raise();

}
//************************************************************************************************
/*!Opens the screenshot dialog box
*/
void MainWindow::takeScreenshot()
{

	QString format="png";
	QString fileName;
	//="d:/screenshot.png";	
	//QPixmap screen;
	centrWin->releaseKeys();
	if (lastScreenshotDirectory=="")
	{
		fileName =QFileDialog::getSaveFileName(this, tr("Save Screenshot"),
                                          "./",
                                          tr("png Files (*.png)"));
    }
	else
	{
		fileName =QFileDialog::getSaveFileName(this, tr("Save Screenshot"),
                                          lastScreenshotDirectory,
                                          tr("png Files (*.png)"));
    }
	 
	 if (fileName.isEmpty())
         return;

	//screen = QPixmap::grabWidget(centrWin->viewingArea);
	//screen.save(fileName, format.toAscii());
	QImage screenshot=centrWin->viewingArea->grabFrameBuffer();
	screenshot.save(fileName, format.toAscii());
	lastScreenshotDirectory=QFileInfo(fileName).absolutePath();
	centrWin->grabKeys();
}
//******************************************************************************
/*!Returns a string with the mouse help text
*/
QString MainWindow::mouseString() const
{
  QString text("<table border=\"1\" cellspacing=\"0\">\n");
  const QString trtd("<tr><td>");
  const QString tdtr("</td></tr>\n");
  const QString tdtd("</td><td>");

  text += "<tr bgcolor=\"#aaaacc\"><th align=\"center\">Button(s)</th><th align=\"center\">Description</th></tr>\n";
  text += trtd + " Left Mouse Button " + tdtd + " Rotate Model "  + tdtr;
  text += trtd + " Right Mouse Button " + tdtd + " Translate Model "  + tdtr;
  text += trtd + " Middle Mouse Button " + tdtd + " Zoom Model "  + tdtr;
  text += trtd + " Mouse Wheel " + tdtd + " Zoom Model "  + tdtr;
  text += trtd + " Shift + Left Mouse Button " + tdtd + " Select Node and Display Properties "  + tdtr;
  text += trtd + " Double Right Click " + tdtd + " Center Model "  + tdtr;
  
  
  
  
  return text;
}

//******************************************************************************
/**Returns a string wthe the command help text
*/
QString MainWindow::commandString() const
{
  QString text("<table border=\"1\" cellspacing=\"0\">\n");
  const QString trtd("<tr><td>");
  const QString tdtr("</td></tr>\n");
  const QString tdtd("</td><td>");

  text += "<tr bgcolor=\"#aaaacc\"><th align=\"center\"> Command(s) </th><th align=\"center\"> Description </th></tr>\n";
  text += trtd + " line &lt;sartpoint&gt; &lt;endpoint&gt; " + tdtd + " Creates a straight line between startpoint and endpoint"  + tdtr;
  text += trtd + " max &lt;value&gt; " + tdtd + " Sets the maximum fringe value to the specified value; . If &lt;value&gt; is omitted the max value will be set to the automatically calculated value."  + tdtr;
  text += trtd + " min &lt;value&gt; " + tdtd + " Sets the minimum fringe value to the specified value; . If &lt;value&gt; is omitted the min value will be set to the automatically calculated value."  + tdtr;
  text += trtd + " pnt &lt;x&gt; &lt;y&gt; &lt;z&gt; " + tdtd + " Creates a point at coordinates x y z"  + tdtr;
  text += trtd + " scal d &lt;value&gt; " + tdtd + " Sets the displacement scale factor to &lt;value&gt; "  + tdtr;
  text += trtd + " udef " + tdtd + " Toggle the undeformed mesh on/off "  + tdtr;
  
  
  
  return text;
}
//******************************************************************************
/**Returns a string wthe the keyboard help text
*/
QString MainWindow::keyboardString() const
{
  QString text("<table border=\"1\" cellspacing=\"0\">\n");
  const QString trtd("<tr><td>");
  const QString tdtr("</td></tr>\n");
  const QString tdtd("</td><td>");

  text += "<tr bgcolor=\"#aaaacc\"><th align=\"center\"> Command(s) </th><th align=\"center\"> Description </th></tr>\n";
  text += trtd + " STRG + O " + tdtd + " Open a frd-file"  + tdtr;
  text += trtd + " STRG + Q " + tdtd + " Quit Application"  + tdtr;
  
  
  
  return text;
}

//****************************************************************
void MainWindow::writeOutFaces()
{
	QString fileName;
    if (lastDirectory=="")
	 {
		fileName = QFileDialog::getSaveFileName(this, tr("Save Outside Faces Centroids"),
                                          "./",
                                          "frd Files (*.frd);;All files (*.*)");
     }
	else
	 {
		fileName = QFileDialog::getSaveFileName(this, tr("Save Outside Faces Centroids"),lastDirectory,"frd files (*.frd);;All files (*.*)");
	}
	if (!fileName.isEmpty())
    {
		emit writeOFaces(fileName);
	}

}
//**********************************************************************
void MainWindow::interpolateResults()
{
	QString fileName;
    if (lastDirectory=="")
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Interpolate results from an frd file"),
                                          "./",
                                          "frd Files (*.frd);;All files (*.*)");
     }
	else
	 {
		fileName = QFileDialog::getOpenFileName(this, tr("Interpolate resuts from an frd file"),
												lastDirectory,
												"frd files (*.frd);;All files (*.*)");
	}
	if (!fileName.isEmpty())
    {
		emit interparaFrd(fileName);
		//emit sendMessage("Interpolation not yet implemented!");
	}
}
//*********************************************************************************
void MainWindow::changeHex8()
{

        modelDatabase->changeHex8toHex20();

}
//*********************************************************************************
void MainWindow::test()
{
	emit sendMessage(QString("test-slot-mw"));
	planeToolBar->hide();
}

//************************************************************************************
void MainWindow::showPlaneTools()
{
	emit sendMessage(QString("showPlaneTools"));
	planeToolBar->show();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();
}
//************************************************************************************
void MainWindow::showNodeTools()
{
        //emit sendMessage(QString("showGeoTools"));
        nodeToolBar->show();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//*************************************************************************************
void MainWindow::hideNodeTools()
{
        emit sendMessage(QString("hideNodeTools"));
        nodeToolBar->hide();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

    }
//************************************************************************************
void MainWindow::showGeoTools()
{
	emit sendMessage(QString("showGeoTools"));
	geometryToolBar->show();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

    }
//*************************************************************************************
void MainWindow::hideGeoTools()
{
        emit sendMessage(QString("hideGeoTools"));
        geometryToolBar->hide();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

    }
//************************************************************************************
void MainWindow::hidePlaneTools()
{
	emit sendMessage(QString("hidePlaneTools"));
	planeToolBar->hide();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

    }
//************************************************************************************
void MainWindow::createOffsetPlaneClicked()
{
	long baseplane=1;
	emit offsetPlane(baseplane);
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//************************************************************************************
void MainWindow::readCsd(const QString &fileName)
{
	emit sendMessage(QString("Reading CSD data"));
}
//**************************************************************************************
void MainWindow::abqInputDeckClicked()
{
    centrWin->releaseKeys();
        QString inpFile =QFileDialog::getSaveFileName(this, tr("Create Input File"),
                                          QDir::currentPath(),
                                          tr("ABAQUS/CCX Input File (*.inp)"));
     if (inpFile.isEmpty())
         return;

	emit sendMessage("Generating ABAQUS/CCX Input Deck"); 
        setCursor(1);
        modelDatabase->exportDummyAbqInputData(inpFile);
        emit sendMessage(QString("Input file written to %1").arg(inpFile));
        centrWin->grabKeys();
        setCursor(0);
    }
//**************************************************************************************
void MainWindow::createPointXYZclicked()
{
	emit sendMessage("Create a new point"); 
    geoDatabase->createPoint(1.0,0.0,0.0,0);
}

//***********************************************************************************************
/*!Import data from an Vtk grid data file */
void MainWindow::importVtk()
{
        centrWin->releaseKeys();
        QString fileName;
     if (lastDirectory=="")
         {
                fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","vtu files (*.vtu);;vtm files (*.vtm);;vtmb files (*.vtmb);;All files (*.*)");
     }
        else
         {
                fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"vtu files (*.vtu);;vtm files (*.vtm);;vtmb files (*.vtmb);;All files (*.*)");
         }
        QTime t;
        t.start();
        QApplication::setOverrideCursor(Qt::WaitCursor);
        statusBar()->showMessage(tr("Importing file"), 2000);

         if (!fileName.isEmpty())
        {
                if (fileName.endsWith(".vtu"))
                {
                    modelDatabase->importVtkUnstructured(fileName);
                    lastDirectory=QFileInfo(fileName).absolutePath();

                    double time =t.elapsed()/1000.;
                    emit newFileLoaded();
                    emit sendMessage(QString(tr("File imported in %0 seconds").arg(time)));
                }
                else
                {
                    if (fileName.endsWith(".vtmb") || fileName.endsWith(".vtm") )
                    {
                        modelDatabase->importVtkMultiblock(fileName);
                        lastDirectory=QFileInfo(fileName).absolutePath();
                        double time =t.elapsed()/1000.;
                        emit newFileLoaded();
                        emit sendMessage(QString(tr("File imported in %0 seconds").arg(time)));

                    }
                    else
                    {
                        lastDirectory=QFileInfo(fileName).absolutePath();
                        emit sendMessage(QString(tr("Unknown Filetype! Operation aborted")));
                    }
                }

        }
  QApplication::restoreOverrideCursor();
}
//**************************************************************
/*!Update the result selector*/
void MainWindow::updateResultSelector()
{
   // emit sendMessage(QString("Results Update %1").arg(modelDatabase->resultDataList.size()));
    resultSelect->typeSelector->clear();
    resultSelect->typeSelector->addItem("none");
    for (int i=0;i< modelDatabase->resultDataList.size();i++)
    {
            resultSelect->typeSelector->addItem(modelDatabase->resultDataList[i].resultLabel);
    }
}
//******************************************************************
/*! Set the cursor appearance*/
void MainWindow::setCursor(int mode=0)
{
    switch (mode)
      {
      case 0 :
            QApplication::restoreOverrideCursor();
      break;
      case 1:
             QApplication::setOverrideCursor(Qt::WaitCursor);
      break;
      case 2 :
            QApplication::setOverrideCursor(Qt::BusyCursor);
      break;
      }
}
//*********************************************************
/*! Set the cursor to busy appearance*/
void MainWindow::setBusyCursor()
{
    QApplication::setOverrideCursor(Qt::BusyCursor);
}
//*********************************************************
/*! Set the cursor to normal (arrow) appearance*/
void MainWindow::restoreCursor()
{
    QApplication::restoreOverrideCursor();
}

//********************************************************
void MainWindow::showCosTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        cosToolBar->show();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideCosTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        cosToolBar->hide();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//************************************************************
double MainWindow::getFeatureEdgeAngle()
{
    return(centrWin->viewingArea->featureEdgeAngle);
}
//*************************************************************
//************************************************************
void MainWindow::setFeatureEdgeAngle(double angle)
{
    centrWin->viewingArea->featureEdgeAngle=angle;
}
//*************************************************************
//************************************************************
void MainWindow::exportVTKclicked()
{
    QString fileName;
    if (lastDirectory=="")
     {
        fileName = QFileDialog::getSaveFileName(this, tr("Save VTK mesh data "),
                                          "./",
                                          "vtu files (*.vtu)");
     }
    else
     {
        fileName = QFileDialog::getSaveFileName(this, tr("Save VTK mesh data"),lastDirectory,"vtu files (*.vtu)");
    }
    if (!fileName.isEmpty())
    {
        emit writeVTU(fileName);
    }
}

//************************************************************
void MainWindow::exportOpenFoamClicked()
{
    QString fileName;
    if (lastDirectory=="")
     {
        fileName = QFileDialog::getExistingDirectory(this, tr("Save OpenFOAM mesh data "),
                                          "./");
     }
    else
     {
        fileName = QFileDialog::getExistingDirectory(this, tr("Save OpenFOAM mesh data"),lastDirectory);
    }
    if (!fileName.isEmpty())
    {
        emit writeOpenFoam(fileName);
    }
}
//*************************************************************
//************************************************************
void MainWindow::exportFrdClicked()
{
    QString fileName;
    if (lastDirectory=="")
     {
        fileName = QFileDialog::getSaveFileName(this, tr("Save frd mesh data "),
                                          "./",
                                          "frd files (*.frd)");
     }
    else
     {
        fileName = QFileDialog::getSaveFileName(this, tr("Save frd mesh data"),lastDirectory,"frd files (*.frd)");
    }
    if (!fileName.isEmpty())
    {
        emit writeFRD(fileName,0);
    }
}
//********************************************************
void MainWindow::showFeDataTools()
{
        emit sendMessage(QString("showFETools"));
        feDataToolBar->show();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideFeDataTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        feDataToolBar->hide();
        centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::showBCTools(int k)
{
        //emit sendMessage(QString("showFETools"));
        bcToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideBCTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        bcToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
//********************************************************
void MainWindow::showNodeSetTools(int k)
{
        //emit sendMessage(QString("showFETools"));
        nSetToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideNodeSetTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        nSetToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::showFaceSetTools()
{
        //emit sendMessage(QString("showFETools"));
        faceSetToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideFaceSetTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        faceSetToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
//********************************************************
void MainWindow::showElSetTools()
{
       elSetToolBar->show();
}
//********************************************************
void MainWindow::hideElSetTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        elSetToolBar->hide();
}
//********************************************************
void MainWindow::showMatTools(int k)
{
        //emit sendMessage(QString("showFETools"));
        materialToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideMatTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        materialToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::showFeSecTools(int k)
{
        //emit sendMessage(QString("showFETools"));
        feSectionToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideFeSecTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        feSectionToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::showElementTools()
{
        //emit sendMessage(QString("showFETools"));
        elementToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideElementTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        elementToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::showLoadTools()
{
        //emit sendMessage(QString("showFETools"));
        loadToolBar->show();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::hideLoadTools()
{
        //emit sendMessage(QString("hidePlaneTools"));
        loadToolBar->hide();
        //centrWin->viewingArea->resizeGL(centrWin->viewingArea->width(),centrWin->viewingArea->height());
        //centrWin->viewingArea->updateGL();

}
//********************************************************
void MainWindow::saveCSData(const QString &fileName)
{
    QString nodes="";
    QString faces="";
    QString elements="";
    int counter=0;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("CandyStore Data"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    //Write the meshfile
    QString meshFileName=fileName + ".frx";
    emit writeFRD(meshFileName,0);
    //

    QXmlStreamWriter* writer =new QXmlStreamWriter(&file);
    writer->setAutoFormatting(TRUE);
    writer->writeStartDocument();
    writer->writeDTD("<!DOCTYPE csc>");
    writer->writeStartElement("csdata");
    writer->writeTextElement("meshfile",meshFileName);
    writer->writeStartElement("meshdata");
    //Don't write the outside set, as it will be generated automatically
    for (int r=1;r<modelDatabase->setList.size();r++)
    {
        writer->writeStartElement("set");
        writer->writeTextElement("name",QString(modelDatabase->setList[r].name));
        writer->writeTextElement("numnodes",QString::number(modelDatabase->setList[r].nodeList.size()));
        for (long n=0;n<modelDatabase->setList[r].nodeList.size();n++)
        {
            nodes += QString::number(modelDatabase->setList[r].nodeList[n]) +",";
            counter++;
            if (counter==10)
            {
                writer->writeTextElement("nodes",nodes);
                nodes="";
                counter=0;
            }
        }
        writer->writeTextElement("nodes",nodes);
        //***********************
        //Write the faces of the set
        writer->writeTextElement("numfaces",QString::number(modelDatabase->setList[r].faceIndexList.size()));
        for (long n=0;n<modelDatabase->setList[r].faceIndexList.size();n++)
        {
            faces += QString::number(modelDatabase->setList[r].faceIndexList[n]) +",";
            counter++;
            if (counter==10)
            {
                writer->writeTextElement("faces",faces);
                faces="";
                counter=0;
            }
        }
        writer->writeTextElement("faces",faces);
        //***********************
        //Write the elements of the set
        writer->writeTextElement("numelements",QString::number(modelDatabase->setList[r].faceIndexList.size()));
        for (long n=0;n<modelDatabase->setList[r].elementList.size();n++)
        {
            elements += QString::number(modelDatabase->setList[r].elementList[n]) +",";
            counter++;
            if (counter==10)
            {
                writer->writeTextElement("elements",elements);
                elements="";
                counter=0;
            }
        }
        writer->writeTextElement("elements",elements);
        //***************************
        writer->writeEndElement();
    }
    writer->writeEndElement();
    //writer->writeTextElement("location",curFile);
    //writer->writeEndElement();
    /*writer->writeStartElement("resulttable");
    for (int r=0;r<modelDatabase->resultList.size();r++)
       {
           writer->writeStartElement("resultindex");
           writer->writeTextElement("resultstep",QString::number(modelDatabase->resultList[r].resultStep));
           writer->writeTextElement("resulttype",QString::number(modelDatabase->resultList[r].resultType));
           writer->writeTextElement("frqtime",QString::number(modelDatabase->resultList[r].frqtime));
           writer->writeTextElement("resultpos",QString::number(modelDatabase->resultList[r].resultPosition));
           writer->writeEndElement();
       }
   writer->writeEndElement();*/
   //
   writer->writeStartElement("fedata");
   for (int r=0;r<modelDatabase->materialList.size();r++)
      {
          writer->writeStartElement("material");
          writer->writeTextElement("name",QString(modelDatabase->materialList[r].name));
          writer->writeTextElement("ymodulus",QString::number(modelDatabase->materialList[r].elasticDataList[0].yModulus));
          writer->writeTextElement("poisson",QString::number(modelDatabase->materialList[r].elasticDataList[0].poison));
          writer->writeTextElement("density",QString::number(modelDatabase->materialList[r].densityList[0].value));
          writer->writeEndElement();
       }
   for (int r=0;r<modelDatabase->feSectionList.size();r++)
      {
          writer->writeStartElement("fesection");
          writer->writeTextElement("name",QString(modelDatabase->feSectionList[r].name));
          writer->writeTextElement("set",QString(modelDatabase->feSectionList[r].set));
          writer->writeTextElement("material",QString(modelDatabase->feSectionList[r].material));
          writer->writeEndElement();
       }

   for (int r=0;r<modelDatabase->bcList.size();r++)
      {
          writer->writeStartElement("bc");
          writer->writeTextElement("name",QString(modelDatabase->bcList[r].name));
          writer->writeTextElement("set",QString(modelDatabase->bcList[r].nodeset));
          writer->writeTextElement("fdof",QString::number(modelDatabase->bcList[r].firstDof));
          writer->writeTextElement("ldof",QString::number(modelDatabase->bcList[r].lastDof));
          writer->writeTextElement("value",QString::number(modelDatabase->bcList[r].value));
          writer->writeEndElement();
       }
   for (int r=0;r<modelDatabase->loadList.size();r++)
      {
          writer->writeStartElement("load");
          writer->writeTextElement("name",QString(modelDatabase->loadList[r].name));
          writer->writeTextElement("set",QString(modelDatabase->loadList[r].targetSet));
          writer->writeTextElement("type",QString::number(modelDatabase->loadList[r].type));
          writer->writeTextElement("direction",QString::number(modelDatabase->loadList[r].direction));
          writer->writeTextElement("point1",QString(modelDatabase->loadList[r].getPoint1String()));
          writer->writeTextElement("point2",QString(modelDatabase->loadList[r].getPoint2String()));
          writer->writeTextElement("value",QString::number(modelDatabase->loadList[r].value));

          writer->writeEndElement();
       }

   writer->writeEndElement();
   //
   writer->writeStartElement("geometry");
    for (int r=0;r<geoDatabase->planeList.size();r++)
       {
           writer->writeStartElement("plane");
           writer->writeTextElement("label",QString::number(1));
           writer->writeTextElement("origin",QString::number(2));
           writer->writeTextElement("normal",QString::number(3));
           writer->writeTextElement("visible",QString::number(4));
           writer->writeEndElement();
       }
    writer->writeEndDocument();
}
//****************************************************
//********************************************************
/*! Reading CS Data*/
void MainWindow::readCSData(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("CandyStore Data"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QXmlStreamReader reader;// =new QXmlStreamReader(&file);
    reader.setDevice(&file);
    //reader.setNamespaceProcessing(false);
    int nValues;
    QString direction="";
    QString type="";
    QString meshfile="";
    QString name="";
    QString yMod="";
    QString poisson="";
    QString density="";
    QString set="";
    QString material="";
    QString fdof="";
    QString ldof="";
    QString value="";
    QList<long> setNodes;
    QList<long> setFaces;
    QList<long> setElements;
    setNodes.clear();
    QString point1="";
    QString point2="";

    while (reader.readNextStartElement()) {
        //QString n=(QString(reader.name()));
        emit sendMessage("n");
             if (reader.name() == "csdata")
        {
                 emit sendMessage("csdata");
                while (reader.readNextStartElement())
                 {
                      emit sendMessage(QString("Name: %") + reader.name().toString());
                    //******************
                    //Getting the location of the meshfile
                      if (reader.name() == "meshfile")
                        {

                            meshfile=reader.readElementText();
                            emit sendMessage("Meshfile found: " + meshfile) ;
                            loadFile(meshfile,0);
                            lastDirectory=QFileInfo(meshfile).absolutePath();

                        }
                      else if (reader.name() == "meshdata")
                        {
                          while (reader.readNextStartElement())
                           {
                             emit sendMessage(QString("Name: %") + reader.name().toString());
                             if (reader.name() == "set")
                               {
                                  while (reader.readNextStartElement())
                                 {
                                      if (reader.name() == "name")
                                      {
                                          name=reader.readElementText();

                                      }
                                      else if (reader.name() == "numnodes")
                                      {
                                          yMod=reader.readElementText();

                                      }
                                      else if (reader.name() == "nodes")
                                      {
                                          poisson=reader.readElementText();
                                          nValues=poisson.count(",");
                                          for (int k=0;k<nValues;k++)
                                          {
                                              setNodes.append(poisson.section(",",k,k).toLong());
                                              emit sendMessage(QString("%1").arg(setNodes[k]));
                                          }
                                      }
                                      else if (reader.name() == "faces")
                                      {
                                          poisson=reader.readElementText();
                                          nValues=poisson.count(",");
                                          for (int k=0;k<nValues;k++)
                                          {
                                              setFaces.append(poisson.section(",",k,k).toLong());
                                              emit sendMessage(QString("%1").arg(setFaces[k]));
                                          }
                                      }
                                      else if (reader.name() == "elements")
                                      {
                                          poisson=reader.readElementText();
                                          nValues=poisson.count(",");
                                          for (int k=0;k<nValues;k++)
                                          {
                                              setElements.append(poisson.section(",",k,k).toLong());
                                              emit sendMessage(QString("%1").arg(setElements[k]));
                                          }
                                      }
                                      else
                                      {
                                          reader.skipCurrentElement();
                                      }
                                 }

                                 //************************
                                 //Put the set in the database
                                 modelDatabase->tempSet.name=name;
                                 modelDatabase->tempSet.nodeList=setNodes;
                                 modelDatabase->tempSet.faceIndexList=setFaces;
                                 modelDatabase->tempSet.elementList=setElements;
                                 modelDatabase->setList.append(modelDatabase->tempSet);

                                 //*************************
                                 setNodes.clear();
                               }

                             else
                             {
                                 reader.skipCurrentElement();
                              }
                            }
                        }

                      //******************************
                    else if (reader.name() == "fedata")
                      {
                        while (reader.readNextStartElement())
                         {
                           emit sendMessage(QString("Name: %") + reader.name().toString());
                           if (reader.name() == "material")
                             {
                                while (reader.readNextStartElement())
                               {
                                    if (reader.name() == "name")
                                    {
                                        name=reader.readElementText();
                                        emit sendMessage("Material found: " + name) ;
                                    }
                                    else if (reader.name() == "ymodulus")
                                    {
                                        yMod=reader.readElementText();
                                        emit sendMessage("yModulus found: " + yMod) ;
                                    }
                                    else if (reader.name() == "poisson")
                                    {
                                        poisson=reader.readElementText();
                                        emit sendMessage("Poisson found: " + poisson) ;
                                    }
                                    else if (reader.name() == "density")
                                    {
                                        density=reader.readElementText();
                                        emit sendMessage("Density found: " + density) ;
                                    }
                                    else
                                    {
                                        reader.skipCurrentElement();
                                    }
                               }
                               //************************
                               //Put the material in the database
                               evaluateXMLMaterialData(name,yMod,poisson,density);
                               //*************************
                             }
                           else  if (reader.name() == "fesection")
                           {
                              while (reader.readNextStartElement())
                             {
                                  if (reader.name() == "name")
                                  {
                                      name=reader.readElementText();
                                      emit sendMessage("FE Section found: " + name) ;
                                  }
                                  else if (reader.name() == "set")
                                  {
                                      set=reader.readElementText();
                                      emit sendMessage("Set found: " + set) ;
                                  }
                                  else if (reader.name() == "material")
                                  {
                                      material=reader.readElementText();
                                      emit sendMessage("Material found: " + material) ;
                                  }
                                  else
                                  {
                                      reader.skipCurrentElement();
                                  }
                              }
                              evaluateXMLFeSectionData(name,set,material);
                           }
                           else  if (reader.name() == "bc")
                           {
                              while (reader.readNextStartElement())
                             {
                                  if (reader.name() == "name")
                                  {
                                      name=reader.readElementText();
                                      emit sendMessage("BC found: " + name) ;
                                  }
                                  else if (reader.name() == "set")
                                  {
                                      set=reader.readElementText();
                                      emit sendMessage("Set found: " + set) ;
                                  }
                                  else if (reader.name() == "fdof")
                                  {
                                      fdof=reader.readElementText();
                                  }
                                  else if (reader.name() == "ldof")
                                  {
                                      ldof=reader.readElementText();
                                  }
                                  else if (reader.name() == "value")
                                  {
                                      value=reader.readElementText();
                                  }
                                  else
                                  {
                                      reader.skipCurrentElement();
                                  }
                              }
                              evaluateXMLBCData(name,set,fdof,ldof,value);
                           }
                           //***************
                           else  if (reader.name() == "load")
                           {
                              while (reader.readNextStartElement())
                             {
                                  if (reader.name() == "name")
                                  {
                                      name=reader.readElementText();
                                      emit sendMessage("Load found: " + name) ;
                                  }
                                  else if (reader.name() == "set")
                                  {
                                      set=reader.readElementText();
                                      //emit sendMessage("Set found: " + set) ;
                                  }
                                  else if (reader.name() == "type")
                                  {
                                      type=reader.readElementText();
                                  }
                                  else if (reader.name() == "direction")
                                  {
                                      direction=reader.readElementText();
                                  }
                                  else if (reader.name() == "value")
                                  {
                                      value=reader.readElementText();
                                  }
                                  else if (reader.name() == "point1")
                                  {
                                      point1=reader.readElementText();
                                  }
                                  else if (reader.name() == "point2")
                                  {
                                      point2=reader.readElementText();
                                  }
                                  else
                                  {
                                      reader.skipCurrentElement();
                                  }
                              }
                              evaluateXMLLoadData(name,set,type,direction,value,point1,point2);
                           }
                           else
                           {
                               reader.skipCurrentElement();
                            }
                       }
                      }
                    else
                    {
                        reader.skipCurrentElement();
                    }

                 }
        }
        else
            reader.skipCurrentElement();
    }

}
//********************************************************************************
/*! Open the file open dialog with the file filters set for csd files.*/
void MainWindow::readCSD()
{
   QString fileName;
    if (lastDirectory=="")
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","CSD files (*.csd);;All files (*.*)");
    }
   else
    {
       fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"CSD files (*.csd);;All files (*.*)");
    }

    if (!fileName.isEmpty())
   {
       setCursor(1);
       emit sendMessage(QString("Loading CandyStore Data from file %1").arg(fileName));
       //emit loadCSD(fileName);
       readCSData(fileName);
       lastDirectory=QFileInfo(fileName).absolutePath();
       emit sendMessage(lastDirectory);
    }
    setCursor(0);
}
//********************************************************************************
/*! Check the material dat of the xml file and cretae it in the database.*/
void MainWindow::evaluateXMLMaterialData(QString name,QString ym,QString poisson, QString density)
{
    emit createMaterial(name,ym.toDouble(),poisson.toDouble(),density.toDouble());

}
//**********************************************************************************
void MainWindow::evaluateXMLFeSectionData(QString name,QString set,QString material)
{
    emit createFeSection(name,set,material,1,1);

}
//**********************************************************************************
void MainWindow::evaluateXMLBCData(QString name,QString set,QString fdof,QString ldof, QString value)
{
    emit createBC(name,set,fdof.toInt(),ldof.toInt(),value.toDouble());

}
//**********************************************************************************
void MainWindow::evaluateXMLLoadData(QString name,QString set,QString type,QString direction, QString value,QString p1, QString p2)
{
    if (type.toInt()<2)
    {
        emit createLoad(name,type.toInt(),set,value.toDouble(),direction.toInt());
    }
    else
    {
        emit sendMessage("Points :" + p1 + ";" + p2);
        emit createCentrifugalLoad(name,set,p1,p2,value.toDouble());
    }
}
//***********************************************************************
/*! Open an fbd file an run the commands stored in it*/
void MainWindow::runFbd()
{
    QString fileName;
 if (lastDirectory=="")
     {
            fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"./","fbd files (*.fbd);;All files (*.*)");
 }
    else
     {
            fileName = QFileDialog::getOpenFileName(this, tr("Open File"),lastDirectory,"inp files (*.fbd);;All files (*.*)");
     }

     if (!fileName.isEmpty())
    {
            setCursor(1);
            emit runScript(fileName);
            lastDirectory=QFileInfo(fileName).absolutePath();
            //emit sendMessage(lastDirectory);
    }
 setCursor(0);
}
