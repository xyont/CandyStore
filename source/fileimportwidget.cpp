#include "fileimportwidget.h"

fileImportWidget::fileImportWidget(QWidget *parent) :
    QWidget(parent)
{

    closeButton = new QPushButton("Close",this);
    startButton=new QPushButton("Apply",this);
    header = new QLabel("Import File: ",this);
    header->setFrameStyle(QFrame::Box);

    typeHeader = new QLabel("File Type:",this);
    typeComboBox= new QComboBox();
    typeComboBox->addItem("ABAQUS Input File (*.inp)");
    typeComboBox->addItem("ANSYS CDB File (*.cdb)");
    typeComboBox->addItem("VTK File (*.vtm)(*.vtu)");
    typeComboBox->addItem("STL File (*.stl)");



    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(header);
    vLayout->addStretch(1);

    vLayout->addWidget(typeHeader);
    vLayout->addWidget(typeComboBox);


    vLayout->addStretch(1);

    vLayout->addWidget(startButton);
    vLayout->addWidget(closeButton);
    this->setLayout(vLayout);

    connect(startButton,SIGNAL(clicked()),this,SLOT(startClicked()));
}

//****************************************************
/*!Check values and create the load */
void fileImportWidget::startClicked()
{

  // emit closeButton->clicked();
}
