#ifndef MATERIAL_H
#define MATERIAL_H
#include <QString>
#include <QList>


struct elastic
{
    double yModulus;
    double poison;
    double temperature;
};

struct density
{
    double value;
    double temperature;
};
struct thermalExpansion
{
    double value;
    double temperature;
};

class material
{
public:
    material();

    QString name;
    QList<elastic> elasticDataList;
    QList<density> densityList;
    QList<thermalExpansion> thermalExpansionList;

    void setName(QString);
    void addElastic(double,double,double);
    void addDensity(double,double);
};

#endif // MATERIAL_H
