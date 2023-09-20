#ifndef OPENFOAMFACE_H
#define OPENFOAMFACE_H

class openfoamface
{
public:
    openfoamface();
    long int ownerElement;
    int ownerFace;
    long int neighbourElement;
    int neighbourFace;
    long vertices[20];
    int type;

    bool operator<(const openfoamface &f1) const;
    bool operator>(const openfoamface &f1) const;
};

#endif // OPENFOAMFACE_H
