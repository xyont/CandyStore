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
 #include "mergeNodesWidget.h"

 /**
 * \file mergeNodesWidget.cpp
 * \brief A brief file description
 **/

 /*! Standard Constructor*/
mergeNodesWidget::mergeNodesWidget(QWidget *parent)
        :QWidget(parent)
 {
        closeButton = new QPushButton("Close",this);
        startButton=new QPushButton("Apply",this);
        header = new QLabel("Merge Nodes ",this);
        header->setFrameStyle(QFrame::Box);
        coordinateHeader = new QLabel("Tolerance:",this);
        targetSetHeader = new QLabel("Target set:",this);
        targetSetComboBox= new QComboBox();
        //DispScalEdit = new QLineEdit(this);
        //DispScalButton = new QPushButton("Select",this);
        //DispScalButton->setFixedWidth(50);
        lowerNodes=new QCheckBox("Keep lower node IDs");
        higherNodes=new QCheckBox("Keep higher node IDs");
        //allNodes->setCheckState(Qt::Checked);
        //outsideFaces=new QCheckBox("outside Faces");
        xEdit = new QLineEdit(this);
        //yEdit = new QLineEdit(this);
        //zEdit = new QLineEdit(this);


        QVBoxLayout *vLayout = new QVBoxLayout;
        //QHBoxLayout *DispScalLayout = new QHBoxLayout;


        //DispScalLayout->addWidget(DispScalEdit);
        //DispScalLayout->addWidget(DispScalButton);

        vLayout->addWidget(header);
        vLayout->addStretch(1);
        vLayout->addWidget(targetSetHeader);
        vLayout->addWidget(targetSetComboBox);
        vLayout->addWidget(coordinateHeader);
        vLayout->addWidget(xEdit);
        vLayout->addWidget(lowerNodes);
        vLayout->addWidget(higherNodes);
        vLayout->addStretch(1);
        vLayout->addWidget(startButton);
        vLayout->addWidget(closeButton);
        this->setLayout(vLayout);

        //connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
        connect(startButton,SIGNAL(clicked()),this,SLOT(startMerge()));
        connect(lowerNodes,SIGNAL(toggled(bool)),this,SLOT(toggleKeepNodesLow(bool)));
        connect(higherNodes,SIGNAL(toggled(bool)),this,SLOT(toggleKeepNodesHigh(bool)));
        modeFlag=0;
        lowerNodes->setChecked(true);
        higherNodes->setChecked(false);

 }
 //***********************************************************************
/*!Standard Destructor*/
mergeNodesWidget::~mergeNodesWidget()
 {


 }
 //****************************************************
/*!Evaluates the merge tolerance and start merge */
void mergeNodesWidget::startMerge()
 {
        bool ok;//,ok2,ok3;
        double tol=xEdit->text().toDouble(&ok);


        if (ok==true)
        {
                emit sendMessage(QString("Merging close Nodes (Tolerance=%1) in set %2").arg(tol).arg(targetSetComboBox->currentText()));
                if (targetSetComboBox->currentIndex()==0)
                {
                    emit mergeNodes(tol,modeFlag);

                }
                else
                {
                    emit mergeNodesSet(tol,modeFlag,targetSetComboBox->currentIndex()-1);
                }
        }
        else
                emit sendMessage("Illegal tolerance value");
        return;
 }
// **********************************************************
  /*! Toggles the keep nodes checkboxes*/
void mergeNodesWidget::toggleKeepNodesLow(bool checked)
{
    if (checked==true)
    {
        higherNodes->setChecked(false);
         modeFlag=0;
}
else
    {
        higherNodes->setChecked(true);
        modeFlag=1;
        }
}
// **********************************************************
  /*! Toggles the keep nodes checkboxes*/
void mergeNodesWidget::toggleKeepNodesHigh(bool checked)
{
    if (checked==true)
    {
        lowerNodes->setChecked(false);
        modeFlag=1;
    }
else
        lowerNodes->setChecked(true);
        modeFlag=0;
    }
