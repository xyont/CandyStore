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
 * \file deleteCOSWidget.h
 * \brief A brief file description
 **/
#ifndef deleteCOSWidget_H
#define deleteCOSWidget_H

#include <QtGui>
//#include "vector3d/vector3d.h"

    class QLineEdit;
    class QHBoxLayout;
   // class QVboxLayout;
    class QLabel;
    class QPushButton;
//!   Create COS Widget
/*!
        The Create COS Widget provides functionality to control the
        creation of coordinate systems,...
 */
    class deleteCOSWidget : public QWidget
    {
        Q_OBJECT

    public:
        deleteCOSWidget(QWidget *parent=0);
        ~deleteCOSWidget();

        QPushButton *closeButton;
        QPushButton *deleteButton;
        QComboBox *cosSelect;

        QLabel *header;
        QLabel *cosSelectHeader;


        public slots:
        void fillSelectCosBox(const QString&);
        void clearCosBox();


        private slots:
        void deleteCos();


        signals:


        };
#endif
