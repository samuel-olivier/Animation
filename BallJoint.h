#ifndef BALLJOINT_H
#define BALLJOINT_H

#include "Joint.h"

#include "DOF.h"

class BallJoint : public Joint
{
public:
    BallJoint(Node* parent);
    virtual ~BallJoint();
};

#endif // BALLJOINT_H
