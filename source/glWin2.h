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
 #ifndef GLVIEWER2_H
 #define GLVIEWER2_H
 
//#include <QtGui>
#include <QtOpenGL>
#include "femodel.h"
#include "geomodel.h"
#include "commandline.h"
#include "./vector3d/vector3d.h"
#include "./trackball/trackball.h"
#include "octree.h"
#include "timeanimationclass.h"

#if defined(Q_WS_MAC)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

class femodel;
class geomodel;

class GLViewer2 : public QGLWidget
{

Q_OBJECT


public:
    int readFileFlag;
    int octreeFlag;
	GLViewer2(femodel *feDatabase, geomodel *geoDatabase);
	void setOrthoProjection();
	void setNodeStatus(short status);
	void setInitScaleFactor();
	QList<float> resultColors;
	QStringList scaleLabels;
	double fringeMax;
	double fringeMin;
	double fringeMaxAuto;
	double fringeMinAuto;
	
	void showNodeLabels();
	short selectRotFlag;
	void drawPlanes();
	/*QList<long> cuttingPlaneLabels;*/
	commandLine *cLine;
	void drawResultFileInfo();
        void drawTimeInformation();
	double featureEdgeAngle;
	void drawCurves();
    int qaddFlag;
    int rFlag;
    QList<int> nSetPlottingStack;
    QList<int> elSetPlottingStack;
    QList<int> faceSetPlottingStack;
    double currentTime;

	
	
	
public slots:
    void stopTimeAnimation();
    void startTimeAnimation(timeAnimationClass*);
    void toggleOctreeFlag();
    void setViewXY();
    void setViewXZ();
    void setViewYX();
    void setViewYZ();
    void setViewZX();
    void setViewZY();
    void drawOctree();
    void drawOcNode(octNode*);
    void drawCubeWireframe(vector3d center,double length);
    void activateReadFlag();
    void deactivateReadFlag();
    void resetData();
    void addFaceSetToPlottingStack(int);
    void addElSetToPlottingStack(int);
    void addNsetToPlottingStack(int);
    void setSingleElSetToPlottingStack(int);
    void setSingleFaceSetToPlottingStack(int);
    void setSingleNSetToPlottingStack(int);
    void removeFaceSetFromPlottingStack(int);
    void removeNsetFromPlottingStack(int);
    void removeElSetFromPlottingStack(int);
	void setViewingCenterRadius();
	void setScaleLabels(double min,double max); 
	void setFringeAutoValues(double min,double max);
	void setFringeMax(double max);
	void setFringeMin(double max);
	void setFringeMaxAuto();
	void setFringeMinAuto();
	void setScaleTitle(const QString &title);
	void setResultPlotStatus(short status);
	void setWireframeMode();
	void setShadedMode();
	void setSelectRotFlag();
	void drawSelectedNode(long index);
	void addCuttingPlane(long p);
	void removeCuttingPlane(long c);
	void toggleNodeStatus();
	void setDispScale(float d);
	void toggleShowUndef(bool check);
	void newFileActions();
	void zoomPlus();
	void zoomMinus();
	void startAnimation();
	void stopAnimation();
	void updateView();
    void modelScaled(double);
    void resizeGL(int w, int h);
    void selectNode();
    void selectFace();
    void selectElement();
    //void selectPoint();
    void setSelectionMode(int);
    void RPressed();
    void setFeatureEdgeAngle(int);
    void createDisplacedFaceDisplayList(int);
    void createSTLTriaDisplayList();
    void createPointsDisplayList();
    void togglePointStatus();
	
protected :
  void paintGL();
  void initializeGL();
  virtual void keyPressEvent(QKeyEvent *e);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  
  private :
    int timeAnimFlag;

    vector3d testorigin;
    vector3d testnormal;
    void drawPlane(vector3d,vector3d);
    QPoint firstRPoint;
    int rflag;
    int selectionMode;
    int selectionWidth;
    int selectionHeight;
    void drawSurfaces();
    void plotElement(long,float*);
    void plotNSet(int);
    void plotFaceSet(int);
    void plotElSet(int);
    void plotElementSets();
    void drawSelectionBox();
    void startScreenCoords();
    void endScreenCoords();
    void drawArrow(vector3d,vector3d);
    void drawArrowFrom(vector3d start,vector3d d,double l);
    void drawArrowTo(vector3d end,vector3d d,double l);
    double currentAnimScaleFactor;
    double animationScaleFactors[100];
    int animCount;
    int numberAnimationSteps;
    int currentTimeAnimFrame;
    QTimer *animationTimer;
    QTimer *timeAnimationTimer;
    long processHits(GLint hits, GLuint buffer[]);
    long processPointHits(GLint hits, GLuint buffer[]);
    void processFaceHits(GLint hits, GLuint buffer[]);
    void processHitsNodeSelect(GLint hits, GLuint buffer[]);
    void processHitsFaceSelect(GLint hits, GLuint buffer[]);
    void processHitsElementSelect(GLint hits, GLuint buffer[]);
    void setSelectionWidth(GLdouble);
    void setSelectionHeight(GLdouble);
    long pickedName;
    void setRotCenter(vector3d center);
    void setRotCenter(double x,double y,double z);
    double rotMatrix[4][4];
    double rMem[4][4];
    double dr[4][4];
    trackball *tball;
    quaternion rotquat;
    void moveModel();
    double horizontalClipping;
    double verticalClipping;
    double zClipping;
    void centerModel();
    QPoint lastPos;
    vector3d rotCenter;
    vector3d translations;
    double v[4];
    double vmem[4];
    bool showUndef;
    float numberResultColors;
    femodel *feData;
    geomodel *geoData;
    short nodeStatus;
    short faceStatus; //0=Shaded,1=Wireframe,Outside Edges Only, 2=Wireframe,All Edges
    short resultPlotStatus; //0=no Results ; 1=fringe ; 2=iso;3=numeric
    QString scaleTitle;
    void drawWithNames();
    void drawPointsWithNames();
    void drawFacesWithNames();
    void drawElementsWithNames();
    double aspectRatio;
    int edgeStatus; //0=FeatureEdges,1=Outside Edges
    int pointStatus;
  
	void createNodesDisplayList();
	void createOutsideNodesDisplayList();
    void createDisplacedNodesDisplayList();
	void createDisplacedOutsideNodesDisplayList();
	void createFaceDisplayList();
    void createEdgeDisplayList();

	void createScaleFactorAnimationFaceLists();
    void drawNodes();
    void drawDispNodes();
    void drawFaces();
    void drawDispFaces();
    void drawEdges();
    void drawAllEdges();
    void drawDispEdges();
    void drawDispFeatureEdges();
    void drawFeatureEdges();
    void drawBorderEdges();
    float dispScale;
    void drawRefCoordSystem();
    void drawCoordSystem(vector3d, vector3d, vector3d);
    void drawCoords();
    void drawResultColorBox();
    void build_rotmatrix(double m[4][4], quaternion q);
    quaternion MatrixToQuat(double a[4][4] );
    void showNumericResults();
    void createResultColorTexture();
    void createResultIsoTexture();
    void define_rgb(float v, float *r, float *g, float *b);
    void v_multmat(double *v, double *m);
    void m_copy(double *s, double *m);
    void m_sub(double *ms, double *m, double *s);
    long select(const QMouseEvent* event);
    long selectPoints(const QMouseEvent* event);
    void plotNodeSets();
    void plotFaceSets();
    void plotQNodes();
    void plotQFaces();
  
  private slots:
	void setRotCenter(long index);
        void setRotCenterPoint(long index);
	void updateAnimation();
        void updateTimeAnimation();
        void updateNSetDisplayLists();

  signals:
	void sendMessage(const QString&);
	void nodeSelected(long index);
        void pointSelected(long);
	void rotCenterSelected(long index);
    void rotCenterSelectedPoint(long);
    void addNodeToSet(long);
    void addFaceToSet(long);
    void addElementToSet(long);
    void addNodesToSet(QList<long>);
    void addFacesToSet(QList<long>);
    void addElementsToSet(QList<long>);
    void removeNodeFromSet(long);
    void removeFaceFromSet(long);
    void removeElementFromSet(long);
    void updateNodeSetDisplayLists();
};
#endif
