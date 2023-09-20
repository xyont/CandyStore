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
#include "verifyelementswidget.h"

verifyElementsWidget::verifyElementsWidget(QWidget *parent) :
        QWidget(parent)
{
    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel(" Verify Elements ",this);
    header->setFrameStyle(QFrame::Box);
    /*nameLabel = new QLabel("Name:",this);
    firstDOFLabel = new QLabel("Young's Modulus:",this);
    lastDOFLabel = new QLabel("Poisson's Ratio:",this);
    valueLabel = new QLabel("Density:",this);
    */
    actionHeader = new QLabel("Action:",this);
    actionComboBox= new QComboBox();
    /*
    name =  new QLineEdit(this);
    firstDOF = new QLineEdit(this);
    lastDOF = new QLineEdit(this);
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
    */

    QVBoxLayout *vLayout = new QVBoxLayout;
    //QHBoxLayout *DispScalLayout = new QHBoxLayout;


    //DispScalLayout->addWidget(DispScalEdit);
    //DispScalLayout->addWidget(DispScalButton);

    vLayout->addWidget(header);
    vLayout->addStretch(1);
    //vLayout->addWidget(nameLabel);
    //vLayout->addWidget(name);
    vLayout->addWidget(actionHeader);
    vLayout->addWidget(actionComboBox);
    /*vLayout->addWidget(firstDOFLabel);
    vLayout->addWidget(firstDOF);
    vLayout->addWidget(lastDOFLabel);
    vLayout->addWidget(lastDOF);
    vLayout->addWidget(valueLabel);
    vLayout->addWidget(value);
    */
    vLayout->addStretch(1);
    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    actionComboBox->addItem("Expansion Ratio 2D ");
    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));

}
//***************************************************
void verifyElementsWidget::startClicked()
{
    int index=actionComboBox->currentIndex();

    switch (index)
    {
    case 0:
        emit sendMessage("Checking 2D Expansion Ratio");
        emit erat2D();
    break;
    }
}
