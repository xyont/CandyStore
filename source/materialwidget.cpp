#include "materialwidget.h"
#include <QStringList>

materialWidget::materialWidget(QWidget *parent) :
        QWidget(parent)
{

    errorBox=new QMessageBox();

    QStringList eHeaders;
    eHeaders.append("Temperature");
    eHeaders.append("Young's Modulus");
    eHeaders.append("Poisson Ratio");
    eHeaders.append("Density");

    nameLabel=new QLabel("Material Name: ");
    nameEdit=new QLineEdit();
    dataWidget=new QTabWidget();
    closeButton=new QPushButton("Close");
    okButton=new QPushButton("Save");

    elasticData=new QWidget();
    eDataPlot =new QwtPlot(this);
    eDataPlot->enableAxis(QwtPlot::yRight);
    //densPlot=new QwtPlot(this);
    //densPlot->setAxisTitle(QwtPlot::xBottom,"Temperature");
    yMcurve =new QwtPlotCurve("Young's Modulus");
    prCurve =new QwtPlotCurve("Poisson's Ratio");
    densCurve =new QwtPlotCurve("Density");
    yMcurve->attach(eDataPlot);
    prCurve->attach(eDataPlot);
    prCurve->setYAxis(QwtPlot::yRight);
    //densCurve->attach(densPlot);

    eDataPlot->updateAxes();

    QPen pen1;
    QPen pen2;
    QPen pen3;
    pen1.setColor(Qt::red);
    pen2.setColor(Qt::green);
    pen3.setColor(Qt::blue);

    yMcurve->setPen(pen1);
    prCurve->setPen(pen2);
    densCurve->setPen(pen3);

    eDataTable =new QTableWidget();

    eDataTable->setColumnCount(4);
    eDataTable->setRowCount(1);

    for (int e=0;e<4;e++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem();
        eDataTable->setItem(0,e,newItem);
    }




    eDataTable->setHorizontalHeaderLabels(eHeaders);
    QHBoxLayout *el = new QHBoxLayout;
    QVBoxLayout *pl =new QVBoxLayout;
    el->addWidget(eDataTable);

    //pl->addWidget(densPlot);
    pl->addWidget(eDataPlot);

    el->addLayout(pl);

    elasticData->setLayout(el);

    dataWidget->addTab(elasticData,"Elastic");

    QHBoxLayout *nl = new QHBoxLayout;
    QHBoxLayout *bl = new QHBoxLayout;
    QVBoxLayout *vl = new QVBoxLayout;

    nl->addWidget(nameLabel);
    nl->addWidget(nameEdit);

    bl->addStretch();
    bl->addWidget(okButton);
    bl->addWidget(closeButton);
    bl->addStretch();

    vl->addLayout(nl);
    vl->addWidget(dataWidget);
    vl->addLayout(bl);

    this->setLayout(vl);

    connect(eDataTable,SIGNAL(cellChanged(int,int)),this,SLOT(dataChanged(int,int)));
    connect(okButton,SIGNAL(clicked()),this,SLOT(startClicked()));
}

//*******************************************************************************************
void materialWidget::dataChanged(int row, int column)
{
    double t[2];
    double ym[2];
    double pr[2];
    double dens[2];

    eTempList.clear();
    eYmList.clear();
    prList.clear();
    densList.clear();

    eTempList.append(eDataTable->item(0,0)->text().toDouble());
    eYmList.append(eDataTable->item(0,1)->text().toDouble());
    prList.append(eDataTable->item(0,2)->text().toDouble());
    densList.append(eDataTable->item(0,3)->text().toDouble());

//if only one entry exists, the values are constant over time
    if (eTempList.size()==1)
    {
        t[0]=0;
        t[1]=1000;

        ym[0]=eYmList[0];
        ym[1]=eYmList[0];

        pr[0]=prList[0];
        pr[1]=prList[0];

        dens[0]=densList[0];
        dens[1]=densList[0];

        yMcurve->setSamples(t,ym,2);
        prCurve->setSamples(t,pr,2);
        densCurve->setSamples(t,dens,2);
    }

    eDataPlot->replot();

}
//****************************************************
/*!Check values and create the boundary condition */
void materialWidget::startClicked()
{
    if (nameEdit->text()=="")
    {

        errorBox->setText("No name specified. Material has not been created!");
        errorBox->setIcon(QMessageBox::Warning);
        errorBox->show();
        return;
    }
    emit startMaterialCreation(nameEdit->text(),eYmList[0],prList[0],densList[0]);
}
