#include "FreeJoint.h"

FreeJoint::FreeJoint(Node *parent) : Joint(parent)
{
    setType(Type::FreeJoint);
    setDOF(DOF::TranslationX, new DOF(DOF::TranslationX));
    setDOF(DOF::TranslationY, new DOF(DOF::TranslationY));
    setDOF(DOF::TranslationZ, new DOF(DOF::TranslationZ));
    setDOF(DOF::RotationX, new DOF(DOF::RotationX));
    setDOF(DOF::RotationY, new DOF(DOF::RotationY));
    setDOF(DOF::RotationZ, new DOF(DOF::RotationZ));
    Node::setDOFEnabled(DOF::RotationX, true);
    Node::setDOFEnabled(DOF::RotationY, true);
    Node::setDOFEnabled(DOF::RotationZ, true);
    Node::setDOFEnabled(DOF::TranslationX, true);
    Node::setDOFEnabled(DOF::TranslationY, true);
    Node::setDOFEnabled(DOF::TranslationZ, true);
    Node::setType("Free Joint");
}

FreeJoint::~FreeJoint()
{
}
