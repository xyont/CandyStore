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
/**
 * \file 3d-modul.cpp
 * \brief Class methods of the 3d-modul (Visualization)
 **/

#include "3d-modul.h"
#include <QtGui>
/*!Standard Constructor creates a new 3dwidget with a viewing area, a model tree, a command line and a message area*/
   Modul3D::Modul3D(QWidget *parent, femodel *md,geomodel *geom)
    	:QWidget(parent)
    {
	geoData = geom;
    feData =md;

    varioWidget= new QStackedWidget(this);
	varioWidget->setFixedWidth(180);
	
    mainArea = new QStackedWidget(this);

    matWidget=new materialWidget(this);


	resultOptions = new resultOptionsWidget(this);
	resultOptions->setFixedWidth(180);

        graphicOptions = new graphicOptionsWidget(this);
        graphicOptions->setFixedWidth(180);
	
	interpolOptions = new interpolateOptionsWidget(this);
	interpolOptions->setFixedWidth(180);
	
	createPoint = new createPointWidget(this);
	createPoint->setFixedWidth(180);
	
	createCurve = new createCurveWidget(this);
	createCurve->setFixedWidth(180);
	
	createCOS = new createCOSWidget(this);
	createCOS->setFixedWidth(180);

    createBC = new createBoundaryWidget(this);
    createBC->setFixedWidth(180);

        deleteCOS = new deleteCOSWidget(this);
        deleteCOS->setFixedWidth(180);

        mergeNodes = new mergeNodesWidget(this);
        mergeNodes->setFixedWidth(180);

        createMat = new createMaterialWidget(this);
        createMat->setFixedWidth(180);

        createFeSect = new createFeSectionWidget(this);
        createFeSect->setFixedWidth(180);

        verifyElementView = new verifyElementsWidget(this);
        verifyElementView->setFixedWidth(180);

        createMeshView = new createMeshWidget(this);
        createMeshView->setFixedWidth(180);

        createLoadsView = new createLoadsWidget(this,feData);
        createLoadsView->setFixedWidth(180);

        createDiscView = new createDiscWidget(this);
        createDiscView->setFixedWidth(180);

        fileImportView = new fileImportWidget(this);
        fileImportView->setFixedWidth(180);
	
	modelTree = new modelTreeView(this,md,geoData);
	modelTree->setFixedWidth(180);
	
	varioWidget->addWidget(modelTree);
	varioWidget->addWidget(resultOptions);
        varioWidget->addWidget(graphicOptions);
	varioWidget->addWidget(interpolOptions);
	varioWidget->addWidget(createPoint);
	varioWidget->addWidget(createCurve);
	varioWidget->addWidget(createCOS);
    varioWidget->addWidget(createBC);
        varioWidget->addWidget(deleteCOS);
        varioWidget->addWidget(mergeNodes);
        varioWidget->addWidget(createMat);
        varioWidget->addWidget(createFeSect);
       varioWidget->addWidget(verifyElementView);
       varioWidget->addWidget(createMeshView);
       varioWidget->addWidget(createLoadsView);
       varioWidget->addWidget(createDiscView);
       varioWidget->addWidget(fileImportView);



        varioWidget->installEventFilter(this);

	
        infoBox = new modelInfoView(this, md, geoData);
	infoBox->setFixedSize(180,120);
	
	//
	cLine = new commandLine();
	
	viewingArea = new GLViewer2(md,geom);
        viewingArea->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        viewingArea->installEventFilter(this);
	//
	messages = new QTextEdit();
	messages->setFixedHeight(45);
	messages->setReadOnly(true);
	//messages->setStyleSheet( "background-color: black; color: green;" );
	//
        //THis must e replaced in future with an event filter for the 3d widget!!
        //cLine->grabKeyboard();
	
	connect(cLine, SIGNAL(returnPressed()), this, SLOT(commandReceived()));
	connect(resultOptions, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
	connect(resultOptions, SIGNAL(dispScalChanged(float)), viewingArea , SLOT(setDispScale(float)));

	connect(resultOptions, SIGNAL(setShowUndeformedMesh(bool)), viewingArea , SLOT(toggleShowUndef(bool)));
	connect(this, SIGNAL(toggleUndeformed()),resultOptions,SLOT(toggleShowUndeformedMesh()));
	//
	connect(resultOptions, SIGNAL(setFringeMinAuto()), viewingArea , SLOT(setFringeMinAuto()));
	connect(resultOptions, SIGNAL(setFringeMaxAuto()), viewingArea , SLOT(setFringeMaxAuto()));
	connect(resultOptions, SIGNAL(setFringeMin(double)), viewingArea , SLOT(setFringeMin(double)));
	connect(resultOptions, SIGNAL(setFringeMax(double)), viewingArea , SLOT(setFringeMax(double)));
	connect(resultOptions->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
	//
        connect(graphicOptions->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
        connect(graphicOptions,SIGNAL(featureEdgeAngleChanged(int)),viewingArea,SLOT(setFeatureEdgeAngle(int)));
        //
	connect(interpolOptions->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
	connect(interpolOptions, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));

	//
	connect(createPoint->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
	connect(createPoint, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));

	//
	connect(createCurve->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
	connect(createCurve, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
	//
	connect(createCOS->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
	connect(createCOS, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
	//
    connect(mergeNodes->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(mergeNodes, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(createBC->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createBC, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(createMat->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createMat, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(createFeSect->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createFeSect, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(verifyElementView->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(verifyElementView, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(createMeshView->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createMeshView, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    connect(createMeshView, SIGNAL(startMesh(int,double,int)), this, SLOT(startMeshing(int,double,int)));
    //
    connect(createLoadsView->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createLoadsView, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(createDiscView->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(createDiscView, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    connect(createDiscView, SIGNAL(createPoint(double,double,double,int)), geoData , SLOT(createPoint(double,double,double,int)));
    connect(createDiscView, SIGNAL(createCurve(long,long)), geoData , SLOT(createCurve(long,long)));

    connect(fileImportView->closeButton, SIGNAL(clicked()), this, SLOT(showTree()));
    connect(fileImportView, SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
	connect(this->modelTree, SIGNAL(planeSelected(long)), this->infoBox, SLOT(showPlaneInformation(long)));
	connect(geoData,SIGNAL(geoDatabaseModified()),modelTree,SLOT(updateTree()));
    //
    connect(matWidget->closeButton, SIGNAL(clicked()), this, SLOT(show3d()));
    connect(matWidget,SIGNAL(sendMessage(const QString)), this , SLOT(recieveMessage(const QString)));
    //
    connect(this,SIGNAL(nodeSelection()),viewingArea,SLOT(selectNode()));
    connect(this,SIGNAL(faceSelection()),viewingArea,SLOT(selectFace()));
    connect(this,SIGNAL(elementSelection()),viewingArea,SLOT(selectElement()));
    //
    connect(this,SIGNAL(setSelectionMode(int)),viewingArea,SLOT(setSelectionMode(int)));
    connect(this,SIGNAL(updateView()),viewingArea,SLOT(updateView()));
    connect(this,SIGNAL(RPressed()),viewingArea,SLOT(RPressed()));
    connect(this,SIGNAL(updateSets()),viewingArea,SLOT(updateNSetDisplayLists()));
    //
    connect(this,SIGNAL(sendMessage(QString)),this,SLOT(recieveMessage(QString)));
    //
    connect(this, SIGNAL(toggleOctree()),viewingArea, SLOT(toggleOctreeFlag()));
	mainLayout = new QHBoxLayout();
	leftLayout = new QVBoxLayout();
	midLayout = new QVBoxLayout();
	//rightLayout = new QVBoxLayout();
	
	leftLayout->addWidget(varioWidget);
	leftLayout->addWidget(infoBox);
	

    mainArea->addWidget(viewingArea);
    mainArea->addWidget(matWidget);

    midLayout->addWidget(mainArea);
	midLayout->addWidget(messages);
	midLayout->addWidget(cLine);
	
	//rightLayout->addWidget(setManager);
	
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(midLayout);
	
	this->setLayout(mainLayout);
        viewingArea->setFocus();
	messages->append("CandyStore opened succesfully !");
	
}
//*******************************************************************************************
/*! Sets the background color*/
void Modul3D::setBgColor(const QColor &bgc)
	{
		//viewingArea->setBackgroundColor(bgc);
		//viewingArea->updateGL();
	}
//****************************************************************************************
/*! Get commands from the commandLine */
	void Modul3D::commandReceived()
	{
        //Getting the string from the command line
        QString text=cLine->text();
        if (text.size()>0)
        {
            messages->append(text);
            evaluateCommand(text);
        }
   }
//****************************************************************************************
/*! Evaluate commands from the commandLine */
        void Modul3D::evaluateCommand(const QString& text)
        {
        bool ok;//,ok2,ok3;
        //bool setexist=false;
        int setIndex=-1;

		QString message;
		QString command=text.section(' ',0,0);
        //*************************
		cLine->clear();
        //Running a script (fbd) file
        if (command=="run")
        {
           //Check wether a file has been specified
           if (text.section(' ',1,1).size()>0)
           {
                runScript(text.section(' ',1,1));
           }
           else
           {
               messages->append("No file specified!");
           }
           return;
        }
        //**************************
        //Scaling displacements
                if (command=="scal")
		{
			QString option=text.section(' ',1,1);
			if (option=="d")
			{
				float value=text.section(' ',2,2).toFloat(&ok);
				if (ok==true)
				{
					viewingArea->setDispScale(value);
					message="Displacement Scale Factor set to" + text.section(' ',2,2);
					messages->append(message);
					return;
				}
				messages->append("Illegal value");
				return;
			}
			messages->append("Unknown Option");
			return;
		}	
		//*************
		//Command for creating a plane
		if (command=="pln")
		{
			messages->append("Command for creating a new plane");
			return;
		}
		//**********************************
        //Toggle the undeformed structure on off
		if (command=="udef")
		{
			emit toggleUndeformed();
			return;
		}
                //************************************
                //Moving the model around (translation, rotation,scaling)
                if (command=="mov")
                {
                    QString option=text.section(' ',1,1);
                    if (option=="scal")
                    {
                        float value=text.section(' ',2,2).toFloat(&ok);
                        if (ok==true)
                        {
                                emit scaleModel(value);
                                message="Model scaled by factor" + text.section(' ',2,2);
                                messages->append(message);
                                return;
                        }
                        messages->append("Illegal value");
                        return;
                    }
                    messages->append("Unknown Option");
                    return;
                }
		//*****************************
		//Command for creating a new point
		if (command=="pnt")
		{
            evaluatePNT(text);
			return;
		}
                //**************************************************
		//Command for creating a new sraight line 
		if (command=="line")
		{
            evaluateLINE(text);
            return;
		}
                /*!Command for creating a new line by picking points (Calculix style)*/
                if (command=="qlin")
                {
                    messages->append("Select start-point by pressing b over a point, end-point by pressing g");
                    messages->append("Not yet implemeneted");
                    emit qlin();
                }
                //**************
                //Setting a new max fringe value
		if (command=="max")
		{
			if (text.section(' ',1,1).size()==0)
			{
				viewingArea->setFringeMaxAuto();
				return;
			}
			float value=text.section(' ',1,1).toFloat(&ok);
			if (ok==true)
				{
					viewingArea->setFringeMax(value);
					return;
				}
				messages->append("Illegal value");
				return;
		}
		//******************
                //Setting a new min fringe value
		if (command=="min")
		{
			if (text.section(' ',1,1).size()==0)
			{
				viewingArea->setFringeMinAuto();
				return;
			}
			float value=text.section(' ',1,1).toFloat(&ok);
			if (ok==true)
				{
					viewingArea->setFringeMin(value);
					return;
				}
				messages->append("Illegal value");
				return;
		}
		//***********************
                //Exporting model data to a file
		if (command=="send")
		{
			if (text.section(' ',1,1).size()==0)
			{
				messages->append("No set specified!");
				return;
			} 
            //Exporting the whole model
            //*****************
			if ((text.section(' ',1,1))=="all")
			{
                //Abaqus export
				if (text.section(' ',2,2)=="abq")
				{
					messages->append("Exporting set " + text.section(' ',1,1)+ " in abq/ccx format");
                                        if (text.section(' ',3,3).toLong()>0)
                                            emit exportMeshAbq(text.section(' ',1,1),text.section(' ',3,3).toLong());
                                        else
                                            emit exportMeshAbq(text.section(' ',1,1),0);
					return;
				}

                //frd export
                if (text.section(' ',2,2)=="frd")
                {
					messages->append("Exporting set " + text.section(' ',1,1)+ " in frd format");
                                        if (text.section(' ',3,3).toLong()>0)
                                            emit exportMeshFrd(text.section(' ',1,1),text.section(' ',3,3).toLong());
                                        else
                                            emit exportMeshFrd(text.section(' ',1,1),0);
                   return;
                }
                //vtk export
                if ((text.section(' ',2,2)=="vtk"))
                {
                    messages->append("Exporting set " + text.section(' ',1,1)+ " in vtk format");
                    if (text.section(' ',3,3).toLong()>0)
                        ;
                        //emit exportMeshFrd(text.section(' ',1,1),text.section(' ',3,3).toLong());
                    else
                    ;    //emit exportMeshFrd(text.section(' ',1,1),0);




                    return;
                }
                messages->append("Format " + text.section(' ',2,2) + " not known");
                return;


				
			}
                        //********************************
                        //Exporting a specified set
			else
			{
                                setIndex=-1;
                                for (int s=0;s<feData->setList.size();s++)
                                {
                                    if (feData->setList[s].name==text.section(' ',1,1))
                                    {
                                        setIndex=s;
                                    }
                                }
                                //Check wether the set exists
                                if (setIndex==-1)
                                {
                                    messages->append(QString("Set %1 does not exist").arg(text.section(' ',1,1)));
                                    return;
                                }
                                //*****************************************

                                    if (text.section(' ',2,2)=="abq")
                                    {
                                        if (text.section(' ',3,3 )=="nam")
                                        {
                                            messages->append("Exporting labels for set " + text.section(' ',1,1)+ " in abq/ccx format");
                                            if (text.section(' ',4,4).toLong()>0)
                                            {
                                                emit exportLabelsAbq(setIndex,text.section(' ',4,4).toLong());
                                            }
                                            else
                                            {
                                                emit exportLabelsAbq(setIndex,0);
                                            }
                                            return;
                                        }
                                        else
                                        {
                                            messages->append("Exporting set " + text.section(' ',1,1)+ " in abq/ccx format");
                                            if (text.section(' ',3,3).toLong()>0)
                                            {
                                                //emit exportMeshAbq(text.section(' ',1,1),text.section(' ',3,3).toLong());
                                            }
                                            else
                                            {
                                                //emit exportMeshAbq(text.section(' ',1,1),0);
                                            }
                                            return;
                                        }
                                    }
                                    else
                                    {
                                        if (text.section(' ',2,2)=="frd")
                                        {
                                            messages->append("Exporting set " + text.section(' ',1,1)+ " in frd format");
                                            if (text.section(' ',3,3).toLong()>0)
                                                emit writeSetFrd(setIndex,text.section(' ',3,3).toLong());
                                            else
                                                emit writeSetFrd(setIndex,0);
                                            return;
                                        }
                                        else
                                        {
                                            messages->append("Format " + text.section(' ',2,2) + " not known");
                                            return;
                                        }
                                    }
				return;
			}
		}
                //**********************************************
                //Adding entities to a set
                if (command=="qadd")
                {
                    //messages->append("qadd");
                    if (text.section(' ',1,1).size()==0)
                    {
                            messages->append("No set specified!");
                            return;
                    }
                    else
                    {
                        emit newSet(text.section(' ',1,1));
                        viewingArea->qaddFlag=1;
                        feData->currentSet=text.section(' ',1,1);
                        emit updateView();
                        return;
                    }
                }
                //*********************************************
                //Removing entities from a set
                if (command=="qrem")
                {
                    //messages->append("qrem");
                    if (text.section(' ',1,1).size()==0)
                    {
                            messages->append("No set specified!");
                            return;
                    }
                    else
                    {
                        emit newSet(text.section(' ',1,1));
                        viewingArea->qaddFlag=2;
                        feData->currentSet=text.section(' ',1,1);
                        emit updateView();
                        return;
                    }
                }
                //***********************************************
                //Plot additional sets
                if (command=="plus")
                {
                    evaluatePLUS(text);
                    return;
                }
                //***********************************************
                //Remove additional sets from the plotting window
                if (command=="minus")
                {
                    if (text.section(' ',1,1).size()==0)
                    {
                            messages->append("No entity specified!");
                            return;
                    }
                    if (text.section(' ',1,1)=="n")
                    {
                        if (text.section(' ',2,2).size()==0)
                        {
                                messages->append("No set specified!");
                                return;
                        }
                        else
                        {
                            setIndex=-1;
                            for (int s=0;s<feData->setList.size();s++)
                            {
                                if (feData->setList[s].name==text.section(' ',2,2))
                                {
                                    setIndex=s;
                                }
                            }
                            if (setIndex==-1)
                            {
                                messages->append(QString("Set %1 does not exist").arg(text.section(' ',2,2)));
                                return;
                            }
                            else
                            {
                                emit unplotSet(setIndex);
                                return;
                            }
                        }

                    }


                }
                //***********************************************
                //Plot a singele set
                if (command=="plot")
                {
                    if (text.section(' ',1,1).size()==0)
                    {
                            messages->append("No entity specified!");
                            return;
                    }
                    if (text.section(' ',2,2).size()==0)
                    {
                            messages->append("No set specified!");
                            return;
                    }

                    if (text.section(' ',1,1)=="n")
                    {
                            setIndex=-1;
                            for (int s=0;s<feData->setList.size();s++)
                            {
                                // messages->append(QString("Set %1").arg(feData->setList[s].name));
                                //  messages->append(QString("Set %1").arg(text.section(' ',2,2)));
                                if (feData->setList[s].name==text.section(' ',2,2))
                                {
                                    setIndex=s;
                                }
                            }
                            if (setIndex==-1)
                            {
                                messages->append(QString("Set %1 does not exist").arg(text.section(' ',2,2)));
                                return;
                            }
                            else
                            {
                                messages->append(QString("Plotting nodes from set %1").arg(text.section(' ',2,2)));

                                emit plotSingleNSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
                                return;
                            }


                    }
                    if (text.section(' ',1,1)=="f")
                    {
                        setIndex=-1;
                        for (int s=0;s<feData->setList.size();s++)
                        {
                            // messages->append(QString("Set %1").arg(feData->setList[s].name));
                            //  messages->append(QString("Set %1").arg(text.section(' ',2,2)));
                            if (feData->setList[s].name==text.section(' ',2,2))
                            {
                                setIndex=s;
                            }
                        }
                        if (setIndex==-1)
                        {
                            messages->append(QString("Set %1 does not exist").arg(text.section(' ',2,2)));
                            return;
                        }
                        else
                        {
                            messages->append(QString("Plotting faces from set %1").arg(text.section(' ',2,2)));

                            emit plotSingleFaceSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
                            return;
                        }
                    }
                    //*******************************************************
                    if (text.section(' ',1,1)=="e")
                    {
                        setIndex=-1;
                        for (int s=0;s<feData->setList.size();s++)
                        {
                            // messages->append(QString("Set %1").arg(feData->setList[s].name));
                            //  messages->append(QString("Set %1").arg(text.section(' ',2,2)));
                            if (feData->setList[s].name==text.section(' ',2,2))
                            {
                                setIndex=s;
                            }
                        }
                        if (setIndex==-1)
                        {
                            messages->append(QString("Set %1 does not exist").arg(text.section(' ',2,2)));
                            return;
                        }
                        else
                        {
                            messages->append(QString("Plotting elements from set %1").arg(text.section(' ',2,2)));

                            emit plotSingleElSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
                            return;
                        }
                    }

                }
                //***********************************************
                //CReate a bristle pack
                if (command=="borste")
                {
                    int numberX=1;
                    int numberReihen=1;
                    double angle=45.0;
                    messages->append("Creating Bristle Pack");
                    if (text.section(' ',1,1).toDouble()>0)
                    {
                        angle=90 - text.section(' ',1,1).toDouble();
                    }
                    if (text.section(' ',2,2).toInt()>0)
                    {
                        numberX=text.section(' ',2,2).toInt();
                    }
                    if (text.section(' ',3,3).toInt()>0)
                    {
                        numberReihen=text.section(' ',3,3).toInt();
                    }
                    feData->createBristlePack(angle,90,1,1,4.5,0.1,numberX,numberReihen,1,1,20);
                    viewingArea->newFileActions();
                    modelTree->updateTree();
                        return;
                }
                //***********************************************
                //Reverse a line or surface
                if (command=="rev")
                {
                    evaluateREV(text);
                    return;
                }
                //***********************************************
                //Export the openfoam surface
                if (command=="foam")
                {
                    QString dir=QFileInfo(feData->openFile).absolutePath();

                    messages->append("Exporting to OpenFoam format");
                    emit exportFoam(dir);
                    return;
                }
                //***********************************************
                //Calculate the expansion ratio of tria3 meshes
                if (command=="erat")
                {
                    messages->append("Calculating expansion ratios");
                    emit calcExpRatio();
                    return;
                }
                //***********************************************
                //Move nodes radially
                if (command=="movrad")
                {
                    messages->append("Moving nodes radially");
                    emit moveRadially(text.section(' ',1,1).toDouble());
                    return;
                }
                //***********************************************
                //Create 2DMesh out of stl trias
                if (command=="mesh2d")
                {
                    int soFlag=0; //secondOrderFLag
                    if (text.section(' ',2,2).toInt()==1)
                    {
                        soFlag=1;
                    }

                    if (text.section(' ',1,1).toDouble()>0)
                    {
                        emit meshSTL(geoData,text.section(' ',1,1).toDouble(),soFlag,0);
                    }
                    else
                    {
                        messages->append("maxH value must be greater than 0 ! Meshing aborted! ");
                    }
                    return;
                }
                //***********************************************
                //Create 2DMesh out of stl trias
                if (command=="mesh3d")
                {
                    int soFlag=0; //secondOrderFLag
                    if (text.section(' ',2,2).toInt()==1)
                    {
                        soFlag=1;
                    }

                    if (text.section(' ',1,1).toDouble()>0)
                    {
                        emit meshSTL(geoData,text.section(' ',1,1).toDouble(),soFlag,1);
                    }
                    else
                    {
                        emit messages->append("maxH value must be greater than 0 ! Meshing aborted! ");
                    }
                    return;
                }
                //***********************************************
                //Deletes all mesh data
                if (command=="delmesh")
                {

                    emit deleteMesh();
                    return;
                }
                //************************************************
                //Creates a material with elastic properties and density
                if (command=="material")
                {
                    emit createMaterial(text.section(' ',1,1),text.section(' ',2,2).toDouble(),text.section(' ',3,3).toDouble(),text.section(' ',4,4).toDouble());
                    return;
                }
                //************************************************
                //Creates a fe section with elset and material
                if (command=="fesect")
                {
                    emit createFeSection(text.section(' ',1,1),text.section(' ',2,2),text.section(' ',3,3),1,1);
                    return;
                }
                //************************************************
                //Extends a node selection
                if (command=="extn")
                {
                    emit autoExtendNodes(text.section(' ',1,1),text.section(' ',2,2).toDouble());
                    return;
                }
                //************************************************
                //Extends a face selection
                if (command=="extf")
                {
                    emit autoExtendFaces(text.section(' ',1,1),text.section(' ',2,2).toDouble());
                    return;
                }
                //Toggles the octree visualisation on/off
                if (command=="octree")
                {
                    emit toggleOctree();
                    return;
                }
                //Creates a surface
                if (command=="surf")
                {
                    evaluateSURF(text);
                    return;
                }
		messages->append("Unknown Command");

	}
//***************************************************************************************
/*! Slot for receiving messages and displaying them in the message area */
void Modul3D::recieveMessage(const QString &message)
	{
		messages->append(message);
        messages->verticalScrollBar()->setValue(messages->verticalScrollBar()->maximum());
	}
//********************************************************************************************************	
/*! No descriptiuon yet*/
void Modul3D::keyPressEvent(QKeyEvent *e)
{



}
//*********************************************************************************************************
/*! Show infos of the clicked item in the treeview in the infoview*/
void Modul3D::showTreeItemInfo()
{

} 
//*********************************************************************************************************
/*! Makes the tree-view visible in the varioWidget*/
void Modul3D::showTree()
{
	varioWidget->setCurrentWidget(modelTree);
        //cLine->grabKeyboard();
}
//*********************************************************************************************************
/*! Makes the result-options-view visible in the varioWidget*/
void Modul3D::showResultOptions()
{
	varioWidget->setCurrentWidget(resultOptions);
        //cLine->releaseKeyboard();
}
//*********************************************************************************************************
/*! Makes the result-options-view visible in the varioWidget*/
void Modul3D::showGraphicOptions()
{
        varioWidget->setCurrentWidget(graphicOptions);
        graphicOptions->setFeatureEdgeAngle(viewingArea->featureEdgeAngle);
}
//****************************************************************************************************
/*! Makes the interpolate-options-view visible in the varioWidget*/
void Modul3D::showInterpolateOptions()
{
	varioWidget->setCurrentWidget(interpolOptions);
        //cLine->releaseKeyboard();
}
//****************************************************************************************************
/*! Makes the creae point-view visible in the varioWidget*/
void Modul3D::showCreatePoint()
{
	varioWidget->setCurrentWidget(createPoint);
        //cLine->releaseKeyboard();
}
//****************************************************************************************************
/*! Makes the create curve-view visible in the varioWidget*/
void Modul3D::showCreateCurve()
{
	varioWidget->setCurrentWidget(createCurve);
}
//****************************************************************************************************
/*! Makes the create COS-view visible in the varioWidget*/
void Modul3D::showCreateCOS()
{
	varioWidget->setCurrentWidget(createCOS);
}
//****************************************************************************************************
/*! Makes the delete COS-view visible in the varioWidget*/
void Modul3D::showDeleteCOS()
{
        if (geoData->cosList.size() >2)
        varioWidget->setCurrentWidget(deleteCOS);
}
//****************************************************************************************************
/*! Makes the merge Nodes view visible in the varioWidget*/
void Modul3D::showMergeNodes()
{
        mergeNodes->targetSetComboBox->clear();
        mergeNodes->targetSetComboBox->addItem("all");
        for (int n=0;n<feData->setList.size();n++)
        {
            mergeNodes->targetSetComboBox->addItem(feData->setList[n].name);
        }
        varioWidget->setCurrentWidget(mergeNodes);
        //cLine->releaseKeyboard();
}
//****************************************************************************************************
/*! Makes the boundary condition view visible in the varioWidget*/
void Modul3D::showBC()
{
        createBC->targetSetComboBox->clear();
        createBC->targetSetComboBox->addItem("all");
        for (int n=0;n<feData->setList.size();n++)
        {
            createBC->targetSetComboBox->addItem(feData->setList[n].name);
        }
        varioWidget->setCurrentWidget(createBC);
        //cLine->releaseKeyboard();
}
//******
//****************************************************************************************************
/*! Makes the fe section view visible in the varioWidget*/
void Modul3D::showFeSect()
{
        //Fill the target set box with all vailable sets
        createFeSect->targetSetComboBox->clear();
        createFeSect->targetSetComboBox->addItem("all");
        for (int n=0;n<feData->setList.size();n++)
        {
            createFeSect->targetSetComboBox->addItem(feData->setList[n].name);
        }
        //**********************
        //Fill the material box with all available materials
        createFeSect->materialComboBox->clear();

        for (int n=0;n<feData->materialList.size();n++)
        {
            createFeSect->materialComboBox->addItem(feData->materialList[n].name);
        }
        varioWidget->setCurrentWidget(createFeSect);
}
//**********************************************************************************************
/*! Makes the material view visible in the varioWidget*/
void Modul3D::showMat()
{

        //varioWidget->setCurrentWidget(createMat);
        mainArea->setCurrentWidget(matWidget);
}
//**********************************************************************************************
/*! Makes the verify elements view visible in the varioWidget*/
void Modul3D::showVerifyElements()
{

        varioWidget->setCurrentWidget(verifyElementView);

}
//**********************************************************************************************
/*! Makes the verify elements view visible in the varioWidget*/
void Modul3D::showMeshGeo()
{

        varioWidget->setCurrentWidget(createMeshView);

}
//****************************************************
/*! Makes the widget grab all keyboard commands and redirect them to the commandline*/
void Modul3D::grabKeys()
{
        //cLine->grabKeyboard();
}
//*************************************************************************
/*! Releases the keyboard*/
void Modul3D::releaseKeys()
{
        //cLine->releaseKeyboard();
}
//*****************************************************
/*! Custom event filter to recognize special actions*/
bool Modul3D::eventFilter(QObject *target, QEvent *event)
{
    if (target == viewingArea)
    {
    if (event->type() == QEvent::KeyPress)
        {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);


             if ((keyEvent->key() == Qt::Key_Plus) && QApplication::keyboardModifiers() & Qt::ControlModifier)
             {
                messages->append("Zoom Plus");
                 viewingArea->zoomPlus();
                 return true;
             }

             /*
             if ((keyEvent->key() & Qt::Key_Minus) && keyEvent->modifiers() & Qt::ShiftModifier)
             {
                 viewingArea->zoomMinus();
                 return true;
             }
*/
             if (viewingArea->qaddFlag==1 || viewingArea->qaddFlag==2)
             {
                switch (keyEvent->key())
                {
                    case Qt::Key_R:
                        emit RPressed();
                    break;
                    case Qt::Key_Q:
                        recieveMessage("q-command finished");
                        viewingArea->qaddFlag=0;
                        feData->setList[feData->currentNsetIndex].qCommandFlag=false;
                        feData->currentSet="all";
                        feData->currentNsetIndex=-1;
                        emit updateView();
                        emit updateSets();
                    break;
                    case Qt::Key_N:
                        //recieveMessage("n pressed");
                        emit nodeSelection();
                        emit updateView();
                    break;
                    case Qt::Key_F:
                        recieveMessage("f pressed");
                        emit faceSelection();
                        emit updateView();
                    break;
                    case Qt::Key_E:
                        recieveMessage("e pressed");
                        emit elementSelection();
                        emit updateView();
                    break;
                    case Qt::Key_I:
                        emit setSelectionMode(0);
                        emit updateView();
                    break;
                    case Qt::Key_A:
                        emit setSelectionMode(1);
                        emit updateView();
                    break;
                }
                return true;
             }
             else
             {
                 //Sending the event to the commandline
                 QCoreApplication::sendEvent(cLine,event);
             }
         }
    }
    return false;
}
//**********************************************************
//Converts a color string into an int color index
//If the color string is not known, return value is 0 == red
int Modul3D::getIntColorFromString(const QString &color)
{
    if (color=="g")
        return(1);
    if (color=="green")
        return(1);
    if (color=="b")
        return(2);
    if (color=="blue")
        return(2);

    return(0);
}
//**********************************************************
/*! reads a file and executes the commands in it*/
void  Modul3D::runScript(const QString &fileName)
{
    QString line;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("File not found"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QTextStream in(&file);
/////////////////////////////////////////////////////////////
//////////Reading the file
    while (!in.atEnd())
    {
        line = in.readLine();
        messages->append(line);
        evaluateCommand(line);
    }
}
//*********************************************
/*! Evaluate the pnt command*/
void  Modul3D::evaluatePNT(const QString& text)
{
    bool ok,ok2,ok3,ok4;
    long pointLabel;
    long pointIndex;

    if (text.section(' ',1,1)=="!")
    {
        //Autmatic point labeling
        //Get the label of last point up to now and add 1 for the new label
        if (geoData->pointList.size() > 0)
        {
            pointLabel=(geoData->pointList[geoData->pointList.size()-1].getLabel())+1;
            messages->append(QString("LAbel %1").arg(pointLabel));
        }
        else
        {
            pointLabel=1;
        }
        pointIndex=0;
    }
    else
    {
        //Manual point label
        //Remove a p from the first parameter (Compatibility with fbd)
        pointLabel=text.section(' ',1,1).remove("p").remove("P").toLong(&ok4);
        if (ok4==false)
        {
            messages->append("Illegal point label. Label must be a positive long integer!");
            return;
        }
        if (pointLabel < 0)
        {
            messages->append("Illegal point label. Label must be positive!");
            return;
        }
        //Check wether a point with the given label already exists 
        if (geoData->pointList.size()==0)
        {
            pointIndex=0;
        }
        else
        {
        pointIndex=geoData->getPointIndex(pointLabel);
        emit messages->append(QString("pointIndex: %1").arg(pointIndex));
        if (pointIndex > (-1))
        {
           messages->append("Point %1 already exitst! Point creation cancelled!");
           return;
        }
        }

    }
    double x=text.section(' ',2,2).toDouble(&ok);
    double y=text.section(' ',3,3).toDouble(&ok2);
    double z=text.section(' ',4,4).toDouble(&ok3);

    if (ok==true && ok2==true && ok3==true)
        emit createPointXYZIndex(pointIndex*-1,pointLabel,x,y,z);
    else
        messages->append("Illegal coordinates");
}
//*********************************************
/*! Evaluate the line command*/
void  Modul3D::evaluateLINE(const QString& text)
{
    bool ok,ok2,ok4;
    long lineLabel;
    long lineIndex;

    if (text.section(' ',1,1)=="!")
    {
        //Autmatic line labeling
        //Get the label of last point up to now and add 1 for the new label
        if (geoData->curveList.size() > 0)
        {
            lineLabel=(geoData->curveList[geoData->curveList.size()-1].getLabel())+1;
            messages->append(QString("Label %1").arg(lineLabel));
        }
        else
        {
            lineLabel=1;
        }
        lineIndex=0;
    }
    else
    {
        //Manual line label
        //Remove a l from the first parameter (Compatibility with fbd)
        lineLabel=text.section(' ',1,1).remove("l").remove("L").toLong(&ok4);
        if (ok4==false)
        {
            messages->append("Illegal line label. Label must be a positive long integer!");
            return;
        }
        if (lineLabel < 0)
        {
            messages->append("Illegal line label. Label must be positive!");
            return;
        }

    }
    //Remove the "p"s from the point labels for compatibility reasons
    long s=text.section(' ',2,2).remove("p").remove("P").toLong(&ok);
    long e=text.section(' ',3,3).remove("p").remove("P").toLong(&ok2);


    if (ok==true && ok2==true)
    {
        messages->append(QString("Creating line %1").arg(lineLabel));
        emit createStraightLineLabel(lineLabel,s,e);
    }
    else
        messages->append("Illegal Point Labels");
    return;
}
//*********************************************
/*! Evaluate the rev command*/
void  Modul3D::evaluateREV(const QString& text)
{
    bool ok;
    long lineIndex;
    long lineLabel;

    if (text.section(' ',1,1)=="l")
    {
        lineLabel=text.section(' ',2,2).toLong(&ok);
        if (ok)
        {
            lineIndex=geoData->getLineIndex(lineLabel);
            if (lineIndex>-1)
                geoData->curveList[lineIndex].reverse();

        }
        else
        {
            recieveMessage(QString("Illegal line label: %1").arg(text.section(' ',2,2)));
        }
    }
    else
    {
        recieveMessage(QString("Option %1 not supported").arg(text.section(' ',1,1)));
    }

}
//*********************************************
/*! Evaluate the plus command*/
void  Modul3D::evaluatePLUS(const QString& text)
{
    int setIndex=-1;
    //**************
    //Check wether a entity has been specified
    if (text.section(' ',1,1).size()==0)
    {
            messages->append("No entity specified!");
            return;
    }
    //**************
    //Check wether a set has been specified and get the index of the set
    if (text.section(' ',2,2).size()==0)
    {
            messages->append("No set specified!");
            return;
    }
    else
    {
        for (int s=0;s<feData->setList.size();s++)
        {
            if (feData->setList[s].name==text.section(' ',2,2))
            {
                setIndex=s;
            }
        }
        if (setIndex==-1)
        {
            messages->append(QString("Set %1 does not exist").arg(text.section(' ',2,2)));
            return;
        }
    }
    //***********************************
    //Check which enzity has been specified and emit the corresponding signal
    if (text.section(' ',1,1)=="n")
    {
        messages->append(QString("Plotting nodes from set %1").arg(text.section(' ',2,2)));

        emit plotSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
        return;
    }
    else if (text.section(' ',1,1)=="f")
    {
        messages->append(QString("Plotting faces from set %1").arg(text.section(' ',2,2)));

        emit plotFaceSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
        return;
    }
    else if (text.section(' ',1,1)=="e")
    {
        messages->append(QString("Plotting elements from set %1").arg(text.section(' ',2,2)));

        emit plotElSet(setIndex,getIntColorFromString(text.section(' ',3,3)));
        return;
    }
    else
    {
     emit sendMessage(QString("Unknown entity: %1").arg(text.section(' ',1,1)));
    }
}

//****************************************************
/*! Picks up the signal from the createMeshView*/
void Modul3D::startMeshing(int dimension,double hmax,int so)
{
    emit meshSTL(geoData,hmax,so,dimension);
}
//****************************************************
/*! MAkes the load widget the current widget*/
void Modul3D::showLoadWidget()
{

        varioWidget->setCurrentWidget(createLoadsView);
        createLoadsView->typeComboBox->setCurrentIndex(0);
        createLoadsView->targetSetComboBox->clear();
        for (int n=0;n<feData->setList.size();n++)
        {
            createLoadsView->targetSetComboBox->addItem(feData->setList[n].name);
        }
        createLoadsView->targetSetComboBox->addItem("all");


}
//*********************************************
/*! Evaluate the surf command*/
void  Modul3D::evaluateSURF(const QString& text)
{

    bool ok4;
    long surfaceLabel;
    long surfaceIndex;
    long edge;
    QList<long> edgeList;
    edgeList.clear();
    int nEdges=0;
    QStringList entries=text.split(" ",QString::SkipEmptyParts);
    if (entries.size()< 5)
    {
        emit sendMessage ("Incomplete Command surf (Expected: surf <label> <edge1> <edge2> <edge3>....");
        return;
    }

    if (entries[1]=="!")
    {
        //Autmatic surface labeling
        //Get the label of last surface up to now and add 1 for the new label
        if (geoData->surfaceList.size() > 0)
        {
            surfaceLabel=(geoData->surfaceList[geoData->surfaceList.size()-1].getLabel())+1;
            messages->append(QString("Label %1").arg(surfaceLabel));
        }
        else
        {
            surfaceLabel=1;
        }
        surfaceIndex=0;
    }
    else
    {
        //Manual surface label
        //Remove a s from the first parameter (Compatibility with fbd)
        surfaceLabel=text.section(' ',1,1).remove("s").remove("S").toLong(&ok4);
        if (ok4==false)
        {
            messages->append("Illegal surface label. Label must be a positive long integer!");
            return;
        }
        if (surfaceLabel < 0)
        {
            messages->append("Illegal surface label. Label must be positive!");
            return;
        }

    }
    for (int k=0;k<entries.size();k++)
    {
        emit sendMessage(QString("Entry %1: %2").arg(k).arg(entries[k]));
    }
    nEdges=entries.size()-2;

    for (int k=2;k<entries.size();k++)
    {
        emit sendMessage(QString("Edge %1: Line %2").arg(k-1).arg(entries[k]));
        //Removing l.L for comptatibility reasons
        edge=entries[k].remove("l").remove("L").toLong(&ok4);
        if (ok4)
        {
            edgeList.append(edge);
        }
        else
        {
            emit sendMessage(QString("Illegal line label for edge %1: %2").arg(k-1).arg(entries[k]));
            emit sendMessage("Aborting surface creation!");
            return;
        }

    }
    emit sendMessage(QString("Creating surface %1").arg(surfaceLabel));
    emit createSurface(surfaceLabel,edgeList);
}
//****************************************************
/*! MAkes the create disc widget the current widget*/
void Modul3D::showCreateDisc()
{
        varioWidget->setCurrentWidget(createDiscView);
}
//****************************************************
/*! Makes the file import widget the current widget*/
void Modul3D::showFileImport()
{
        varioWidget->setCurrentWidget(fileImportView);
}
//****************************************************
/*! Makes the 3d widget the main widget*/
void Modul3D::show3d()
{
      mainArea->setCurrentWidget(viewingArea);
}
