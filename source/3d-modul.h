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

 #ifndef MODUL3D_H
 #define MODUL3D_H
 
  #include <QtGui>
  #include "modelTreeView.h"
  #include "modelInfoView.h"
  #include "commandline.h"
  #include "geomodel.h"
  #include "resultOptionsWidget.h"
  #include "graphicoptionswidget.h"
  #include "interpolateOptionsWidget.h"
  #include "createPointWidget.h"
  #include "createCurveWidget.h"
  #include "createCOSWidget.h"
  #include "deletecoswidget.h"
  #include "mergeNodesWidget.h"
  #include "createboundarywidget.h"
  #include "creatematerialwidget.h"
  #include "createfesectionwidget.h"
  #include "verifyelementswidget.h"
  #include "createmeshwidget.h"
  #include "createloadswidget.h"
  #include "creatediscwidget.h"
  #include "fileimportwidget.h"
  #include "glWin2.h"
#include <QStackedWidget>
#include "materialwidget.h"
 
 /*class GLViewer;*/
 class GLViewer2;
 class femodel;


 class Modul3D : public QWidget
{
    Q_OBJECT

    public:
        Modul3D(QWidget *parent=0, femodel *md=0, geomodel *geom=0);
	void setBgColor(const QColor &bgc);
	
	GLViewer2 *viewingArea;
    QStackedWidget *mainArea;
	modelTreeView *modelTree;
	modelInfoView *infoBox;
	geomodel *geoData;
    femodel *feData;
	QStackedWidget *varioWidget;
	resultOptionsWidget *resultOptions;
        graphicOptionsWidget *graphicOptions;
	interpolateOptionsWidget *interpolOptions;
	createPointWidget *createPoint;
	createCurveWidget *createCurve;
	createCOSWidget *createCOS;
        deleteCOSWidget *deleteCOS;
        mergeNodesWidget *mergeNodes;
        createBoundaryWidget *createBC;
        createMaterialWidget *createMat;
        materialWidget *matWidget;
        createFeSectionWidget *createFeSect;
        verifyElementsWidget *verifyElementView;
        createMeshWidget *createMeshView;
        createLoadsWidget *createLoadsView;
        createDiscWidget *createDiscView;
        fileImportWidget *fileImportView;
	void releaseKeys();
	void grabKeys();

    protected:
	
    
        virtual void keyPressEvent(QKeyEvent *e);

    private:
        void evaluatePLUS(const QString&);
        void evaluateREV(const QString&);
        void evaluatePNT(const QString&);
        void evaluateLINE(const QString&);
        void evaluateSURF(const QString&);
        void evaluateCommand(const QString&);
	QHBoxLayout *mainLayout;
	QVBoxLayout *leftLayout;
	QVBoxLayout *midLayout;
	QVBoxLayout *rightLayout;
	
	QTextEdit *messages;
	commandLine *cLine;
        bool eventFilter(QObject *target, QEvent *event);
        int getIntColorFromString(const QString&);

	
	
	QCheckBox *setManager;
	
	public slots:
        void startMeshing(int dimension,double hmax,int so);
        void runScript(const QString&);
        void showTree();
        void showResultOptions();
        void showGraphicOptions();
        void showInterpolateOptions();
        void showCreatePoint();
        void showCreateCurve();
        void showCreateCOS();
        void showDeleteCOS();
        void showMergeNodes();
        void showLoadWidget();
        void showMat();
        void showFeSect();
        void showBC();
        void showVerifyElements();
        void showMeshGeo();
        void commandReceived();
        void recieveMessage(const QString &message);
        void showTreeItemInfo();
        void showCreateDisc();
        void showFileImport();
        void show3d();
	
	signals:

        void sendMessage(const QString&);
        void createMaterial(QString,double,double,double);
        void createFeSection(QString,QString,QString,int,double);
        void deleteMesh();
        void meshSTL(geomodel*,double,int,int);
        void toggleUndeformed();
        void exportMeshFrd(const QString &setname,long);
        void exportMeshAbq(const QString &setname,long);
        void exportLabelsAbq(int,long);
        void createPointXYZ(double,double,double,int);
        void createPointXYZIndex(long,long,double,double,double);
        void createStraightLine(long,long);
        void createStraightLineLabel(long,long,long);
        void createSurface(long,QList<long>);
        void qlin();
        void scaleModel(double);
        void newSet(const QString &setname);
        void nodeSelection();
        void faceSelection();
        void elementSelection();
        void setSelectionMode(int);
        void updateView();
        void RPressed();
        void writeSetFrd(int,long);
        void updateSets();
        void plotSet(int,int);
        void plotFaceSet(int,int);
        void plotElSet(int,int);
        void plotSingleNSet(int,int);
        void plotSingleFaceSet(int,int);
        void plotSingleElSet(int,int);
        void unplotSet(int);
        void exportFoam(const QString);
        void calcExpRatio();
        void moveRadially(double);
        void autoExtendNodes(const QString,double);
        void autoExtendFaces(const QString,double);
        void toggleOctree();
	};
 
 #endif
