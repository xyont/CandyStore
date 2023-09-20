#include "boundarycon.h"

boundarycon::boundarycon()
{
}
// *******************************************************
/*! Setting the values for a boundary condition*/

void boundarycon::setValues(QString nam, QString set,int dof_first, int dof_last, double val)
{
    name=nam;
    nodeset=set;
    firstDof=dof_first;
    lastDof=dof_last;
    value=val;
}


