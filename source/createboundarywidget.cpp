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
#include "createboundarywidget.h"

/*! Standard Constructor*/
createBoundaryWidget::createBoundaryWidget(QWidget *parent)
       :QWidget(parent)
{
    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel("Boundary Conditions ",this);
    header->setFrameStyle(QFrame::Box);
    nameLabel = new QLabel("Name:",this);
    firstDOFLabel = new QLabel("First DOF:",this);
    lastDOFLabel = new QLabel("Last DOF:",this);
    valueLabel = new QLabel("Value:",this);
    targetSetHeader = new QLabel("Target set:",this);
    targetSetComboBox= new QComboBox();
    firstDOF = new QLineEdit(this);
    lastDOF = new QLineEdit(this);
    name =  new QLineEdit(this);
    value =  new QLineEdit(this);
    //DispScalButton = new QPushButton("Select",this);
    //DispScalButton->setFixedWidth(50);
    lowerNodes=new QCheckBox("Keep lower node IDs");
    higherNodes=new QCheckBox("Keep higher node IDs");
    //allNodes->setCheckState(Qt::Checked);
    //outsideFaces=new QCheckBox("outside Faces");
    //xEdit = new QLineEdit(this);
    //yEdit = new QLineEdit(this);
    //zEdit = new QLineEdit(this);


    QVBoxLayout *vLayout = new QVBoxLayout;
    //QHBoxLayout *DispScalLayout = new QHBoxLayout;


    //DispScalLayout->addWidget(DispScalEdit);
    //DispScalLayout->addWidget(DispScalButton);

    vLayout->addWidget(header);
    vLayout->addStretch(1);
    vLayout->addWidget(nameLabel);
    vLayout->addWidget(name);
    vLayout->addWidget(targetSetHeader);
    vLayout->addWidget(targetSetComboBox);
    vLayout->addWidget(firstDOFLabel);
    vLayout->addWidget(firstDOF);
    vLayout->addWidget(lastDOFLabel);
    vLayout->addWidget(lastDOF);
    vLayout->addWidget(valueLabel);
    vLayout->addWidget(value);
    vLayout->addStretch(1);
    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));


}

//****************************************************
/*!Check values and create the boundary condition */
void createBoundaryWidget::startClicked()
{
    if (name->text()=="")
    {
        emit sendMessage(QString("A name must be specified!"));
        return;
    }
    emit startBCCreation(name->text(),targetSetComboBox->currentText(),firstDOF->text().toInt(),lastDOF->text().toInt(),value->text().toDouble());
}

// **********************************************************

// **********************************************************
//***********************************************************************
/*!Standard Destructor*/
createBoundaryWidget::~createBoundaryWidget()
{


}
