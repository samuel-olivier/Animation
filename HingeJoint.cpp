#include "HingeJoint.h"

HingeJoint::HingeJoint(Node *parent) : Joint(parent)
{
    setType(Joint::HingeJoint);
    setDOF(DOF::RotationX, new DOF(DOF::RotationX));
    setDOF(DOF::RotationY, new DOF(DOF::RotationY));
    setDOF(DOF::RotationZ, new DOF(DOF::RotationZ));
    Node::setType("Hinge Joint");
}

HingeJoint::~HingeJoint()
{
}

void HingeJoint::setDOFEnabled(DOF::Type dof, bool isEnabled)
{
    Node::setDOFEnabled(DOF::RotationX, false);
    Node::setDOFEnabled(DOF::RotationY, false);
    Node::setDOFEnabled(DOF::RotationZ, false);
    if (isEnabled == true && (dof == DOF::RotationX || dof == DOF::RotationY || dof == DOF::RotationZ))
        Node::setDOFEnabled(dof, isEnabled);
}
