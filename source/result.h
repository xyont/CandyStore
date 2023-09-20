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
 /**
 * \file result.h
 * \brief A brief file description
 **/
#ifndef RESULT_H
#define RESULT_H

#include <QtGui>
#include "nodalresults.h"

class result
{
public:
result();

QString resultLabel;
QStringList componentLabels;
unsigned int numberComponents;
QList<nodalResults> resultValueList; 
QList<double> minValues;
QList<double> maxValues;

void clearResultData();
void addComponent(const QString &label);

void calcDispMagnitude();
void calcMisesStress();
int calcPrincStress();
long getIndex(long label);

private:
int gl3grades( double a, double b, double c, double *x);

};
#endif
