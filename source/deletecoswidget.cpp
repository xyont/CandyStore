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
 #include "deletecoswidget.h"

 /**
 * \file deletecoswidget.cpp
 * \brief A brief file description
 **/

 /*! Standard Constructor*/
 deleteCOSWidget::deleteCOSWidget(QWidget *parent)
        :QWidget(parent)
 {
        closeButton = new QPushButton("Close",this);
        deleteButton=new QPushButton("Delete",this);
        header = new QLabel(" Delete Coordinate System",this);
        header->setFrameStyle(QFrame::Box);
        //p1Header = new QLabel("Not yet implemented",this);
        cosSelectHeader = new QLabel(QString(tr("Select COS to be deleted")));
        cosSelect = new QComboBox();
        QVBoxLayout *vLayout = new QVBoxLayout();

        vLayout->addWidget(header);
        vLayout->addStretch(1);
        vLayout->addWidget(cosSelectHeader);
        vLayout->addWidget(cosSelect);
        vLayout->addStretch(1);
        vLayout->addWidget(deleteButton);
        vLayout->addWidget(closeButton);
        this->setLayout(vLayout);

        //connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
        //connect(startButton,SIGNAL(clicked()),this,SLOT(createCos()));


 }
 //***********************************************************************
/*!Standard Destructor*/
deleteCOSWidget::~deleteCOSWidget()
 {


 }
 //****************************************************
void deleteCOSWidget::deleteCos()
{

    return;
}
//*******************************************************
void deleteCOSWidget::fillSelectCosBox(const QString &cosName)
{
    cosSelect->addItem(cosName);
}
//********************************************************
void deleteCOSWidget::clearCosBox()
{
    cosSelect->clear();
}

