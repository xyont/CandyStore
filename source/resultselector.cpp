/***************************************************************************
 *   Copyright (C) 2006 by Patrick Prokopczuk   *
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
 * \file resultselector.cpp
 * \brief A brief file description
 **/
#include <QtGui>
#include "resultselector.h"

    resultSelector::resultSelector(QWidget *parent, femodel *md)
    	:QWidget(parent)
    {
		modelData=md;
                //stepSelector = new QComboBox();
		typeSelector = new QComboBox();
		componentSelector = new QComboBox();
                QHBoxLayout *resultsLayout = new QHBoxLayout;
		resultsLayout->addWidget(typeSelector);
		resultsLayout->addWidget(componentSelector);
		this->setLayout(resultsLayout);
                //stepSelector->setMinimumWidth(120);
                //stepSelector->clear();
                //stepSelector->insertItem(1,"-1");
                //connect(stepSelector, SIGNAL(activated(int)), this,SLOT(newStepSelected(int)));
		connect(typeSelector, SIGNAL(activated(int)), this,SLOT(newResultSelected(int)));
		connect(componentSelector, SIGNAL(activated(int)), this,SLOT(newComponentSelected(int)));	
                currentStep=-1;
		currentComp=0;
		currentCompString="";
		currentResultString="";
		currentResultIndex=-1;

    }
//*******************************************************************************************
void resultSelector::newStepSelected(int index)
	{
	
        int selectedStep;
        //QString tempString=stepSelector->currentText().section("-",0,0);
        //emit sendMessage(QString("index %0 selected").arg(index));
        //emit sendMessage(QString("selected step %0 selected").arg(selectedStep));
        selectedStep=index;
        if (index>-1)
		{
			QString t1=QString::number(index) + " -";
                        //int listindex=stepSelector->findText(t1);
                        //stepSelector->setCurrentIndex(listindex);
		}
        if (selectedStep==-1)
		{
			currentStep=selectedStep;
			typeSelector->clear();
			componentSelector->clear();
			modelData->clearResultValues();
			modelData->clearDisp();
			emit fringePlotAvailable(0);
            emit updateFaces(-1);
			return;
		}

		if (selectedStep != currentStep)
		{
			currentStep=selectedStep;
			typeSelector->clear();
			componentSelector->clear();
			typeSelector->addItem(QString::number(selectedStep)); 
                        emit loadStep(selectedStep);
                        currentResultIndex=-1;
			emit fringePlotAvailable(0);
            emit updateFaces(-1);
		}
		modelData->clearResultValues();
		// Check wether the selected result in the last step is available in the current step
		for (int i=0;i< modelData->resultDataList.size();i++)
		{
			if (modelData->resultDataList[i].resultLabel == currentResultString)
			{
					newResultSelected(i+1);
					typeSelector->setCurrentIndex(i+1);
					int cindex=modelData->resultDataList[i].componentLabels.indexOf(currentCompString);
					if (cindex > -1)
					{
						newComponentSelected(cindex+1);
						componentSelector->setCurrentIndex(cindex+1);
					}
					
			}
		}
		
	}
//*****************************************************************************
void resultSelector::newResultSelected(int title)
{
	if (title==0)
	{
		componentSelector->clear();
		currentResultIndex=title;
		currentResultString="";
		modelData->clearResultValues();
		return;
	
	}

	if (title!=currentResultIndex)
	{
		currentResultIndex=title;
		currentResultString=typeSelector->itemText(title);
		componentSelector->clear();
		componentSelector->addItem("none");
		componentSelector->addItems(modelData->resultDataList[currentResultIndex-1].componentLabels);
		modelData->clearResultValues();
		emit fringePlotAvailable(0);
        emit updateFaces(-1);
	}
}
//*********************************************************************************
void resultSelector::newComponentSelected(int index)
{
	if (index==0)
	{
		currentComp=index;
		currentCompString="";
		modelData->clearResultValues();
		emit fringePlotAvailable(0);
		return;
	}
		currentComp=index;
		currentCompString=componentSelector->itemText(index);
		modelData->setNodeValues(currentResultIndex,index);
		modelData->calcFaceCentroidResults(currentResultIndex,index);

		emit fringePlotAvailable(1);
        emit updateFaces(-1);
}
//**************************************************************************
/*! Everything that has to be done if a new file is loaded*/
void resultSelector::newFileActions()
{
        //stepSelector->clear();
	typeSelector->clear();
	componentSelector->clear();
	currentStep=0;
	currentComp=0;
	currentResultIndex=0;
	updateResultBoxStep();
}
//************************************************************************************************
/*!Fills the resultselector combobox with all available result steps
*/
void resultSelector::updateResultBoxStep()
{
        /*QString tempStep="";
	stepSelector->clear();
        stepSelector->insertItem(1,"-1");
	if (modelData->resultList.size()>0)
	{
	
	for (int r=0;r<modelData->resultList.size();r++)
		{
		tempStep=QString::number(modelData->resultList[r].resultStep);
		if (modelData->resultList[r].resultType==0)
		{
			tempStep=tempStep+ " - t="+QString::number(modelData->resultList[r].frqtime);
		}
		else
		{
			tempStep=tempStep+ " - f="+QString::number(modelData->resultList[r].frqtime);
		}
		
		stepSelector->insertItem(r+2,tempStep);
		}
	}
        */
}
