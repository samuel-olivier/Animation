#include "PrismaticJoint.h"

PrismaticJoint::PrismaticJoint(Node *parent) : Joint(parent)
{
    setType(Joint::PrismaticJoint);
    setDOF(DOF::TranslationX, new DOF(DOF::TranslationX));
    setDOF(DOF::TranslationY, new DOF(DOF::TranslationY));
    setDOF(DOF::TranslationZ, new DOF(DOF::TranslationZ));
    Node::setType("Pismatic Joint");
}

PrismaticJoint::~PrismaticJoint()
{
}

void PrismaticJoint::setDOFEnabled(DOF::Type dof, bool isEnabled)
{
    Node::setDOFEnabled(DOF::TranslationX, false);
    Node::setDOFEnabled(DOF::TranslationY, false);
    Node::setDOFEnabled(DOF::TranslationZ, false);
    if (isEnabled == true && (dof == DOF::TranslationX || dof == DOF::TranslationY || dof == DOF::TranslationZ))
        Node::setDOFEnabled(dof, true);
}
