#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QtGui>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

class materialWidget : public QWidget
 {
    Q_OBJECT
public:
    materialWidget(QWidget *parent = 0);
    QLabel *nameLabel;
    QLineEdit *nameEdit;
    QTabWidget *dataWidget;
    QPushButton *closeButton;
    QPushButton *okButton;
    QMessageBox *errorBox;

    QWidget *elasticData;
    QTableWidget *eDataTable;
    QwtPlot *eDataPlot;
    QwtPlot *densPlot;
    QwtPlotCurve *yMcurve;
    QwtPlotCurve *prCurve;
    QwtPlotCurve *densCurve;


    QList<double> eTempList;
    QList<double> eYmList;
    QList<double> prList;
    QList<double> densList;

 signals:
    //! Sends a  QString
    void sendMessage(const QString&);
    void startMaterialCreation(QString,double,double,double);

public slots:
    void dataChanged(int,int);
    void startClicked();
};

#endif // MATERIALWIDGET_H
