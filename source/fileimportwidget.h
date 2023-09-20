#ifndef FILEIMPORTWIDGET_H
#define FILEIMPORTWIDGET_H

#include <QtGui>

class fileImportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit fileImportWidget(QWidget *parent = 0);

    QPushButton *closeButton;
    QPushButton *startButton;

    QLabel *typeHeader;
    QComboBox *typeComboBox;

    QLabel *header;

signals:
     void sendMessage(const QString&);
public slots:
     void startClicked();

};

#endif // FILEIMPORTWIDGET_H
