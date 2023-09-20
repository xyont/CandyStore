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
 #include "modelInfoView.h"
 #include "femodel.h"
 modelInfoView::modelInfoView(QWidget *parent, femodel *md, geomodel *gm)
	:QTextEdit(parent)
 {
	setReadOnly(true);
	append("Candy Store");
	feData=md;
        geoData=gm;
	currentNodeIndex=-1;
	

 }
 modelInfoView::~modelInfoView()
 {
 
 }
 
 void modelInfoView::setNodes(long numberNodes)
 {

 }
 
  void modelInfoView::setElements(long numberElements)
 {

 }
 
void modelInfoView::updateInfo()
{
	vector3d coord;
	
	if (currentNodeIndex > -1)
	{
		coord = feData->nodeList[currentNodeIndex].getCoordinates();
		clear();
		QString line1="Node" + QString::number(feData->nodeList[currentNodeIndex].getLabel());
		QString line3=QString::number(coord.x) + " ; " + QString::number(coord.y) + " ; " + QString::number(coord.z) ;
		QString line5=QString::number(feData->nodeList[currentNodeIndex].getResultValue());
		append(line1);
		append("Coords:");
		append(line3);
		append("Result Value:");
		append(line5);
	}
}
//****************************************************************************
//SLOT
/*!Shows information of a node*/
void modelInfoView::showNodeInformation(long label)
{
    long index;
	vector3d coord;
        if (label > 0)
	{
                index = feData->getNodeIndex(label);
                coord = feData->getNodeCoords(label);

		clear();
                QString line1="Node" + QString::number(label);
		QString line3=QString::number(coord.x) + " ; " + QString::number(coord.y) + " ; " + QString::number(coord.z) ;
		QString line5=QString::number(feData->nodeList[index].getResultValue());
		append(line1);
		append("Coords:");
		append(line3);
		append("Result Value:");
		append(line5);
	}	

}
//****************************************************************************
//SLOT
/*!Shows information of a point*/
void modelInfoView::showPointInformation(long label)
{
    long index;
        vector3d coord;
        if (label > 0)
        {
                index = geoData->getPointIndex(label);
                coord = geoData->getPointCoords(label);

                clear();
                QString line1="Point" + QString::number(label);
                QString line3=QString::number(coord.x) + " ; " + QString::number(coord.y) + " ; " + QString::number(coord.z) ;

                append(line1);
                append("Coords:");
                append(line3);

        }

}
//******************************************************************************
void modelInfoView::showPlaneInformation(long label)
{

		clear();
		QString line1="Plane " + QString::number(label);
		append(line1);
		append("Origin:");
		//append(line3);
		append("Normal");
		//append(line5);
}
//******************************************************************************
void modelInfoView::showMaterialInformation(int index)
{

        clear();
        QString line1="Material " + QString::number(index);
        append(line1);
        append(QString("Name: ")+feData->materialList[index].name);
        append(QString("YModulus: ")+QString::number(feData->materialList[index].elasticDataList[0].yModulus));
        append(QString("Poisson: ")+QString::number(feData->materialList[index].elasticDataList[0].poison));
        append(QString("Density: ")+QString::number(feData->materialList[index].densityList[0].value));


}
//******************************************************************************
void modelInfoView::showSectionInformation(int index)
{

        clear();
        QString line1="FE-Section " + QString::number(index);
        append(line1);
        append(QString("Name: ")+feData->feSectionList[index].name);
        append(QString("Set: ")+QString(feData->feSectionList[index].set));
        append(QString("Material: ")+QString(feData->feSectionList[index].material));
        //append(QString("Density: ")+QString::number(feData->fesectionList[index].densityList[0].value));


}
//******************************************************************************
void modelInfoView::showBCInformation(int index)
{

        clear();
        QString line1="BC " + QString::number(index);
        append(line1);
        append(QString("Name: ")+feData->bcList[index].name);
        append(QString("Set: ")+QString(feData->bcList[index].nodeset));
        append(QString("First DOF: ")+QString::number(feData->bcList[index].firstDof));
        append(QString("Last DOF: ")+QString::number(feData->bcList[index].lastDof));
        append(QString("Value: ")+QString::number(feData->bcList[index].value));
        //append(QString("Last DOF: ")+QString::number(feData->fesectionList[index].densityList[0].value));

}
