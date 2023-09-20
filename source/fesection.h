#ifndef FESECTION_H
#define FESECTION_H

#include <QString>

class feSection
{
public:
    feSection();

    QString name;
    QString set;
    QString material;
    int type;
    double thickness;
};

#endif // FESECTION_H
