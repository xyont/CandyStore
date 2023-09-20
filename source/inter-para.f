      program inter 
C-----------------------------------------------------------------------
C     Maximal zul�ssige Knotenzahl (Felddimensionierung in Unterprogrammen)
      integer nmax
C     Maximal zul�ssige Knotennummer (Felddimensionierung in Unterprogrammen)
      integer nrmax
C     Anzahl der Parameter in der Kommandozeile      
      integer n
C     Filenamen (neue Knoten, Ergebnisknoten, Interpoliert)
      character*40  filenod, fileerg, fileout
      integer nin,nerg,nout
C-----------------------------------------------------------------------
C-    Maximal zul�ssige Knotenzahl (Felddimensionierung in Unterprogrammen)
c      nmax=10000
C-    Maximal zul�ssige Knotennummer (Felddimensionierung in Unterprogrammen)
c      nrmax=100000
C-    Filenamen aus der Kommandozeile �bernehmen
      N=IARGC() 
      if (N.ne.2) then
         print*
         print*,'ERROR: unerwartete Anzahl von Parametern'
	 print*,'  usage: inter-para <newgeo.frd> <ergebnis.frd>'
	 print*,'  <newgeo.frd>   : FRD-File mit neuer Geometrie, ',
     +          'auf die gemappt werden soll'
	 print*,'  <ergebnis.frd> : FRD-File mit Ergebnissen '
	 print*,'  Ergebnisse werden geschrieben nach: inter-para.frd'
	 print*
	 stop
      endif
      call GETARG(1, filenod) 
      call GETARG(2, fileerg) 
C-    Filenamen zuweisen
C      filenod='nodes.frd'
C      fileerg='ergeb.frd'
      fileout='inter-para.frd'
C---  Files �ffnen      
      nout=9
      nerg=8
      nin=7
      open(nin,file=filenod)
      open(nerg,file=fileerg)
      open(nout,file=fileout)
C---  maximale Knoten und Elementnummern in den benutzten Files feststellen      
      call CheckNum(nnmax,nemax,mnmax,memax,nin)
      nmax=max(nnmax,nemax)
      mrmax=max(mnmax,memax)
      call CheckNum(nnmax,nemax,mnmax,memax,nerg)
      if (nmax.lt.max(nnmax,nemax)) nmax=max(nnmax,nemax)
      if (nrmax.lt.max(mnmax,memax)) nrmax=max(mnmax,memax)
C     Files zur�ckspulen
      rewind(nin)
      rewind(nerg)
C---  Eigentliches Programm aufrzufen      
      call InterPara(nmax,nrmax,nin,nerg,nout,filenod,fileerg,fileout)
C---  Files nach Programmende schlie�en      
      close(nout)
      close(nin)
      close(nerg)
      stop
C-----------------------------------------------------------------------
      end
      
      
      subroutine InterPara(nmax,nrmax,nin,nerg,nout,
     +                     filenod,fileerg,fileout)
C-----------------------------------------------------------------------
C Finden von dem Element in anderem Modell, in dem der neue Knoten enthalten ist
C Ermitteln der Parameter, die den nuen Knoten im alten Modell beschreiben
C Aufmappen der Eregebnisse auf den neuen Knoten mit Hilfe der Parameter
C-----------------------------------------------------------------------
C benutzte Unterprogrammsammlungen:
C     frd-tools.f
C     interpolate-3d.f
C-----------------------------------------------------------------------
      implicit none
C--   Hilfsgr��en
C     Laufvariable
      integer i,j,n
      integer form
      character*1   a1
      character*80  a80
      character*110 a110
      character*8 a8
C     Maximal zul�ssige Knotenanzahl benutzt zur Felddimensionierung
      integer nmax
C     Maximal zul�ssige Knotennummer benutzt zur Felddimensionierung
      integer nrmax
C--   Knoten und Elementinformationen
C     Knotenanzahlen (neue Knoten, Ergebnisknoten)
      integer nnd,ned
C     Elementanzahlen (Ergebniselemente)
      integer melem
C     Knotennummern (neue Knoten, Ergebnisknoten)
      integer nn(nmax), ne(nmax)
C     Knotenreihenfolgennummern 
      integer nr(nmax)
C     Knotenkoordinaten x,y,z (neue Knoten, Ergebnisknoten)
      real x(nmax), y(nmax), z(nmax)
      real xe(nmax),ye(nmax),ze(nmax)
C     Elementnummern, Elementknoten, Knotenzahl je Element
      integer mel(nmax), mnn(nmax,20), mnd(nmax)
C     Anzahl der Elemente je Knoten
      integer nel(nmax)
C     Elemente, die jeweils an einem Knoten H�ngen
C     ACHTUNG! reine Reihenfolgenummern bzgl. der Einlesereihenfolge
C     der Elemente, KEINE absoluten Elementnummern
      integer nelem(nmax,40)
C     n�chste Knoten zum neuen Knoten, 
C     reine Reihenfolgeangabe, keine Knotennummer !!!
      integer nc(nmax)
C     Parameter f�r jeden neuen Knoten im Ergebniselement
      real xi1(nmax), xi2(nmax), xi3(nmax), xi4(nmax)
C     Element (Reihenfolgennummer), in dem der jeweilige Knoten liegt
      integer nelin(nmax)
C--   Ein- und Ausgabe
C     Unit-Nummern f�r Ein- und Ausgaben(neue Knoten, Ergebnisknoten)
      integer nin, nerg, nout
C     Aussprungcode bei Fehler beim Lesen
      integer nexit
C     Filenamen (neue Knoten, Ergebnisknoten, Interpoliert)
      character*40  filenod, fileerg, fileout
C     Flag zum wieder rausschreiben des eingelesenen
      integer nf1
C--   Ergebnisdaten
C     Anzahl der Ergebniskomponenten (real, Liste)
      integer ncp,ncps
C     Ergebniszeit/Frequenz
      real time
C     Ergebnisstensor  (neue Knoten,  Ergebnisknoten)
      real ergn(nmax,6), erg(nmax,6)
C     Lastfallname
      character*6 alc
      character*8 aerg,ax(6),aerc(6)
      integer mtx(6),mty(6),iex(6),ict(6)
      integer mety(nmax)
C     Lastfallbeschreibung
      character*49 abesch
C-----------------------------------------------------------------------
C-    Start der Berechnungen
      print*,' --- Program inter-para ---'

C-----File mit neuen Knoten einlesen
C--   Knoten einlesen
      print*,'neue Knoten einlesen von: ',filenod
C     Flag f�r wiederrausschreiben setzen
      nf1=1
      call ReadNode(nnd,nn,x,y,z,nin,nout,nf1,nmax)
      print*,'   ',nnd,' Knoten gefunden'

C--   Elementinformationen einlesen und wieder rausschreiben, wenn vorhanden
C     Flag f�r wiederrausschreiben setzen
      nf1=1
      call ReadElem(melem,mnn,mel,mety,mnd,nin,nout,nf1,nmax)
      print*,'   ',melem,' Elemente gefunden'

C-----File mit Ergebnissen einlesen
C--   Knoten einlesen
      print*,'Ergebnisse einlesen von: ',fileerg
C     Flag f�r wiederrausschreiben setzen
      nf1=0
      call ReadNode(ned,ne,xe,ye,ze,nerg,nout,nf1,nmax)
      print*,'   ',ned,' Knoten gefunden'

C--   Elementinformationen einlesen und wieder rausschreiben
C     Elementinformationen aus File mit neuen Knoten werden �berschrieben
C     weil sie nicht mehr ben�tigt werden.
C     Flag f�r wiederrausschreiben setzen
      nf1=0
      call ReadElem(melem,mnn,mel,mety,mnd,nerg,nout,nf1,nmax)
      print*,'   ',melem,' Elemente gefunden'

C-----zu jedem Knoten die daran h�ngenden Elemente finden
      print*,'Knoten-Elementzuordung erstellen'
      call FindElNod(ned,melem,ne,mnn,mnd,nel,nelem,nmax)

C-----zu jedem Knoten die zur Nummer geh�rende Reihenfolgenummer finden
      call FindNode(ned,ne,nr,nrmax,nmax)

C-----zu jedem neuen Knoten n�chsten Ergebnisknoten finden
      print*,'Nachbarknoten suchen'
      call FindClose(nn,x,y,z,ne,xe,ye,ze,nnd,ned,nc,nmax)

C-----zu jedem Knoten das Element, in dem er liegt bzw. das n�chste finden
C     und die zugeh�rigen Parameter ermitteln
      print*,'Parameter bestimmen'
      call FindPara(nnd,x,y,z,xe,ye,ze,mnn,nc,nr,mety,
     +              nel,nelem,nelin,xi1,xi2,xi3,xi4,nmax,nrmax)
     
C-----Ergebnisse einlesen und auf den n�chsten Knoten aufbringen
 234  continue
C        Flag f�r wiederrausschreiben setzen
         nf1=0
C--      Ergebnisse einlesen
         call readerg(ned,nn,erg,alc,time,abesch,aerg,ncp,ncps,
     +                aerc,ict,mtx,mty,iex,ax,nerg,nmax,nexit)
         if (nexit.eq.2) then
	    print*,'   Ergebnis gefunden. STEP: ',alc,' Time: ',time
	    print*,'     Typ: ',aerg,'    ',abesch
         elseif (nexit.eq.1) then
	    print*,'   keine weiteren Ergebnisinformationen gefumden !'
	    goto 998
         elseif (nexit.eq.0) then
            print*,'   --- unerwartetes Fileende --> PROGRAMMABBRUCH !'
	    goto 999
         elseif (nexit.eq.3) then
            print*,'   --- ungleiche Ergebniszahl --> PROGRAMMABBRUCH !'
	    goto 999
         elseif (nexit.eq.4) then
	    print*,'   --- Materialabh�ngige Daten nicht unterst�tzt'
            print*,'   --> PROGRAMMABBRUCH !'
	    goto 999
         endif

C--      Ergbenisse auf benachbarten neuen Knoten aufbringen     
         call MappPara(nnd,nr,ncp,erg,ergn,mnn,mnd,mety,nelin,
     +                    xi1,xi2,xi3,xi4,nrmax,nmax)

C--      Ergbenisse f�r neuen Knoten rausschreiben
         call WriteErg(nnd,nn,ergn,alc,time,abesch,aerg,ncp,ncps,
     +                   aerc,ict,mtx,mty,iex,ax,nout,nmax)

C     n�chsten Ergebnisfall abhandeln
      goto 234
C-----regul�res Programmende
 998  continue
C     FRD-file abschlie�en
      write(nout,1000),9999
      print*,'Ergebnisse geschrieben nach: ', fileout
      print*,' --- Programm regul�r beendet ---'
      
C-    Zieladresse f�r Programmabbr�che
 999  continue
      return
C-----------------------------------------------------------------------
 1000 format(1x,i4)
C-----------------------------------------------------------------------
      end     
 


      subroutine MappPara(nnd,nr,ncp,erg,ergn,mnn,mnd,mety,nelin,
     +                    xi1,xi2,xi3,xi4,nrmax,nmax)
C-----------------------------------------------------------------------
C-----Ergebnisse auf benachbarten Knoten aufbringen
C-----------------------------------------------------------------------
      implicit none
C     maximal zul�ssige Knotenzahl, verwendet f�r Felddimensionierung      
      integer nmax
C     maximal zul�ssige Knotennummer, verwendet f�r Felddimensionierung      
      integer nrmax
C     Ergebnisse  Ergebnisknoten, neuer Knoten      
      real erg(nmax,6),ergn(nmax,6)
C     Elementknotennummern Ergebnisknoten
      integer mnn(nmax,20)
C     Elementtyp Ergebniselemente
      integer mety(nmax)
C     Elementnummer der Ergebniselemente in dem neuer Knoten ist
      integer nelin(nmax)
C     Parametersatz zu neuem Knoten f�r Element in dem er ist
      real xi1(nmax),xi2(nmax),xi3(nmax),xi4(nmax)
C     Ansatzfunktionen f�r aktuelles element
      real l(20)      
C     Anzahl der Elementknoten der Ergebniselemente
      integer mnd(nmax)
C     Knotenreihenfolgenummer von Knotennummer
      integer nr(nrmax)
C     Anzahl der neuen Knoten
      integer nnd
C     Anzahl der Ergebniskomponenten:  Angabe in Liste  und  reale Anzahl
      integer ncp
C     Laufvariablen      
      integer i,j,k
C-----------------------------------------------------------------------
      do 10 i=1,nnd
         if (mety(nelin(i)).eq.1) then
C           Typ1=HEX8
            call anhe8(xi1(i),xi2(i),xi3(i),
     +                 l(1),l(2),l(3),l(4),l(5),l(6),l(7),l(8))
         else if (mety(nelin(i)).eq.2) then
C           Typ2=PENTA6
            call anpe6(xi1(i),xi2(i),xi3(i),xi4(i),
     +                 l(1),l(2),l(3),l(4),l(5),l(6))
         else if (mety(nelin(i)).eq.3) then
C           Typ3=TET4
            call ante4(xi1(i),xi2(i),xi3(i),xi4(i),
     +                 l(1),l(2),l(3),l(4))
         else if (mety(nelin(i)).eq.4) then
C           Typ4=HEX20
            call anhe8(xi1(i),xi2(i),xi3(i),
     +                 l(1),l(2),l(3),l(4),l(5),l(6),l(7),l(8))
C	    die �brigen Ansatzfunktionswerte auf 0 setzen,
C	    weil nur HEX8-Ansatzfunktionen verwendet wurden.
            do 44 j=9,20
	    	l(j)=0
 44	    continue
         else if (mety(nelin(i)).eq.5) then
C           Typ5=PENTA15
            call anpe6(xi1(i),xi2(i),xi3(i),xi4(i),
     +                 l(1),l(2),l(3),l(4),l(5),l(6))
C	    die �brigen Ansatzfunktionswerte auf 0 setzen,
C	    weil nur PENTA6-Ansatzfunktionen verwendet wurden.
            do 45 j=7,15
	    	l(j)=0
 45	    continue
         else if (mety(nelin(i)).eq.6) then
C           Typ6=TET10
            call ante4(xi1(i),xi2(i),xi3(i),xi4(i),
     +                 l(1),l(2),l(3),l(4))
C	    die �brigen Ansatzfunktionswerte auf 0 setzen,
C	    weil nur TET4-Ansatzfunktionen verwendet wurden.
            do 46 j=5,10
	    	l(j)=0
 46	    continue
         endif
	 
C	 Ergebnisse der Elementknoten mit Gewichtung aus Ansatzfunktion
C	 zu neuem Knotenergebnis kombinieren	 
	 do 20 j=1,ncp
	    ergn(i,j)=0.
	    do 30 k=1,mnd(nelin(i))
               ergn(i,j)=ergn(i,j)+erg(nr(mnn(nelin(i),k)),j)*l(k)
 30	    continue
 20      continue
 10   continue
      return
C-----------------------------------------------------------------------
      end

      subroutine FindNode(nnd,nn,nr,nrmax,nmax)
C-----------------------------------------------------------------------
C-----zu jedem Knoten die zur Nummer geh�rende Reihenfolgenummer finden
C-----------------------------------------------------------------------
      implicit none
C     max zul. Knotenanzahl, max zul. Knotennummer (f. Felddimensionierung)      
      integer nmax,nrmax
C     Hilfsgr��en
      integer i
C     Anzahl der Knoten, Knotennummern, Reinhenfolgennummer zu Knotennummer
      integer nnd,nn(nmax),nr(nrmax)
C-----------------------------------------------------------------------
      do 10 i=1,nnd
         nr(nn(i))=i
 10   continue
      return
C-----------------------------------------------------------------------
      end
      
 

      subroutine FindElNod(nnd,melem,nn,mnn,mnd,nel,nelem,nmax)
C-----------------------------------------------------------------------
C-----Finden aller Elemente, die an einem Knoten h�ngen
C-----------------------------------------------------------------------
      implicit none
C     maximal zul�ssige Knotenzahl, verwendet f�r Felddimensionierung      
      integer nmax
C     Anzahl der Knoten, der Elemente
      integer nnd, melem
C     Knotennummern, Elementknotennummern
      integer nn(nmax), mnn(nmax,20)
C     Anzahl der Knoten je Element
      integer mnd(nmax)
C     Anzahl der Elemente je Knoten
      integer nel(nmax)
C     Elemente, die jeweils an einem Knoten H�ngen
C     ACHTUNG! reine Reihenfolgenummern bzgl. der Einlesereihenfolge
C     der Elemente, KEINE absoluten Elementnummern
      integer nelem(nmax,40)
C     Laufvariablen      
      integer i,j,k
C-----------------------------------------------------------------------
      do 10 i=1,nnd
         nel(i)=0
	 do 20 j=1,melem
            do 30 k=1,mnd(j)
	       if (mnn(j,k).eq.nn(i)) then
	          nel(i)=nel(i)+1
		  nelem(i,nel(i))=j
	       endif
 30	    continue
 20      continue
 10   continue
      return
C-----------------------------------------------------------------------
      end

 
      
      subroutine FindClose(nn,x,y,z,ne,xe,ye,ze,nnd,ned,nc,nmax)
C-----------------------------------------------------------------------
C-----finden des jeweils n�chstbenachbarten Knotens
C-----------------------------------------------------------------------
      implicit none
C     maximal zul�ssige Knotenzahl, verwendet f�r Felddimensionierung      
      integer nmax
C     Koordinaten  neue Knoten  und  Ergebnisknoten
      real x(nmax), y(nmax), z(nmax)
      real xe(nmax),ye(nmax),ze(nmax)
C     Abst�nde      
      real dist(nmax),d
C     Knotennummern  neue Knoten,  Ergebnisknoten
      integer nn(nmax),ne(nmax)
C     n�chste Knoten zum neuen Knoten, 
C     reine Reihenfolgeangabe, keine Knotennummer !!!
      integer nc(nmax)
C     Anzahl der  neuen Knoten  und  Ergebnisknoten
      integer nnd,ned
C     Laufvariablen      
      integer i,j
C-----------------------------------------------------------------------
      do 10 i=1,nnd
         dist(i)=1.e3
	 do 20 j=1,ned
            d=sqrt((x(i)-xe(j))**2+(y(i)-ye(j))**2+(z(i)-ze(j))**2)
	    if (d.lt.dist(i)) then
	       dist(i)=d
	       nc(i)=j
	    endif
 20      continue
 10   continue
      return
C-----------------------------------------------------------------------
      end

 
      
      subroutine FindPara(nnd,x,y,z,xe,ye,ze,mnn,nc,nr,mety,
     +                    nel,nelem,nelin,xi1,xi2,xi3,xi4,nmax,nrmax)
C-----------------------------------------------------------------------
C-----zu jedem Knoten das Element, in dem er liegt bzw. das n�chste finden
C     in dem die zugeh�rigen Parameter ermittelt und bewertet werden
C-----------------------------------------------------------------------
      implicit none
C     maximal zul�ssige Knotenzahl, verwendet f�r Felddimensionierung      
      integer nmax
C     maximal zul�ssige Knotennummer, verwendet f�r Felddimensionierung      
      integer nrmax
C     Koordinaten  neue Knoten  und  Ergebnisknoten
      real x(nmax), y(nmax), z(nmax)
      real xe(nmax),ye(nmax),ze(nmax)
C     Elementknotenkoordinaten
      real xee(8), yee(8), zee(8)
C     n�chste Knoten zum neuen Knoten, 
C     reine Reihenfolgeangabe, keine Knotennummer !!!
      integer nc(nmax)
C     Knotenreihenfolgennummern von Knotennummern   
      integer nr(nrmax)
C     Elementknotennummern   
      integer mnn(nmax,20)
C     Anzahl der  neuen Knoten
      integer nnd
C     Elementtyp
      integer mety(nmax)
C     Anzahl der Elemente je Knoten
      integer nel(nmax)
C     Elemente, die jeweils an einem Knoten H�ngen
C     ACHTUNG! reine Reihenfolgenummern bzgl. der Einlesereihenfolge
C     der Elemente, KEINE absoluten Elementnummern
      integer nelem(nmax,40)
C     bestes Element
      integer nbest
C     Index, wie gut der Parametersatz ist
      real index, index1
C     Aktuelles Element
      integer iel
C     Parameter tempor�r
      real xii1(40), xii2(40), xii3(40), xii4(40)
C     Parameter f�r jeden Knoten
      real xi1(nmax), xi2(nmax), xi3(nmax), xi4(nmax)
C     Element (Reihenfolgennummer), in dem der jeweilige Knoten liegt
      integer nelin(nmax)
C-    Hilfsvariablen
C     Laufvariablen      
      integer i,j,k
C-----------------------------------------------------------------------
C     Schleife �ber alle zu findenden Knoten
      do 10 i=1,nnd
         index=10000.
C        Schleife �ber alle Elemente, die am n�chsten Ergebnisknoten h�ngen
	 do 20 j=1,nel(nc(i))
C	    Hilfsgr��e: Reihenfolgennummer des Elemente am Nachbarknoten
	    iel=nelem(nc(i),j)
C           Elementknotenkoordinaten f�r subroutine zuweisen
C           da nur lineare Ans�tze ber�cksichtigt werden,
C           reichen die ersten 8 Knoten
	    do 30 k=1,8
	       xee(k)=xe(nr(mnn(iel,k)))
	       yee(k)=ye(nr(mnn(iel,k)))
	       zee(k)=ze(nr(mnn(iel,k)))
 30	    continue
C           Aufrufen der Parameterbestimmung je nach Elementtyp
C           Nichtlineare Elemente gleichen Typs werden wie lineare behandelt
            if (mety(iel).eq.1) then
C              Typ1=HEX8
               call interhe8(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j))
               xii4(j)=0.
	    else if (mety(iel).eq.2) then
C              Typ2=PENTA6
               call interpe6(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j),xii4(j))
	    else if (mety(iel).eq.3) then
C              Typ3=TET4
               call interte4(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j),xii4(j))
            else if (mety(iel).eq.4) then
C              Typ4=HEX20
               call interhe8(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j))
               xii4(j)=0.
	    else if (mety(iel).eq.5) then
C              Typ5=PENTA15
               call interpe6(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j),xii4(j))
	    else if (mety(iel).eq.6) then
C              Typ6=TET10
               call interte4(xee,yee,zee,x(i),y(i),z(i),
     +                       xii1(j),xii2(j),xii3(j),xii4(j))
            end if
	    index1=abs(xii1(j))+abs(xii2(j))+abs(xii3(j))+abs(xii4(j))
	    if (index1.lt.index) then
	       index=index1
	       nbest=j
            end if
C           checken, ob Knoten im Element liegt, also ob alle Parameter
C           vom Betrag kleiner als 1 sind.
C           Wenn ja wird nicht weitergesucht.	    
	    if (((abs(xii1(j)).le.1.).and.(abs(xii2(j)).le.1.)).and.
     +          ((abs(xii3(j)).le.1.).and.(abs(xii4(j)).le.1.))) then
               nbest=j
	       goto 21
            end if
 20      continue
C	 Warnung ins Protokoll, wenn kein Element gefunden wurde in dessen
C	 Inneren der Knoten liegt
	 print*,'Kein Element gefunden in dem der Knoten ',i,' liegt.'
	 print*,'ermittelter Parametersatz fuer bestes Element',nbest,':'
	 print*,'xi1=',xii1(nbest),'xi2=',xii2(nbest),'xi3=',xii3(nbest)
C        Parameter auf Betr�ge von 1 begrenzen, um unsinnige Extrapolationen
C        zu verhindern. Damit wird quasi der Oberfl�chenwert verwendet.	 
	 if (xii1(nbest).gt.1.) xii1(nbest)=1.
	 if (xii1(nbest).lt.-1.) xii1(nbest)=-1.
	 if (xii2(nbest).gt.1.) xii2(nbest)=1.
	 if (xii2(nbest).lt.-1.) xii2(nbest)=-1.
	 if (xii3(nbest).gt.1.) xii3(nbest)=1.
	 if (xii3(nbest).lt.-1.) xii3(nbest)=-1.
	 if (xii4(nbest).gt.1.) xii4(nbest)=1.
	 if (xii4(nbest).lt.-1.) xii4(nbest)=-1.
C        Aussprung, wenn zugeh�riger Parametersatz vorzeitig gefunden wurde
 21	 continue
         nelin(i)=nelem(nc(i),nbest)
	 xi1(i)=xii1(nbest)
	 xi2(i)=xii2(nbest)
	 xi3(i)=xii3(nbest)
	 xi4(i)=xii4(nbest)
C	 Ausgabe der aktuellen Parameter zur Kontrolle	
C	 print*,'node:       ',i 
C	 print*,'xi1=',xi1(i),'   xi2=',xi2(i),'   xi3=',xi3(i)
 10   continue
      return
C-----------------------------------------------------------------------
      end


C
C
C
C
C
C
C
C
C
C
C
C
C-----------------------------------------------------------------------
C
C   I N   N TTTTT EEEE RRR  PPP   OOO  L      A   TTTTT EEEE    333  DDD
C   I NN  N   T   E    R  R P  P O   O L     A A    T   E          3 D  D
C   I N N N   T   EE   RRR  PPP  O   O L    A   A   T   EE   --  33  D   D
C   I N  NN   T   E    R  R P    O   O L    AAAAA   T   E          3 D  D
C   I N   N   T   EEEE R  R P     OOO  LLLL A   A   T   EEEE    333  DDD
C
C-----------------------------------------------------------------------
C
      subroutine interpe6(x,y,z,x0,y0,z0,xi1,xi2,xi3,xi4)
c-----------------------------------------------------------------------
c Ermitteln der Parameter xi1,xi2,xi3,xi4
C eines gegebenen Punktes X0,Y0,Z0
c im lokalen Koordinatensystem eines QUAD4-Elementes
c-----------------------------------------------------------------------
      implicit none
      real x(6),y(6),z(6),x0,y0,z0,xi1,xi2,xi3,xi4
      real xo1,xo2,xo3,xo4
      real l1p6,l2p6,l3p6,l4p6,l5p6,l6p6
      real dx(4),f(4),a(4,4)
      real d(4)
      integer marke,ip(4),i
c-----------------------------------------------------------------------

C-----Iterative Ermittlung der Parameter xi1-4
C     mit dem Newton-Verfahren f�r nichtlineare Gleichungssysteme

C     Startl�sung
      xi1=0.
      xi2=0.
      xi3=0.
      xi4=-0.
         call anpe6(xi1,xi2,xi3,xi4,l1p6,l2p6,l3p6,l4p6,l5p6,l6p6)
      do 10 i=1,10
         xo1=xi1
         xo2=xi2
         xo3=xi3
         xo4=xi4
C        Funktionalmatrix ermitteln
C        (Ableitung jeder Funktion nach jeder Variablen)
         call funpe6(xo1,xo2,xo3,xo4,x,y,z,a)
C        Werte der Ansatzfunktionen f�r den aktuellen Parametersatz
         call anpe6(xo1,xo2,xo3,xo4,l1p6,l2p6,l3p6,l4p6,l5p6,l6p6)
C        Funktionen f�r die 2 Koordinaten = rechte Seite des Systems
	 f(1)=-(l1p6*x(1)+l2p6*x(2)+l3p6*x(3)
     +         +l4p6*x(4)+l5p6*x(5)+l6p6*x(6)-x0)
	 f(2)=-(l1p6*y(1)+l2p6*y(2)+l3p6*y(3)
     +         +l4p6*y(4)+l5p6*y(5)+l6p6*y(6)-y0)
	 f(3)=-(l1p6*z(1)+l2p6*z(2)+l3p6*z(3)
     +         +l4p6*z(4)+l5p6*z(5)+l6p6*z(6)-z0)
         f(4)=-(xo1+xo2+xo3-1)
C        L�sen des linearen Gleichungssystemes mit 4 Gleichungen
         call fgauss(4,a,4,f,dx,marke,d,ip)
C        Neuer Parametersatz
         xi1=xo1+dx(1)
         xi2=xo2+dx(2)
         xi3=xo3+dx(3)
         xi4=xo4+dx(4)
C        checken ob alle �nderungen im Parametersatz <10e-3 sind
         if(sqrt((xi1-xo1)**2+(xi2-xo2)**2+(xi3-xo3)**2+(xi4-xo4)**2)
     +      .le. 1.e-3) goto 110
 10   continue
 110  continue
C      print*,i,' Iterationen mit newton'
      return
c-----------------------------------------------------------------------
      end

      subroutine anpe6(xi1,xi2,xi3,xi4,l1p6,l2p6,l3p6,l4p6,l5p6,l6p6)
c-----------------------------------------------------------------------
c-----Ansatzfunktion 1 f�r Penta6-Element
c-----------------------------------------------------------------------
      implicit none
      real xi1,xi2,xi3,xi4, l1p6,l2p6,l3p6,l4p6,l5p6,l6p6
c-----------------------------------------------------------------------
      l1p6=xi1*(1.-xi4)/2.
      l2p6=xi2*(1.-xi4)/2.
      l3p6=xi3*(1.-xi4)/2.
      l4p6=xi1*(1.+xi4)/2.
      l5p6=xi2*(1.+xi4)/2.
      l6p6=xi3*(1.+xi4)/2.
      return
c-----------------------------------------------------------------------
      end

      subroutine funpe6(xi1,xi2,xi3,xi4,x,y,z,a)
c-----------------------------------------------------------------------
c-----Funktionalmatrix der Koordinatenfunktionen aufstellen f�r Penta6-Element
C     (Ableitung jeder Funktion nach jeder Variablen)
c-----------------------------------------------------------------------
      implicit none
      real xi1,xi2,xi3,xi4,x(6),y(6),z(6),a(4,4)
c-----------------------------------------------------------------------
      a(1,1)=0.5*(1-xi4)*x(1)+0.5*(1+xi4)*x(4)
      a(1,2)=0.5*(1-xi4)*x(2)+0.5*(1+xi4)*x(5)
      a(1,3)=0.5*(1-xi4)*x(3)+0.5*(1+xi4)*x(6)
      a(1,4)=0.5*(-x(1)*xi1-x(2)*xi2-x(3)*xi3
     +             +x(4)*xi1+x(5)*xi2+x(6)*xi3)
      a(2,1)=0.5*(1-xi4)*y(1)+0.5*(1+xi4)*y(4)
      a(2,2)=0.5*(1-xi4)*y(2)+0.5*(1+xi4)*y(5)
      a(2,3)=0.5*(1-xi4)*y(3)+0.5*(1+xi4)*y(6)
      a(2,4)=0.5*(-y(1)*xi1-y(2)*xi2-y(3)*xi3
     +             +y(4)*xi1+y(5)*xi2+y(6)*xi3)
      a(3,1)=0.5*(1-xi4)*z(1)+0.5*(1+xi4)*z(4)
      a(3,2)=0.5*(1-xi4)*z(2)+0.5*(1+xi4)*z(5)
      a(3,3)=0.5*(1-xi4)*z(3)+0.5*(1+xi4)*z(6)
      a(3,4)=0.5*(-z(1)*xi1-z(2)*xi2-z(3)*xi3
     +             +z(4)*xi1+z(5)*xi2+z(6)*xi3)
      a(4,1)=1.
      a(4,2)=1.
      a(4,3)=1.
      a(4,4)=0.
      return
c-----------------------------------------------------------------------
      end



      subroutine interte4(x,y,z,x0,y0,z0,xi1,xi2,xi3,xi4)
c-----------------------------------------------------------------------
c Ermitteln der Parameter xi1, xi2, xi3, xi4
C eines gegebenen Punktes X0,Y0,Z0
c im lokalen Koordinatensystem eines TET4-Elementes
c-----------------------------------------------------------------------
      implicit none
      real x(4),y(4),z(4),x0,y0,z0,xi1,xi2,xi3,xi4
      real dx(4),f(4),a(4,4)
      real d(4)
      integer marke,ip(4),i
c-----------------------------------------------------------------------

C-----Direkte Ermittlung der Parameter xi1, xi2, xi3, x4
C     mit dem Gauss?schen Eleiminationsverfahren f�r lineare Gleichungssysteme
C     Matrix ermitteln = Matrix des Gleichungssystemes
      a(1,1)=x(1)
      a(1,2)=x(2)
      a(1,3)=x(3)
      a(1,4)=x(4)
      a(2,1)=y(1)
      a(2,2)=y(2)
      a(2,3)=y(3)
      a(2,4)=y(4)
      a(3,1)=z(1)
      a(3,2)=z(2)
      a(3,3)=z(3)
      a(3,4)=z(4)
      a(4,1)=1.
      a(4,2)=1.
      a(4,3)=1.
      a(4,4)=1.
C     Aufstellen der rechten Seite des Gleichungssystemes
      f(1)=x0
      f(2)=y0
      f(3)=z0
      f(4)=1.
C     L�sen des Linearen Gleichungssystemes
      call fgauss(4,a,4,f,dx,marke,d,ip)
C     Neuer Parametersatz
      xi1=dx(1)
      xi2=dx(2)
      xi3=dx(3)
      xi4=dx(4)
      return
c-----------------------------------------------------------------------
      end

      subroutine ante4(xi1,xi2,xi3,xi4,l1te4,l2te4,l3te4,l4te4)
c-----------------------------------------------------------------------
c-----Ansatzfunktion f�r Tet4-Element
C     Ansatzfunktionen und Parameter sind identisch
C     Funktion und neue Namen nur wegen der formalen �bereinstimmung 
C     mit den anderen Elementtypen benutzt
c-----------------------------------------------------------------------
      implicit none
      real xi1,xi2,xi3,xi4, l1te4,l2te4,l3te4,l4te4
c-----------------------------------------------------------------------
      l1te4=xi1
      l2te4=xi2
      l3te4=xi3
      l4te4=xi4
      return
c-----------------------------------------------------------------------
      end


      subroutine interhe8(x,y,z,x0,y0,z0,xi,et,ze)
c-----------------------------------------------------------------------
c Ermitteln der Parameter xi, eta, zeta
C eines gegebenen Punktes X0,Y0,Z0
c im lokalen Koordinatensystem eines HEX8-Elementes
c-----------------------------------------------------------------------
      implicit none
      real x(8),y(8),z(8)
      real x0,y0,z0
      real xi,et,ze
      real xo,eo,zo
      real l1h8,l2h8,l3h8,l4h8,l5h8,l6h8,l7h8,l8h8
      real dx(3),f(3),a(3,3)
      real deta,detr1,detr2,detr3
C     Flag von Gleichungsl�ser (0=Matrix ist singul�r)
      integer Marke
      integer i
c-----------------------------------------------------------------------
C-----Iterative Ermittlung der Parameter xi, eta und zeta
C     mit dem Newton-Verfahren f�r nichtlineare Gleichungssysteme

C     Startl�sung
      xi=0.
      et=0.
      ze=0.
      do 10 i=1,10
         xo=xi
	 eo=et
	 zo=ze
C        Funktionalmatrix ermitteln = Matrix des Gleichungssystemes
C        (Ableitung jeder Funktion nach jeder Variablen)
         call funhe8(xo,eo,zo,x,y,z,a)
C        Aufstellen der rechten Seite des Gleichungssystemes
C        Werte der Ansatzfunktionen f�r den aktuellen Parametersatz
         call anhe8(xo,eo,zo,l1h8,l2h8,l3h8,l4h8,l5h8,l6h8,l7h8,l8h8)
C        Funktionen f�r die 3 Koordinaten = rechte Seite des Systems
	 f(1)=-(l1h8*x(1)+l2h8*x(2)+l3h8*x(3)+l4h8*x(4)+
     +          l5h8*x(5)+l6h8*x(6)+l7h8*x(7)+l8h8*x(8)-x0)
         f(2)=-(l1h8*y(1)+l2h8*y(2)+l3h8*y(3)+l4h8*y(4)+
     +          l5h8*y(5)+l6h8*y(6)+l7h8*y(7)+l8h8*y(8)-y0)
         f(3)=-(l1h8*z(1)+l2h8*z(2)+l3h8*z(3)+l4h8*z(4)+
     +          l5h8*z(5)+l6h8*z(6)+l7h8*z(7)+l8h8*z(8)-z0)
C        L�sen des Linearen Gleichungssystemes
         call lingl3(a,f,dx,MARKE)
C        Neuer Parametersatz
	 xi=xo+dx(1)
	 et=eo+dx(2)
	 ze=zo+dx(3)
C        checken ob alle �nderungen im Parametersatz <10e-3 sind
         if( ((abs(xi-xo).le.1.e-3).and.(abs(et-eo).le.1.e-3))
     +       .and.(abs(ze-zo).le.1.e-3) ) goto 110
 10   continue
 110  continue
C      print*,i,' Iterationen mit newton'
      return
c-----------------------------------------------------------------------
      end



      subroutine anhe8(xi,et,ze,l1h8,l2h8,l3h8,l4h8,l5h8,l6h8,l7h8,l8h8)
c-----------------------------------------------------------------------
c-----Ansatzfunktion 1 f�r Hex8-Element
c-----------------------------------------------------------------------
      implicit none
      real xi,et,ze
      real l1h8,l2h8,l3h8,l4h8,l5h8,l6h8,l7h8,l8h8
c-----------------------------------------------------------------------
      l1h8=(1.-xi)*(1.-et)*(1.-ze)/8.
      l2h8=(1.+xi)*(1.-et)*(1.-ze)/8.
      l3h8=(1.+xi)*(1.+et)*(1.-ze)/8.
      l4h8=(1.-xi)*(1.+et)*(1.-ze)/8.
      l5h8=(1.-xi)*(1.-et)*(1.+ze)/8.
      l6h8=(1.+xi)*(1.-et)*(1.+ze)/8.
      l7h8=(1.+xi)*(1.+et)*(1.+ze)/8.
      l8h8=(1.-xi)*(1.+et)*(1.+ze)/8.
      return
c-----------------------------------------------------------------------
      end



      subroutine funhe8(xi,et,ze,x,y,z,a)
c-----------------------------------------------------------------------
c-----Funktionalmatrix der Koordinatenfunktionen aufstellen
C     (Ableitung jeder Funktion nach jeder Variablen)
c-----------------------------------------------------------------------
      implicit none
      real xi,et,ze
      real x(8),y(8),z(8)
      real a(3,3)
c-----------------------------------------------------------------------
	 a(1,1)=(-(1-et)*(1-ze)*x(1)+(1-et)*(1-ze)*x(2)
     +           +(1+et)*(1-ze)*x(3)-(1+et)*(1-ze)*x(4)
     +           -(1-et)*(1+ze)*x(5)+(1-et)*(1+ze)*x(6)
     +           +(1+et)*(1+ze)*x(7)-(1+et)*(1+ze)*x(8))/8.
	 a(1,2)=(-(1-xi)*(1-ze)*x(1)-(1+xi)*(1-ze)*x(2)
     +           +(1+xi)*(1-ze)*x(3)+(1-xi)*(1-ze)*x(4)
     +           -(1-xi)*(1+ze)*x(5)-(1+xi)*(1+ze)*x(6)
     +           +(1+xi)*(1+ze)*x(7)+(1-xi)*(1+ze)*x(8))/8.
	 a(1,3)=(-(1-xi)*(1-et)*x(1)-(1+xi)*(1-et)*x(2)
     +           -(1+xi)*(1+et)*x(3)-(1-xi)*(1+et)*x(4)
     +           +(1-xi)*(1-et)*x(5)+(1+xi)*(1-et)*x(6)
     +           +(1+xi)*(1+et)*x(7)+(1-xi)*(1+et)*x(8))/8.
	 a(2,1)=(-(1-et)*(1-ze)*y(1)+(1-et)*(1-ze)*y(2)
     +           +(1+et)*(1-ze)*y(3)-(1+et)*(1-ze)*y(4)
     +           -(1-et)*(1+ze)*y(5)+(1-et)*(1+ze)*y(6)
     +           +(1+et)*(1+ze)*y(7)-(1+et)*(1+ze)*y(8))/8.
	 a(2,2)=(-(1-xi)*(1-ze)*y(1)-(1+xi)*(1-ze)*y(2)
     +           +(1+xi)*(1-ze)*y(3)+(1-xi)*(1-ze)*y(4)
     +           -(1-xi)*(1+ze)*y(5)-(1+xi)*(1+ze)*y(6)
     +           +(1+xi)*(1+ze)*y(7)+(1-xi)*(1+ze)*y(8))/8.
	 a(2,3)=(-(1-xi)*(1-et)*y(1)-(1+xi)*(1-et)*y(2)
     +           -(1+xi)*(1+et)*y(3)-(1-xi)*(1+et)*y(4)
     +           +(1-xi)*(1-et)*y(5)+(1+xi)*(1-et)*y(6)
     +           +(1+xi)*(1+et)*y(7)+(1-xi)*(1+et)*y(8))/8.
	 a(3,1)=(-(1-et)*(1-ze)*z(1)+(1-et)*(1-ze)*z(2)
     +           +(1+et)*(1-ze)*z(3)-(1+et)*(1-ze)*z(4)
     +           -(1-et)*(1+ze)*z(5)+(1-et)*(1+ze)*z(6)
     +           +(1+et)*(1+ze)*z(7)-(1+et)*(1+ze)*z(8))/8.
	 a(3,2)=(-(1-xi)*(1-ze)*z(1)-(1+xi)*(1-ze)*z(2)
     +           +(1+xi)*(1-ze)*z(3)+(1-xi)*(1-ze)*z(4)
     +           -(1-xi)*(1+ze)*z(5)-(1+xi)*(1+ze)*z(6)
     +           +(1+xi)*(1+ze)*z(7)+(1-xi)*(1+ze)*z(8))/8.
	 a(3,3)=(-(1-xi)*(1-et)*z(1)-(1+xi)*(1-et)*z(2)
     +           -(1+xi)*(1+et)*z(3)-(1-xi)*(1+et)*z(4)
     +           +(1-xi)*(1-et)*z(5)+(1+xi)*(1-et)*z(6)
     +           +(1+xi)*(1+et)*z(7)+(1-xi)*(1+et)*z(8))/8.
      return
c-----------------------------------------------------------------------
      end
CC
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C-----------------------------------------------------------------------
C
C   FFFF  RRR   DDD     TTTTT  OOO   OOO  L     SSS
C   F     R  R  D  D      T   O   O O	O L    S
C   FFF   RRR   D   D --  T   O   O O	O L     SSS
C   F     R  R  D  D      T   O   O O	O L        S
C   F     R  R  DDD       T    OOO   OOO  LLLL  SSS
C
C-----------------------------------------------------------------------
C
C
      
          
      subroutine CheckNum(nnmax,nemax,mnmax,memax,nin)
C-----------------------------------------------------------------------
C-----Bestimmen der in einem FRD-File enthaltenen Element- und Knotenanzahl
C-----------------------------------------------------------------------
      implicit none
C     Anzahl der Knoten und Elemente im File
      integer nnmax, nemax
C     H�chste Nummer der Knoten und Elemente im File
      integer mnmax, memax
C     Hilfsvariable
      integer nn, n, i
C     Eingabekanal
      integer nin
C     Elementtyp
      integer mety
C     Flag f�r Lang oder Kurzformat
      integer nform
C     Textstrings als Einlesepuffer
      character*1 a1
      character*80 a80
      character*110 a110
C-----------------------------------------------------------------------
      do 10 i=1,1000
         read(nin,1000),a80
C-       suchen nach dem Beginn der Knotendefinitionen
         read(a80,1010),n,a1
	 if ((n.eq.2).and.(a1.eq.'C')) goto 20
 10   continue
      print*,'kein Knotendefinitionsteil gefunden'
      print*,'--- Programm ABGEBROCHEN ---'
      stop

C-    Knoten einlesen
 20   continue
      nnmax=0
      mnmax=0
C     Checken ob long oder short format
      read(a80,1010),nn,a1,nform
      if(nform.eq.1)then
 21      continue
            read(nin,1030),n,nn
C           Aussprung, wenn Ende des Knotenblocks erreicht ist	    
            if (n.eq.-3) goto 25
	    nnmax=nnmax+1
	    if (nn.gt.mnmax) mnmax=nn
         goto 21
      else
 22      continue
            read(nin,1020),n,nn
C           Aussprung, wenn Ende des Knotenblocks erreicht ist	    
            if (n.eq.-3) goto 25
	    nnmax=nnmax+1
	    if (nn.gt.mnmax) mnmax=nn
         goto 22
      end if
 25   continue 

C-----Einlesen der Elementinformationen
      memax=0 
      nemax=0
      read(nin,1110),n,a1,nform
      if ((n.eq.3).and.(a1.eq.'C')) then
	 if (nform.eq.1) then
C        f�r long-format	 
 30      continue 
            read(nin,1130),n,nn,mety
            if (n.eq.-3) goto 33
	    if (mety.eq.1) then
C              Type1=HEX8
               read(nin,1150)
	    elseif (mety.eq.2) then
C              Type1=PENTA6
               read(nin,1150)
	    elseif (mety.eq.3) then
C              Type1=TET4
               read(nin,1150)
	    elseif (mety.eq.4) then
C              Type1=HEX20
               read(nin,1150)
               read(nin,1150)
	    elseif (mety.eq.5) then
C              Type5=PENTA15
               read(nin,1150)
               read(nin,1150)
	    elseif (mety.eq.6) then
C              Type1=TET10
               read(nin,1150)
	    endif
	    nemax=nemax+1
	    if (nn.gt.memax) memax=nn
         goto 30
	 else
C        f�r short-format	 
 31      continue 
            read(nin,1120),n,nn,mety
            if (n.eq.-3) goto 33
            if (mety.eq.1) then
C              Type1=HEX8
               read(nin,1140)
	    elseif (mety.eq.2) then
C              Type1=PENTA6
               read(nin,1140)
	    elseif (mety.eq.3) then
C              Type1=TET4
               read(nin,1140)
	    elseif (mety.eq.4) then
C              Type1=HEX20
               read(nin,1140)
               read(nin,1140)
	    elseif (mety.eq.5) then
C              Type1=PENTA15
               read(nin,1140)
	    elseif (mety.eq.6) then
C              Type1=TET10
               read(nin,1140)
	    endif
	    nemax=nemax+1
	    if (nn.gt.memax) memax=nn
         goto 31
	 endif
      else 
	 print*,'   --- keine Elementinformationen gefumden !'
	 rewind(nin)
      endif
 33   continue
 
      return
C-----------------------------------------------------------------------
 1000 format(a80)
c-Element/node Header
c- 1x=1*Space i4=4*integer a1=textstring 67x=67*Space i1=1*integer
 1010 format(1x,i4,a1,67x,i1)
c-Knotenbeschreibung short/long
 1020 format(1x,i2,i5,3e12.5)
 1030 format(1x,i2,i10,3e12.5)
C-----------------------------------------------------------------------
 1100 format(a80)
 1105 format(a110)
c-Element/node Header
 1110 format(1x,i4,a1,67x,i1)
c-Elementdefinition short/long
 1120 format(1x,i2,i5,2i5,a5)
 1130 format(1x,i2,i10,2i5,a5)
c-Elementknoten short/long
 1140 format(1x,i2,15i5)
 1150 format(1x,i2,10i10)
C-----------------------------------------------------------------------
      end
  
      
      
      subroutine readnode(nnd,nn,x,y,z,nin,nout,nf1,nmax)
C-----------------------------------------------------------------------
C-----Lesen von Knoteninformationen vom FRD-file bis Ende des Blocks err.
C-----------------------------------------------------------------------
      implicit none
      integer nform
C     max zul. Knotenanzahl (f. Felddimensionierung)      
      integer nmax
C     Hilfsgr��en
      integer n,i,m
C     Anzahl der Knoten, Knotennummern
      integer nnd,nn(nmax)
C     Ein- und Ausgabekanal, Flag ob Ausgabe erw�nscht ist
      integer nin,nout,nf1
C     Knotenkoordinaten      
      real x(nmax),y(nmax),z(nmax)
      character*1 a1
      character*80 a80
C-----------------------------------------------------------------------
      do 10 i=1,1000
         read(nin,1000),a80
         if (nf1.eq.1) write(nout,1000),a80
C-       suchen nach dem Beginn der Knotendefinitionen
         read(a80,1010),n,a1
	 if ((n.eq.2).and.(a1.eq.'C')) goto 20
 10   continue
      print*,'kein Knotendefinitionsteil gefunden'
      print*,'--- Programm ABGEBROCHEN ---'
      stop

C-    Knoten einlesen
 20   continue
C     Checken ob long oder short format
      nnd=0
      read(a80,1010),n,a1,nform
      if(nform.eq.1)then
 21      continue
            m=nnd+1
            read(nin,1030),n,nn(m),x(m),y(m),z(m)
C           Aussprung, wenn Ende des Knotenblocks erreicht ist	    
            if (n.eq.-3) goto 25
C           rausschreiben der Knoten in Outfile
            if (nf1.eq.1) write(nout,1030),n,nn(m),x(m),y(m),z(m)
	    nnd=m
         goto 21
      else
 22      continue
            m=nnd+1
            read(nin,1020),n,nn(nnd+1),x(nnd+1),y(nnd+1),z(nnd+1)
C           Aussprung, wenn Ende des Knotenblocks erreicht ist	    
            if (n.eq.-3) goto 25
C           rausschreiben der Knoten in Outfile
            if (nf1.eq.1) write(nout,1020),n,nn(m),x(m),y(m),z(m)
	    nnd=m
         goto 22
      end if
 25   continue 
      if (nf1.eq.1) write(nout,1020),-3
      return
C-----------------------------------------------------------------------
 1000 format(a80)
c-Element/node Header
 1010 format(1x,i4,a1,67x,i1)
c-Knotenbeschreibung short/long
 1020 format(1x,i2,i5,3e12.5)
 1030 format(1x,i2,i10,3e12.5)
C-----------------------------------------------------------------------
      end
      
      subroutine ReadElem(melem,mnn,mel,mety,mnd,nin,nout,nf1,nmax)
C-----------------------------------------------------------------------
C-----Lesen von Elementinformationen vom FRD-file bis Ende des Blocks err.
C-----------------------------------------------------------------------
      implicit none
      integer i,m
      integer nform,nmax,n
      integer melem,mety(nmax),mgr(nmax)
      character*5 amat(nmax)
C     Ein- und Augabekanalnummern,   Flag ob Ausgabe gew�nscht      
      integer nin,nout,nf1
C     Elementnummern  ,   Elementknoten    
      integer mel(nmax),mnn(nmax,20)
C     Anzahl deer Knoten je Element
      integer mnd(nmax)
      character*1 a1
      character*110 a110
C-----------------------------------------------------------------------
      melem=0 
C-----Einlesen der Elementinformationen
      read(nin,1010),n,a1,nform
      if ((n.eq.3).and.(a1.eq.'C')) then
         if (nf1.eq.1) write(nout,1010),3,'C',nform
	 if (nform.eq.1) then
C        f�r long-format	 
 30      continue 
            m=melem+1
            read(nin,1030),n,mel(m),mety(m),mgr(m),amat(m)
            if (n.eq.-3) then
	       if (nf1.eq.1) write(nout,1040),-3
	       goto 33
	    endif
	    if (nf1.eq.1) write(nout,1030),-1,mel(m),mety(m),mgr(m),
     +                                      amat(m)
	    if (mety(m).eq.1) then
C              Type1=HEX8
               read(nin,1050)n,(mnn(m,i),i=1,8)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,8)
	       mnd(m)=8
	    elseif (mety(m).eq.2) then
C              Type1=PENTA6
               read(nin,1050)n,(mnn(m,i),i=1,6)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,6)
	       mnd(m)=6
	    elseif (mety(m).eq.3) then
C              Type1=TET4
               read(nin,1050)n,(mnn(m,i),i=1,4)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,4)
	       mnd(m)=4
	    elseif (mety(m).eq.4) then
C              Type1=HEX20
               read(nin,1050)n,(mnn(m,i),i=1,10)
               read(nin,1050)n,(mnn(m,i),i=11,20)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,10)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=11,20)
	       mnd(m)=20
	    elseif (mety(m).eq.5) then
C              Type5=PENTA15
               read(nin,1050)n,(mnn(m,i),i=1,10)
               read(nin,1050)n,(mnn(m,i),i=11,15)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,10)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=11,15)
	       mnd(m)=15
	    elseif (mety(m).eq.6) then
C              Type1=TET10
               read(nin,1050)n,(mnn(m,i),i=1,10)
	       if (nf1.eq.1) write(nout,1050)-2,(mnn(m,i),i=1,10)
	       mnd(m)=10
	    endif
	    melem=m
         goto 30
	 else
C        f�r short-format	 
 31      continue 
            m=melem+1
            read(nin,1020),n,mel(m),mety(m),mgr(m),amat(m)
            if (n.eq.-3) then
 	       if (nf1.eq.1) write(nout,1040),-3
	       goto 33
	    endif
	    if (nf1.eq.1) write(nout,1020),-1,mel(m),mety(m),mgr(m),
     +                                      amat(m)
            if (mety(m).eq.1) then
C              Type1=HEX8
               read(nin,1040)n,(mnn(m,i),i=1,8)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,8)
	       mnd(m)=8
	    elseif (mety(m).eq.2) then
C              Type1=PENTA6
               read(nin,1040)n,(mnn(m,i),i=1,6)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,6)
	       mnd(m)=6
	    elseif (mety(m).eq.3) then
C              Type1=TET4
               read(nin,1040)n,(mnn(m,i),i=1,4)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,4)
	       mnd(m)=4
	    elseif (mety(m).eq.4) then
C              Type1=HEX20
               read(nin,1040)n,(mnn(m,i),i=1,15)
               read(nin,1040)n,(mnn(m,i),i=16,20)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,15)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=16,20)
	       mnd(m)=20
	    elseif (mety(m).eq.5) then
C              Type1=PENTA15
               read(nin,1040)n,(mnn(m,i),i=1,15)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,15)
	       mnd(m)=15
	    elseif (mety(m).eq.6) then
C              Type1=TET10
               read(nin,1040)n,(mnn(m,i),i=1,10)
	       if (nf1.eq.1) write(nout,1040)-2,(mnn(m,i),i=1,10)
	       mnd(m)=10
	    endif
	    melem=m
         goto 31
	 endif
      else 
	 print*,'   --- keine Elementinformationen gefumden !'
	 rewind(nin)
      endif
 33   continue
 
      return
C-----------------------------------------------------------------------
 1000 format(a80)
 1005 format(a110)
c-Element/node Header
 1010 format(1x,i4,a1,67x,i1)
c-Elementdefinition short/long
 1020 format(1x,i2,i5,2i5,a5)
 1030 format(1x,i2,i10,2i5,a5)
c-Elementknoten short/long
 1040 format(1x,i2,15i5)
 1050 format(1x,i2,10i10)
C-----------------------------------------------------------------------
      end
      
      

      subroutine WriteErg(nnd,nn,erg,alc,time,abesch,aerg,ncp,ncps,
     +                   aerc,ict,mtx,mty,iex,ax,nout,nmax)
C-----------------------------------------------------------------------
C-----Schreiben von Ergebnisinformationen
C     geschrieben wird generell im Long-Format
C-----------------------------------------------------------------------
      implicit none
C--   Hilfsgr��en
C     Laufvariable
      integer i,m,j
C     Maximal zul�ssige Knotenanzahl benutzt zur Felddimensionierung
      integer nmax
C--   Knoten und Elementinformationen
C     Anzahl der Knoten, Knotennummern
      integer nnd,nn(nmax)
C--   Ein- und Ausgabe
C     Unit-Nummern f�r Ein- und Ausgaben(neue Knoten, Ergebnisknoten)
      integer nout
C--   Ergebnisdaten
C     Anzahl der Ergebniskomponenten (Angabe in Liste, reale Anzahl)
      integer ncps,ncp
      integer mtx(6),mty(6),iex(6),ict(6)
C     Zeit f�r Ergebnis/Frequenz
      real time
C     Ergebnistensor (max 3x3, symmetrisch, dadurch 6 Komponenten)      
      real erg(nmax,6)
      character*8 aerg,ax(6),aerc(6)
      character*6 alc
      character*49 abesch
C-----------------------------------------------------------------------
C-----Schreiben der Ergebnisinformationen
C     first record (Ergebnisheader)
      write(nout,1010),100,'C',alc,time,abesch,1
C     second record
      write(nout,1011)-4,aerg,ncps,1
      do 10 i=1,ncps
C	 third record
         write(nout,1011)-5,aerc(i),1,ict(i),mtx(i),mty(i),iex(i),ax(i)
 10   continue
C     Ergebnisse f�r jeden Knoten 
      do 30 i=1,nnd
         write(nout,1070)-1,nn(i),(erg(i,j),j=1,ncp)
 30   continue
C     Blockende
      write(nout,1011)-3
      return
C-----------------------------------------------------------------------
c-Results Header
 1010 format(1x,i4,a1,a6,e12.5,a49,i1)
 1011 format(1x,i2,2x,a8,5i5,A8)
c-Results long
 1070 format(1x,i2,i10,6e12.5)
C-----------------------------------------------------------------------
      end



      subroutine ReadErg(nnd,nn,erg,alc,time,abesch,aerg,ncp,ncps,
     +                  aerc,ict,mtx,mty,iex,ax,nin,nmax,nexit)
C-----------------------------------------------------------------------
C-----Lesen von Ergebnisinformationen vom FRD-file bis Ende des Blocks err.
C-----------------------------------------------------------------------
      implicit none
C     Laufvariablen
      integer i,m,j
      integer nform,nmax,n
      integer nnd,nn(nmax)
C     Kanalnummer f�r Eingabefile
      integer nin
      integer mel(nmax)
C     Anzahl der Ergebniskomponenten:  Angabe in Liste  und  reale Anzahl
      integer ncps,ncp
C     Materialabh�ngigkeit
      integer irty
      integer mtx(6),mty(6),iex(6),ict(6)
      integer nexit
      real time,erg(nmax,6)
      character*1 a1
      character*80 a80
      character*8 aerg,ax(6),aerc(6)
      character*6 alc
      character*49 abesch
C-    Dummy Parameter
      integer mx
C-----------------------------------------------------------------------
C-----Einlesen der Ergebnisinformationen
 23   continue
C     first record (Ergebnisheader)
      read(nin,1111),n,a1,alc,time,abesch,nform
      if ((n.eq.100).and.(a1.eq.'C')) then
C	 second record
	 read(nin,1011)n,aerg,ncps,irty
	 if (irty.eq.2) then
C	    Aussprung mit Code f�r nicht unterst�tztes Feature
	    nexit=4
	    goto 33
	 endif
	 ncp=ncps
	 do 10 i=1,ncps
C	    third record
            read(nin,1011)n,aerc(i),mx,ict(i),mtx(i),mty(i),iex(i),ax(i)
	    if (iex(i).eq.1) ncp=ncp-1
 10      continue
	 if (nform.eq.1) then
C           f�r long-format	 
            do 30 i=1,nnd
               read(nin,1070)n,mx,(erg(i,j),j=1,ncp)
 30	    continue
C           Checken,ob Ende des Ergebnisblocks erreicht ist	    
            read(nin,1070)n
            if (n.eq.-3) then
C	       Aussprung mit Code f�r regul�res Ende
	       nexit=2
	    else
C	       Aussprung mit Code f�r ungleichviele Ergebnisse und Knoten
	       nexit=3
	    endif
	 else
C           f�r short-format	 
            do 40 i=1,nnd
               read(nin,1060)n,mx,(erg(i,j),j=1,ncp)
 40	    continue
C           Checken,ob Ende des Ergebnisblocks erreicht ist	    
            read(nin,1060)n
            if (n.eq.-3) then
C	       Aussprung mit Code f�r regul�res Ende
	       nexit=2
	    else
C	       Aussprung mit Code f�r ungleichviele Ergebnisse und Knoten
	       nexit=3
	    endif
	 endif
      else if (n.eq.9999) then
C	 Aussprung mit Code f�r regul�res Fileende
	 nexit=1
      else if ((n.eq.1).and.(a1.eq.'P')) then
         goto 23
      else
C	 Aussprung mit Code f�r unerwartetes Fileende
	 nexit=0 
      endif
 33   continue
 
      return
C-----------------------------------------------------------------------
 1000 format(a80)
c-Results Header
 1010 format(1x,i4,a1,a6,e12.5,a49,i1)
 1111 format(1x,i4,a1,a6,e12.5,a49,i2)
 1011 format(1x,i2,2x,a8,5i5,A8)
c-Results short/long additinal line for material dependence
 1040 format(1x,i2,i5, 3i5)
 1050 format(1x,i2,i10,3i5)
c-Results short/long
 1060 format(1x,i2,i5, 6e12.5)
 1070 format(1x,i2,i10,6e12.5)
C-----------------------------------------------------------------------
      end

C
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C
C-----------------------------------------------------------------------
C
C   L	 I N   N  GGGG L	
C   L	 I NN  N G     L    
C   L	 I N N N G  GG L	
C   L	 I N  NN G   G L	
C   LLLL I N   N  GGG  LLLL  
C
C-----------------------------------------------------------------------
C
C
C
      subroutine lingl3(a,f,x,MARKE)
c-----------------------------------------------------------------------
c-----lineares Gleichungssystem der gr��e 3 l�sen
c-----------------------------------------------------------------------
      implicit none
C     Flag, ob das Gleichungssystem singul�r ist (0=singul�r)
      integer MARKE
C     Matrix, rechte Seite, L�sung
      real a(3,3),f(3),x(3)
C     Determinante und entwickelte Determinanten
      real detA, detR1, detR2, detR3      
c-----------------------------------------------------------------------
      MARKE=1
C     Determinante der Funktionalmatrix (entwickelt nach der ersten Spalte)
      detA=a(1,1)*(a(2,2)*a(3,3)-a(2,3)*a(3,2))
     +	  -a(2,1)*(a(1,2)*a(3,3)-a(3,2)*a(1,3))
     +	  +a(3,1)*(a(1,2)*a(2,3)-a(2,2)*a(1,3))
C     Flag f�r Singularit�t setzen, wenn detA sehr klein ist,
C     die Detrminante also quasi singul�r ist     
      if (abs(detA).lt.1.e-15) then
         MARKE=0
	 return
      endif
C     Determinanten (entwickelt nach der esten Spalte)
      detR1=f(1)*(a(2,2)*a(3,3)-a(2,3)*a(3,2))
     +	      -f(2)*(a(1,2)*a(3,3)-a(3,2)*a(1,3))
     +	      +f(3)*(a(1,2)*a(2,3)-a(2,2)*a(1,3))
      detR2=a(1,1)*(f(2)*a(3,3)-a(2,3)*f(3))
     +	      -a(2,1)*(f(1)*a(3,3)-f(3)*a(1,3))
     +	      +a(3,1)*(f(1)*a(2,3)-f(2)*a(1,3))
      detR3=a(1,1)*(a(2,2)*f(3)-f(2)*a(3,2))
     +	      -a(2,1)*(a(1,2)*f(3)-a(3,2)*f(1))
     +	      +a(3,1)*(a(1,2)*f(2)-a(2,2)*f(1))
C     L�sungsvektor
      x(1)=detR1/detA
      x(2)=detR2/detA
      x(3)=detR3/detA
      return
c-----------------------------------------------------------------------
      end


      subroutine lingl2(a,f,x,MARKE)
c-----------------------------------------------------------------------
c-----lineares Gleichungssystem der gr��e 3 l�sen
c-----------------------------------------------------------------------
      implicit none
C     Flag, ob das Gleichungssystem singul�r ist (0=singul�r)
      integer MARKE
C     Determinante und entwickelte Determinanten
      real detA, detR1, detR2
C     Matrix, rechte Seite, L�sung
      real a(2,2),f(2),x(2)
c-----------------------------------------------------------------------
C     Determinante der Funktionalmatrix
      detA=a(1,1)*a(2,2)-a(1,2)*a(2,1)
C     Determinanten
      detR1=a(2,2)*f(1)-a(1,2)*f(2)
      detR2=-a(2,1)*f(1)+a(1,1)*f(2)
C     neuer Parametersatz xi, eta und zeta
      x(1)=detR1/detA
      x(2)=detR2/detA
      return
c-----------------------------------------------------------------------
      end


      SUBROUTINE FGAUSS(N,A,IA,Y,X,MARKE,D,IPIVOT)
C
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
C
      IMPLICIT REAL (A-H,O-Z)
      DIMENSION A(1:IA,1:N),Y(1:N),X(1:N),D(1:N)
      INTEGER IPIVOT(1:N)
C
C  ZERLEGUNG DER MATRIX A MIT HILFE DER SUBROUTINE FGAUSZ.
C
      CALL FGAUSZ(N,A,IA,IPIVOT,MARKE,D)
C
C  VORWAERTS- UND RUECKWAERTSELIMINATION MIT HILFE DER SUBROUTINE
C  FGAUSL, UM DIE LOESUNG DES GLEICHUNGSSYSTEMS ZU BERECHNEN.
C
      IF(MARKE.NE.0) CALL FGAUSL(N,A,IA,IPIVOT,Y,X)
      RETURN
      END
C
C*****************************************************************
      INTEGER FUNCTION MASCHD(X)
      REAL X
      MASCHD=0
      IF(1.e0.LT.X) MASCHD=1
      RETURN
      END
C*****************************************************************
C
      SUBROUTINE FGAUSZ(N,A,IA,IPIVOT,MARKE,D)
C
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
C
      IMPLICIT REAL (A-H,O-Z)
      DIMENSION A(1:IA,1:N),D(1:N)
      INTEGER IPIVOT(1:N)
C
C  LOKALE SPEICHERUNG DER FEHLERSCHRANKE RELERR FUER DEN FALL,
C  DASS DAS UNTERPROGRAMM MEHRFACH AUFGERUFEN WIRD.
C
      SAVE RELERR,IFLAG
      DATA IFLAG/0/
      MARKE=1
C
C  BERECHNUNG DER MASCHINENGENAUIGKEIT UND SETZEN DES RELATIVEN
C  FEHLERS.
C
      IF(IFLAG.EQ.0)THEN
         IFLAG=1
         FMACHN=1.D0
   10    FMACHN=0.5D0*FMACHN
         IF(MASCHD(1.e0+FMACHN).EQ.1) GOTO 10
         RELERR=8.D0*FMACHN
      END IF
C
C  BERECHNEN DER ABSOLUTEN ZEILENSUMMEN UND VORBESETZEN DES
C  PIVOTVEKTORS.
C
      DO 20 I=1,N
         IPIVOT(I)=I
         SUM=ABS(A(I,1))
         DO 30 K=2,N
            SUM=SUM+ABS(A(I,K))
   30    CONTINUE
         IF(SUM.EQ.0.D0)THEN
            MARKE=0
            RETURN
         ELSE
            D(I)=1.D0/SUM
         END IF
   20 CONTINUE
      IF(N.EQ.1) RETURN
C
C  DREIECKSZERLEGUNG.
C
      DO 40 I=1,N-1
C
C  PIVOTZEILE BESTIMMEN.
C
         PIVOT=ABS(A(I,I))*D(I)
         IPVT=I
         DO 50 J=I+1,N
            HILF=ABS(A(J,I))*D(J)
            IF(HILF.GT.PIVOT) THEN
               PIVOT=HILF
               IPVT=J
            END IF
   50    CONTINUE
         IF(PIVOT.EQ.0.D0) THEN
            MARKE=0
            RETURN
         ELSE
            IF(IPVT.NE.I) THEN
C
C  I-TE UND IPVT-TE ZEILE VERTAUSCHEN.
C
               MARKE=-MARKE
               J=IPIVOT(I)
               IPIVOT(I)=IPIVOT(IPVT)
               IPIVOT(IPVT)=J
               HILF=D(I)
               D(I)=D(IPVT)
               D(IPVT)=HILF
               DO 60 J=1,N
                  HILF=A(I,J)
                  A(I,J)=A(IPVT,J)
                  A(IPVT,J)=HILF
   60          CONTINUE
            END IF
C
C  ELIMINATIONSSCHRITT AUSFUEHREN.
C
            DO 70 J=I+1,N
               A(J,I)=A(J,I)/A(I,I)
               FAK=A(J,I)
               DO 80 K=I+1,N
                  A(J,K)=A(J,K)-FAK*A(I,K)
   80          CONTINUE
   70       CONTINUE
         END IF
   40 CONTINUE
      IF(ABS(A(N,N)).EQ.0.D0) MARKE=0
      RETURN
      END
C
C
      SUBROUTINE FGAUSL(N,A,IA,IPIVOT,Y,X)
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
C
      IMPLICIT REAL (A-H,O-Z)
      DIMENSION A(1:IA,1:N),Y(1:N),X(1:N)
      INTEGER IPIVOT(1:N)
      IF(N.EQ.1) THEN
         X(1)=Y(1)/A(1,1)
         RETURN
      END IF
C
C  VORWAERTSELIMINATION DURCHFUEHREN.
C
      IPVT=IPIVOT(1)
      X(1)=Y(IPVT)
      DO 10 I=2,N
         SUM=0.D0
         DO 20 J=1,I-1
            SUM=SUM+A(I,J)*X(J)
   20    CONTINUE
         IPVT=IPIVOT(I)
         X(I)=Y(IPVT)-SUM
   10 CONTINUE
C
C  BERECHNEN DER LOESUNG DURCH RUECKWAERTSELIMINATION.
C
      X(N)=X(N)/A(N,N)
      DO 50 I=N-1,1,-1
         SUM=0.D0
         DO 40 K=N,I+1,-1
            SUM=SUM+A(I,K)*X(K)
   40    CONTINUE
         X(I)=(X(I)-SUM)/A(I,I)
   50 CONTINUE
      RETURN
      END
