#ifndef AXIALJOINT_H
#define AXIALJOINT_H

#include "Joint.h"

#include "DOF.h"

class TranslationalJoint : public Joint
{
public:
    TranslationalJoint(Node* parent);
    virtual ~TranslationalJoint();
};

#endif // AXIALJOINT_H
