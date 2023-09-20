/***************************************************************************
 *   Copyright (C) 2010 by Patrick Prokopczuk   *
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
 #include "createCOSWidget.h"
 
 /**
 * \file createCOSWidget.cpp
 * \brief A brief file description
 **/
 
 /*! Standard Constructor*/ 
 createCOSWidget::createCOSWidget(QWidget *parent)
    	:QWidget(parent)
 {
	closeButton = new QPushButton("Close",this);
        startButton=new QPushButton("Apply",this);
	header = new QLabel(" Create Coordinate System",this);
	header->setFrameStyle(QFrame::Box);
        //p1Header = new QLabel("Not yet implemented",this);
        nameHeader = new QLabel("Name",this);
        nameEdit = new QLineEdit(this);
        originHeader = new QLabel(QString(tr("Origin (x y z)")));
        originEdit = new QLineEdit(this);
        v1Header = new QLabel(QString(tr("Vector for direction 1 (x y z)")));
        v1Edit = new QLineEdit(this);
        v2Header = new QLabel(QString(tr("Vector in plane 1-3  (x y z)")));
        v2Edit = new QLineEdit(this);
	//p2Edit = new QLineEdit(this);
	//zEdit = new QLineEdit(this);
	
	
	QVBoxLayout *vLayout = new QVBoxLayout;
	//QHBoxLayout *p1Layout = new QHBoxLayout;
	//QHBoxLayout *p2Layout = new QHBoxLayout;

	
	//p1Layout->addWidget(p1Header);
	//p1Layout->addWidget(p1Edit);
	//p1Layout->addWidget(p1Button);

	//p2Layout->addWidget(p2Header);
	//p2Layout->addWidget(p2Edit);
	//p2Layout->addWidget(p2Button);


	
	vLayout->addWidget(header);
	vLayout->addStretch(1);
	//vLayout->addLayout(p1Layout);
	//vLayout->addLayout(p2Layout);
        vLayout->addWidget(nameHeader);
        vLayout->addWidget(nameEdit);
        vLayout->addWidget(originHeader);
        vLayout->addWidget(originEdit);
        vLayout->addWidget(v1Header);
        vLayout->addWidget(v1Edit);
        vLayout->addWidget(v2Header);
        vLayout->addWidget(v2Edit);
	vLayout->addStretch(1);
        vLayout->addWidget(startButton);
	vLayout->addWidget(closeButton);
	this->setLayout(vLayout);
	
	//connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
        connect(startButton,SIGNAL(clicked()),this,SLOT(createCos()));
	
	
 }
 //***********************************************************************
/*!Standard Destructor*/
createCOSWidget::~createCOSWidget()
 {
	
 
 }
 //****************************************************
void createCOSWidget::createCos()
{
    bool ok,ok2,ok3;
    int type=0;
    vector3d ori,v1,v2;
    ori.x=originEdit->text().section(" ",0,0).toDouble(&ok);
    ori.y=originEdit->text().section(" ",1,1).toDouble(&ok2);
    ori.z=originEdit->text().section(" ",2,2).toDouble(&ok3);

    if (ok==true && ok2==true && ok3==true)
    {
        ;
    }
            //emit createPointXYZ(x,y,z);
    else
    {
            emit sendMessage("Illegal origin coordinates");
            return;
    }
    v1.x=v1Edit->text().section(" ",0,0).toDouble(&ok);
    v1.y=v1Edit->text().section(" ",1,1).toDouble(&ok2);
    v1.z=v1Edit->text().section(" ",2,2).toDouble(&ok3);

    if (ok==true && ok2==true && ok3==true)
    {
        ;
    }
            //emit createPointXYZ(x,y,z);
    else
    {
            emit sendMessage("Illegal coordinates for direction 1 vector");
            return;
    }
    v2.x=v2Edit->text().section(" ",0,0).toDouble(&ok);
    v2.y=v2Edit->text().section(" ",1,1).toDouble(&ok2);
    v2.z=v2Edit->text().section(" ",2,2).toDouble(&ok3);

    if (ok==true && ok2==true && ok3==true)
    {
        ;
    }
            //emit createPointXYZ(x,y,z);
    else
    {
            emit sendMessage("Illegal coordinates for vector in plane 1-3 ");
            return;
    }


        emit createCoord(nameEdit->text(),ori,v1,v2,type);
    return;
}
