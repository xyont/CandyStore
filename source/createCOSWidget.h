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
 * \file createCOSWidget.h
 * \brief A brief file description
 **/
#ifndef createCOSWidget_H
#define createCOSWidget_H

    #include <QtGui>
#include "vector3d/vector3d.h"

    class QLineEdit;
    class QHBoxLayout;
    class QVboxLayout;
    class QLabel;
    class QPushButton;
//!   Create COS Widget
/*! 
	The Create COS Widget provides functionality to control the 
	creation of coordinate systems,...
 */
    class createCOSWidget : public QWidget
    {
        Q_OBJECT

    public:
	createCOSWidget(QWidget *parent=0);
	~createCOSWidget();
	
	
	QPushButton *closeButton;
	QPushButton *startButton;
	QCheckBox *allNodes;
	QCheckBox *outsideNodes;
	QCheckBox *outsideFaces;
	
	QSpacerItem *vSpacer;
	QSpacerItem *vSpacer2;
	QLabel *header;
        QLabel *v1Header;
        QLabel *v2Header;
        QLabel *p1Header;
        QLabel *originHeader;
        QLabel *nameHeader;
	QLabel *fringeMaxHeader;
	QLineEdit *p1Edit;
        QLineEdit *nameEdit;
        QLineEdit *v1Edit;
        QLineEdit *v2Edit;
        QLineEdit *originEdit;
	QPushButton *p1Button;
	QPushButton *p2Button;
	QPushButton *fringeMaxButton;
	QString resultFile;
	QString lastInterpolDirectory;
	
	public slots:
	
	
	
	private slots:
        void createCos();
	
	
	signals:
	//! Sends a  QString 
	void sendMessage(const QString&);
        void createCoord(const QString&, vector3d, vector3d, vector3d, int);
	
	};
#endif
