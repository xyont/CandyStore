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
 #include "createCurveWidget.h"
 
 /**
 * \file createCurveWidget.cpp
 * \brief A brief file description
 **/
 
 /*! Standard Constructor*/ 
 createCurveWidget::createCurveWidget(QWidget *parent)
    	:QWidget(parent)
 {
	closeButton = new QPushButton("Close",this);
	startButton=new QPushButton("Apply",this);
	header = new QLabel(" Create Curves ",this);
	header->setFrameStyle(QFrame::Box);
	p1Header = new QLabel("Startpoint",this);
	p2Header = new QLabel("Endpoint",this);
	//DispScalEdit = new QLineEdit(this);
	p1Button = new QPushButton("Pick",this);
	p1Button->setFixedWidth(50);
	p2Button = new QPushButton("Pick",this);
	p2Button->setFixedWidth(50);
	//allNodes->setCheckState(Qt::Checked);
	//outsideFaces=new QCheckBox("outside Faces");
	p1Edit = new QLineEdit(this);
	p2Edit = new QLineEdit(this);
	//zEdit = new QLineEdit(this);
	
	
	QVBoxLayout *vLayout = new QVBoxLayout;
	QHBoxLayout *p1Layout = new QHBoxLayout;
	QHBoxLayout *p2Layout = new QHBoxLayout;

	
	p1Layout->addWidget(p1Header);
	p1Layout->addWidget(p1Edit);
	p1Layout->addWidget(p1Button);

	p2Layout->addWidget(p2Header);
	p2Layout->addWidget(p2Edit);
	p2Layout->addWidget(p2Button);


	
	vLayout->addWidget(header);
	vLayout->addStretch(1);
	vLayout->addLayout(p1Layout);
	vLayout->addLayout(p2Layout);
	//vLayout->addWidget(yEdit);
	//vLayout->addWidget(zEdit);
	vLayout->addStretch(1);
	vLayout->addWidget(startButton);
	vLayout->addWidget(closeButton);
	this->setLayout(vLayout);
	
	//connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
	connect(startButton,SIGNAL(clicked()),this,SLOT(createCurve()));
	
	
 }
 //***********************************************************************
/*!Standard Destructor*/
createCurveWidget::~createCurveWidget()
 {
	
 
 }
 //****************************************************
void createCurveWidget::createCurve()
 {
		bool ok,ok2;
			long s=p1Edit->text().toLong(&ok);
			long e=p2Edit->text().toLong(&ok2);		
			
			
			if (ok==true && ok2==true)
				emit createStraightLine(s,e);
			else
				emit sendMessage("Illegal Point Labels");
			return;
	
	return;
 }