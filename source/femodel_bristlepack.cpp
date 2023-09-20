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
#include "femodel.h"
#include <math.h>
#include "vector3d/vector3d.h"


/*! Creates a mesh of a bristle pack*/
void femodel::createBristlePack(double alpha, double beta, double rb, double h1, double Gesamthoehe, double d, int AnzXRichtung,int AnzReihen, double diffx, double diffz, int AnzMittel)
{
    clearDatabase();
///////////////////////////////////////////////////
    tempNode.setOutside(false);
//diffx*sin(alpha*M_PI/180)<d
    double deltax=d/sin(alpha*M_PI/180);
    double deltaReihe=pow((0.75*pow(d,2)),0.5)*1.01;
    sets tempSet;
    struct Element		//globale Struktur, Datensatz            
    {
        int nummer;
        int m;
        int n;
        int o;
        int p;
        int q;
        int r;
        int s;
        int t;
    };
    QList<node> masterKnoten;
    masterKnoten.clear();
    QList<element> masterElements;
    element tempElement;
    node tempNode;

    //Definitionen

    //double alpha;			//Legewinkel
    //double beta;			//Einspannwinkel
    double phi;				//Winkel den der Radius überspannt
    //double rb;				//Radius des Bogens
    //double h1;				//Höhe Abschnittsende 1
     double h2;				//Höhe Abschnittsende 2
    double h3=Gesamthoehe;				//Höhe Abschnittsende 3=radiale Höhe der Bürste
    double l1;				//Länge 1. Abschnitt
    double l2;				//Länge 2. Abschnitt
    double l3;				//Länge 3. Abschnitt
    double lg;				//Gesamtlänge der Bürste bzw des Drahtes

    double zw1;				//Zwischenwerte falls sie gebraucht werden
    double zw2;
    int AnzBorsten=0;
    //int AnzMittel;			//Anzahl der Mittelpuntke --> Kreisebenen je Borste
    //int AnzXRichtung;		//Anzahl der Borsten in der jeweiligen Richtung im Paket
   // int AnzZRichtung;

    //Berechnung der Mittellinie

        phi = beta-alpha;
        l2 = rb*M_PI/180*phi;
        zw1 = beta*M_PI/180;					//für sin und cos im Bogenmaß
        l1 = h1/sin(zw1);
        zw1 = 0.5*M_PI-(beta*M_PI/180);			//90°=M_PI/2
        zw2 = alpha*M_PI/180;
        h2 = h1+rb*(cos(zw2)-sin(zw1));
        zw1 = alpha*M_PI/180;
        l3 = (h3-h2)/sin(zw1);
        lg = l1+l2+l3;
        //Berechnen der Paketdaten
        AnzBorsten = AnzReihen*AnzXRichtung;
        //
        /**********Knoten der Mittellinie**********/

        //cout << "\nerzeuge mittellinienknoten.txt";

        //Definitionen

            int AnzAbschnitt1;
            int AnzAbschnitt2;
            int AnzAbschnitt3;
            double Schritt;					//Schrittweite entlang Gesamtlänge
            double phiSchritt;				//Winkelschrittweite
            int i,j,k,l;					//Laufvariablen

          //  Ziel3 << "**Art der Vernetzung :\t\t\t\t" << AnzMittel << " Kreisebenen" << endl;

            double xRichtung[AnzMittel+1];
            double yRichtung[AnzMittel+1];

        //Funktionsabschnitte berechnen und in das Feld einlesen

            AnzAbschnitt1 = l1/lg*AnzMittel;	//Anzahl abschnittsweiser Punkte

            AnzAbschnitt2 = l2/lg*AnzMittel;
            AnzAbschnitt3 = l3/lg*AnzMittel;
            emit sendMessage(QString("a1 %1 a2 %2 a3 %3").arg(AnzAbschnitt1).arg(AnzAbschnitt2).arg(AnzAbschnitt3));
            if (AnzAbschnitt1+AnzAbschnitt2+AnzAbschnitt3 > (AnzMittel-2))
                AnzAbschnitt3 = AnzAbschnitt3 -1;
            Schritt = lg/AnzMittel;				//äquidistante Schrittweite
            Schritt= l1/(AnzAbschnitt1+1);
            for(i=0;i<=AnzAbschnitt1;i++)
                {
                    xRichtung[i] = (i)*Schritt*cos(beta*M_PI/180);
                    yRichtung[i] = (i)*Schritt*sin(beta*M_PI/180);

                    if(xRichtung[i]<=1*exp(-9))			//vs numerische Abweichungen ohne Betrag
                        xRichtung[i]=0;
                }
            //Schritt= l2/AnzAbschnitt2;
            for(j=0;j<=AnzAbschnitt2;j++)
                {
                    zw1 = (90-beta)*M_PI/180;							//definiere gamma = 90°-beta
                    phiSchritt = Schritt/rb*180/M_PI;					//Schritt hier Bogenlänge
                    zw2 = (90-beta)*M_PI/180+j*phiSchritt*M_PI/180;		//gamma+j*dphi >beides in rad

                    xRichtung[j+i] = l1*cos(beta*M_PI/180)+rb*cos(zw1)-rb*cos(zw2);
                    yRichtung[j+i] = h1-rb*sin(zw1)+rb*sin(zw2);

                    if(xRichtung[j+i]<=1*exp(-9))
                        xRichtung[j+i]=0;
                }
            //Schrittweite fürletzten Abschnitt anpassen
            Schritt=l3/ AnzAbschnitt3;
            for(k=0;k<=AnzAbschnitt3;k++)
                {
                    xRichtung[k+j+i] = l1*cos(beta*M_PI/180)+rb*cos((90-beta)*M_PI/180)-rb*sin(alpha*M_PI/180)+(k)*Schritt*cos(alpha*M_PI/180);
                    yRichtung[k+j+i] = h2+(k)*Schritt*sin(alpha*M_PI/180);
                }

        //Initialisierung des letzten Endpunktes, sodass die radiale Höhe eingehalten wird
        //beim 1. und 2.Abschnitt wird Bedingung eingehalten, nächster Abschnitt beginnt auf richtiger Höhe
        //3. Abschnitt
            xRichtung[AnzMittel] = l1*cos(beta*M_PI/180)+rb*cos((90-beta)*M_PI/180)-rb*sin(alpha*M_PI/180)+l3*cos(alpha*M_PI/180);
            yRichtung[AnzMittel] = h3;

            // **********Erzeugung der Kreisknoten********** /

            //cout << "\nerzeuge kreisknoten.txt";

            //Definitionen

                double r;
                double a;						//Faktor, beschreibt den Abstand zum Mittelpunkt
                double psi;						//Laufvariable, beschreibt den Winkel im Kreis
                double xKreisKnoten[73];		//73 Knoten pro Kreis (18 * 4 + 1 Mitte + 1 Speicherplatz
                double yKreisKnoten[73];

                r=d/2;

            //Kreisknoteninitialisierung

                for(i=1;i<=18;i++)
                {
                    switch(i)
                    {
                        case 1 :	a=0.25;		psi=0*M_PI/180;		break;
                        case 2 :	a=0.5;		psi=0*M_PI/180;		break;
                        case 3 :	a=0.75;		psi=0*M_PI/180;		break;
                        case 4 :	a=1;		psi=0*M_PI/180;		break;
                        case 5 :	a=0.746154;	psi=15*M_PI/180;	break;
                        case 6 :	a=1;		psi=15*M_PI/180;	break;
                        case 7 :	a=0.530769;	psi=26*M_PI/180;	break;
                        case 8 :	a=0.784615;	psi=30*M_PI/180;	break;
                        case 9 :	a=1;		psi=30*M_PI/180;	break;
                        case 10:	a=0.353553;	psi=45*M_PI/180;	break;
                        case 11:	a=0.646154;	psi=45*M_PI/180;	break;
                        case 12:	a=0.846154;	psi=45*M_PI/180;	break;
                        case 13:	a=1;		psi=45*M_PI/180;	break;
                        case 14:	a=0.784615;	psi=60*M_PI/180;	break;
                        case 15:	a=1;		psi=60*M_PI/180;	break;
                        case 16:	a=0.530769;	psi=64*M_PI/180;	break;
                        case 17:	a=0.746154;	psi=75*M_PI/180;	break;
                        case 18:	a=1;		psi=75*M_PI/180;	break;
                        default:	a=0;		psi=0;
                    }

                    xKreisKnoten[i]=a*r*cos(psi);			//1.Quadrant
                    yKreisKnoten[i]=a*r*sin(psi);

                    xKreisKnoten[i+18]=a*r*cos(psi+M_PI/2);	//2.Quadrant
                    yKreisKnoten[i+18]=a*r*sin(psi+M_PI/2);

                    xKreisKnoten[i+36]=a*r*cos(psi+M_PI);	//3.Quadrant
                    yKreisKnoten[i+36]=a*r*sin(psi+M_PI);

                    xKreisKnoten[i+54]=a*r*cos(psi+3*M_PI/2);	//4.Quadrant
                    yKreisKnoten[i+54]=a*r*sin(psi+3*M_PI/2);
                }

            //vs. numersiche Abweichung mit Betrag

                for(i=0;i<=73;i++)
                    {
                    if(fabs(xKreisKnoten[i])<=1*exp(-9))
                        xKreisKnoten[i]=0;
                    if(fabs(yKreisKnoten[i])<=1*exp(-9))
                        yKreisKnoten[i]=0;
                    }

            //der Mittelpunkt s.o.

                xKreisKnoten[0]=0;
                yKreisKnoten[0]=0;


                //**********Erzeugung Knotendatei********** /

                //cout << "\nerzeuge nodes.msh und fix.bou";

                //Zusammenrechnen, wieder abschnittsweise --> 3 Abschnitte der Mittellinie

                  //  double xKO[73*AnzMittel*100+1];		//benötigt nur 73 Punkte pro Kreis * Anzahl Mittelp...
                  //  double yKO[73*AnzMittel*100+1];		//...da aber der Index mit 100 multipliziert wird...
                  //  double zKO[73*AnzMittel*100+1];		//...greift Programm auf entsprechend höhere Feldelemente zu

                    for(i=0;i<=AnzAbschnitt1;i++)			//für jeden Punkt i im ersten Abschnitt...
                    {
                        for(j=0;j<=72;j++)					//...Koordinaten eines ganzen Kreis einschreiben...
                            {

                                tempNode.setLabel(i*100+j+1);
                                tempNode.setCoordinates(xRichtung[i]-xKreisKnoten[j]*sin(beta*M_PI/180),yRichtung[i]+xKreisKnoten[j]*cos(beta*M_PI/180), yKreisKnoten[j]);
                                masterKnoten.append(tempNode);

                            }
                    }

                    for(k=0;k<=AnzAbschnitt2;k++)		//im zweiten Abschnitt...
                    {
                        for(j=0;j<=72;j++)
                            {
                                zw1 = (90-beta)*M_PI/180;				//gamma = 90°-beta
                                phiSchritt = Schritt/rb*180/M_PI;		//Schritt hier Bogenlänge
                                zw2 = zw1+k*phiSchritt*M_PI/180;		//gamma + k*dphi > beides in rad

                                tempNode.setLabel((k+i)*100+j+1);
                                tempNode.setCoordinates(xRichtung[k+i]-xKreisKnoten[j]*cos(zw2),yRichtung[k+i]+xKreisKnoten[j]*sin(zw2), yKreisKnoten[j]);
                                masterKnoten.append(tempNode);

                            }
                    }

                    for(l=0;l<=AnzAbschnitt3;l++)		//im dritten Abschnitt...
                    {
                        for(j=0;j<=72;j++)
                            {
                                tempNode.setLabel((l+k+i)*100+j+1);
                                if (l<AnzAbschnitt3)
                                {
                                    tempNode.setCoordinates(xRichtung[l+k+i]-xKreisKnoten[j]*sin(alpha*M_PI/180),yRichtung[l+k+i]+xKreisKnoten[j]*cos(alpha*M_PI/180), yKreisKnoten[j]);
                                }
                                else
                                {
//                                    tempNode.setCoordinates(xRichtung[l+k+i]-xKreisKnoten[j]+((Gesamthoehe-(yRichtung[l+k+i]+xKreisKnoten[j]*cos(alpha*M_PI/180)))*cos(alpha*M_PI/180)),Gesamthoehe, yKreisKnoten[j]);
                                    tempNode.setCoordinates(xRichtung[l+k+i]-(xKreisKnoten[j]/sin(alpha*M_PI/180)),Gesamthoehe, yKreisKnoten[j]);

                                }
                                masterKnoten.append(tempNode);

                            }
                    }

                    //Borsten erzeugen

                    for (int rr=0;rr<AnzReihen;rr++)
                    {
                        for (int x=0;x<AnzXRichtung;x++)
                        {
                            tempSet.name="FIX_" + QString::number(rr) + QString::number(x);
                            for(long m=0;m<masterKnoten.size();m++)
                            {
                                tempNode.setLabel(masterKnoten[m].getLabel()+rr*100000+x*10000);
                                tempNode.setCoordinates(masterKnoten[m].getCoordinates());
                                tempNode.move(deltax*x+0.5*rr*deltax,0,deltaReihe*rr);
                                nodeList.append(tempNode);
                                //
                                if (m<73)
                                {
                                    tempSet.nodeList.append(tempNode.getLabel());
                                }
                            }
                            //Create a set for the boundary condition for each bristle and the bc
                            tempSet.qCommandFlag=false;
                            setList.append(tempSet);

                            tempBc.setValues(tempSet.name + "_fix",tempSet.name,1,3,0);

                            bcList.append(tempBc);
                            tempSet.nodeList.clear();
                        }
                    }
                    emit sendMessage(QString("%1 Nodes created").arg(nodeList.size()));

                    // **********Erzeugung der Elemente********** /

                    //cout << "\nerzeuge element.msh";

                    //Definitionen

                        Element element[60]; 	//Anzahl Kreiszwischenräume
                        int m,n,o,p;

                        i=0;		//Rücksetzen der Laufparameter
                        j=0;

                    //Initialisieren

                        for(i=1;i<=15;i++)		//15 Elemente pro Viertel
                        {
                            switch(i)
                            {
                                case 1 :	m=1;	n=2;	o=11;	p=20;	break;
                                case 2 :	m=2;	n=3;	o=8;	p=11;	break;
                                case 3 :	m=3;	n=4;	o=6;	p=8;	break;
                                case 4 :	m=4;	n=5;	o=7;	p=6;	break;

                                case 5 :	m=20;	n=11;	o=17;	p=21;	break;
                                case 6 :	m=11;	n=8;	o=12;	p=17;	break;
                                case 7 :	m=8;	n=6;	o=9;	p=12;	break;
                                case 8 :	m=6;	n=7;	o=10;	p=9;	break;

                                case 9 :	m=21;	n=17;	o=18;	p=22;	break;
                                case 10:	m=17;	n=12;	o=15;	p=18;	break;
                                case 11:	m=12;	n=9;	o=13;	p=15;	break;
                                case 12:	m=9;	n=10;	o=14;	p=13;	break;

                                case 13:	m=22;	n=18;	o=19;	p=23;	break;
                                case 14:	m=18;	n=15;	o=16;	p=19;	break;
                                case 15:	m=15;	n=13;	o=14;	p=16;	break;

                                default:	m=0;	n=0;	o=0;	p=0;
                            }

                            element[i-1].nummer = i;		//1. Viertel
                            element[i-1].m = m; 			//aktuelle Ebene
                            element[i-1].n = n;
                            element[i-1].o = o;
                            element[i-1].p = p;
                            element[i-1].q = m+100;			//nächste Ebene: +100
                            element[i-1].r = n+100;
                            element[i-1].s = o+100;
                            element[i-1].t = p+100;

                            element[i-1+15].nummer = i+15;	//2. Viertel; Knotenoffset: +18; Elementoffset: +15
                            element[i-1+15].m = m+18;
                            element[i-1+15].n = n+18;
                            element[i-1+15].o = o+18;
                            element[i-1+15].p = p+18;
                            element[i-1+15].q = m+100+18;
                            element[i-1+15].r = n+100+18;
                            element[i-1+15].s = o+100+18;
                            element[i-1+15].t = p+100+18;

                            element[i-1+30].nummer = i+30;	//3. Viertel; Knotenoffset: +36; Elementoffset: +30
                            element[i-1+30].m = m+36;
                            element[i-1+30].n = n+36;
                            element[i-1+30].o = o+36;
                            element[i-1+30].p = p+36;
                            element[i-1+30].q = m+100+36;
                            element[i-1+30].r = n+100+36;
                            element[i-1+30].s = o+100+36;
                            element[i-1+30].t = p+100+36;

                            element[i-1+45].nummer = i+45;	//4. Viertel; Knotenoffset: +54; Elementoffset: +45
                            element[i-1+45].m = m+54;
                            element[i-1+45].n = n+54;
                            element[i-1+45].o = o+54;
                            element[i-1+45].p = p+54;
                            element[i-1+45].q = m+100+54;
                            element[i-1+45].r = n+100+54;
                            element[i-1+45].s = o+100+54;
                            element[i-1+45].t = p+100+54;
                        }

                    //besondere Elemente

                        element[15].m = element[0].m;		//bei den mittleren Elementen...
                        element[15].n = element[0].p;
                        element[15].o = element[16].p;
                        element[15].p = element[19].m;
                        element[15].q = element[0].q;
                        element[15].r = element[0].t;
                        element[15].s = element[16].t;
                        element[15].t = element[19].q;

                        element[30].m = element[15].m;		//...(also Elementnummer 1,16,31,46)...
                        element[30].n = element[15].p;
                        element[30].o = element[31].p;
                        element[30].p = element[34].m;
                        element[30].q = element[15].q;
                        element[30].r = element[15].t;
                        element[30].s = element[31].t;
                        element[30].t = element[34].q;

                        element[45].m = element[30].m;		//...hat der Mittelpunkt die Knotenummer 0
                        element[45].n = element[30].p;
                        element[45].o = element[46].p;
                        element[45].p = element[49].m;
                        element[45].q = element[30].q;
                        element[45].r = element[30].t;
                        element[45].s = element[46].t;
                        element[45].t = element[49].q;

                        element[45].m = element[0].m;		//da ein Kreis nur 73 Knoten hat...
                        element[45].n = element[30].p;		//...und das Programm hier weiterzählen würde,...
                        element[45].o = element[46].p;		//...werden die überschneidenden Punkte...
                        element[45].p = element[0].n;		//...extra initialisiert
                        element[45].q = element[0].q;
                        element[45].r = element[30].t;
                        element[45].s = element[46].t;
                        element[45].t = element[0].r;

                        element[49].m = element[1].m;
                        element[49].n = element[45].o;
                        element[49].o = element[50].p;
                        element[49].p = element[1].n;
                        element[49].q = element[1].q;
                        element[49].r = element[45].s;
                        element[49].s = element[50].t;
                        element[49].t = element[1].r;

                        element[53].m = element[2].m;
                        element[53].n = element[49].o;
                        element[53].o = element[54].p;
                        element[53].p = element[2].n;
                        element[53].q = element[2].q;
                        element[53].r = element[49].s;
                        element[53].s = element[54].t;
                        element[53].t = element[2].r;

                        element[57].m = element[3].m;
                        element[57].n = element[53].o;
                        element[57].o = element[58].p;
                        element[57].p = element[3].n;
                        element[57].q = element[3].q;
                        element[57].r = element[53].s;
                        element[57].s = element[58].t;
                        element[57].t = element[3].r;


                    //Ausgabe


                                    //für nur eine Borste

                          //  Ziel4 << "*ELEMENT, TYPE=C3D8,ELSET=BORSTE" << cc+1 << endl;
                          //  Ziel6 << "BORSTE" << cc+1 << "," << endl;
                    for (int rrr=0;rrr<AnzReihen;rrr++)
                    {
                        for (int xx=0;xx<AnzXRichtung;xx++)
                        {
                            for(j=0;j<AnzMittel;j++)
                            {
                                for(i=0;i<60;i++)
                                {

                                    tempElement.label=element[i].nummer+j*100+xx*10000+rrr*100000;
                                    tempElement.type=1;
                                    tempElement.nodeList.append(element[i].m+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].n+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].o+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].p+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].q+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].r+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].s+j*100+xx*10000+rrr*100000);
                                    tempElement.nodeList.append(element[i].t+j*100+xx*10000+rrr*100000);

                                    elementList.append(tempElement);
                                    tempElement.nodeList.clear();
                                    //Ziel4 << element[i].nummer+j*100 << "," << element[i].m+j*100 << "," << element[i].n+j*100 << "," << element[i].o+j*100 << "," << element[i].p+j*100 << "," << element[i].q+j*100 << "," << element[i].r+j*100 << "," << element[i].s+j*100 << "," << element[i].t+j*100 << endl;
                                }
                            }
                        }
                    }
// ************************************************************
//Create the mpcs between first and last bristle in each row
    for (int row=0;row<AnzReihen;row++)
    {
        //Start with 1 because the first nodes are fixed -> no mpcs needed
        //Stop with AnzMIttel-1 one , because only inner nodes should be tied -> Contact Problems otherwise
        for (int t=1;t<(AnzMittel-1);t++)
        {
            tempMpc.setValues(row*100000+t*100+1,row*100000+t*100+1+(AnzXRichtung-1)*10000,1);
            mpcList.append(tempMpc);
            tempMpc.setValues(row*100000+t*100+1,row*100000+t*100+1+(AnzXRichtung-1)*10000,2);
            mpcList.append(tempMpc);
            tempMpc.setValues(row*100000+t*100+1,row*100000+t*100+1+(AnzXRichtung-1)*10000,3);
            mpcList.append(tempMpc);
        }
    }

                    emit sendMessage("Getting max model coords");
                    if (nodeList.size()>0)
                    {
                        maxCoords=getMaxModelCoords();
                        setModelCenter();
                    }
                    emit sendMessage("Getting faces");
                    getFaces();
                    emit sendMessage("Getting outside edges");
                    getOutsideEdges();
                    emit sendMessage("Getting outside nodes");
                    getOutsideNodes();
                    emit sendMessage("Getting outside elements");
                    getOutsideElements();
}
