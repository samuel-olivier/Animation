#ifndef HINGEJOINT_H
#define HINGEJOINT_H

#include "Joint.h"

class HingeJoint : public Joint
{
public:
    HingeJoint(Node* parent);
    virtual ~HingeJoint();

    virtual void    setDOFEnabled(DOF::Type dof, bool isEnabled);

};

#endif // HINGEJOINT_H
