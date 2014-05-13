#include "BallJoint.h"

BallJoint::BallJoint(Node *parent) : Joint(parent)
{
    Node::setType("Ball Joint");
    setType(Type::BallJoint);
    setDOF(DOF::RotationX, new DOF(DOF::RotationX));
    setDOF(DOF::RotationY, new DOF(DOF::RotationY));
    setDOF(DOF::RotationZ, new DOF(DOF::RotationZ));
    Node::setDOFEnabled(DOF::RotationX, true);
    Node::setDOFEnabled(DOF::RotationY, true);
    Node::setDOFEnabled(DOF::RotationZ, true);
}

BallJoint::~BallJoint()
{
}
