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
 * \file mergeNodesWidget.h
 * \brief A brief file description
 **/
#ifndef mergeNodesWidget_H
#define mergeNodesWidget_H

    #include <QtGui>

    class QLineEdit;
    class QHBoxLayout;
        class QVboxLayout;
    class QLabel;
        class QPushButton;
//!   Merge Nodes Widget
/*!
        The Merge Nodes Widget provides functionality to control merging of nodes

 */
    class mergeNodesWidget : public QWidget
    {
        Q_OBJECT

    public:
        mergeNodesWidget(QWidget *parent=0);
        ~mergeNodesWidget();


        QPushButton *closeButton;
        QPushButton *startButton;
        QCheckBox *lowerNodes;
        QCheckBox *higherNodes;
        QCheckBox *outsideFaces;

        QSpacerItem *vSpacer;
        QSpacerItem *vSpacer2;
        QLabel *header;
        QLabel *coordinateHeader;
        QLabel *targetSetHeader;
        QComboBox *targetSetComboBox;
        QLabel *fringeMaxHeader;
        QLineEdit *yEdit;
        QLineEdit *xEdit;
        QLineEdit *zEdit;
        QPushButton *DispScalButton;
        QPushButton *fringeMinButton;
        QPushButton *fringeMaxButton;
        QString resultFile;
        QString lastInterpolDirectory;
        int modeFlag;

        public slots:



        private slots:
        void startMerge();
        void toggleKeepNodesLow(bool);
        void toggleKeepNodesHigh(bool);

        signals:
        //! Sends a  QString
        void sendMessage(const QString&);
        //! Starts merging
        void mergeNodes(double,int);
        void mergeNodesSet(double,int,int);
        };
#endif
