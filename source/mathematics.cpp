#include "mathematics.h"
#include <math.h>
mathematics::mathematics()
{

}
//Code mit modifitierungen aus :
//Karlheinz Zeiner
//Programmieren lernen mit C
//
//4. Auflage
//HANSER
//ISBN 3-446-21596-4
//******************************
//  Gauss'sches Eliminationsverfahren mit Zeilenpivotisierung
//    Argumente:
//    double a[N][N+1] erweiterte Koeffizientenmatrix   Read/Write
//    int n            Anzahl der Gleichungen           Read
//    double x[N]      Loesungen                        Write
//  Resultat:
//  int Fehlercode  0 fuer Fehler, 1 fuer Erfolg

int mathematics::Gaussalg (double a[][NMAXGAUSS], int n, double x[])
{
  int   i, j;                    // Zeile, Spalte
  int   s;                       // Elimininationsschritt
  int   pzeile;                  // Pivotzeile
  int   fehler = 0;              // Fehlerflag
  double f;                      // Multiplikationsfaktor
  const double Epsilon = 0.01;   // Genauigkeit
  double Maximum;                // Zeilenpivotisierung


  s = 0;
  do {             // die einzelnen Eliminationsschritte

    Maximum = fabs(a[s][s]);   // groesstes Element
    pzeile = s ;               // suchen
    for (i = s+1; i < n; i++)
      if (fabs(a[i][s]) > Maximum) {
        Maximum = fabs(a[i][s]) ;
        pzeile = i;
      }
    fehler = (Maximum < Epsilon);
    if (fehler) break;           // nicht loesbar

    if (pzeile != s)  // falls erforderlich, Zeilen tauschen
    { double h;
      for (j = s ; j <= n; j++) {
        h = a[s][j];
        a[s][j] = a[pzeile][j];
        a[pzeile][j]= h;
      }
    }

    // Elimination --> Nullen in Spalte s ab Zeile s+1
    for (i = s + 1; i < n; i++ ) {
      f = -(a[i][s]/a[s][s]);       // Multiplikationsfaktor
      a[i][s] = 0.0;
      for (j = s+1; j <= n ; j++)   // die einzelnen Spalten
        a[i][j] += f*a[s][j];       // Addition der Zeilen i, s
    }

    s++;
  } while ( s < n-1 ) ;

  if (fehler)
  {

    return 0;
  }
  else
  {
    // Berechnen der Loesungen aus der entstandenen Dreiecksmatrix
    // letzte Zeile
    x[n-1] =  a[n-1][n] / a[n-1][n-1];
    // restliche Zeilen
    for (i = n-2 ; i >= 0; i-- )
    {
      for (j = n-1 ; j > i ; j-- )
      {
          a[i][n] -= x[j]*a[i][j];    // rechte Seite berechnen
      }
      x[i] = a[i][n] / a[i][i];       // Loesung
    }
    return 1;
  }

}

