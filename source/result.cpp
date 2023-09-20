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

#include "result.h"
#include <math.h>
#define	 _princMin 1.E-10 
#define	 _princMin2 1.E-16 
#define  _gl3grad_Pi          3.14159265358979323846264338327950288
result::result()
{
	clearResultData();
}


void result::clearResultData()
{
	componentLabels.clear();
	numberComponents=0;
	resultValueList.clear();

}

void result::addComponent(const QString &label)
{
	componentLabels.append(label);
	numberComponents++;
	minValues.append(0.0);
	maxValues.append(0.0);
}
/*! Calculates the magnitude of displacement results and adds the component to the resultdata
*/
void result::calcDispMagnitude()
{
	int comps=numberComponents;
	double sumsquare;
	addComponent("Mag");
	for (int i=0; i<resultValueList.size();i++)
	{
		sumsquare=0;
		for (int c=0; c<comps; c++)
		{
			sumsquare+=pow(resultValueList[i].resultValues[c],2);
		}
		resultValueList[i].resultValues.append(sqrt(sumsquare));
		if (sqrt(sumsquare)>maxValues[comps])
		{
			maxValues[comps]=sqrt(sumsquare);
		}
	}
}
/**Calculates the Mises stress  
*/
void result::calcMisesStress()
{
	double mises;
	double m;
	double s11,s22,s33,s12,s23,s13;
	int comps=numberComponents;
	addComponent("vMises");
	for (int i=0; i<resultValueList.size();i++)
	{
		mises=0;
		if (comps==6)
		{
		s11=resultValueList[i].resultValues[0];
		s22=resultValueList[i].resultValues[1];
		s33=resultValueList[i].resultValues[2];
		s12=resultValueList[i].resultValues[3];
		s23=resultValueList[i].resultValues[4];
		s13=resultValueList[i].resultValues[5];

		m=pow(s11,2)+pow(s22,2)+pow(s33,2)-s11*s22-s22*s33-s33*s11+3*pow(s12,2)+3*pow(s23,2)+3*pow(s13,2);
		mises=sqrt(m);
		}
	
	resultValueList[i].resultValues.append(mises);
		
	if ( mises>maxValues[comps])
	{
			maxValues[comps]=mises;
	}
	}

}
/**Calculates the principal stresses
 Modified version of code written by K.Wittig
*/ 
int result::calcPrincStress()
{
	double p1,p2,p3,maxP,midP,minP,worstPs;
	double mean=0, y[3],a,b,c,p_[3];
	double s11=0,s22=0,s33=0,s12=0,s23=0,s13=0;
	int comps=numberComponents;
	addComponent("maxPS");
	addComponent("minPS");
	addComponent("worstPS");
	for (int i=0; i<resultValueList.size();i++)
	{
		p1=0;
		p2=0;
		p3=0;
		mean=0;
		if (comps>=6)
		{
			s11=resultValueList[i].resultValues[0];
			s22=resultValueList[i].resultValues[1];
			s33=resultValueList[i].resultValues[2];
			s12=resultValueList[i].resultValues[3];
			s23=resultValueList[i].resultValues[4];
			s13=resultValueList[i].resultValues[5];
		}
		//***************************************************************************
		//Calculate the coefficients for the 3rd order equation
		a=-(s11+s22+s33);
		b= (s11 * s22) + (s22 * s33) + (s33 * s11) - (s12 * s12) - (s23 * s23) - (s13 * s13 );
		c=-( s11*s22*s33 + 2* s12*s23*s13 - s11*s23*s23 - s22*s13*s13 - s33*s12*s12 );
		//****************************************************************************
		//Check wether all principals are zero -> nothing to calculate
		if ( ( fabs(a)<= _princMin2 )&&( fabs(b)<= _princMin2 )&&( fabs(c)<= _princMin2 ) )
		{
			maxP=0;
		}
		//Check wether only one one principal is != zero
		else if ( ( fabs(b)< _princMin2 )&&( fabs(c)< _princMin2 ) )
		{
			maxP=0.;
			midP=0.;
			minP=0.;
			if(-a<0) 
				minP=-a;
			else     
				maxP=-a;
		}
		/* check wether a hydrostatic situation exists ((s11=s22=s33)*/
		else if(( fabs((s11-s22)/mean)<_princMin) &&  (fabs((s11-s33)/mean)<_princMin)  )
		{
			/* Shear stresses have to be zero*/
			if( (fabs(s12/mean)<_princMin) &&  (fabs(s23/mean)<_princMin) &&  (fabs(s13/mean)<_princMin)  )
			{
				a=0; 
				a=a+s11+s22+s33;
				a*=0.33333333;
				p1=a;
				p2=a;
				p3=a;
				return (1);
			}
			if ( gl3grades( a, b, c, p_ ) < 0 )  
				return (-2);
		}
		else if ( gl3grades( a, b, c, p_ ) < 0 )  
			return (-2);
		//Assign the correct values to maxP,minP,midP,worstP)	
		
		y[0]=p_[0];
		y[1]=p_[1];
		y[2]=p_[2];
		
		if ( (y[0]>=y[1]) && (y[0]>=y[2]) ) /* 1st greatest */
		{
			maxP=p_[0];
			if (y[1]>y[2])
			{
				midP=p_[1];
				minP=p_[2];
			}
			else
			{
				midP=p_[2];
				minP=p_[1];
			}
		}
		else if ( (y[1]>=y[0]) && (y[1]>=y[2]) ) /* 2nt greatest */
		{
			maxP=p_[1];
			if (y[0]>y[2])
			{
				midP=p_[0];
				minP=p_[2];
			}
			else
			{
				midP=p_[2];
				minP=p_[0];
			}
		}
		else if ( (y[2]>=y[0]) && (y[2]>=y[1]) ) /* 3rd greatest */
		{
			maxP=p_[2];
			if (y[0]>y[1])
			{
				midP=p_[0];
				minP=p_[1];
			}
			else
			{
				midP=p_[1];
				minP=p_[0];
			}
		}
		else
		{
			//do nothing
		} 
		//calculate worstPS
		if (fabs(maxP)> fabs(minP))
			worstPs=maxP;
		else
			worstPs=minP;
		//Append the rsults to the result data
		//maxPrinc
		resultValueList[i].resultValues.append(maxP);
	
		if ( maxP>maxValues[comps])
		{
			maxValues[comps]=maxP;
		}
		if ( maxP<minValues[comps])
		{
			minValues[comps]=maxP;
		}
		//minPrinc
		resultValueList[i].resultValues.append(minP);
		if ( minP>maxValues[comps+1])
		{
			maxValues[comps+1]=minP;
		}
		if ( minP<minValues[comps+1])
		{
			minValues[comps+1]=minP;
		}
		//worstPrinc
		resultValueList[i].resultValues.append(worstPs);
		if ( worstPs>maxValues[comps+2])
		{
			maxValues[comps+2]=worstPs;
		}
		if ( worstPs<minValues[comps+2])
		{
			minValues[comps+2]=worstPs;
		}
	}
return(1);
}

/* ---------------------------------------------------------------- */
/* loest gl 0=x**3+ax**2+bx+c gl3grades nach Bronstein S.118        */
/* ACHTUNG: nur fuer eigenwertsuche symmetrischer matrizen          */
/* ---------------------------------------------------------------- */
/**Solves 0=x^3+ax^2+bx+c
Code written by K.Wittig
*/ 
int result::gl3grades( double a, double b, double c, double *x)
{
  double p,q,phi;

  p= (b - a*a/3.)/3.;
  q= (a*a*a*2./27. - a*b/3. + c)/2.;
  
  p= sqrt(fabs(p))*fabs(q)/q;
  phi=  q /p/p/p ;
  if(phi>1.0) 
	phi=0.; 
  else if (phi<-1.0) 
	phi=_gl3grad_Pi;
  else 
	phi = acos( phi );
  
  x[0]= -2*p*cos(phi/3.)  -a/3.;
  x[1]=  2*p*cos(_gl3grad_Pi/3.- phi/3.)  -a/3.;
  x[2]=  2*p*cos(_gl3grad_Pi/3.+ phi/3.)  -a/3.;

  return (1);
}
//*****************************************************************************
/*! Get the index of node label in the result data list. If the node is not in the list -1 is returned*/
long result::getIndex(long label)
{
    unsigned long links=1;
    unsigned long rechts=resultValueList.size();
    unsigned long mitte;
        while(1)
        {
            if (links>rechts) return (-1);
        mitte=(links+rechts)/2;
        if (resultValueList[mitte-1].nodeLabel==label) return(mitte-1);

        if (label<resultValueList[mitte-1].nodeLabel)
            rechts=mitte-1;
        else
            links=mitte+1;
        }
}
