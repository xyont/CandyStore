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
//
#ifndef CREATEBOUNDARYWIDGET_H
#define CREATEBOUNDARYWIDGET_H

#include <QtGui>

class QLineEdit;
class QHBoxLayout;
class QVboxLayout;
class QLabel;
class QPushButton;

class createBoundaryWidget : public QWidget
{
    Q_OBJECT

public:
    createBoundaryWidget(QWidget *parent=0);
    ~createBoundaryWidget();



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
    QLineEdit *firstDOF;
    QLineEdit *lastDOF;
    QLineEdit *name;
    QLineEdit *value;
    QLabel *nameLabel;
    QLabel *firstDOFLabel;
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

    signals:
    //! Sends a  QString
    void sendMessage(const QString&);
    //! Starts the BC creation
    void startBCCreation(QString,QString, int,int,double);

};

#endif // CREATEBOUNDARYWIDGET_H
