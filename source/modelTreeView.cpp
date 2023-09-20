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
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *esult
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 #include "modelTreeView.h"
 #include "QMessageBox"
 #include "femodel.h"
 modelTreeView::modelTreeView(QWidget *parent, femodel *fedatabase, geomodel *geodatabase)
	:QTreeView(parent)
 {
    currentRowSelection=-1;
	fedata=fedatabase;
	geodata=geodatabase;
	header()->hide();
	model =new QStandardItemModel(this);
    parentItem = model->invisibleRootItem();
	nodeItem =new QStandardItem(QString("Nodes (0)"));
	elementItem = new QStandardItem(QString("Elements (0)"));
    faceItem = new QStandardItem(QString("Faces (0)"));
	resultItem =new QStandardItem(QString("Results (0)"));
	geometryItem = new QStandardItem(QString("Geometry (0)"));
    cosItem = new  QStandardItem(QString("COS (0)"));
    feItem = new QStandardItem(QString("FE-Data (0)"));
	parentItem->appendRow(nodeItem);
	parentItem->appendRow(elementItem);
    parentItem->appendRow(faceItem);
	parentItem->appendRow(resultItem);
    parentItem->appendRow(feItem);
	parentItem->appendRow(geometryItem);
	
    geometryItem->appendRow(cosItem);
	this->setModel(model);
	
	connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(evaluateClick(QModelIndex)));
	
 }
 //****************************************************************************************
 modelTreeView::~modelTreeView()
 {
	delete nodeItem;
	delete elementItem;
	//elete planeItem;*/
 }
 //******************************************************************************************
 /*! Resets the model tree */
 void modelTreeView::resetData()
 {
     setCurrentIndex(nodeItem->index());
     int nrows=0;
     nodeItem->setText(QString("Nodes (0)"));
     nrows=nodeItem->rowCount();
     nodeItem->removeRows(0,nrows);
     elementItem->setText(QString("Elements (0)"));
     nrows=elementItem->rowCount();
     elementItem->removeRows(0,nrows);
     faceItem->setText(QString("Faces (0)"));
     nrows=faceItem->rowCount();
     faceItem->removeRows(0,nrows);
     resultItem->setText(QString("Results (0)"));
     nrows=resultItem->rowCount();
     resultItem->removeRows(0,nrows);
     feItem->setText(QString("FE-Data (0)"));
     nrows=feItem->rowCount();
     feItem->removeRows(0,nrows);
     geometryItem->setText(QString("Geometry (0)"));
     nrows=geometryItem->rowCount();
     geometryItem->removeRows(1,nrows-1);

 }
//*******************************************************************
 void modelTreeView::setNodes(long numberNodes)
 {
	nodeItem->setText(QString("Nodes (%0)").arg(numberNodes));
        updateNSets();
 }
 //*****************************************************************************************
 /*! Update the node sets list in the model tree*/
 void modelTreeView::updateNSets()
 {
     int numberR=nodeItem->rowCount();
     nodeItem->removeRows(0,numberR);
     nSetList.clear();
     long numberSets=fedata->setList.size();
        for (int k=0;k<numberSets;k++)
        {
            tempItem = new QStandardItem(QString("%0 (%1)").arg(fedata->setList[k].name).arg(fedata->setList[k].nodeList.size()));
            nSetList.append(tempItem);
        }
        nodeItem->appendRows(nSetList);
 }

 //*****************************************************************************************
 void modelTreeView::updateElSets()
 {
     int numberR=elementItem->rowCount();
     elementItem->removeRows(0,numberR);
     elSetList.clear();
     long numberSets=fedata->setList.size();
        for (int k=0;k<numberSets;k++)
        {
            if (fedata->setList[k].elementList.size()>0)
            {
                tempItem = new QStandardItem(QString("%0 (%1)").arg(fedata->setList[k].name).arg(fedata->setList[k].elementList.size()));
                elSetList.append(tempItem);
            }
        }
        elementItem->appendRows(elSetList);
 }
 //*****************************************************************************************
 void modelTreeView::updateFaceSets()
 {
     int numberR=faceItem->rowCount();
     faceItem->removeRows(0,numberR);
    faceSetList.clear();
    //***************************************************
    //
    long numberSets=fedata->setList.size();
        for (int k=0;k<numberSets;k++)
        {
            if (fedata->setList[k].faceIndexList.size()>0)
            {
                tempItem = new QStandardItem(QString("%0 (%1)").arg(fedata->setList[k].name).arg(fedata->setList[k].faceIndexList.size()));
                faceSetList.append(tempItem);
            }

        }
    //
    faceItem->appendRows(faceSetList);

}
 //**************************************************************************************
 void modelTreeView::updateCos()
 {
     int numberR=cosItem->rowCount();
     cosItem->removeRows(0,numberR);
     cosList.clear();
     int numberCos=geodata->cosList.size();
     //emit sendMessage(QString("numberCos %1").arg(numberCos));
     for (int k=0;k<numberCos;k++)
     {
          tempItem = new QStandardItem(QString("%0 (%1)").arg(k).arg(geodata->cosList[k].name));
            cosList.append(tempItem);
     }
     cosItem->appendRows(cosList);
     cosItem->setText(QString("COS (%0)").arg(numberCos));
}
 //*******************************************************************************************
  void modelTreeView::setElements(long numberElements)
 {
	elementItem->setText(QString("Elements (%0)").arg(numberElements));
        updateElSets();
 }
  //***********************************
  void modelTreeView::setFaces(long numberFaces)
 {
        faceItem->setText(QString("Faces (%0)").arg(numberFaces));
        updateFaceSets();
 }
 //*****************************************************************************************
  void modelTreeView::setGeometricEntities(long numberGeo)
 {
	geometryItem->setText(QString("Geometry (%0)").arg(numberGeo));
 }
 //*****************************************************************************************
  void modelTreeView::updateFEData()
 {
    //emit sendMessage("Upating FE data in tree");
    long numberLoad=fedata->loadList.size();
    long numberBC=fedata->bcList.size();
    long numberMPC=fedata->mpcList.size();
    long numberMaterial=fedata->materialList.size();
    long numberFeSection=fedata->feSectionList.size();

    bcList.clear();

    long totalEntities=numberBC+numberMPC+numberMaterial+numberFeSection+numberLoad;
	
    int numberR=feItem->rowCount();

    feItem->removeRows(0,numberR);
	
    if (numberBC>0)
	{
        bcItem = new QStandardItem(QString("BC (%0)").arg(numberBC));
        feItem->appendRow(bcItem);
        for (int i = 0; i < numberBC; i++)
		{
            tempItem = new QStandardItem(QString("%0 ").arg(fedata->bcList[i].name));
            bcList.append(tempItem);
		}
        bcItem->appendRows(bcList);

    }
    //********************************************************************
    QString typeString="";

    if (numberLoad>0)
    {
        loadItem = new QStandardItem(QString("Loads (%0)").arg(numberLoad));
        feItem->appendRow(loadItem);
        for (int i = 0; i < numberLoad; i++)
        {
            switch (fedata->loadList[i].type)
            {
            case 0:
                typeString="CLOAD";
            break;
            case 1:
                typeString="DLOAD";
            break;
            case 2:
                typeString="DLOAD";
            break;
            }

            QStandardItem *item = new QStandardItem(QString("%0 (%1)").arg(fedata->loadList[i].name).arg(typeString));
            loadItem->appendRow(item);
        }

    }
    //*******************************************************************
    //numberR=feItem->rowCount();
    //emit sendMessage(QString(" rows %1").arg(numberR));
    if (numberMPC>0)
    {
        mpcItem = new QStandardItem(QString("MPCs (%0)").arg(numberMPC));
        feItem->appendRow(mpcItem);

    }
    if (numberMaterial>0)
    {
        materialItem = new QStandardItem(QString("Materials (%0)").arg(numberMaterial));
        feItem->appendRow(materialItem);
        for (int i = 0; i < numberMaterial; i++)
        {
            QStandardItem *item = new QStandardItem(QString("%0 ").arg(fedata->materialList[i].name));
            materialItem->appendRow(item);
        }

    }
    if (numberFeSection>0)
    {
        feSectionItem = new QStandardItem(QString("FE-Sections (%0)").arg(numberFeSection));
        feItem->appendRow(feSectionItem);
        for (int i = 0; i < numberFeSection; i++)
        {
            QStandardItem *item = new QStandardItem(QString("%0 ").arg(fedata->feSectionList[i].name));
            feSectionItem->appendRow(item);
        }

    }
    //******************************************************

    feItem->setText(QString("FE-Data (%0)").arg(totalEntities));


}

  //*****************************************************************************************
   void modelTreeView::updateGeometryInfo(geomodel *gm)
  {

     long numberPlanes=gm->planeList.size();
     long numberPoints=gm->pointList.size();
     long numberCurves=gm->curveList.size();
     long numberSurfaces=gm->surfaceList.size();
     long numberSTL=gm->stlTriaList.size();
     long numberCos=gm->cosList.size();
     long totalEntities=numberPlanes+numberPoints+numberCurves+numberSurfaces+numberCos+numberSTL;

     int numberR=geometryItem->rowCount();

     geometryItem->removeRows(1,numberR-1);

     if (numberPlanes>0)
     {
         planeItem = new QStandardItem(QString("Planes (%0)").arg(numberPlanes));
         geometryItem->appendRow(planeItem);
         for (int i = 0; i < numberPlanes; i++)
         {
             QStandardItem *item = new QStandardItem(QString("plane %0").arg(gm->planeList[i].getLabel()));
             planeItem->appendRow(item);
         }
     }
     if (numberPoints>0)
     {
         pointItem = new QStandardItem(QString("Points (%0)").arg(numberPoints));
         geometryItem->appendRow(pointItem);
     }
     if (numberCurves>0)
     {
         curveItem = new QStandardItem(QString("Curves (%0)").arg(numberCurves));
         geometryItem->appendRow(curveItem);
     }
     if (numberSurfaces>0)
     {
        surfaceItem = new QStandardItem(QString("Surfaces (%0)").arg(numberSurfaces));
         geometryItem->appendRow(surfaceItem);
     }
     if (numberSTL>0)
     {
         stlItem = new QStandardItem(QString("STL Trias (%0)").arg(numberSTL));
         geometryItem->appendRow(stlItem);
     }

     setGeometricEntities(totalEntities);
  }
 //*********************************************************************************************
  void modelTreeView::setResults(femodel *md)
 {

	int numberR=resultItem->rowCount();
        resultItem->removeRows(0,numberR);
        timeResultList.clear();
        freqResultList.clear();

	long numberResults=md->resultList.size();
	//**********************
	//check for frequency results and time results
	int time=0;
        int freq=0;
	QString tempStep;
	
	resultItem->setText(QString("Results (%0)").arg(numberResults));
	for (int i = 1; i <= numberResults; ++i) 
	{
		tempStep=QString::number(md->resultList[i-1].resultStep);
		if (md->resultList[i-1].resultType==0)
		{
			tempStep=tempStep+ " - t="+QString::number(md->resultList[i-1].frqtime);
                        tempItem = new QStandardItem(QString("- ") + tempStep);
                        timeResultList.append(tempItem);
                        time++;
		}
		else
		{
			tempStep=tempStep+ " - f="+QString::number(md->resultList[i-1].frqtime);
                        tempItem = new QStandardItem(QString("- ") + tempStep);
                        freqResultList.append(tempItem);
                        freq++;
		}
	
        
        }
        //**********
        if (time>0)
        {
            timeItem = new QStandardItem(QString("Time Results (%0)").arg(time));
            resultItem->appendRow(timeItem);
            timeItem->appendRows(timeResultList);
        }
        if (freq>0)
        {
            freqItem = new QStandardItem(QString("Frequency Results (%0)").arg(freq));
            resultItem->appendRow(freqItem);
            freqItem->appendRows(freqResultList);
        }

 }
 //*********************
 void modelTreeView::setNodeSet(femodel *md)
 {
		
 }
//***********************************************************************
/*! Updates the model tree with the current model informations*/
void modelTreeView::updateTree()
{
 setCurrentIndex(nodeItem->index());
 setElements(fedata->elementList.size());
 setNodes(fedata->nodeList.size());
 setFaces(fedata->faceList.size());
 setResults(fedata);
 updateGeometryInfo(geodata);
 updateNSets();
 updateFaceSets();
 updateElSets();
 updateCos();
 updateFEData();
}
//*************************************************************************************************************
void modelTreeView::selectionChanged( const QItemSelection & selected, const QItemSelection & deselected )
{
	QModelIndexList selection=selected.indexes();
	QString text=selection[0].data(0).toString();
	QString flag=text.section(" ",0,0);
	QString lc= text.section(" ",1,1);
	
}
//***************************************************************************
void modelTreeView::evaluateClick(QModelIndex mi)
{
    emit sendMessage(QString("Click %0, %1 ").arg(mi.row()).arg(mi.column()));
        QString text=mi.data().toString();
        QString pText;
        QString pFlag;
        //emit sendMessage(QString(parentText));
        QString flag=text.section(" ",0,0);
        QString lc= text.section(" ",1,1);
        //Check wether the item has a parent
        QModelIndex pIndex=mi.parent();
        if (pIndex.isValid())
        {
            pText=pIndex.data().toString();
            pFlag=pText.section(" ",0,0);
        }
        else
        {
            pText="";
            pFlag="";
        }
	
	if (flag =="-")
	{
		int step=lc.toInt();
                if (step>-1)
		{
			//emit sendMessage("step");
			emit stepSelected(step);
			emit sendMessage(QString("Result %0 selected").arg(lc)); 
			return;
		}
	}
	if (flag=="Results")
	{
        emit stepSelected(-1);
		emit sendMessage(QString("Result deselected")); 
	}
	if (flag=="plane")
	{
		long label=lc.toLong();
		emit sendMessage(QString("Plane %0 selected").arg(lc));
		emit planeSelected(label);
	}
	else
	{
		emit planeDeselected();
	}
    //************************
    //Geometry Selected
    flag=="Geometry" ?	emit geometrySelected() : emit geometryDeselected();
    //*************************

        if(flag=="Nodes")
        {
                emit nodesSelected();
        }
        else
        {
                emit nodesDeselected();
        }
        //***************
        flag=="FE-Data" ? emit feDataSelected() : emit feDataDeselected();
        //***************
        if(pFlag=="COS" || flag=="COS")
        {
                emit cosRootSelected();
        }
        else
        {
            emit cosRootDeselected();
        }
    //******************
    //Material selected
        if(pFlag=="Materials")
        {
            int matIndex=mi.row();
            currentRowSelection=matIndex;
            emit matSelected(matIndex);
        }
        else
        {
            emit matDeselected();
        }
    //******************
    //Material selected
            if(pFlag=="FE-Sections")
            {
                int matIndex=mi.row();
                currentRowSelection=matIndex;
                emit feSectionSelected(matIndex);
            }
            else
            {
                emit feSectionDeselected();
            }
    //******************
            //******************
    //BC selected
            if(pFlag=="BC")
            {
                int matIndex=mi.row();
                currentRowSelection=matIndex;
                emit bcSelected(matIndex);
            }
            else
            {
                emit bcDeselected();
            }
            //Load selected
                    if(pFlag=="Loads")
                    {
                        int matIndex=mi.row();
                        currentRowSelection=matIndex;
                        emit loadSelected(matIndex);
                    }
                    else
                    {
                        emit loadDeselected();
                    }
    //******************
    //Elements Item clicked
        flag == "Elements" ? emit elementsSelected() : elementsDeselected();
    //******************
    //MPC Item clicked
        flag == "MPCs" ? emit mpcSelected() : mpcDeselected();
    //*****************************
        //Nodeset selected
                if(pFlag=="Nodes")
                {
                    int matIndex=mi.row();
                    currentRowSelection=matIndex;
                    emit nodeSetSelected(matIndex);
                }
                else
                {
                    emit nodeSetDeselected();
                }
      //***************************
      //Faceset selected
        if(pFlag=="Faces")
        {
            int matIndex=mi.row();
            currentRowSelection=matIndex;
            emit faceSetSelected();
        }
            else
        {
            emit faceSetDeselected();
        }
    //*****************************
    //Element Set selected
        if(pFlag=="Elements")
        {
            int matIndex=mi.row();
            currentRowSelection=matIndex;
            emit elSetSelected();
        }
        else
        {
            emit elSetDeselected();
        }

}
//************************************************
void modelTreeView::showNSet()
{
    emit sendMessage(QString("Showing set %1").arg(currentRowSelection));
    emit plotNSet(currentRowSelection,1);

}
//************************************************
//************************************************
void modelTreeView::hideNSet()
{
    emit sendMessage(QString("Hiding set %1").arg(currentRowSelection));
    emit unplotNSet(currentRowSelection,1);

}
//************************************************
void modelTreeView::showFaceSet()
{
    emit sendMessage(QString("Showing set %1").arg(currentRowSelection));
    emit plotFaceSet(currentRowSelection,1);

}
//************************************************
//************************************************
void modelTreeView::hideFaceSet()
{
    emit sendMessage(QString("Hiding set %1").arg(currentRowSelection));
    emit unplotFaceSet(currentRowSelection,1);
}
//************************************************
void modelTreeView::showElSet()
{
    emit sendMessage(QString("Showing set %1").arg(currentRowSelection));
    emit plotElSet(currentRowSelection,1);

}
//************************************************
//************************************************
void modelTreeView::hideElSet()
{
    emit sendMessage(QString("Hiding set %1").arg(currentRowSelection));
    emit unplotElSet(currentRowSelection,1);
}
//************************************************
void modelTreeView::deleteBC()
{
    emit sendMessage(QString("Deleting BC %1").arg(currentRowSelection));
    setCurrentIndex(feItem->index());
    //updateTree();
    emit deleteBouCon(currentRowSelection);
    currentRowSelection=-1;
    emit bcDeselected();
    emit feDataSelected();
   // setCurrentIndex(bcItem->index());
}
//************************************************
void modelTreeView::deleteMat()
{
    emit sendMessage(QString("Deleting Material %1").arg(currentRowSelection));
    setCurrentIndex(feItem->index());
    //updateTree();
    emit deleteMaterial(currentRowSelection);
    currentRowSelection=-1;
    emit matDeselected();
    emit feDataSelected();
   // setCurrentIndex(bcItem->index());
}
//************************************************
void modelTreeView::deleteFeSec()
{
    emit sendMessage(QString("Deleting FE Section %1").arg(currentRowSelection));
    setCurrentIndex(feItem->index());
    //updateTree();
    emit deleteFeSection(currentRowSelection);
    currentRowSelection=-1;
    emit feSectionDeselected();
    emit feDataSelected();
   // setCurrentIndex(bcItem->index());
}
//************************************************
void modelTreeView::deleteLoad()
{
    emit sendMessage(QString("Deleting Load %1").arg(currentRowSelection));
    setCurrentIndex(feItem->index());
    //updateTree();
    emit deleteLoads(currentRowSelection);
    currentRowSelection=-1;
    emit loadDeselected();
    emit feDataSelected();
   // setCurrentIndex(bcItem->index());
}
