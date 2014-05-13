#ifndef PRISMATICJOINT_H
#define PRISMATICJOINT_H

#include "Joint.h"

class PrismaticJoint : public Joint
{
public:
    PrismaticJoint(Node* parent);
    virtual ~PrismaticJoint();

    virtual void    setDOFEnabled(DOF::Type dof, bool isEnabled);

};

#endif // PRISMATICJOINT_H
