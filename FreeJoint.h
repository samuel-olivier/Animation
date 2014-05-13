#ifndef FREEJOINT_H
#define FREEJOINT_H

#include "Joint.h"

class FreeJoint : public Joint
{
public:
    FreeJoint(Node* parent);
    virtual ~FreeJoint();};

#endif // FREEJOINT_H
