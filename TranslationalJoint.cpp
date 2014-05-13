#include "TranslationalJoint.h"

TranslationalJoint::TranslationalJoint(Node *parent) : Joint(parent)
{
    setType(Type::TranslationalJoint);
    setDOF(DOF::TranslationX, new DOF(DOF::TranslationX));
    setDOF(DOF::TranslationY, new DOF(DOF::TranslationY));
    setDOF(DOF::TranslationZ, new DOF(DOF::TranslationZ));
    Node::setDOFEnabled(DOF::TranslationX, true);
    Node::setDOFEnabled(DOF::TranslationY, true);
    Node::setDOFEnabled(DOF::TranslationZ, true);
    Node::setType("Translation Joint");
}

TranslationalJoint::~TranslationalJoint() {
}
