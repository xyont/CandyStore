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
 #include "interpolateOptionsWidget.h"
 
 /**
 * \file resultOptionsWidget.cpp
 * \brief A brief file description
 **/
 
 /*! Standard Constructor*/ 
 interpolateOptionsWidget::interpolateOptionsWidget(QWidget *parent)
    	:QWidget(parent)
 {
	closeButton = new QPushButton("Close",this);
	startButton=new QPushButton("Start Interpolation",this);
	header = new QLabel(" Interpolate Results",this);
	header->setFrameStyle(QFrame::Box);
	ResultFileHeader = new QLabel("Result File",this);
	targetSelectHeader = new QLabel("Interpolate on:",this);
	DispScalEdit = new QLineEdit(this);
	DispScalButton = new QPushButton("Select",this);
	DispScalButton->setFixedWidth(50);
	allNodes=new QCheckBox("all Nodes");
	allNodes->setCheckState(Qt::Checked);
	outsideFaces=new QCheckBox("outside Faces");
        outsideResults= new QCheckBox("Use only outside results");
        ResultFileTypeHeader= new QLabel("Interpolate from:");
        resultFileType=new QComboBox(this);
        resultFileType->addItem("frd file ");
        resultFileType->addItem("unstructured vtk file ");
        resultFileType->addItem("multiblock vtk file ");
	
	
	QVBoxLayout *vLayout = new QVBoxLayout;
	QHBoxLayout *DispScalLayout = new QHBoxLayout;
	
	
	DispScalLayout->addWidget(DispScalEdit);
	DispScalLayout->addWidget(DispScalButton);
	
	vLayout->addWidget(header);
	vLayout->addStretch(1);
        vLayout->addWidget(ResultFileTypeHeader);
        vLayout->addWidget(resultFileType);
	vLayout->addWidget(ResultFileHeader);
	vLayout->addLayout(DispScalLayout);
        vLayout->addWidget(outsideResults);
	vLayout->addWidget(targetSelectHeader);
	vLayout->addWidget(allNodes);
	vLayout->addWidget(outsideFaces);
	vLayout->addStretch(1);
	vLayout->addWidget(startButton);
	vLayout->addWidget(closeButton);
	this->setLayout(vLayout);
	
	connect(DispScalButton,SIGNAL(clicked()),this,SLOT(changeResultFile()));
	connect(startButton,SIGNAL(clicked()),this,SLOT(startInterpol()));

	
 }
 //***********************************************************************
void interpolateOptionsWidget::changeResultFile()
{
	if (lastInterpolDirectory=="")
	 {
            switch (resultFileType->currentIndex())
            {
            case 0:
                resultFile= QFileDialog::getOpenFileName(this, tr("Select Result File"),"./","frd files (*.frd);;All files (*.*)");
            break;
            case 1:
                resultFile= QFileDialog::getOpenFileName(this, tr("Select Result File"),"./","vtu files (*.vtu);;All files (*.*)");
            break;
            case 2:
                resultFile= QFileDialog::getOpenFileName(this, tr("Select Result File"),"./","vtm files (*.vtm);;vtmb files (*.vtmb);;All files (*.*)");
            break;
            }
        }
	else
	 {
            switch (resultFileType->currentIndex())
            {
            case 0:
		resultFile = QFileDialog::getOpenFileName(this, tr("Select Result File"),lastInterpolDirectory,"frd files (*.frd);;All files (*.*)");
            break;
            case 1:
                resultFile = QFileDialog::getOpenFileName(this, tr("Select Result File"),lastInterpolDirectory,"vtu files (*.vtu);;All files (*.*)");
            break;
            case 2:
                resultFile = QFileDialog::getOpenFileName(this, tr("Select Result File"),lastInterpolDirectory,"vtm files (*.vtm);;vtmb files (*.vtmb);;All files (*.*)");
            break;
            }
        }
	lastInterpolDirectory=QFileInfo(resultFile).absolutePath();
	DispScalEdit->setText(resultFile);
}
//**************************************************************************
void interpolateOptionsWidget::startInterpol()
{
	QString filename;
	filename=DispScalEdit->text();
	if (!filename.isEmpty())
	{
		
		emit sendMessage(QString("Starting Interpolation from file %1").arg(resultFile));
		if (allNodes->isChecked())
                {


                    switch (resultFileType->currentIndex())
                    {
                    case 0:
                        emit startInterpolation(resultFile);
                    break;
                    case 1:
                        emit startInterpolationVtkUnstructured(resultFile);
                    break;;
                    break;
                    case 2:
                        emit startInterpolationVtkMultiblock(resultFile);
                    break;
                    }
                }
		if (outsideFaces->isChecked())
                {
                    switch (resultFileType->currentIndex())
                    {
                    case 0:
                        emit startFaceInterpolation(resultFile);
                    break;
                    case 1:
                        emit startFaceInterpolationVtkUnstructured(resultFile);
                    break;
                    case 2:
                        if (outsideResults->isChecked())
                        {
                            emit startFaceInterpolationVtkMultiblock(resultFile,true);
                        }
                        else
                        {
                             emit startFaceInterpolationVtkMultiblock(resultFile,false);
                        }

                    break;
                    }
                }
        }
}

 interpolateOptionsWidget::~interpolateOptionsWidget()
 {
	//delete vSpacer;
 
 }
 
