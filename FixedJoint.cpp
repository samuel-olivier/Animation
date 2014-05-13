#include "FixedJoint.h"

FixedJoint::FixedJoint(Node* parent) : Joint(parent)
{
    setType(Joint::FixedJoint);
    Node::setType("Fixed Joint");
}

FixedJoint::~FixedJoint() {
}
