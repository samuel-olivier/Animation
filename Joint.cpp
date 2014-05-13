#include "Joint.h"

#include <qmath.h>

#include "Box.h"

Joint::Joint(Node* parent) : Node(parent)
{
    _box = new Box(this);
    _box->setName("Box");
    Node::setType("Joint");
    _type = Undefined;
}

Joint::~Joint() {
}

void Joint::init()
{
    _box->init();
    Node::init();
}

void Joint::update(float elapsed)
{
    Node::update(elapsed);
}

void Joint::resetLocal()
{
    Node::resetLocal();
    local().translate(_offset);
}

void Joint::updateMatrix()
{
    Node::updateMatrix();
    DOF* tX = getDOF(DOF::TranslationX),
            *tY = getDOF(DOF::TranslationY),
            *tZ = getDOF(DOF::TranslationZ),
            *rX = getDOF(DOF::RotationX),
            *rY = getDOF(DOF::RotationY),
            *rZ = getDOF(DOF::RotationZ);
    _local.translate((tX && tX->isEnabled()) ? tX->value() : 0, (tY && tY->isEnabled()) ? tY->value() : 0, (tZ && tZ->isEnabled()) ? tZ->value() : 0);
    if (rZ && rZ->isEnabled())
        _local.rotate(rZ->value() * 180 / M_PI, 0, 0, 1);
    if (rY && rY->isEnabled())
        _local.rotate(rY->value() * 180 / M_PI, 0, 1, 0);
    if (rX && rX->isEnabled())
        _local.rotate(rX->value() * 180 / M_PI, 1, 0, 0);
}

void Joint::setBox(const QVector3D &min, const QVector3D &max)
{
    _box->setFromMinMax(min, max);
}

void Joint::setMinBox(const QVector3D &min)
{
    _box->setMin(min);
}

void Joint::setMaxBox(const QVector3D &max)
{
    _box->setMax(max);
}

const QVector3D &Joint::minBox() const
{
    return _box->min();
}

const QVector3D &Joint::maxBox() const
{
    return _box->max();
}

const QVector3D &Joint::offset() const
{
    return _offset;
}

void Joint::setOffset(const QVector3D &newOffset)
{
    _offset = newOffset;
}

void Joint::setOffset(float x, float y, float z)
{
    setOffset(QVector3D(x, y, z));
}

Joint::Type Joint::type() const
{
    return _type;
}

void Joint::setType(Joint::Type type)
{
    _type = type;
}
