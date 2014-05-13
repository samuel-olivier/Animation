#include "BoxWireFrame.h"

#include <QVector>

BoxWireFrame::BoxWireFrame(Node *parent) : Mesh(parent)
{
    setType("Box");
    _isWireFrame = true;
}

BoxWireFrame::~BoxWireFrame()
{
}

void    BoxWireFrame::init()
{
    Mesh::init();
    QVector<float> normals = {
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
    setNormals(normals);
    setFromMinMax(QVector3D(-0.5, -0.5, -0.5), QVector3D(0.5, 0.5, 0.5));
    QVector<int> faces = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
    setFaces(faces);
}

void BoxWireFrame::setFromMinMax(QVector3D const& min, QVector3D const&max)
{
    _min = min;
    _max = max;
    _setFromMinMax();
}

void BoxWireFrame::setMin(const QVector3D &min)
{
    _min = min;
    _setFromMinMax();
}

void BoxWireFrame::setMax(const QVector3D &max)
{
    _max = max;
    _setFromMinMax();
}

const QVector3D &BoxWireFrame::min() const
{
    return _min;
}

const QVector3D &BoxWireFrame::max() const
{
    return _max;
}

void BoxWireFrame::_setFromMinMax()
{
    QVector<float> vertices = {
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

