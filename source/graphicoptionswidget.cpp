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
 #include "graphicoptionswidget.h"

 /**
 * \file graphicoptionswidget.cpp
 * \brief A brief file description
 **/

 /*! Standard Constructor for the graphic options widget*/
 graphicOptionsWidget::graphicOptionsWidget(QWidget *parent)
        :QWidget(parent)
 {
        closeButton = new QPushButton("Close",this);
        header = new QLabel("  Graphic Options",this);
        header->setFrameStyle(QFrame::Box);
        featureEdgeHeader = new QLabel("Feature Edge Angle",this);
        featureEdgeSlider = new QSlider(Qt::Horizontal);
        featureEdgeSlider->setRange(0,90);
        featureEdgeSlider->setTickInterval(10);
        featureEdgeSlider->setTickPosition(QSlider::TicksBothSides);
        featureEdgeSpinbox = new QSpinBox(this);
        featureEdgeSpinbox->setMinimum(0);
        featureEdgeSpinbox->setMaximum(90);
        setFeatureEdgeAngle(45);
        featureEdgeButton = new QPushButton("Apply",this);
        //DispScalButton->setFixedWidth(50);
        vSpacer = new QSpacerItem(170,10);
        vSpacer2 = new QSpacerItem(170,20);
       /* showUndef =new QCheckBox("Show undeformed structure");
        showUndef->setChecked(true);
        fringeMinHeader = new QLabel("Minimum Fringe Value",this);
        fringeMaxHeader = new QLabel("Maximum Fringe Value",this);
        fringeMinAuto =new QCheckBox("Auto");
        fringeMinAuto->setChecked(true);
        fringeMaxAuto =new QCheckBox("Auto");
        fringeMaxAuto->setChecked(true);
        fringeMinEdit = new QLineEdit(this);
        fringeMinEdit->setReadOnly(TRUE);
        fringeMinButton = new QPushButton("Apply",this);
        fringeMaxEdit = new QLineEdit(this);
        fringeMaxEdit->setReadOnly(TRUE);
        fringeMaxButton = new QPushButton("Apply",this);
*/
        //horizontal line
        QFrame *line = new QFrame(this);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

/*        QFrame *line2 = new QFrame(this);
    line2->setObjectName(QString::fromUtf8("line2"));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
*/
/*
        QFrame *line3 = new QFrame(this);
    line3->setObjectName(QString::fromUtf8("line3"));
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);
    line_2 = new QFrame(Form);
    line_2->setObjectName(QString::fromUtf8("line_2"));
    line_2->setGeometry(QRect(210, 70, 3, 61));
    line_2->setFrameShape(QFrame::VLine);
    line_2->setFrameShadow(QFrame::Sunken);
        */

        QVBoxLayout *vLayout = new QVBoxLayout;
        QHBoxLayout *featureEdgeLayout = new QHBoxLayout;
        //QHBoxLayout *fringeMinLayout = new QHBoxLayout;
        //QHBoxLayout *fringeMaxLayout = new QHBoxLayout;



        featureEdgeLayout->addWidget(featureEdgeSpinbox);
        featureEdgeLayout->addWidget(featureEdgeButton);

        /*fringeMinLayout->addWidget(fringeMinAuto);
        fringeMinLayout->addWidget(fringeMinEdit);
        fringeMinLayout->addWidget(fringeMinButton);

        fringeMaxLayout->addWidget(fringeMaxAuto);
        fringeMaxLayout->addWidget(fringeMaxEdit);
        fringeMaxLayout->addWidget(fringeMaxButton);
*/
        vLayout->addWidget(header);
        //vLayout->addSpacerItem(vSpacer);
        vLayout->addSpacing(10);
        vLayout->addWidget(featureEdgeHeader);
        vLayout->addWidget(featureEdgeSlider);
        vLayout->addLayout(featureEdgeLayout);
        //vLayout->addSpacerItem(vSpacer2);
        vLayout->addSpacing(10);
        vLayout->addWidget(line);
        vLayout->addSpacing(10);
    /*    vLayout->addWidget(fringeMinHeader);
        vLayout->addLayout(fringeMinLayout);
        vLayout->addSpacing(10);
        vLayout->addWidget(fringeMaxHeader);
        vLayout->addLayout(fringeMaxLayout);
        vLayout->addSpacing(10);
        vLayout->addWidget(line2);
        vLayout->addSpacing(10);
        vLayout->addWidget(showUndef);*/
        vLayout->addStretch(1);
        vLayout->addWidget(closeButton);
        this->setLayout(vLayout);

        connect(featureEdgeSlider,SIGNAL(valueChanged(int)),featureEdgeSpinbox,SLOT(setValue(int)));
        connect(featureEdgeSpinbox,SIGNAL(valueChanged(int)),featureEdgeSlider,SLOT(setValue(int)));
        connect(featureEdgeButton,SIGNAL(clicked()),this,SLOT(newFeatureEdgeAngle()));
        /*connect(fringeMinButton,SIGNAL(clicked()),this,SLOT(newFringeMin()));
        connect(fringeMaxButton,SIGNAL(clicked()),this,SLOT(newFringeMax()));
        connect(showUndef, SIGNAL(toggled(bool)), this , SLOT(showUndeformedChanged(bool)));*/

 }


 graphicOptionsWidget::~graphicOptionsWidget()
 {
        //delete vSpacer;

 }

 /*! Gets the value of the feature edge slider and emits a signal */
 void graphicOptionsWidget::newFeatureEdgeAngle()
 {
        emit featureEdgeAngleChanged(featureEdgeSpinbox->value());
 }
 /*! Sets the line edit for the min fringe value ReadOnly Status */
 void graphicOptionsWidget::setFringeMinEditable(bool edit)
 {
        fringeMinEdit->setReadOnly(edit);
 }
 /*! Sets the line edit for the max fringe value ReadOnly Status */
 void graphicOptionsWidget::setFringeMaxEditable(bool edit)
 {
        fringeMaxEdit->setReadOnly(edit);
 }
 //************************************************************************************************
 /*! Checks the state of the auto-checkbox and either emits a autofringe signal or the value of the corresponding lineedit*/
 void graphicOptionsWidget::newFringeMin()
 {
        if (fringeMinAuto->isChecked())
        {
                emit setFringeMinAuto();
        }
        else
        {
                bool ok;
                double value=fringeMinEdit->text().toDouble(&ok);
                if (ok==true)
                {
                        emit setFringeMin(value);
                        return;
                }
                emit sendMessage("Illegal value");
        }
}
 //***************************************************************************************
/*! Checks the state of the auto-checkbox and either emits a autofringe signal or the value of the corresponding lineedit */
 void graphicOptionsWidget::newFringeMax()
 {
        if (fringeMaxAuto->isChecked())
        {
                emit setFringeMaxAuto();
        }
        else
        {
        bool ok;
                double value=fringeMaxEdit->text().toDouble(&ok);
                if (ok==true)
                {
                        emit setFringeMax(value);
                        return;
                }
                emit sendMessage("Illegal value");
        }
 }
 //******************************************************************************************
 /*!  Picks up the status of the undeformed mesh(show/hide) and emits a signal with the status */

 void graphicOptionsWidget::showUndeformedChanged(bool status)
 {

        emit setShowUndeformedMesh(status);

 }
//*****************************************************************************************
/*! Sets the display values in the slider and spinbox for the feature edge to value <angle>*/
void graphicOptionsWidget::setFeatureEdgeAngle(int angle)
 {
        featureEdgeSlider->setValue(angle);
        featureEdgeSpinbox->setValue(angle);
 }

