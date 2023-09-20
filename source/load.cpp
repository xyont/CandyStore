#include "load.h"

load::load()
{
}
//********************************
QString load::getPoint1String()
{
    QString test="";

    test=QString::number(point1.x) + "," + QString::number(point1.y) + "," + QString::number(point1.z);
    return(test);
}
//*******************************
QString load::getPoint2String()
{
    QString test="";

    test=QString::number(point2.x) + "," + QString::number(point2.y) + "," + QString::number(point2.z);
    return(test);
}
