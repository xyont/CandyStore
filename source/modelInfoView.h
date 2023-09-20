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
 
#ifndef MODELINFOVIEW_H
 #define MODELINFOVIEW_H
 
 #include <QTextEdit>
 #include "femodel.h"
 #include "geomodel.h"
 
 class modelInfoView: public QTextEdit
 {
 
 Q_OBJECT
 
 public:
        modelInfoView(QWidget *parent, femodel *md, geomodel*);
	~modelInfoView();
        void	setNodes(long numberNodes);
        void	setElements(long numberElements);

	femodel *feData;
        geomodel *geoData;
	
public slots:
        void showPointInformation(long);
        void showPlaneInformation(long);
        void showNodeInformation(long);
        void showMaterialInformation(int);
        void showSectionInformation(int);
        void showBCInformation(int);
        void updateInfo();

private:

	long currentNodeIndex;
        //QTableWidgetItem *elementItem;
        //QTableWidgetItem *nodeItem;
 };
#endif
