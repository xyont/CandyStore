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
 * \file resultOptionsWidget.h
 * \brief A brief file description
 **/
#ifndef resultOptionsWidget_H
#define resultOptionsWidget_H

    #include <QtGui>

    class QLineEdit;
    class QHBoxLayout;
	class QVboxLayout;
    class QLabel;
	class QPushButton;
//!   Result Options Widget
/*! 
	The Result Options Widget provides functionality to control the 
	result options, such as min/max fringe values, displacement scale factor,...
 */
    class resultOptionsWidget : public QWidget
    {
        Q_OBJECT

    public:
	resultOptionsWidget(QWidget *parent=0);
	~resultOptionsWidget();
	
	
	QPushButton *closeButton;
	QCheckBox *showUndef;
	QCheckBox *fringeMinAuto;
	QCheckBox *fringeMaxAuto;
	
	QSpacerItem *vSpacer;
	QSpacerItem *vSpacer2;
	QLabel *header;
	QLabel *DispScalHeader;
	QLabel *fringeMinHeader;
	QLabel *fringeMaxHeader;
	QLineEdit *DispScalEdit;
	QLineEdit *fringeMinEdit;
	QLineEdit *fringeMaxEdit;
	QPushButton *DispScalButton;
	QPushButton *fringeMinButton;
	QPushButton *fringeMaxButton;
	
	public slots:
	
	void toggleShowUndeformedMesh();
	
	private slots:
	
	void setFringeMaxEditable(bool edit);
	void setFringeMinEditable(bool edit);
	void newFringeMin();
	void newFringeMax();
	void newDispScal();
	void showUndeformedChanged(bool status);
	
	signals:
	//! Sends a  QString 
	void sendMessage(const QString&);
	//! Sends the new value of the displacement scale factor
	void dispScalChanged(float value);
	//! Emitted when the max fringe value is set to auto mode
	void setFringeMaxAuto();
	//! Emitted when the min fringe value is set to auto mode
	void setFringeMinAuto();
	//! Sends a signal with the min fringe value
	void setFringeMin(double value);
	//! Sends a signal with the max fringe value
	void setFringeMax(double value);
	//! Emitted when the status of the show undeformed mesh checkbox is changed
	void setShowUndeformedMesh(bool stat);
	};
#endif
