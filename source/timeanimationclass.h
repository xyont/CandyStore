#ifndef TIMEANIMATIONCLASS_H
#define TIMEANIMATIONCLASS_H

#include <QObject>
#include "result.h"

class timeAnimationClass : public QObject
{
    Q_OBJECT

public:
    explicit timeAnimationClass(QObject *parent = 0);
    QList<result> resultList;
    QList<double> timepoints;
    int nTimesteps;
    int firstTimestep;
    int lastTimestep;

signals:

public slots:

};

#endif // TIMEANIMATIONCLASS_H
