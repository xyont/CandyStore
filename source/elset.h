#ifndef ELSET_H
#define ELSET_H
#include <QString>
#include <QList>


class elSet
{
public:
    elSet();
    QString name;
    QList<long> elLabels;
    long getIndex(long searchElement, int* exists);
};

#endif // ELSET_H
