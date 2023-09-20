#include "creatematerialwidget.h"

createMaterialWidget::createMaterialWidget(QWidget *parent) :
    QWidget(parent)
{
    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel(" Materials",this);
    header->setFrameStyle(QFrame::Box);
    nameLabel = new QLabel("Name:",this);
    firstDOFLabel = new QLabel("Young's Modulus:",this);
    lastDOFLabel = new QLabel("Poisson's Ratio:",this);
    valueLabel = new QLabel("Density:",this);
    //targetSetHeader = new QLabel("Target set:",this);
    //targetSetComboBox= new QComboBox();
    name =  new QLineEdit(this);
    firstDOF = new QLineEdit(this);
    lastDOF = new QLineEdit(this);
    value =  new QLineEdit(this);
    //DispScalButton = new QPushButton("Select",this);
    //DispScalButton->setFixedWidth(50);
    lowerNodes=new QCheckBox("Keep lower node IDs");
    higherNodes=new QCheckBox("Keep higher node IDs");
    //allNodes->setCheckState(Qt::Checked);
    //outsideFaces=new QCheckBox("outside Faces");
    //xEdit = new QLineEdit(this);
    //yEdit = new QLineEdit(this);
    //zEdit = new QLineEdit(this);


    QVBoxLayout *vLayout = new QVBoxLayout;
    //QHBoxLayout *DispScalLayout = new QHBoxLayout;


    //DispScalLayout->addWidget(DispScalEdit);
    //DispScalLayout->addWidget(DispScalButton);

    vLayout->addWidget(header);
    vLayout->addStretch(1);
    vLayout->addWidget(nameLabel);
    vLayout->addWidget(name);
    //vLayout->addWidget(targetSetHeader);
    //vLayout->addWidget(targetSetComboBox);
    vLayout->addWidget(firstDOFLabel);
    vLayout->addWidget(firstDOF);
    vLayout->addWidget(lastDOFLabel);
    vLayout->addWidget(lastDOF);
    vLayout->addWidget(valueLabel);
    vLayout->addWidget(value);
    vLayout->addStretch(1);
    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));

}
//****************************************************
/*!Check values and create the boundary condition */
void createMaterialWidget::startClicked()
{
    if (name->text()=="")
    {
        emit sendMessage(QString("A name must be specified!"));
        return;
    }
    emit startMaterialCreation(name->text(),firstDOF->text().toDouble(),lastDOF->text().toDouble(),value->text().toDouble());
}
