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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
/*#include <QGLViewer/qglviewer.h>*/
#include "3d-modul.h"
#include "femodel.h"
#include "geomodel.h"
#include "resultselector.h"
#include "modelTreeView.h"
#include "readfrdthread.h"
#include <QtHelp>

 class QAction;
 class QMenu;
 class QTextEdit;
 class QToolBar; 
 

 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     MainWindow();
     QString ccxLocation;
 public slots:
     void updateResultSelector();
     void setCursor(int);
     void setBusyCursor();
     void restoreCursor();
protected:
     void closeEvent(QCloseEvent *event);
     Modul3D *centrWin;
     readFrdMeshThread *readfrd;
     readFrdResultInfoThread *readresultinfofrd;
     readFrdResultDataThread *readresultdatafrd;
 private slots:

     void runFbd();
     void setCcxLocation();
     void readCSD();
     void setHelpFile();
     void exportVTKclicked();
     void exportFrdClicked();
     void exportOpenFoamClicked();
     void test();
     void newFile();
     void open();

     void importInp();
     void importCDB();
     void importVtk();
     void importStl();
     void save();
     void saveAs();
     void saveCSData(const QString&);
     void readCSData(const QString&);
     void openRecentFile();
     void about();
	 void setBgColor();
	 void toggleNodes();
     void togglePoints();
	 void loadNewResults(int step);
	 void help();
	 void takeScreenshot();
	 void writeOutFaces();
	 void interpolateResults();
	 void showPlaneTools();
      void showGeoTools();
         void hideGeoTools();
         void showNodeTools();
         void hideNodeTools();
	 void hidePlaneTools();
         void showCosTools();
         void hideCosTools();
         void showFeDataTools();
         void hideFeDataTools();
         void showBCTools(int);
         void hideBCTools();
         void showFaceSetTools();
         void hideFaceSetTools();
         void showElSetTools();
         void hideElSetTools();
         void showNodeSetTools(int);
         void hideNodeSetTools();
         void showMatTools(int);
         void hideMatTools();
         void showFeSecTools(int);
         void hideFeSecTools();
         void showElementTools();
         void hideElementTools();
         void showLoadTools();
         void hideLoadTools();
	 void readCsd(const QString &fileName);
	 void createOffsetPlaneClicked();
	 void abqInputDeckClicked();
	 void createPointXYZclicked();
         void changeHex8();


 private:

    void evaluateXMLLoadData(QString name,QString set,QString type,QString direction, QString value,QString, QString);
    void evaluateXMLBCData(QString name,QString set,QString fdof,QString ldof, QString value);
    void evaluateXMLFeSectionData(QString name,QString set,QString material);
    void evaluateXMLMaterialData(QString name,QString ym,QString poisson, QString density);
    QHelpEngine *helpEngine;
    QString helpfile;
    int helpfileSet;
    double getFeatureEdgeAngle();
    void setFeatureEdgeAngle(double);
     void createActions();
     void createMenus();
     void createToolBars();
     void loadFile(const QString &fileName, int type);
     void saveFile(const QString &fileName);
     void setCurrentFile(const QString &fileName, int type);
     void updateRecentFileActions();
     QString strippedName(const QString &fullFileName);
     void readSettings();
     void writeSettings();
	 QString mouseString () const;
	 QString commandString () const;
	 QString keyboardString () const;
     QString curFile;
	 QString lastDirectory; 
	 QString lastScreenshotDirectory;
	
	 
     QTextEdit *textEdit;
     QMenu *fileMenu;
     QMenu *recentFilesMenu;
	 QMenu *geometryMenu;
	 QMenu *geoCreateMenu;
     QMenu *helpMenu;
     QMenu *optionsMenu;
	 QMenu *resultMenu;
	 QMenu *utilityMenu;
	 QMenu *analysisMenu;
         QToolButton *viewOrientationToolButton;
         QToolBar *fileToolBar;
     QToolBar *viewToolBar;
     QToolBar *viewOrientationToolBar;
	 QToolBar *resultToolBar;
	 QToolBar *printToolBar;
	 QToolBar *testToolBar;
	 QToolBar *planeToolBar;
     QToolBar *nodeToolBar;
     QToolBar *geometryToolBar;
     QToolBar *cosToolBar;
     QToolBar *feDataToolBar;
     QToolBar *bcToolBar;
     QToolBar *materialToolBar;
     QToolBar *feSectionToolBar;
     QToolBar *elementToolBar;
     QToolBar *stlToolBar;
     QToolBar *nSetToolBar;
     QToolBar *faceSetToolBar;
     QToolBar *elSetToolBar;
     QToolBar *loadToolBar;
     QAction *createLoadAct;
     QAction *showFaceSetAct;
     QAction *hideFaceSetAct;
     QAction *showNSetAct;
     QAction *hideNSetAct;
     QAction *showElSetAct;
     QAction *hideElSetAct;
     QAction *deleteBCAct;
     QAction *deleteMaterialAct;
     QAction *deleteFeSectionAct;
     QAction *deleteLoadAct;
     QAction *setCcxLocationAct;
     QAction *setHelpFileAct;
     QAction *newAct;
     QAction *openAct;
     QAction *openCSDAct;
     QAction *importAct;
     QAction *importCDBAct;
	 QAction *importInpAct;
	 QAction *importVTKAct;
     QAction *importSTLAct;
     QAction *saveAct;
     QAction *saveAsAct;
     QAction *exitAct;
     QAction *aboutAct;
     QAction *aboutQtAct;
	 QAction *aboutMouseBindingsAct;
     QAction *separatorAct;
	 QAction *setBackgroundColorAct;
	 QAction *toggleNodesAct;
	 QAction *viewWireframeAct;
	 QAction *viewShadedAct;
	 QAction *setRotCenterAct;
	 QAction *resultOptionAct;
     QAction *graphicOptionsAct;
	 QAction *screenshotAct;
	 QAction *zoomInAct;
	 QAction *zoomOutAct;
	 QAction *startAnimAct;
         QAction *startTimeAnimAct;
	 QAction *stopAnimAct;
	 QAction *writeOutsideFacesAct;
	 QAction *interpolateResultsAct;
	 QAction *planeOffsetAct;
	 QAction *planeDeleteAct;
	 QAction *abqInputDeckAct;
	 QAction *createPointXYZAct;
	 QAction *createCurveAct;
     QAction *createCOSAct;
     QAction *hex8toHex20Act;
     QAction *mergeNodesAct;
     QAction *deleteCOSAct;
     QAction *exportVTKAct;
     QAction *exportOpenFoamAct;
     QAction *exportFrdAct;
     QAction *toggleShowPointsAct;
     QAction *createBCAct;
     QAction *createMaterialAct;
     QAction *createFeSectionAct;
     QAction *verifyElementsAct;
          QAction *meshStlAct;
     QAction *viewXYAct;
     QAction *viewYXAct;
     QAction *viewXZAct;
     QAction *viewYZAct;
     QAction *viewZXAct;
     QAction *viewZYAct;
     QAction *runFBDAct;
     QAction *createDiscAct;

     QWidget *helpWidget;

	 femodel *modelDatabase;
	 geomodel *geoDatabase;
	 resultSelector *resultSelect;
	 
     enum { MaxRecentFiles = 5 };
     QAction *recentFileActs[MaxRecentFiles];

	short nodeStatus;
	
	signals:
	/*! Sends a QString */
	void sendMessage(const QString&);
	/*! Emits a toggle nodes signal*/
	void toggleNodesSignal();
    void togglePointsSignal();
	/*! Emitted when a new file is loaded*/
	void newFileLoaded();
	void writeOFaces(QString);
	void interparaFrd(const QString&);
	void offsetPlane(long);
    void writeVTU(const QString&);
    void writeFRD(const QString&,long);
    void loadSTL(const QString&);
    void resetData();
    void writeOpenFoam(const QString&);
    void saveCS(const QString&);
    void loadCSD(const QString&);
    void createMaterial(QString, double,double,double);
    void createFeSection(QString,QString,QString,int,double);
    void createBC(QString,QString,int,int,double);
    void createLoad(QString,int,QString,double,int);
    void createCentrifugalLoad(QString,QString,QString,QString,double);
    void updateView();
    void showLoadWidget();
    void runScript(const QString&);
};

 #endif 
