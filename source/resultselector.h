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
 * \file resultselector.h
 * \brief A brief file description
 **/
#ifndef resultSelector_H
#define resultSelector_H

    #include <QWidget>
	#include "femodel.h"

    class QTextEdit;
    class QComboBox;
    class QLineEdit;
    class QHBoxLayout;
    class QLabel;

    class resultSelector : public QWidget
    {
        Q_OBJECT

    public:
    resultSelector(QWidget *parent=0, femodel *md=0);
	QComboBox *stepSelector;
	QComboBox *typeSelector;
	QStringList components;
	int currentStep;
	int currentResultIndex;
	int currentComp;
	QString currentResultString;
	QString currentCompString;
   QComboBox *componentSelector;
   
   
   // protected:
   //     void closeEvent(QCloseEvent *event);

    public slots:
	void newStepSelected(int index);
	private slots:
	void newComponentSelected(int index);
	void newResultSelected(int title);
	void newFileActions();
        
    private:
	QHBoxLayout *resultsLayout;
	femodel *modelData;
	void updateResultBoxStep();
    
    signals:
        void loadStep(int stepnumber);
	void fringePlotAvailable(short status);
        void updateFaces(int);
};
#endif
