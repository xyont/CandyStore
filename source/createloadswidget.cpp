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
#include "createloadswidget.h"

createLoadsWidget::createLoadsWidget(QWidget *parent, femodel *fe) :
    QWidget(parent)
{
    feData= fe;
    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel("Loads: ",this);
    header->setFrameStyle(QFrame::Box);
    nameLabel = new QLabel("Name: ",this);
    directionLabel = new QLabel("Direction :",this);
    rotAxisHeader = new QLabel("Rotation Axis:",this);
    rotAxisHeader->hide();
    point1Header = new QLabel("Point 1 on Axis (x,y,z):",this);
    point1Header->hide();
    point2Header = new QLabel("Point 2 on Axis (x,y,z):",this);
    point2Header->hide();
    valueLabel = new QLabel("Value:",this);
    targetSetHeader = new QLabel("Target node set:",this);
    targetSetComboBox= new QComboBox();
    typeHeader = new QLabel("Load Type:",this);
    typeComboBox= new QComboBox();
    direction = new QLineEdit(this);
    //lastDOF = new QLineEdit(this);
    name =  new QLineEdit(this);
    value =  new QLineEdit(this);
    //DispScalButton = new QPushButton("Select",this);
    //DispScalButton->setFixedWidth(50);
    //lowerNodes=new QCheckBox("Keep lower node IDs");
    //higherNodes=new QCheckBox("Keep higher node IDs");
    //allNodes->setCheckState(Qt::Checked);
    //outsideFaces=new QCheckBox("outside Faces");
    point1Edit = new QLineEdit(this);
    point2Edit = new QLineEdit(this);
    point1Edit->hide();
    point2Edit->hide();



    QVBoxLayout *vLayout = new QVBoxLayout;
    //QHBoxLayout *DispScalLayout = new QHBoxLayout;


    //DispScalLayout->addWidget(DispScalEdit);
    //DispScalLayout->addWidget(DispScalButton);

    vLayout->addWidget(header);
    vLayout->addStretch(1);
    vLayout->addWidget(typeHeader);
    vLayout->addWidget(typeComboBox);
    vLayout->addStretch(1);
    vLayout->addWidget(nameLabel);
    vLayout->addWidget(name);
    vLayout->addWidget(targetSetHeader);
    vLayout->addWidget(targetSetComboBox);

    vLayout->addStretch(1);

    vLayout->addWidget(rotAxisHeader);
    vLayout->addWidget(point1Header);
    vLayout->addWidget(point1Edit);
    vLayout->addWidget(point2Header);
    vLayout->addWidget(point2Edit);
    vLayout->addWidget(directionLabel);
    vLayout->addWidget(direction);
    vLayout->addStretch(1);
    vLayout->addWidget(valueLabel);
    vLayout->addWidget(value);


    vLayout->addStretch(1);
    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    typeComboBox->addItem("Force (CLOAD)");
    typeComboBox->addItem("Pressure (DLOAD)");
    typeComboBox->addItem("Centrifugal (DLOAD)");


    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));
    connect(typeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(loadTypeChanged(int)));

}

//****************************************************
/*!Check values and create the load */
void createLoadsWidget::startClicked()
{

    if (name->text()=="")
    {
        emit sendMessage(QString("A name must be specified! Load Creation aborted!"));
        return;
    }
    if (typeComboBox->currentIndex()<2)
    {
        emit startLoadCreation(name->text(),typeComboBox->currentIndex(),targetSetComboBox->currentText(),
                           value->text().toDouble(),direction->text().toInt());
    }
    else
    {
         emit startCentrifugalLoadCreation(name->text(),targetSetComboBox->currentText(),point1Edit->text(),point2Edit->text(),
                                           value->text().toDouble());
    }
}
//****************************************************
/*!Check values and create the load */
void createLoadsWidget::loadTypeChanged(int type)
{
    switch (type)
    {
    case 0:
        targetSetHeader->setText("Target node set");
        valueLabel->setText("Value:");
        directionLabel->show();
        direction->show();
        rotAxisHeader->hide();
        point1Header->hide();
        point2Header->hide();
        point1Edit->hide();
        point2Edit->hide();

    break;
    case 1:
        targetSetHeader->setText("Target face set");
        valueLabel->setText("Value:");
        directionLabel->hide();
        direction->hide();
        rotAxisHeader->hide();
        point1Header->hide();
        point2Header->hide();
        point1Edit->hide();
        point2Edit->hide();
    break;
    case 2:
        targetSetHeader->setText("Target element set");
        valueLabel->setText("RPM [1/min]:");
        directionLabel->hide();
        direction->hide();
        rotAxisHeader->show();
        point1Header->show();
        point2Header->show();
        point1Edit->show();
        point2Edit->show();
    break;
    }
}
