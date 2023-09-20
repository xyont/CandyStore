#include "openfoamface.h"

openfoamface::openfoamface()
{
}
//**************************************************************************************************************
/*!
< Operator
A openfoamface is smaller than another when 1. its owner cell is smaller 2. the neihbour cell is smaller
*/
bool openfoamface::operator<(const openfoamface &f1) const
{
    if (ownerElement < f1.ownerElement)
    {
        return(true);
    }
    else
    {
        if (ownerElement > f1.ownerElement)
        {
            return(false);
        }
        else
        {
            if (neighbourElement < f1.neighbourElement)
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
    }
    return (false);
}
//**************************************************************************************************************
/*!
> Operator -

*/
bool openfoamface::operator>(const openfoamface &f1) const
{
    if (this->ownerElement > f1.ownerElement)
    {
        return(true);
    }
    else
    {
        if (this->ownerElement < f1.ownerElement)
        {
            return(false);
        }
        else
        {
            if (this->neighbourElement > f1.neighbourElement)
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
    }
    return (false);
}
//*********************************************************************************
