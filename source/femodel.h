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
 #ifndef FEMODEL_H
 #define FEMODEL_H

 #include "sets.h"
 #include "geomodel.h"
 #include "glWin2.h"
 #include <QObject>
 #include <QtXml>
 #include "node.h" 
 #include "element.h"
 #include "face.h"
 #include "edge.h"
 #include "resultindex.h"
 #include "result.h"
// #include "nodeset.h"
// #include "elset.h"
// #include "faceset.h"
 #include "interpolator.h"
 #include "pvdhandler.h"
 #include "mpc.h"
 #include "boundarycon.h"
 #include "openfoamface.h"
 #include "material.h"
 #include "fesection.h"
 #include "load.h"
 #include "octree.h"
 #include "timeanimationclass.h"

//#include <vtkSmartPointer.h>
#include <vtkCell.h>

#include <vtkIdList.h>
//#include <vtkPoints.h>
#include <vtkPointData.h>
#include <vtkFieldData.h>
#include <vtkDataObject.h>
#include <vtkCompositeDataSet.h>
#include <vtkMultiBlockDataSet.h>

//#include <vtkPolyData.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
//#include <vtkStructuredGridSource.h>
#include <vtkAppendFilter.h>


//#include <vtkPLOT3DReader.h>
//#include <vtkXMLPolyDataReader.h>
#include <vtkXMLMultiBlockDataReader.h>
#include <vtkXMLStructuredDataReader.h>
//#include <vtkXMLUnstructuredDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkXMLReader.h>

//!  All data and methods related to the femodel
/*!  
	The femodel class holds all
	data related to the femodel( for example nodes, elements,
	results,...) and the methods for reading and working with 
	the FE-model.
 */
    class geomodel;
    //class octree;

    class femodel : public QObject
    {
	
		Q_OBJECT
		
 public:
    femodel();
    timeAnimationClass *timeAnimation;
    octree *nodeOctree;
    boundarycon tempBc;
    int fileflag;
    QString currentSet;
    QString ccxLocation;
    int currentSetIndex;
    int currentNsetIndex;
    QList<sets> setList;
    mpc tempMpc;
    boundarycon tempBC;
    QList<boundarycon> bcList;
    QList<mpc> mpcList;
    //QVector<nSet> nSetList;
    //QVector<elSet> elSetList;
    //QVector<faceSet> faceSetList;
	QVector<node> nodeList;
	QVector<element> elementList;
	QList<face> faceList;
	QVector<edge> edgeList;
	QVector<resultindex> resultList;
	QVector<result> resultDataList;
    QList<interpolator> interpolatorList;
    QList<material> materialList;
    QList<feSection> feSectionList;
    QList<load> loadList;
    QList<long> borderEdges;

	int readfrd(const QString&);
    int readfrdResultInfo(const QString&);
	int readVtk(const QString&);
	int readfrdresults(int resultstep);
    int readinp(const QString&);
    int readcdb(const QString&);
	QStringList readCCXNodes(QTextStream&);
	QStringList readCCXElements(QTextStream&, const QStringList&);
	QStringList readCCXUnknown(QTextStream&);
    bool readCCXisComment(const QString&);
	QStringList readCCXInclude(const QString&, const QStringList&, const QStringList&);
	//QStringList readCCXInclude(const QString&, const QStringList&, const QStringList&);
	void getFaces();
	vector3d maxCoords;
	vector3d modelCenter;
	vector3d getNodeCoords(long label);
	double getNodeResults(long label);
    long getNodeResultListIndex(long label);
	vector3d getDisplacements(long label);
	long getNodeIndex(long label);
    long getFaceIndex(face,int*);
    long getEdgeIndex(edge,int*);
	long getElementIndex(long label);
	float modelRadius;
	QStringList ResultTitles;
	QStringList ResultComponents;
	QString openFile;
	bool dispAvailable;
	float dispRatio; //ratio between modelradius and max displacement 
	result resultData;
    void setNodeValues(int ,unsigned int );
	void clearResultValues();
	void clearDisp();
//	nSet tempNSet;
	void calcFaceCentroidResults(int r, int c);
	double currentMax;
	double currentMin;
	void sortFaceList(long links,long rechts);
	void sortEdgeList(long links,long rechts);
	interpolator tempInterpol;
    sets tempSet;

public slots:

        void createTimeAnimation();
        void sendM(const QString&);
        void addElementsToSet(QList<long>);
        void removeElementFromSet(long);
        void addFaceToSet(long);
        void addFacesToSet(QList<long>);
        void autoSelectNodes(const QString,double);
        void autoSelectFaces(const QString set, double featureAngle );
        void addNodesToSet(QList<long> labelList);
        void deleteMaterial(int);
        void deleteFeSection(int);
        void deleteBC(int);
        void deleteLoads(int);
        void createLoad(QString,int,QString,double,int);
        void createCentrifugalLoad(QString n,QString set, QString point1,QString point2, double rpm);
        void createMaterial(QString,double,double,double);
        void createFeSection(QString,QString,QString,int,double);
        void deleteAllMesh();
        void createBC(const QString&,const QString&,int,int,double);
        void createMeshFromStl(geomodel*, double,int,int);
        void create3DMeshFrom2D(double,int, int);
        void writeMeshFrd(const QString&,long);
        void calcExpRatio();
        void createBristlePack(double alpha, double beta, double rb, double h1, double h3, double d, int AnzXRichtung,int AnzZRichtung, double diffx, double diffz,int);
        void writeVTK(const QString&);
        void writeOutsideFaces(const QString& fileName);
        void writeOutsideFacesABQ(const QString& fileName);
        void interpolateFrdResults(const QString&);
        void interpolateFrdResultsFace(const QString&);
        void interpolateVtkUnstructuredResults(const QString&);
        void interpolateVtkMultiblockResults(const QString&);
        void interpolateVtkUnstructuredResultsFace(const QString&);
        void interpolateVtkMultiblockResultsFace(const QString&, bool);
        void interpolateVtkPvdResults(const QString&);
        void interpolateVtkPvdResultsFace(const QString&);
        void exportMeshFrd(const QString&,long);
        void exportMeshAbq(const QString&,long);
        void exportMeshOpenFoam(const QString&);
        void exportDummyAbqInputData(const QString&);
        void changeHex8toHex20();
        void importVtkUnstructured(const QString&);
        void importVtkMultiblock(const QString&);
        void mergeNodes(double,int );
        void mergeNodesSet(double,int,int);
        void scaleModel(double);
        void createNewSet(const QString &name);
        void addNodeToSet(long);
        void addElementToSet(long);
        void removeNodeFromSet(long);
        void removeFaceFromSet(long);
        void writeSetFrd(int,long);
        void writeSetAbq(const QString&,int,long);
        void writeSetAbqLabelsOnly(int, long);
        int readVtkResults(int);
        void setSingleNodeSetPlotFlag(int,int);
        void setSingleFaceSetPlotFlag(int index,int colorIndex);
        void setNodeSetPlotFlag(int,int);
        void unsetNodeSetPlotFlag(int);
        void moveNodesRadially(double);
	
	private:
        void writeResultsOpenFoam(const QString&);
        void writePointsOpenFoam(const QString&,int);
        void writeFacesOpenFoam(const QString&);
        qint64 getFilePosition(int);
        int getIntElType(const QString&);
        void createInterpolators(femodel*,bool);
        void createInterpolatorsFace(femodel*,bool);
        void interpolateAndWriteResults(femodel*,int);
        void writeInterpolatorData(const QString&, const QString&);
        void getMultiblockData(vtkAppendFilter*,vtkMultiBlockDataSet*);
        void readVtkMeshFromUnstructuredGrid(vtkUnstructuredGrid*);
        void findParaHe8(femodel*,long, vector3d,double*);
        void findParaTet4(femodel*,long, vector3d,double*);
        int solveLinGl3(double*,double*,double*);
        void clearDatabase();
        void setModelCenter();
        void createSurfaceNodeSet();
        int readinpdata(const QString&);
        double calcTriArea(vector3d p1,vector3d p2, vector3d p3);
        QString frdHeader(const QString&);
        QString resultHeader(int step,double,const QString &comment,int numberComponents,QStringList& );
        QString openFoamHeader();
        QString openFoamPointHeader();
        QString openFoamFaceHeader();
        QString openFoamOwnerHeader(long, long );
        QString openFoamNeighbourHeader(long, long);
        QString openFoamBoundaryHeader(long, long);
        QString openFoamScalarResultHeader(const QString&);
        QString openFoamVectorResultHeader(const QString&);

        void writeFRDresults(const QString&,int,long);
        void writeMeshAbq(const QString&,int*,long);
	
        vector3d getMaxModelCoords();
        void setMaxCoords(double x, double y, double z);
        node tempNode;

        //elSet tempElSet;
        //faceSet tempFaceSet;
        element tempElement;
        face tempFace;
        edge tempEdge;
        resultindex tempResultIndex;
        void getQuad8Faces(long elementListNumber);
        void getHex20Faces(long elementListNumber);
        void getTet10Faces(long elementListNumber);
        void getTria3Faces(long elementListNumber);
        void getTria6Faces(long);
        void getTet4Faces(long elementListNumber);
        void getHex8Faces(long elementListNumber);
        void getPenta6Faces(long elementListNumber);
        void getPenta15Faces(long elementListNumber);
        void getOutsideEdges();
        void getOutsideElements();
        void calcFaceNormals();
        void calcFaceCentroids();
        void calcFaceCentroidDisps();
        void calcEdgeAngles();
        nodalResults tempNodeResult;
        QString currentTitle;
        void getOutsideNodes();
        void updateFacesEdges();
        qint64 filePos;

	signals:
        void sendMessage(const QString&);
        void sendResultTitle(const QString&);
        void sendMinMaxScale(double min, double max);
        void sendFringeAutoValues(double min, double max);
        void newResults();
        void modelUpdate();
        void showVtkResults();
        void modelScaled(double);
        void setCursor(int);
        void update();
        void newResultValues(int);
        void fringePlot(short);
        void startTimeAnim(timeAnimationClass*);
        void stepSelected(int);
        void createTimeFrame(int);
};

#endif
