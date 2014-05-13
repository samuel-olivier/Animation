#include <qmath.h>
#include "Node.h"

Node::Node(Node *parent) : _parent(NULL)
{
    setParent(parent);
    _visible = true;
    setType("Node");
    setName("Node");
}

Node::~Node()
{
    setParent(NULL);
    deleteChildren();
}

void Node::init()
{
}

void Node::addChild(Node *child)
{
    _children.insert(child);
    child->_parent = this;
}

void Node::removeChild(Node *child)
{
    _children.remove(child);
    child->_parent = NULL;
}

const QSet<Node *> &Node::children() const
{
    return _children;
}

void Node::deleteChildren()
{
    for (auto it = _children.begin(); it != _children.end();) {
        Node* toDelete = *it;
        it = _children.erase(it);
        delete toDelete;
    }
}

void Node::setParent(Node *parent)
{
    if (_parent)
        _parent->removeChild(this);
    _parent = parent;
    if (_parent)
        _parent->addChild(this);
}

Node *Node::parent()
{
    return _parent;
}

const QString &Node::name()
{
    return _name;
}

void Node::setName(const QString &newName)
{
    _name = newName;
}

Node *Node::childByName(const QString &childName)
{
    if (childName == name())
        return this;
    foreach (Node* child, _children) {
        Node* found = child ? child->childByName(childName) : NULL;
        if (found)
            return found;
    }
    return NULL;
}

const QString &Node::type()
{
    return _type;
}

void Node::setType(const QString &type)
{
    _type = type;
}

bool Node::visible() const
{
    return _visible;
}

void Node::setVisible(bool isVisible)
{
    _visible = isVisible;
    foreach (Node* child, _children) {
        child->setVisible(isVisible);
    }
}

void Node::update(float elapsed)
{
    updateMatrix();
    _world = _parent ? _parent->world() : QMatrix4x4();
    _world *= _local;
    foreach (Node* child, _children) {
        child->update(elapsed);
    }
}

void Node::updateMatrix()
{
    resetLocal();
}

void Node::resetLocal()
{
    _local.setToIdentity();
}

void Node::draw()
{
    foreach (Node* child, _children) {
        child->draw();
    }
}

void Node::setLocal(const QMatrix4x4 &matrix)
{
    _local = matrix;
}

QMatrix4x4 &Node::local()
{
    return _local;
}

const QMatrix4x4 &Node::world() const
{
    return _world;
}

QList<DOF*> const& Node::DOFs() const
{
    return _DOFList;
}

DOF* Node::getDOF(DOF::Type type) const
{
    return _DOFs.value(type, NULL);
}

float Node::DOFValue(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->value() : 0;
}

void Node::setDOFValue(DOF::Type type, float value)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setValue(value);
}

float Node::DOFMin(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->min() : 0;
}

void Node::setDOFMin(DOF::Type type, float min)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setMin(min);
}

float Node::DOFMax(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->max() : 0;
}

void Node::setDOFMax(DOF::Type type, float max)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setMax(max);
}

bool Node::DOFHasMin(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->hasMin() : true;
}

void Node::setDOFHasMin(DOF::Type type, bool hasMin)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setHasMin(hasMin);
}

bool Node::DOFHasMax(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->hasMax() : true;
}

void Node::setDOFHasMax(DOF::Type type, bool hasMax)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setHasMax(hasMax);
}

bool Node::DOFIsEnabled(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->isEnabled() : false;
}

void Node::setDOFEnabled(DOF::Type type, bool enabled)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setEnabled(enabled);
}

QString Node::DOFName(DOF::Type type) const
{
    DOF* c = getDOF(type);

    return c ? c->name() : "";
}

void Node::setDOFName(DOF::Type type, const QString &name)
{
    DOF* toSet = getDOF(type);
    if (toSet)
        toSet->setName(name);
}

void Node::setDOF(DOF::Type type, DOF *dof)
{
    _DOFs[type] = dof;
    _DOFList << dof;
}
