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
 
 #include "commandline.h"
 
 commandLine::commandLine()
 {
	
	stringBuffer.clear();
	connect(this, SIGNAL(returnPressed()), this, SLOT(newCommand()));
	bufferIndex=0;
 }
 
 void commandLine::keyPressEvent(QKeyEvent *e)
{
	bool handled = false;
	int size=0;
	if (e->key()==Qt::Key_Up)
	{
		size=stringBuffer.size();
		if(size==0)
			return;
		if (bufferIndex >0 )
					bufferIndex--;
		handled=true;
		setText(stringBuffer[bufferIndex]);
		
	}
	else if (e->key()==Qt::Key_Down)
	{
		
		size=stringBuffer.size();
		if (size==0)
			return;
		if (bufferIndex>size-1)
			bufferIndex=size-1;
		handled=true;
		if (bufferIndex <size-1)
			bufferIndex++;
		setText(stringBuffer[bufferIndex]);
		
	}
	if (!handled)
	{
		QLineEdit::keyPressEvent(e);
	}
}

void commandLine::newCommand()
{
	
	if (stringBuffer.size() > 19)
	{
		stringBuffer.removeFirst();
		
	}
	stringBuffer.append(this->text());
	bufferIndex=stringBuffer.size();
	
}
