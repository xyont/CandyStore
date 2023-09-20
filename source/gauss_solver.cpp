#include "gauss_solver.h"
#include <math.h>

gauss_solver::gauss_solver()
{
}
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
void gauss_solver::FGAUSS(int N,double* A,int IA,double* Y,double* X, int* MARKE, double* D, int* IPIVOT)
{
        /*C
C*****************************************************************
C                                                                *
C  LOESUNG EINES LINEAREN GLEICHUNGSSYSTEMS   A * X = Y  NACH    *
C  DEM ELIMINATIONSVERFAHREN VON GAUSS MIT SKALIERTER SPALTEN-   *
C  PIVOTSUCHE (VGL. ABSCHNITT 4.5.1).                            *
C                                                                *
C                                                                *
C  EINGABEPARAMETER:                                             *
C  =================                                             *
C  N        : ORDNUNG DES GLEICHUNGSSYSTEMS.                     *
C  A        : 2-DIM. FELD A(1:IA,1:N); MATRIX A DES GLEICHUNGS-  *
C             SYSTEMS, (A = A(ORG)).                             *
C  IA       : FUEHRENDE DIMENSION VON A, WIE IM RUFENDEN PRO-    *
C             GRAMM VEREINBART.                                  *
C  Y        : 1-DIM. FELD Y(1:N); RECHTE SEITE DES GLEICHUNGS-   *
C             SYSTEMS.                                           *
C                                                                *
C                                                                *
C  AUSGABEPARAMETER:                                             *
C  =================                                             *
C  A        : 2-DIM. FELD A(1:IA,1:N), DAS DIE ZERLEGUNGSMATRI-  *
C             ZEN C UND B VON  P * A(ORG) = C * B  ENTHAELT MIT  *
C             P = PERMUTATIONSMATRIX, C = NORMIERTE UNTERE DREI- *
C             ECKSMATRIX UND  B = OBERE DREIECKSMATRIX.          *
C  X        : 1-DIM. FELD X(1:N); LOESUNG DES GLEICHUNGSSYSTEMS. *
C  MARKE    : = 1, GERADE ANZAHL VON ZEILENVERTAUSCHUNGEN.       *
C             =-1, UNGERADE ANZAHL VON ZEILENVERTAUSCHUNGEN.     *
C             = 0, EINGABEMATRIX A IST NUMERISCH SINGULAER.      *
C             FUER DIE DETERMINANTE GILT:                        *
C                DET(A(ORG)) = MARKE*A(1,1)*...*A(N,N).          *
C  D        : 1-DIM. FELD D(1:N); KEHRWERT DER ABSOLUTEN ZEILEN- *
C             SUMMEN (SKALIERUNGSFAKTOREN) VON A(ORG):           *
C             D(I) = 1./(ABS(A(I,1)) + ... + ABS(A(I,N)))  FUER  *
C             I = 1(1)N.                                         *
C  IPIVOT   : 1-DIM. FELD IPIVOT(1:N); GIBT DIE ZEILENVERTAU-    *
C             SCHUNGEN BEI DER SKALIERTEN SPALTENPIVOTSUCHE AN   *
C             UND LEGT DAMIT DIE PERMUTATIONSMATRIX  P  FEST.    *
C             IST Z.B. IPIVOT(2) = 7, SO BEDEUTET DIES, DASS DIE *
C             7. ZEILE VON A(ORG) DIE 2. ZEILE VON  P * A(ORG)   *
C             IST.                                               *
C                                                                *
C----------------------------------------------------------------*
C
C  Dokumentation:
C  --------------
C  Formelsammlung zur Numerischen Mathematik mit Standard-
C  FORTRAN-77-Programmen
C       Prof. Dr. Gisela Engeln-Muellges
C       o. Prof.em.Dr. Fritz Reutter
C  6., voellig neu bearbeitete und erweiterte Auflage 1988
C  BI Wissenschaftsverlag Mannheim/Wien/Zuerich
C  ISBN 3-411-03112-3
C
C----------------------------------------------------------------
C                                                                *
C  BENOETIGTE UNTERPROGRAMME: FGAUSZ, FGAUSL, MASCH              *
C                                                                *
C                                                                *
C  QUELLEN   : FORMELSAMMLUNG ZUR NUMERISCHEN MATHEMATIK MIT     *
C              STANDARD-FORTRAN 77 PROGRAMMEN VON                *
C              G. ENGELN-MUELLGES, F. REUTTER, 5. AUFLAGE 1986.  *
C                                                                *
C*****************************************************************
C                                                                *
C  AUTOREN   : GISELA ENGELN-MUELLGES, GUIDO DUBOIS              *
C  DATUM     : 25.04.88                                          *
C  QUELLCODE : FORTRAN 77                                        *
C                                                                *
C*****************************************************************
C*/

/*
C  ZERLEGUNG DER MATRIX A MIT HILFE DER SUBROUTINE FGAUSZ.
C*/
    FGAUSZ(N,A,IA,IPIVOT,MARKE,D);
/*C
C  VORWAERTS- UND RUECKWAERTSELIMINATION MIT HILFE DER SUBROUTINE
C  FGAUSL, UM DIE LOESUNG DES GLEICHUNGSSYSTEMS ZU BERECHNEN.
C*/
    if (*MARKE == 0)
    {
        FGAUSL(N,A,IA,IPIVOT,Y,X);
    }
}

//*****************************************************************
int gauss_solver::MASCHD(double X)
{
    if(1E0 < X)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}
//*****************************************************************
void gauss_solver::FGAUSZ(int N,double* A, int IA,int* IPIVOT,int* MARKE, double* D)
{
/*
C*****************************************************************
C                                                                *
C  ZERLEGUNG EINER MATRIX A IN DAS PRODUKT DER MATRIZEN C UND B  *
C  SO, DASS GILT  P * A = C * B  MIT P = PERMUTATIONSMATRIX,     *
C  C = NORMIERTE UNTERE DREIECKSMATRIX UND B = OBERE DREIECKSMA- *
C  TRIX NACH DEM ELIMINATIONSVERFAHREN VON GAUSS MIT SKALIERTER  *
C  SPALTENPIVOTSUCHE.                                            *
C                                                                *
C                                                                *
C  EINGABEPARAMETER:                                             *
C  =================                                             *
C  N        : ORDNUNG DES GLEICHUNGSSYSTEMS.                     *
C  A        : 2-DIM. FELD A(1:IA,1:N); MATRIX A DES GLEICHUNGS-  *
C             SYSTEMS, (A = A(ORG)).                             *
C  IA       : FUEHRENDE DIMENSION VON A, WIE IM RUFENDEN PRO-    *
C             GRAMM VEREINBART.                                  *
C                                                                *
C                                                                *
C  AUSGABEPARAMETER:                                             *
C  =================                                             *
C  A        : 2-DIM. FELD A(1:IA,1:N), DAS DIE ZERLEGUNGSMATRI-  *
C             ZEN C UND B VON  P * A(ORG) = C * B  ENTHAELT MIT  *
C             P = PERMUTATIONSMATRIX. DIE OBERE DREIECKSMATRIX B *
C             WIRD AUF DAS OBERE DREIECK VON A GESPEICHERT, DIE  *
C             NORMIERTE UNTERE DREIECKSMATRIX C  OHNE DIE DIAGO- *
C             NAL-EINSEN AUF DAS UNTERE DREIECK VON  A.          *
C  IPIVOT   : 1-DIM. FELD IPIVOT(1:N); GIBT DIE ZEILENVERTAU-    *
C             SCHUNGEN BEI DER SKALIERTEN SPALTENPIVOTSUCHE AN   *
C             UND LEGT DAMIT DIE PERMUTATIONSMATRIX  P  FEST.    *
C             IST Z.B. IPIVOT(2) = 7, SO BEDEUTET DIES, DASS DIE *
C             7. ZEILE VON A(ORG) DIE 2. ZEILE VON  P * A(ORG)   *
C             IST.                                               *
C  MARKE    : = 1, GERADE ANZAHL VON ZEILENVERTAUSCHUNGEN.       *
C             =-1, UNGERADE ANZAHL VON ZEILENVERTAUSCHUNGEN.     *
C             = 0, EINGABEMATRIX A IST NUMERISCH SINGULAER.      *
C             FUER DIE DETERMINANTE GILT:                        *
C                DET(A(ORG)) = MARKE*A(1,1)*...*A(N,N).          *
C  D        : 1-DIM. FELD D(1:N); KEHRWERT DER ABSOLUTEN ZEILEN- *
C             SUMMEN (SKALIERUNGSFAKTOREN) VON A(ORG):           *
C             D(I) = 1./(ABS(A(I,1)) + ... + ABS(A(I,N)))  FUER  *
C             I = 1(1)N.                                         *
C                                                                *
C----------------------------------------------------------------*
C
C  Dokumentation:
C  --------------
C  Formelsammlung zur Numerischen Mathematik mit Standard-
C  FORTRAN-77-Programmen
C       Prof. Dr. Gisela Engeln-Muellges
C       o. Prof.em.Dr. Fritz Reutter
C  6., voellig neu bearbeitete und erweiterte Auflage 1988
C  BI Wissenschaftsverlag Mannheim/Wien/Zuerich
C  ISBN 3-411-03112-3
C
C----------------------------------------------------------------
C                                                                *
C  BENOETIGTE UNTERPROGRAMME: MASCHD                             *
C                                                                *
C                                                                *
C  QUELLEN   : FORMELSAMMLUNG ZUR NUMERISCHEN MATHEMATIK MIT     *
C              STANDARD-FORTRAN 77 PROGRAMMEN VON                *
C              G. ENGELN-MUELLGES, F. REUTTER, 5. AUFLAGE 1986.  *
C                                                                *
C*****************************************************************
C                                                                *
C  AUTOREN   : GISELA ENGELN-MUELLGES, GUIDO DUBOIS              *
C  DATUM     : 25.04.88                                          *
C  QUELLCODE : FORTRAN 77                                        *
C                                                                *
C*****************************************************************
C*/

    *MARKE=1;
/*C
C  BERECHNUNG DER MASCHINENGENAUIGKEIT UND SETZEN DES RELATIVEN
C  FEHLERS.
C*/
    double SUM=0;
    double FMACHN=1.0;
    double PIVOT;
    double HILF;
    double FAK;
    int IPVT;
    int J;
    {
       FMACHN=0.5*FMACHN;
    }while (MASCHD(1E0+FMACHN) == 1);

//    double RELERR=8*FMACHN;
/*C
C  BERECHNEN DER ABSOLUTEN ZEILENSUMMEN UND VORBESETZEN DES
C  PIVOTVEKTORS.
C*/
    for (int I=0;I<N;I++)
    {
       IPIVOT[I]=I+1;
       SUM=fabs(A[I*N]);
       for (int K=1;K<N;K++)
       {
          SUM=SUM+fabs(A[I*N+K]);
       }
       if(SUM==0)
       {
          *MARKE=0;
          return;
       }
       else
       {
          D[I]=1.0/SUM;
       }
    }
    if(N==1)
    {
        return;
    }
/*C
C  DREIECKSZERLEGUNG.
C*/
    for (int I=0;I<(N-1);I++)
    {
/*C
C  PIVOTZEILE BESTIMMEN.
C*/
       PIVOT=fabs(A[I*N+I])*D[I];
       IPVT=I;
       for (J=I+1;J<N;J++)
       {
          HILF=fabs(A[J*N+I])*D[J];
          if(HILF > PIVOT)
          {
             PIVOT=HILF;
             IPVT=J;
          }
       }
       if(PIVOT == 0E+0)
       {
          *MARKE=0;
          return;
       }
       else
       {
          if(! IPVT == I)
          {
/*C
C  I-TE UND IPVT-TE ZEILE VERTAUSCHEN.
C*/
             *MARKE=-1*(*MARKE);
             J=IPIVOT[I];
             IPIVOT[I]=IPIVOT[IPVT];
             IPIVOT[IPVT]=J;
             HILF=D[I];
             D[I]=D[IPVT];
             D[IPVT]=HILF;
             for ( J=0;J<N;J++)
             {
                HILF=A[I*N+J];
                A[I*N+J]=A[IPVT*N+J];
                A[IPVT*N+J]=HILF;
             }
          }
/*C
C  ELIMINATIONSSCHRITT AUSFUEHREN.
C*/
          for (int J=I+1;J<N;J++)
          {
             A[J*N+I]=A[J*N+I]/A[I*N+I];
             FAK=A[J*N+I];
             for (int K=I+1;K<N;K++)
             {
                A[J*N+K]=A[J*N+K]-FAK*A[I*N+K];
            }
         }
      }
   }
    if(fabs(A[(N-1)*N+(N-1)]) == 0E+0)
    {
        *MARKE=0;
    }
    return;
}
////////////////////////////////////////////////////////////C
/////////////////////////////////////////////////////////////
void gauss_solver::FGAUSL(int N,double* A,int IA,int* IPIVOT,double* Y,double* X)
{

/*

C
C*****************************************************************
C                                                                *
C  BERECHNUNG DER LOESUNG EINES LINEAREN GLEICHUNGSSYSTEMS       *
C  A * X = Y, NACHDEM A MIT HILFE DER SUBROUTINE FGAUSZ NACH     *
C  DEM ELIMINATIONSVERFAHREN VON GAUSS ZERLEGT WURDE.            *
C                                                                *
C                                                                *
C  EINGABEPARAMETER:                                             *
C  =================                                             *
C  N        : ORDNUNG DES GLEICHUNGSSYSTEMS.                     *
C  A        : 2-DIM. FELD A(1:IA,1:N)  MIT DEN ZERLEGUNGSMATRI-  *
C             ZEN  C  UND  B  VON  P * A(ORG) = C * B  MIT       *
C             P = PERMUTATIONSMATRIX.                            *
C             AUSGABEMATRIX DER SUBROUTINE FGAUSZ.               *
C  IA       : FUEHRENDE DIMENSION VON A, WIE IM RUFENDEN PRO-    *
C             GRAMM VEREINBART.                                  *
C  IPIVOT   : 1-DIM. FELD IPIVOT(1:N); GIBT DIE ZEILENANORDNUNG  *
C             DER ZERLEGUNGSMATRIX  P * A(ORG)  IN BEZUG AUF     *
C             A(ORG) AN.  AUSGABEVEKTOR DER SUBROUTINE FGAUSZ.   *
C  Y        : 1-DIM. FELD Y(1:N); RECHTE SEITE DES GLEICHUNGS-   *
C             SYSTEMS.                                           *
C                                                                *
C                                                                *
C  AUSGABEPARAMETER:                                             *
C  =================                                             *
C  X        : 1-DIM. FELD X(1:N); LOESUNG DES GLEICHUNGSSYSTEMS. *
C                                                                *
C----------------------------------------------------------------*
C
C  Dokumentation:
C  --------------
C  Formelsammlung zur Numerischen Mathematik mit Standard-
C  FORTRAN-77-Programmen
C       Prof. Dr. Gisela Engeln-Muellges
C       o. Prof.em.Dr. Fritz Reutter
C  6., voellig neu bearbeitete und erweiterte Auflage 1988
C  BI Wissenschaftsverlag Mannheim/Wien/Zuerich
C  ISBN 3-411-03112-3
C
C----------------------------------------------------------------
C                                                                *
C  BENOETIGTE UNTERPROGRAMME: KEINE                              *
C                                                                *
C                                                                *
C  QUELLEN   : FORMELSAMMLUNG ZUR NUMERISCHEN MATHEMATIK MIT     *
C              STANDARD-FORTRAN 77 PROGRAMMEN VON                *
C              G. ENGELN-MUELLGES, F. REUTTER, 5. AUFLAGE 1986.  *
C                                                                *
C*****************************************************************
C                                                                *
C  AUTOREN   : GISELA ENGELN-MUELLGES, GUIDO DUBOIS              *
C  DATUM     : 25.04.88                                          *
C  QUELLCODE : FORTRAN 77                                        *
C                                                                *
C*****************************************************************
C*/
    double SUM=0;
    int IPVT;

    if(N==1)
    {
       X[0]=Y[0]/A[0];
       return;
    }
/*C
C  VORWAERTSELIMINATION DURCHFUEHREN.
C*/
    IPVT=IPIVOT[0];
    X[0]=Y[IPVT];
    for (int I=1;I<N;I++)
    {
       SUM=0.0;
       for (int J=0;J<I-1;J++)
       {
          SUM=SUM+A[I*N+J]*X[J];
       }
       IPVT=IPIVOT[I];
       X[I]=Y[IPVT]-SUM;
    }
/*C
C  BERECHNEN DER LOESUNG DURCH RUECKWAERTSELIMINATION.
C*/
    X[N-1]=X[N-1]/A[(N-1)*N+(N-1)];
    for (int I=N-2;I>-1;I--)
    {
       SUM=0.0;
       for (int K=N-1;I+1;I--)
        {
          SUM=SUM+A[I*N+K]*X[K];
        }
       X[I]=(X[I]-SUM)/A[I*N+I];
   }
   return;
}
