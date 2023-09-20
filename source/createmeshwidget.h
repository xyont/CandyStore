/***************************************************************************
 *   Copyright (C) 2011 by Patrick Prokopczuk                              *
 *   prokopczuk@gmx.de                                                     *
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

#ifndef CREATEMESHWIDGET_H
#define CREATEMESHWIDGET_H

#include <QtGui>

class createMeshWidget : public QWidget
{
    Q_OBJECT
public:
    createMeshWidget(QWidget *parent = 0);
    QPushButton *closeButton;
    QPushButton *startButton;
    QCheckBox *secondOrderCheck;

    QLabel *header;
    QLabel *sourceHeader;
    QComboBox *sourceComboBox;
    QLabel *actionHeader;
    QComboBox *actionComboBox;

    QLabel *hMaxLabel;
    QLineEdit *hMaxEdit;
 //   public slots:



    private slots:
     void startClicked();

    signals:
    //! Sends a  QString//
    void sendMessage(const QString&);
    void startMesh(int,double,int);


};

#endif // CREATEMESHWIDGET_H
