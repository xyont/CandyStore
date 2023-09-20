#ifndef OCTREE_H
#define OCTREE_H

#include "./vector3d/vector3d.h"
#include <QList>
#include <QObject>
//#include "femodel.h"
class femodel;

class octNode //: public QObject
{

    //Q_OBJECT


public:
    explicit octNode();
    ~octNode();
    explicit octNode(const octNode&) ;
    octNode(femodel *fe=0);

    void setCenter(vector3d);
    void divide();

    vector3d getCenter();
    QList<long> items;
    double edgeLength;
    QList<octNode> children;
    bool hasItems;
    bool hasChildren;

    void setChildrenCenterAndSize();
    void putItemIntoChild(long);
    octNode *childNode;

private:
    femodel *fedata;
    vector3d center;


};

//******************************
class octree
{
public:
    octree(femodel *fe);
    void initialise();


    octNode *rootNode;
    femodel *fedata;

};
//*******************************


#endif // OCTREE_H
