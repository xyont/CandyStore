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
 #ifndef GEOMODEL_H
 #define GEOMODEL_H

 #include "glWin2.h"
 #include <QObject>
 
 #include "plane.h"
 #include "point.h"
 #include "curve.h"
 #include "surface.h"
 #include "stltriangle.h"
 #include "coordinatesystem.h"

#include <vtkSTLReader.h>
 
//!  All data and methods related to the geometry-model
/*!  
	The geomodel class holds all
	data related to the geometric model( for example points, lines,
	rplanes,...) and the methods for reading and working with 
	the geometry-model.
 */

    class geomodel : public QObject
    {
	
		Q_OBJECT
		
    public:
        geomodel();
        void initialize();
	QList<plane> planeList;
	QList<point> pointList;
	QList<curve> curveList;
        QList<surface> surfaceList;
        QList<stltriangle> stlTriaList;
        QList<coordinateSystem> cosList;
	plane tempPlane;
        coordinateSystem tempCos;
	float modelRadius;
	
    public slots:

        void readSTL(const QString&);
        void createCos(const QString&, vector3d,vector3d,vector3d,int);
	void createOffsetPlane(long);
	void deletePlane(long);
        void createSTLTriangle(long,long,long);
        long createPoint(double,double,double,int);
        void createPoint(long,long,double,double,double);
        void createPointArcCenter(long,vector3d,vector3d,vector3d);
        long createCurve(long,long);
        void createCurve(long,long,long);
        void createSurface(long,QList<long>);
	vector3d getPointCoords(long);
	long getPointIndex(long label);
        long getLineIndex(long label);
        long getSurfaceIndex(long label);
        vector3d getMaxCoords();

    private:

        bool closedLoop(QList<long>);
	
    signals:

        void newCosCreated();
	void sendMessage(const QString&);
	void newPlaneCreated();
	void geoDatabaseModified();
	};
#endif
