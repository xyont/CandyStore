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
 #include "resultOptionsWidget.h"
 
 /**
 * \file resultOptionsWidget.cpp
 * \brief A brief file description
 **/
 
 /*! Standard Constructor for the result options widget*/
 resultOptionsWidget::resultOptionsWidget(QWidget *parent)
    	:QWidget(parent)
 {
	closeButton = new QPushButton("Close",this);
	header = new QLabel("  Result Options",this);
	header->setFrameStyle(QFrame::Box);
	DispScalHeader = new QLabel("Disp. Scale Factor",this);
	DispScalEdit = new QLineEdit(this);
	DispScalButton = new QPushButton("Apply",this);
	DispScalButton->setFixedWidth(50);
	vSpacer = new QSpacerItem(170,10);
	vSpacer2 = new QSpacerItem(170,20);
	showUndef =new QCheckBox("Show undeformed structure");
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
	
	//horizontal line
	QFrame *line = new QFrame(this);
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
	
	QFrame *line2 = new QFrame(this);
    line2->setObjectName(QString::fromUtf8("line2"));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
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
	QHBoxLayout *DispScalLayout = new QHBoxLayout;
	QHBoxLayout *fringeMinLayout = new QHBoxLayout;
	QHBoxLayout *fringeMaxLayout = new QHBoxLayout;
	
	
	
	DispScalLayout->addWidget(DispScalEdit);
	DispScalLayout->addWidget(DispScalButton);
	
	fringeMinLayout->addWidget(fringeMinAuto);
	fringeMinLayout->addWidget(fringeMinEdit);
	fringeMinLayout->addWidget(fringeMinButton);
	
	fringeMaxLayout->addWidget(fringeMaxAuto);
	fringeMaxLayout->addWidget(fringeMaxEdit);
	fringeMaxLayout->addWidget(fringeMaxButton);
	
	vLayout->addWidget(header);
	//vLayout->addSpacerItem(vSpacer);
	vLayout->addSpacing(10);
	vLayout->addWidget(DispScalHeader);
	vLayout->addLayout(DispScalLayout);
	//vLayout->addSpacerItem(vSpacer2);
	vLayout->addSpacing(10);
	vLayout->addWidget(line);
	vLayout->addSpacing(10);
	vLayout->addWidget(fringeMinHeader);
	vLayout->addLayout(fringeMinLayout);
	vLayout->addSpacing(10);
	vLayout->addWidget(fringeMaxHeader);
	vLayout->addLayout(fringeMaxLayout);
	vLayout->addSpacing(10);
	vLayout->addWidget(line2);
	vLayout->addSpacing(10);
	vLayout->addWidget(showUndef);
	vLayout->addStretch(1);
	vLayout->addWidget(closeButton);
	this->setLayout(vLayout);
	
	connect(DispScalButton,SIGNAL(clicked()),this,SLOT(newDispScal()));
	connect(fringeMinAuto,SIGNAL(clicked(bool)),this,SLOT(setFringeMinEditable(bool)));
	connect(fringeMaxAuto,SIGNAL(clicked(bool)),this,SLOT(setFringeMaxEditable(bool)));
	connect(fringeMinButton,SIGNAL(clicked()),this,SLOT(newFringeMin()));
	connect(fringeMaxButton,SIGNAL(clicked()),this,SLOT(newFringeMax()));
	connect(showUndef, SIGNAL(toggled(bool)), this , SLOT(showUndeformedChanged(bool)));
	
 }


 resultOptionsWidget::~resultOptionsWidget()
 {
	//delete vSpacer;
 
 }
 
 /*! Gets the value of the displacement scale factor line edit and emits a signal with the new  value*/
 void resultOptionsWidget::newDispScal()
 {
	bool ok;
	float value=DispScalEdit->text().toFloat(&ok);
				if (ok==true)
				{
					//viewingArea->setDispScale(value);
					emit sendMessage(QString("Displacement Scale Factor set to %1").arg(DispScalEdit->text()));
					emit dispScalChanged(value);
					return;
				}
				emit sendMessage("Illegal value");
	
 }
 /*! Sets the line edit for the min fringe value ReadOnly Status */
 void resultOptionsWidget::setFringeMinEditable(bool edit)
 {
	fringeMinEdit->setReadOnly(edit);
 }
 /*! Sets the line edit for the max fringe value ReadOnly Status */
 void resultOptionsWidget::setFringeMaxEditable(bool edit)
 {
	fringeMaxEdit->setReadOnly(edit);
 }
 /*! Checks the state of the auto-checkbox and either emits a autofringr signal or the value of the corresponding lineedit*/  
 void resultOptionsWidget::newFringeMin()
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
/*! Checks the state of the auto-checkbox and either emits a autofringe signal or the value of the corresponding lineedit */ 
 void resultOptionsWidget::newFringeMax()
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
 /*!  Picks up the status of the undeformed mesh(show/hide) and emits a signal with the status */

 void resultOptionsWidget::showUndeformedChanged(bool status)
 {

	emit setShowUndeformedMesh(status);
	
 }

/*!  Toggles the show undeformed  checkbox and emits a signal with the status of the checkbox*/ 
void resultOptionsWidget::toggleShowUndeformedMesh()
 {
	if (showUndef->isChecked())
	{
		showUndef->setChecked(false);
		emit setShowUndeformedMesh(false);
	}
	else
	{
		showUndef->setChecked(true);
		emit setShowUndeformedMesh(true);
	}
 }
