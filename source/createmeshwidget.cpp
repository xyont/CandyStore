/***************************************************************************
 *   Copyright (C) 2011 by Patrick Prokopczuk   *
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
#include "createmeshwidget.h"

createMeshWidget::createMeshWidget(QWidget *parent) :
    QWidget(parent)
{
    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel(" Create Mesh ",this);
    header->setFrameStyle(QFrame::Box);

    sourceHeader = new QLabel("Source Geometry:");
    sourceComboBox = new QComboBox();
    actionHeader = new QLabel("Action:",this);
    actionComboBox= new QComboBox();
    secondOrderCheck =new QCheckBox("Second Order Elements");
    hMaxLabel= new QLabel("max Element Length (hmax):",this);
    hMaxEdit = new QLineEdit(this);

    QVBoxLayout *vLayout = new QVBoxLayout;
    //QHBoxLayout *DispScalLayout = new QHBoxLayout;


    //DispScalLayout->addWidget(DispScalEdit);
    //DispScalLayout->addWidget(DispScalButton);

    vLayout->addWidget(header);
    vLayout->addStretch(1);
    vLayout->addWidget(sourceHeader);
    vLayout->addWidget(sourceComboBox);
    vLayout->addStretch(1);
    vLayout->addWidget(actionHeader);
    vLayout->addWidget(actionComboBox);
    vLayout->addWidget(hMaxLabel);
    vLayout->addWidget(hMaxEdit);
    vLayout->addWidget(secondOrderCheck);

    vLayout->addStretch(1);
    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    actionComboBox->addItem("Mesh 2D ");
    actionComboBox->addItem("Mesh 3D ");

    sourceComboBox->addItem("STL Geometry");
    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));
}
//*******************************
void createMeshWidget::startClicked()
{
    //Check the source (no need at the moment, only stl supported)
    //
    //Check the secondorderBox
    int secondOrder=0;
    double hmax=hMaxEdit->text().toDouble();
    if (hmax<1E-9)
    {
        emit sendMessage("Illegal hmax value");
        return;
    }
    secondOrderCheck->isChecked() ? secondOrder=1 :secondOrder=0;
    //Check the action

    emit startMesh(actionComboBox->currentIndex(),hmax,secondOrder);
}
