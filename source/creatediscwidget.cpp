#include "creatediscwidget.h"
#include <math.h>

createDiscWidget::createDiscWidget(QWidget *parent) :
        QWidget(parent)
{

closeButton = new QPushButton("Close",this);
startButton=new QPushButton("Apply",this);
header = new QLabel("Disc Creator: ",this);
header->setFrameStyle(QFrame::Box);

discDataLabel =new QLabel("Disc Data:",this);
rimLoadLabel =new QLabel("Rim Load:",this);
massLabel =new QLabel("Mass:",this);
radiusLabel= new QLabel("Radius COG:",this);
rotSpeedLabel =new QLabel("Rotational Speed [rpm]:",this);
axialPosLabel = new QLabel("Axial Position: ",this);
innerRadiusLabel = new QLabel("Inner Radius: ",this);
outerRadiusLabel = new QLabel("Outer Radius: ",this);
hubWidthLabel = new QLabel("Hub Width: ",this);
rimWidthLabel = new QLabel("Rim Width: ",this);

massEdit =new QLineEdit(this);
radiusEdit= new QLineEdit(this);
rotSpeedEdit = new QLineEdit(this);
axialPosEdit =new QLineEdit(this);
innerRadiusEdit = new QLineEdit(this);
outerRadiusEdit = new QLineEdit(this);
hubWidthEdit = new QLineEdit(this);
rimWidthEdit = new QLineEdit(this);


QVBoxLayout *vLayout = new QVBoxLayout;
vLayout->addWidget(header);
vLayout->addStretch(1);

vLayout->addWidget(rimLoadLabel);
vLayout->addWidget(massLabel);
vLayout->addWidget(massEdit);
vLayout->addWidget(radiusLabel);
vLayout->addWidget(radiusEdit);
vLayout->addWidget(rotSpeedLabel);
vLayout->addWidget(rotSpeedEdit);
vLayout->addStretch(1);
vLayout->addWidget(discDataLabel);
vLayout->addWidget(axialPosLabel);
vLayout->addWidget(axialPosEdit);
vLayout->addWidget(innerRadiusLabel);
vLayout->addWidget(innerRadiusEdit);
vLayout->addWidget(outerRadiusLabel);
vLayout->addWidget(outerRadiusEdit);
vLayout->addWidget(hubWidthLabel);
vLayout->addWidget(hubWidthEdit);
vLayout->addWidget(rimWidthLabel);
vLayout->addWidget(rimWidthEdit);



vLayout->addStretch(1);
vLayout->addWidget(startButton);
vLayout->addWidget(closeButton);
this->setLayout(vLayout);

connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));

}

//****************************************************
/*!Check values and create the disc*/
void createDiscWidget::startClicked()
{
    int nSegments=20;

    double innerR=innerRadiusEdit->text().toDouble();
    double outerR=outerRadiusEdit->text().toDouble();
    double hubW=hubWidthEdit->text().toDouble();
    double rimW=rimWidthEdit->text().toDouble();
    double aPos=axialPosEdit->text().toDouble();
    double rpm=rotSpeedEdit->text().toDouble();
    double mass=massEdit->text().toDouble();
    double r_rimload=radiusEdit->text().toDouble();
    emit sendMessage("Starting disc creation");
    //startDiscCreation(innerR,outerR,hubW,rimW,aPos);
    createGeometry(innerR,outerR,nSegments,rimW,hubW,aPos,rpm,mass,r_rimload);

}
//****************************************************
/*! Create the disc*/
void createDiscWidget::startDiscCreation(double iR,double oR, double hW, double rW, double axialPos)
{
    double hubHeight=5.0;
    double hubWebHeight=5.0;
    double webWidth=3.0;
    double rimHeight=5.0;
    double rimWebHeight=5.0;


    //Create Points for lines
    long pointOffset=createPoint(iR,(-hW/2)+axialPos,0,0)-1;
    //emit sendMessage(QString("offset %1").arg(offset));
    createPoint(iR,0+axialPos,0,0);
    createPoint(iR,(hW/2)+axialPos,0,0);
    //
    createPoint(iR+hubHeight,(-hW/2)+axialPos,0,0);
    createPoint(iR+hubHeight,(0)+axialPos,0,0);
    createPoint(iR+hubHeight,(hW/2)+axialPos,0,0);
    //
    createPoint(iR+hubHeight+hubWebHeight,(-webWidth/2)+axialPos,0,0);
    createPoint(iR+hubHeight+hubWebHeight,(0)+axialPos,0,0);
    createPoint(iR+hubHeight+hubWebHeight,(webWidth/2)+axialPos,0,0);
    //
    createPoint(oR-rimHeight-rimWebHeight,(-webWidth/2)+axialPos,0,0);
    createPoint(oR-rimHeight-rimWebHeight,(0)+axialPos,0,0);
    createPoint(oR-rimHeight-rimWebHeight,(webWidth/2)+axialPos,0,0);
    //
    createPoint(oR-rimHeight,(-rW/2)+axialPos,0,0);
    createPoint(oR-rimHeight,(0)+axialPos,0,0);
    createPoint(oR-rimHeight,(rW/2)+axialPos,0,0);
    //
    createPoint(oR,(-rW/2)+axialPos,0,0);
    createPoint(oR,(0)+axialPos,0,0);
    createPoint(oR,(rW/2)+axialPos,0,0);

    //create lines
    long lineOffset=createCurve(1+pointOffset,2+pointOffset);
    createCurve(2+pointOffset,3+pointOffset);
    createCurve(1+pointOffset,4+pointOffset);
    createCurve(4+pointOffset,5+pointOffset);
    createCurve(5+pointOffset,6+pointOffset);
    createCurve(6+pointOffset,3+pointOffset);
    createCurve(5+pointOffset,2+pointOffset);
    //
    createCurve(4+pointOffset,7+pointOffset);
    createCurve(5+pointOffset,8+pointOffset);
    createCurve(6+pointOffset,9+pointOffset);
    //
    createCurve(7+pointOffset,8+pointOffset);
    createCurve(8+pointOffset,9+pointOffset);
    //
    createCurve(7+pointOffset,10+pointOffset);
    createCurve(8+pointOffset,11+pointOffset);
    createCurve(9+pointOffset,12+pointOffset);
    //
    createCurve(10+pointOffset,11+pointOffset);
    createCurve(11+pointOffset,12+pointOffset);
    //
    createCurve(10+pointOffset,13+pointOffset);
    createCurve(11+pointOffset,14+pointOffset);
    createCurve(12+pointOffset,15+pointOffset);
    //
    createCurve(13+pointOffset,14+pointOffset);
    createCurve(14+pointOffset,15+pointOffset);
    //
    createCurve(13+pointOffset,16+pointOffset);
    createCurve(14+pointOffset,17+pointOffset);
    createCurve(15+pointOffset,18+pointOffset);
    //
    createCurve(16+pointOffset,17+pointOffset);
    createCurve(17+pointOffset,18+pointOffset);

}
//***********************************************
//***********************************************
/*! Calculate stresses in a disc*/
void createDiscWidget::calculateStress(double rev,double r_aussen, double randlast,
        double r_randlast, int nSegments,double *breite_segment, double yModulus,double density,double alpha, double *radius,double *temperatur)
{
    // Hilfsvariablen:

    // Ende Hilfsvariablen

    double sigma_radial[nSegments+1];
    double sigma_tangential[nSegments+1];
    double sigma_radial_0[nSegments + 1];
    double sigma_tangential_0[nSegments + 1];
    double radialspannung[nSegments + 1];
    double tangentialspannung[nSegments + 1];
    double vergleichsspannung[nSegments + 1];

    // Randlast berechnen:
    double omega = M_PI * rev / 30;
    double sigma_radial_rand = ((randlast * r_randlast * pow(omega, 2)) / (2 * M_PI * r_aussen * breite_segment[nSegments]));

    emit sendMessage(QString("Sigma radial rand: %1").arg(sigma_radial_rand));

    // Randwerte vorgeben:
    sigma_tangential[0] = 0;
    sigma_tangential_0[0] = 1;
    sigma_radial[0] = 0;
    sigma_radial_0[0]=0;


    // Spannungen in der drehenden Scheibe:

    for (int i = 1; i < nSegments+1; i++)
    {
        sigma_radial[i] = sigma_radial[i - 1]
            + (((sigma_tangential[i - 1] - sigma_radial[i - 1] - density * pow((radius[i - 1] * omega), 2))) * ((radius[i] - radius[i - 1]) / radius[i - 1]))
            - (sigma_radial[i - 1] * ((breite_segment[i] - breite_segment[i - 1]) / breite_segment[i - 1]));

        sigma_tangential[i] = sigma_tangential[i-1]
            + ((0.3 * (sigma_radial[i] - sigma_radial[i - 1]))
            - (1.3 * (sigma_tangential[i - 1] - sigma_radial[i - 1]) * ((radius[i] - radius[i - 1]) / radius[i - 1]))
            - (yModulus * alpha * (temperatur[i] - temperatur[i - 1])));

    }
    //

    omega = 0;

    // Spannungen ohne Rotation:

    for (int i = 1; i < nSegments + 1; i++)
    {
        sigma_radial_0[i] = sigma_radial_0[i - 1]
            + (((sigma_tangential_0[i - 1] - sigma_radial_0[i - 1] - ((density) * pow((radius[i - 1] * omega), 2)) ) * ((radius[i] - radius[i - 1]) / radius[i - 1]))
            - (sigma_radial_0[i - 1] * ((breite_segment[i] - breite_segment[i - 1]) / breite_segment[i - 1])));

        sigma_tangential_0[i] = sigma_tangential_0[i - 1]
            + ((0.3 * (sigma_radial_0[i] - sigma_radial_0[i - 1]))
            - (1.3 * (sigma_tangential_0[i - 1] - sigma_radial_0[i - 1]) * ((radius[i] - radius[i - 1]) / radius[i - 1])));

    }
    //

    // Korrekturfaktor berechnen:
    double xi = ((sigma_radial_rand - sigma_radial[nSegments]) / sigma_radial_0[nSegments]);

    // Endgültige Spannung berechnen:

    for (int i = 0; i < nSegments + 1; i++)
    {
        radialspannung[i] = sigma_radial[i] + (xi * sigma_radial_0[i]);

        tangentialspannung[i] = sigma_tangential[i] + (xi * sigma_tangential_0[i]);
    }

    //

    // Vergleichsspannung berechnen:

    for (int i = 0; i < nSegments+1; i++)
    {
        vergleichsspannung[i] = sqrt(pow(radialspannung[i], 2) + pow(tangentialspannung[i], 2) - radialspannung[i] * tangentialspannung[i]);
        emit sendMessage(QString("Schnitt %1: Sigma_R: %2 Sigma_T %3 vMises: %4").arg(i).arg(radialspannung[i]).arg(tangentialspannung[i]).arg(vergleichsspannung[i]));
    }

}
//**************************************
//**************************************
/*! Generieren der Disc-Startkontur*/
void createDiscWidget::createGeometry(double r_innen, double r_aussen, int segments, double kopfbreite,
                                      double fussbreite, double axialPos, double rpm, double mass_rimload, double r_rimload)
{

            double min_dicke = 3.0; // Minimale Dicke in der Mitte der Scheibe

            //double _hoehe = r_aussen - r_innen;

            //double _hoehe_segments = (_hoehe / segments);
            //double _hoehe_segments_draw = (_hoehe_segments * Global.transform_scale);

            double breite_segment[segments+1];
            double radius[segments + 1];
            double temperatur[segments+1];

            int zaehler = 2;

                // Breiten der Segmente generieren:

                    int hoehe_uebergang2 = (int)(round(segments*0.125));
                    int hoehe_uebergang = (int)(segments*0.375);
                    int hoehe_mitte = (int)((segments + 1) - 3 - (hoehe_uebergang) - (hoehe_uebergang2));


                    double faktor1 = pow(((min_dicke) / fussbreite), ((double)(1) / (double)(hoehe_uebergang)));
                    double faktor2 = pow((kopfbreite / (min_dicke)), ((double)(1) / (double)(hoehe_uebergang2)));

                    breite_segment[0] = fussbreite;
                    breite_segment[1] = fussbreite;

                    for (int i = 1; i <= hoehe_uebergang; i++) // Übergang von Innenrand zu Mitte
                    {
                        breite_segment[zaehler] = (breite_segment[zaehler - 1] * faktor1);
                        zaehler++;
                    }

                    breite_segment[zaehler - 1] = min_dicke;

                    for (int i = 1; i <= hoehe_mitte; i++) // Mittlerer Teil
                    {
                        breite_segment[zaehler] = breite_segment[zaehler - 1];
                        zaehler++;
                    }

                    breite_segment[zaehler - 1] = min_dicke;

                    for (int i = 1; i <= hoehe_uebergang2; i++) // Übergang von Mitte zu Außenrand
                    {
                        breite_segment[zaehler] = (breite_segment[zaehler - 1] * faktor2);
                        zaehler++;
                    }

                    breite_segment[zaehler] = breite_segment[zaehler - 1];


               //

                    // Radiusarray befüllen:
                    for (int i = 0; i < segments + 1; i++)
                    {
                        radius[i] = ((double)(r_innen + ((double)(r_aussen - r_innen) / segments) * i));
                    }
                    //Temperature-Array initializing
                    for (int i = 0; i < segments + 1; i++)
                    {
                        temperatur[i] = 293.0;
                    }
                    //

                    //create the points
                    long pOffset=createPoint(radius[0],-(breite_segment[0])/2+axialPos,0,0)-1;
                    createPoint(radius[0],+(breite_segment[0])/2+axialPos,0,0);
                    for (int p=1;p<segments+1;p++)
                    {
                        createPoint(radius[p],-(breite_segment[p])/2+axialPos,0,0);
                        createPoint(radius[p],+(breite_segment[p])/2+axialPos,0,0);
                    }
                    //create the lines
                    for (int p=1;p<segments*2;p=p+2)
                    {
                        createCurve(p+pOffset,p+2+pOffset);
                        createCurve(p+1+pOffset,p+3+pOffset);
                    }
                    //create upper and lower line
                    createCurve(1+pOffset,2+pOffset);
                    createCurve(segments*2+1+pOffset,segments*2+2+pOffset);

                    calculateStress(rpm,r_aussen,mass_rimload,r_rimload,segments,breite_segment,210000,8E-9,11E-6,radius,temperatur);

}
//*************************************
//************************************
/*! Dispalcement of a thin ring under rotation*/
//reference:Sawyers Gas Turbine Engine Handbook Vol 1, 1972
//
/*
double createDiscWidget::thinRingDisplacement(double radius,double rpm,double density, double yModulus, double alpha, double temperature)
{
    double omega=rpm/30*M_PI;
    double dr=pow(radius,3)*density*pow(omega,2)+alpha*radius*temperature;
    return(dr); ^ ^
}
*/
