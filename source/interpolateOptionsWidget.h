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
 * \file interpolateOptionsWidget.h
 * \brief A brief file description
 **/
#ifndef interpolateOptionsWidget_H
#define interpolateOptionsWidget_H

    #include <QtGui>

    class QLineEdit;
    class QHBoxLayout;
	class QVboxLayout;
    class QLabel;
	class QPushButton;
//!   Interpolate Options Widget
/*! 
	The Interpolate Options Widget provides functionality to control the 
	interpolate options,...
 */
    class interpolateOptionsWidget : public QWidget
    {
        Q_OBJECT

    public:
	interpolateOptionsWidget(QWidget *parent=0);
	~interpolateOptionsWidget();
	
        QComboBox *resultFileType;
	QPushButton *closeButton;
	QPushButton *startButton;
	QCheckBox *allNodes;
	QCheckBox *outsideNodes;
	QCheckBox *outsideFaces;
        QCheckBox *outsideResults;
	
	QSpacerItem *vSpacer;
	QSpacerItem *vSpacer2;
	QLabel *header;
        QLabel *ResultFileTypeHeader;
	QLabel *ResultFileHeader;
	QLabel *targetSelectHeader;
	QLabel *fringeMaxHeader;
	QLineEdit *DispScalEdit;
	QLineEdit *fringeMinEdit;
	QLineEdit *fringeMaxEdit;
	QPushButton *DispScalButton;
	QPushButton *fringeMinButton;
	QPushButton *fringeMaxButton;
	QString resultFile;
	QString lastInterpolDirectory;
	
	public slots:
	
	
	
	private slots:
	void changeResultFile();
	void startInterpol();
	
	
	signals:
	//! Sends a  QString 
	void sendMessage(const QString&);
	//! Starts the interpolation
        void startInterpolation(const QString&);
        void startInterpolationVtkUnstructured(const QString&);
        void startInterpolationVtkMultiblock(const QString&);

	//! Starts the face interpolation
	void startFaceInterpolation(const QString&);
        void startFaceInterpolationVtkUnstructured(const QString&);
        void startFaceInterpolationVtkMultiblock(const QString&,bool);
	};
#endif
