/***************************************************************************
 *   Copyright (C) 2011 by Patrick Prokopczuk   *
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
#ifndef CREATELOADSWIDGET_H
#define CREATELOADSWIDGET_H

#include <QtGui>
#include "femodel.h"

class createLoadsWidget : public QWidget
{
    Q_OBJECT
public:
    createLoadsWidget(QWidget *parent = 0, femodel *fe=0);

    femodel *feData;
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
    QLabel *rotAxisHeader;
    QLabel *point1Header;
    QLabel *point2Header;
    QComboBox *targetSetComboBox;
    QLabel *typeHeader;
    QComboBox *typeComboBox;
    QLabel *fringeMaxHeader;
    QLineEdit *direction;
    QLineEdit *lastDOF;
    QLineEdit *name;
    QLineEdit *value;
    QLineEdit *point1Edit;
    QLineEdit *point2Edit;
    QLabel *nameLabel;
    QLabel *directionLabel;
    QLabel *lastDOFLabel;
    QLabel *valueLabel;
    QPushButton *DispScalButton;
    QPushButton *fringeMinButton;
    QPushButton *fringeMaxButton;
    QString resultFile;
    QString lastInterpolDirectory;
    int modeFlag;

    public slots:



    private slots:
        void startClicked();
        void loadTypeChanged(int);

    signals:
    //! Sends a  QString
    void sendMessage(const QString&);
    //! Starts the load creation
    void startLoadCreation(QString,int,QString, double,int);
    void startCentrifugalLoadCreation(QString,QString,QString,QString,double);

};

#endif // CREATELOADSWIDGET_H
