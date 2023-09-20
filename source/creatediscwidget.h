#ifndef CREATEDISCWIDGET_H
#define CREATEDISCWIDGET_H

#include <QtGui>

class createDiscWidget : public QWidget
{
    Q_OBJECT

public:
    createDiscWidget(QWidget *parent = 0);

    QPushButton *closeButton;
    QPushButton *startButton;


    QSpacerItem *vSpacer;
    QSpacerItem *vSpacer2;
    QLabel *header;
    QLabel *radiusLabel;
    QLabel *innerRadiusLabel;
    QLabel *outerRadiusLabel;
    QLabel *hubWidthLabel;
    QLabel *rimWidthLabel;
    QLabel *axialPosLabel;
    QLabel *discDataLabel;
    QLabel *rimLoadLabel;
    QLabel *massLabel;
    QLabel *rotSpeedLabel;
    QLineEdit *massEdit;
    QLineEdit *radiusEdit;
    QLineEdit *rotSpeedEdit;
    QLineEdit *innerRadiusEdit;
    QLineEdit *outerRadiusEdit;
    QLineEdit *hubWidthEdit;
    QLineEdit *rimWidthEdit;
    QLineEdit *axialPosEdit;

    void calculateStress(double rev, double r_aussen, double randlast,
            double r_randlast, int,double*,double,double,double,double*,double*);

    void createGeometry(double r_innen, double r_aussen, int segments, double kopfbreite,
                        double fussbreite,double,double rpm,double mass_rimload,double r_rimload);

    public slots:



    private slots:
        void startClicked();
        void startDiscCreation(double iR,double oR, double hW, double rW,double);

    signals:
    //! Sends a  QString
    void sendMessage(const QString&);
    //! Starts the disc creation

    long createPoint(double,double,double,int);
    long createCurve(long,long);

};

#endif // CREATEDISCWIDGET_H
