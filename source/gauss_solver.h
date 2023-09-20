/***************************************************************************
 *   Copyright (C) 2010 by Patrick Prokopczuk                              *
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
#ifndef GAUSS_SOLVER_H
#define GAUSS_SOLVER_H

//!  methods to solve an equation solver with the direct elimination method by Gauss
/*!

 */

class gauss_solver
{
public:
    gauss_solver();


    static void FGAUSZ(int N,double* A, int IA,int* IPIVOT,int* MARKE, double* D);
    static int MASCHD(double);
    static void FGAUSS(int N,double* Matrix,int IA,double* Y,double* X, int* MARKE, double* D, int* IPIVOT);
    static void FGAUSL(int N,double* A,int IA,int* IPIVOT,double* Y,double* X);
};

#endif // GAUSS_SOLVER_H
