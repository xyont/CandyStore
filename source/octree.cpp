#include "octree.h"
#include "femodel.h"

octree::octree(femodel *fe)
{
     fedata=fe;
     rootNode = new octNode(fedata);
     rootNode->hasItems=false;
     rootNode->hasChildren=false;
     rootNode->divide();
     rootNode->edgeLength=1;
     rootNode->setCenter(vector3d(0,0,0));
}
//**********************
//********************************
void octree::initialise()
{
    for (long k=0;k<fedata->nodeList.size();k++)
    {
        rootNode->items.append(fedata->nodeList[k].getLabel());
    }
    rootNode->setCenter(fedata->modelCenter);
    rootNode->edgeLength=fedata->modelRadius*3;
    if (rootNode->items.size() > 0)
    {
        cout << "Items found";
        rootNode->hasItems=true;
	rootNode->hasChildren=true;
	rootNode->divide();
    }

}

//*********************************
 octNode::octNode()
{
    ;
}

octNode::~octNode()
{

}

octNode::octNode(femodel *fe)
{
    fedata=fe;
    //fedata->sendM("Octnode created");

}
//********************************
octNode::octNode(const octNode &oc) 
{
    fedata=oc.fedata;
    items=oc.items;
    center=oc.center;
    children=oc.children;
}
//********************************
/*! Set the center of the octNode*/
void octNode::setCenter(vector3d c)
{
    center=c;
}
//********************************
/*! Return the center of the octNode*/
vector3d octNode::getCenter()
{
    return(center);
}
//*********************************
/*! Divide the octNode into its children*/
void octNode::divide()
{
    hasItems=true;
    hasChildren=true;
    //Do no further splitting if the node has less than 50 items
    if (this->items.size() < 50)
    {
        if (this->items.size()==0)
        {
            hasItems=false;
	    
            //fedata->sendM("Child is empty");
        }
	hasChildren=false;
        return;
    }
    //******************************
    for (int g=0;g<8;g++)
    {
        childNode = new octNode(fedata);
        childNode->hasItems=false;
        childNode->hasChildren=false;
        childNode->items.clear();
        children.append(*childNode);
    }
    setChildrenCenterAndSize();

    // ***************************
    //put items in the new nodes
    for (long n=0;n<items.size();n++)
    {
        putItemIntoChild(items[n]);
    }
    // ****************************
    //Go one level deeper
    for (int n=0;n<8;n++)
    {
        children[n].divide();
    }

}
//*****************************************
void octNode::setChildrenCenterAndSize()
{
    //Calculate the 8 new boxes
    // Ordering: +++, -++, --+, +-+
    //           ++-, -+-, ---, +--
    //*****************
    vector3d offset;

    //firstBox + + +:
      offset.x=0.25*edgeLength;
      offset.y=0.25*edgeLength;
      offset.z=0.25*edgeLength;
      children[0].setCenter(center+offset);
      children[0].edgeLength=0.5*edgeLength;
      //fedata->sendM(QString("center %1 %2 %3 length %4").arg((center+offset).x).arg((center+offset).y).arg((center+offset).z).arg(0.5*edgeLength));
      //second box  - + +
      offset.x=-0.25*edgeLength;
      offset.y=0.25*edgeLength;
      offset.z=0.25*edgeLength;
      children[1].setCenter(center+offset);
      children[1].edgeLength=0.5*edgeLength;
      //third box  - - +
      offset.x=-0.25*edgeLength;
      offset.y=-0.25*edgeLength;
      offset.z=0.25*edgeLength;
      children[2].setCenter(center+offset);
      children[2].edgeLength=0.5*edgeLength;
      //fourth box  + - +
      offset.x=0.25*edgeLength;
      offset.y=-0.25*edgeLength;
      offset.z=0.25*edgeLength;
      children[3].setCenter(center+offset);
      children[3].edgeLength=0.5*edgeLength;
      //fifth box  + + -
      offset.x=0.25*edgeLength;
      offset.y=0.25*edgeLength;
      offset.z=-0.25*edgeLength;
      children[4].setCenter(center+offset);
      children[4].edgeLength=0.5*edgeLength;
      //sixth box  - + -
      offset.x=-0.25*edgeLength;
      offset.y=0.25*edgeLength;
      offset.z=-0.25*edgeLength;
      children[5].setCenter(center+offset);
      children[5].edgeLength=0.5*edgeLength;
      //seventh box  - - -
      offset.x=-0.25*edgeLength;
      offset.y=-0.25*edgeLength;
      offset.z=-0.25*edgeLength;
      children[6].setCenter(center+offset);
      children[6].edgeLength=0.5*edgeLength;
      //eighth box  + - -
      offset.x=0.25*edgeLength;
      offset.y=-0.25*edgeLength;
      offset.z=-0.25*edgeLength;
      children[7].setCenter(center+offset);
      children[7].edgeLength=0.5*edgeLength;

}
//****************************************
void octNode::putItemIntoChild(long label)
{
    double x,y,z;
    vector3d coords=fedata->getNodeCoords(label);
    x=coords.x;
    y=coords.y;
    z=coords.z;



        if (z-center.z > 0.0)
        {
            //Node is in upper half
            if (y- center.y > 0.0 )
            {
                if (x-center.x>0.0)
                {
                    children[0].items.append(label);
                    //fedata->sendM("item in first child");
                }
                else
                {
                    children[1].items.append(label);
                }

            }
            else
            {
                if (x-center.x>0.0)
                {
                    children[3].items.append(label);
                }
                else
                {
                    children[2].items.append(label);
                }
            }

        }
        else
        {
            if (y- center.y > 0.0 )
            {
                if (x-center.x>0.0)
                {
                    children[4].items.append(label);
                }
                else
                {
                    children[5].items.append(label);
                }

            }
            else
            {
                if (x-center.x>0.0)
                {
                    children[7].items.append(label);
                }
                else
                {
                    children[6].items.append(label);
                }
            }
        }



}
//*******************

