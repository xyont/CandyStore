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
 #ifndef MODELTREEVIEW_H
 #define MODELTREEVIEW_H
 
 #include <QTreeWidget>
 #include "femodel.h"
 #include "geomodel.h"
 
 class modelTreeView: public QTreeView
 {
	Q_OBJECT
 
 public:
	modelTreeView(QWidget *parent, femodel *fedatabase, geomodel *geoDatabase);
	~modelTreeView();
private:
    void	setNodes(long);
    void	setElements(long);
    void    setFaces(long);
	void    setNodeSet(femodel *md);
	void	setResults(femodel *md);
    void 	setGeometricEntities(long);
	void	updateGeometryInfo(geomodel *gm);

	femodel *fedata;
	geomodel *geodata;

    int currentRowSelection;
	
        QStandardItem *tempItem;
        QList<QStandardItem*> timeResultList;
        QList<QStandardItem*> freqResultList;
        QList<QStandardItem*> nSetList;
        QList<QStandardItem*> elSetList;
        QList<QStandardItem*> faceSetList;
        QList<QStandardItem*> cosList;
        QList<QStandardItem*> bcList;
        QList<QStandardItem*> materialList;
        QList<QStandardItem*> fesectionList;
        QStandardItemModel *model;
	QStandardItem *parentItem; 
	QStandardItem *nodeItem; 
	QStandardItem *elementItem; 
	QStandardItem *resultItem;
	QStandardItem *timeItem;
	QStandardItem *freqItem;
    QStandardItem *cosItem;
	QStandardItem *geometryItem;
	QStandardItem *pointItem;
    QStandardItem *stlItem;
	QStandardItem *planeItem;
	QStandardItem *curveItem;
        QStandardItem *surfaceItem;
    QStandardItem *faceItem;
    QStandardItem *feItem;
    QStandardItem *bcItem;
    QStandardItem *mpcItem;
    QStandardItem *loadItem;
    QStandardItem *materialItem;
    QStandardItem *feSectionItem;
public slots:
    void    hideFaceSet();
    void    showFaceSet();
    void    hideNSet();
    void    showNSet();
    void    hideElSet();
    void    showElSet();
    void    deleteMat();
    void    deleteFeSec();
    void    deleteBC();
    void    deleteLoad();
	void	updateTree();
    void    updateNSets();
    void    updateElSets();
    void    updateFaceSets();
    void    updateCos();
    void    resetData();
    void    updateFEData();
private slots:
	void evaluateClick(QModelIndex mi);

protected slots:
	void selectionChanged( const QItemSelection & selected, const QItemSelection & deselected );
signals:
	void sendMessage(const QString&);
	void stepSelected(int step);
	void planeSelected(long label);
	void planeDeselected();
	void geometrySelected();
	void geometryDeselected();
    void nodesSelected();
    void nodesDeselected();
    void cosRootSelected();
    void cosRootDeselected();
    void mpcSelected();
    void mpcDeselected();
    void feDataSelected();
    void feDataDeselected();
    void matSelected(int);
    void matDeselected();
    void feSectionSelected(int);
    void feSectionDeselected();
    void bcSelected(int);
    void bcDeselected();
    void loadSelected(int);
    void loadDeselected();
    void deleteBouCon(int);
    void deleteMaterial(int);
    void deleteFeSection(int);
    void deleteLoads(int);
    void elementsSelected();
    void elementsDeselected();
    void stlSelected();
    void stlDeselected();
    void nodeSetSelected(int);
    void nodeSetDeselected();
    void plotNSet(int,int);
    void unplotNSet(int,int);
    void faceSetSelected();
    void faceSetDeselected();
    void plotFaceSet(int,int);
    void unplotFaceSet(int,int);
    void elSetSelected();
    void elSetDeselected();
    void plotElSet(int,int);
    void unplotElSet(int,int);

 };
#endif
