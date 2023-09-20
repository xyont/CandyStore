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
/**
 * \file createToolbars.cpp
 * \brief A brief file description
 **/
 #include <QtGui>
 #include "mainWin.h"
 //*****************************************************************************
/*!Create the menues in the top menu bar*/
 void MainWindow::createMenus()
 {
     fileMenu = menuBar()->addMenu(tr("&File"));
     //fileMenu->addAction(newAct);
     fileMenu->addAction(openAct);
     fileMenu->addAction(openCSDAct);
     fileMenu->addAction(importAct);
     fileMenu->addAction(importInpAct);
     fileMenu->addAction(importCDBAct);
     fileMenu->addAction(importVTKAct);
     fileMenu->addAction(importSTLAct);
     fileMenu->addAction(runFBDAct);
     fileMenu->addAction(exportFrdAct);
     fileMenu->addAction(exportVTKAct);
     fileMenu->addAction(exportOpenFoamAct);
     fileMenu->addAction(saveAsAct);
     separatorAct = fileMenu->addSeparator();
     for (int i = 0; i < MaxRecentFiles; ++i)
     fileMenu->addAction(recentFileActs[i]);
     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);
     updateRecentFileActions();

     menuBar()->addSeparator();

     //geometryMenu = menuBar()->addMenu(tr("Geometry"));
     //geoCreateMenu = geometryMenu->addMenu(tr("Create Geometry"));

     optionsMenu = menuBar()->addMenu(tr("&Options"));
     optionsMenu->addAction(graphicOptionsAct);
     optionsMenu->addAction(setHelpFileAct);
     optionsMenu->addAction(setCcxLocationAct);

     // optionsMenu->addAction(setBackgroundColorAct);

    resultMenu = menuBar() ->addMenu(tr("&Result"));
    resultMenu->addAction(resultOptionAct);

    utilityMenu= menuBar()->addMenu(tr("&Utilities"));
    utilityMenu->addAction(writeOutsideFacesAct);
    utilityMenu->addAction(interpolateResultsAct);
    utilityMenu->addAction(hex8toHex20Act);
    utilityMenu->addAction(createDiscAct);


    analysisMenu= menuBar()->addMenu(tr("&Analysis"));
    analysisMenu->addAction(abqInputDeckAct);
    //analysisMenu->addAction();


     helpMenu = menuBar()->addMenu(tr("&Help"));
     //helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutMouseBindingsAct);
     helpMenu->addAction(aboutQtAct);
 }
