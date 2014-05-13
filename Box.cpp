#include "Box.h"

#include <QVector>

Box::Box(Node *parent) : Mesh(parent)
{
    setType("Box");
}

Box::~Box()
{
}

void    Box::init()
{
    Mesh::init();
    QVector<float> normals = {
        // Front
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        // Face
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        0.0, 0.0, -1.0,
        // Left
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        // Right
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        // Bottom
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        // Top
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
    setNormals(normals);
    setFromMinMax(QVector3D(0, 0, 0), QVector3D(0, 0, 0));
    QVector<int> faces = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    setFaces(faces);
}

void Box::setFromMinMax(QVector3D const& min, QVector3D const&max)
{
    _min = min;
    _max = max;
    _setFromMinMax();
}

void Box::setMin(const QVector3D &min)
{
    _min = min;
    _setFromMinMax();
}

void Box::setMax(const QVector3D &max)
{
    _max = max;
    _setFromMinMax();
}

const QVector3D &Box::min() const
{
    return _min;
}

const QVector3D &Box::max() const
{
    return _max;
}

void Box::_setFromMinMax()
{
    QVector<float> vertices = {
        // Front
        _min.x(), _min.y(), _min.z(),
        _max.x(), _min.y(), _min.z(),
        _max.x(), _max.y(), _min.z(),
        _min.x(), _max.y(), _min.z(),
        // Back
        _min.x(), _min.y(), _max.z(),
        _max.x(), _min.y(), _max.z(),
        _max.x(), _max.y(), _max.z(),
        _min.x(), _max.y(), _max.z(),
        // Left
        _min.x(), _min.y(), _min.z(),
        _min.x(), _max.y(), _min.z(),
        _min.x(), _max.y(), _max.z(),
        _min.x(), _min.y(), _max.z(),
        // Right
        _max.x(), _min.y(), _min.z(),
        _max.x(), _max.y(), _min.z(),
        _max.x(), _max.y(), _max.z(),
        _max.x(), _min.y(), _max.z(),
        // Bottom
        _min.x(), _min.y(), _min.z(),
        _max.x(), _min.y(), _min.z(),
        _max.x(), _min.y(), _max.z(),
        _min.x(), _min.y(), _max.z(),
        // Top
        _min.x(), _max.y(), _min.z(),
        _max.x(), _max.y(), _min.z(),
        _max.x(), _max.y(), _max.z(),
        _min.x(), _max.y(), _max.z(),
    };
    setVertices(vertices);
}
