#ifndef FIXEDJOINT_H
#define FIXEDJOINT_H

#include "Joint.h"

class FixedJoint : public Joint
{
public:
    FixedJoint(Node* parent);
    virtual ~FixedJoint();
};

#endif // FIXEDJOINT_H
