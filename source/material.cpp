#include "material.h"


material::material()
{

}

void material::setName(QString n )
{
    name=n;
}

void material::addElastic(double ym,double poi, double temp=293)
{
    elastic tempElastic;
    tempElastic.yModulus=ym;
    tempElastic.poison=poi;
    tempElastic.temperature=temp;

    elasticDataList.append(tempElastic);

}
void material::addDensity(double dens,double temp=293)
{
    density tempDens;
    tempDens.value=dens;
    tempDens.temperature=temp;

    densityList.append(tempDens);

}
