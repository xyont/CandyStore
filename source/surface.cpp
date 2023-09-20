#include "surface.h"

surface::surface()
{
}
//*****************************************************************************************
/*! Returns the label of the surface */
long surface::getLabel()
{
        return(label);
}
//*****************************************************************************************
/*! Sets the label of the surface */
void surface::setLabel(long l)
{
        label=l;
}//*****************************************************************************************
/*! Sets the edges of the surface */
void surface::setEdges(QList<long> edgeList)
{
    edges.clear();
    edges=edgeList;
    //create a list of the corner points


}
