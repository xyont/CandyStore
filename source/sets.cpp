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
 * \file sets.cpp
 * \brief A brief file description
 **/
#include "sets.h"

sets::sets()
{
    nodePlotColor=0;

}
//**************************************************
long sets::getNodeIndex(long label,int *exists)
{
    unsigned long links=1;
    unsigned long rechts=nodeList.size();
    unsigned long mitte;

            while(1)
            {
            if (links>rechts)
                {
                *exists=0;
                return(rechts);
                }
            mitte=(links+rechts)/2;
            if (nodeList[mitte-1]==label)
            {
                *exists=1;
                return(mitte-1);
            }

            if (label<nodeList[mitte-1])
                    rechts=mitte-1;
            else
                    links=mitte+1;
            }
}
//**************************************************
long sets::getFaceIndex(long label,int *exists)
{
    unsigned long links=1;
    unsigned long rechts=faceIndexList.size();
    unsigned long mitte;

            while(1)
            {
            if (links>rechts)
                {
                *exists=0;
                return(rechts);
                }
            mitte=(links+rechts)/2;
            if (faceIndexList[mitte-1]==label)
            {
                *exists=1;
                return(mitte-1);
            }

            label<faceIndexList[mitte-1] ? rechts=mitte-1 : links=mitte+1;
            }
}
//**************************************************
long sets::getElementIndex(long label,int *exists)
{
    unsigned long links=1;
    unsigned long rechts=elementList.size();
    unsigned long mitte;

            while(1)
            {
            if (links>rechts)
                {
                *exists=0;
                return(rechts);
                }
            mitte=(links+rechts)/2;
            if (elementList[mitte-1]==label)
            {
                *exists=1;
                return(mitte-1);
            }

            if (label<elementList[mitte-1])
                    rechts=mitte-1;
            else
                    links=mitte+1;
            }
}
