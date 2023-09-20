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
 #include "createPointWidget.h"
 
 /**
 * \file createPointWidget.cpp
 * \brief A brief file description
 **/
 
 /*! Standard Constructor*/ 
 createPointWidget::createPointWidget(QWidget *parent)
    	:QWidget(parent)
 {
	closeButton = new QPushButton("Close",this);
	startButton=new QPushButton("Apply",this);
	header = new QLabel("Create Points ",this);
	header->setFrameStyle(QFrame::Box);
        coordinateHeader = new QLabel("Coordinates (x y z)",this);
	//targetSelectHeader = new QLabel("Interpolate on:",this);
	//DispScalEdit = new QLineEdit(this);
	//DispScalButton = new QPushButton("Select",this);
	//DispScalButton->setFixedWidth(50);
	//allNodes=new QCheckBox("all Nodes");
	//allNodes->setCheckState(Qt::Checked);
	//outsideFaces=new QCheckBox("outside Faces");
	xEdit = new QLineEdit(this);
	yEdit = new QLineEdit(this);
	zEdit = new QLineEdit(this);
	
	
	QVBoxLayout *vLayout = new QVBoxLayout;
        //QHBoxLayout *DispScalLayout = new QHBoxLayout;
	
	
	//DispScalLayout->addWidget(DispScalEdit);
	//DispScalLayout->addWidget(DispScalButton);
	
	vLayout->addWidget(header);
	vLayout->addStretch(1);
	vLayout->addWidget(coordinateHeader);
	vLayout->addWidget(xEdit);
        //vLayout->addWidget(yEdit);
        //vLayout->addWidget(zEdit);
	vLayout->addStretch(1);
	vLayout->addWidget(startButton);
	vLayout->addWidget(closeButton);
	this->setLayout(vLayout);
	
	//connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
	connect(startButton,SIGNAL(clicked()),this,SLOT(createPoint()));
	
	
 }
 //***********************************************************************
/*!Standard Destructor*/
createPointWidget::~createPointWidget()
 {
	
 
 }
 //****************************************************
void createPointWidget::createPoint()
 {
    bool ok,ok2,ok3;
    QStringList entries=xEdit->text().split(" ",QString::SkipEmptyParts);

        double x=entries[0].toDouble(&ok);
        double y=entries[1].toDouble(&ok2);
        double z=entries[2].toDouble(&ok3);
	
	if (ok==true && ok2==true && ok3==true)
        emit createPointXYZ(x,y,z,0);
	else
		emit sendMessage("Illegal coordinates");
	return;
 }
