#ifndef JOINT_H
#define JOINT_H

#include "Node.h"

#include <QVector3D>
#include <QPair>
#include <QHash>

#include "DOF.h"

class Box;

class Joint : public Node
{
public:
    enum Type {
        BallJoint,
        TranslationalJoint,
        FixedJoint,
        PrismaticJoint,
        HingeJoint,
        FreeJoint,
        Undefined
    };

    Joint(Node* parent);
    virtual ~Joint();

    virtual void    init();
    virtual void    update(float elapsed);

    virtual void    resetLocal();
    virtual void    updateMatrix();

    void    setBox(QVector3D const& min, QVector3D const& max);
    void    setMinBox(QVector3D const& min);
    void    setMaxBox(QVector3D const& max);
    QVector3D const&    minBox() const;
    QVector3D const&    maxBox() const;

    QVector3D const&    offset() const;
    void                setOffset(QVector3D const& newOffset);
    void                setOffset(float x, float y, float z);

    Type            type() const;

protected:
    void    setType(Type type);

    QVector3D           _offset;
    Box*                _box;
    Type                _type;
};

#endif // JOINT_H
