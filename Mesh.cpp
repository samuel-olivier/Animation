#include "Mesh.h"

#include <QGLBuffer>

#include "Renderer.h"

Mesh::Mesh(Node *parent) : Node(parent)
{
	_GLVertices = new QGLBuffer(QGLBuffer::VertexBuffer);
	_GLFaces = new QGLBuffer(QGLBuffer::IndexBuffer);
    _drawMode = Fill;
    _isWireFrame = false;
    _material = new Material(this);
    setType("Mesh");

}

Mesh::~Mesh()
{
	delete _GLVertices;
	delete _GLFaces;
}

void	Mesh::init()
{
    Node::init();
    _GLVertices->create();
    _GLFaces->create();
}

void Mesh::draw()
{
    if (visible())
        renderer->draw(this);
    Node::draw();
}

void	Mesh::setVertices(const QVector<float>& vertices)
{
    _vertices = vertices;
    _updateVerticesNormalsBuffer();
}

const QVector<float>	Mesh::vertices() const
{
	return _vertices;
}

float	Mesh::vertex(int i) const
{
    return _vertices.at(i);
}

void Mesh::setNormals(const QVector<float> &normals)
{
    _normals = normals;
    _updateVerticesNormalsBuffer();
}

const QVector<float> Mesh::normals() const
{
    return _normals;
}

float Mesh::normal(int i) const
{
    return _normals[i];
}

void Mesh::setTexCoords(const QVector<float> &texCoords)
{
    _texCoords = texCoords;
    _updateVerticesNormalsBuffer();
}

const QVector<float> Mesh::texCoords() const
{
    return _texCoords;
}

float Mesh::texCoord(int i) const
{
    return _texCoords[i];
}

QGLBuffer*	Mesh::GLVertices() const
{
	return _GLVertices;
}

void	Mesh::setFaces(const QVector<int>& faces)
{
	_GLFaces->bind();
    _GLFaces->allocate(faces.data(), faces.size() * sizeof(int));
	_faces = faces;
}

const QVector<int>	Mesh::faces() const
{
	return _faces;
}

int		Mesh::face(int i) const
{
	return _faces[i];
}

QGLBuffer*	Mesh::GLFaces() const
{
    return _GLFaces;
}

void Mesh::setDrawMode(Mesh::DrawMode mode)
{
    _drawMode = mode;
}

Mesh::DrawMode Mesh::drawMode() const
{
    return _drawMode;
}

bool Mesh::isWireFrame() const
{
    return _isWireFrame;
}

Material *Mesh::material() const
{
    return _material;
}

float Mesh::intersects(const QVector3D &pos, const QVector3D &direction) const
{
    if (_isWireFrame)
        return -1;
    float res = -1;
    QMatrix4x4 inv = world().inverted();
    QVector3D invPos = inv.map(pos);
    inv.setColumn(3, QVector4D(0, 0, 0, 1));
    QVector3D invDir = inv.map(direction);
    for (int i = 0; i < _faces.size(); ++i) {
        int p0Idx = _faces[i] * 3,
            p1Idx = _faces[++i] * 3,
            p2Idx = _faces[++i] * 3;
        QVector3D p0(_vertices[p0Idx], _vertices[p0Idx + 1], _vertices[p0Idx + 2]),
                  p1(_vertices[p1Idx], _vertices[p1Idx + 1], _vertices[p1Idx + 2]),
                  p2(_vertices[p2Idx], _vertices[p2Idx + 1], _vertices[p2Idx + 2]);

        float current = _triangleIntersection(invPos, invDir, p0, p1, p2);
        if (current > 0 && (res == -1 || current < res))
            res = current;
    }
    return res;
}

float Mesh::_triangleIntersection(const QVector3D &pos, const QVector3D &direction, const QVector3D &p0, const QVector3D &p1, const QVector3D &p2) const
{
    float u, v, t;
    QVector3D edge1, edge2, tvec, pvec, qvec;
    float det, inv_det;

    // find vectors for two edges sharing vert0
    edge1 = p1 - p0;
    edge2 = p2 - p0;

    // Begin calculating determinant - also used to calculate U parameter
    pvec = QVector3D::crossProduct(direction, edge2);

    // If determinant is near zero, ray lies in plane of triangle
    det = QVector3D::dotProduct(edge1, pvec);

    if (det > -Renderer::Epsilon && det < Renderer::Epsilon)
        return -1.0f;
    inv_det = 1.0 / det;

    // Calculate distance from point 0 to ray origin
    tvec = pos - p0;

    // Calculate U parameter and test bounds
    u = QVector3D::dotProduct(tvec, pvec) * inv_det;
    if (u < 0.0f || u > 1.0f)
        return -1.0f;

    // Prepare to test V parameter
    qvec = QVector3D::crossProduct(tvec, edge1);

    // Calculate V parameter and test bounds
    v = QVector3D::dotProduct(direction, qvec) * inv_det;
    if (v < 0.0f || u + v > 1.0f)
        return -1.0f;

    // Calculate t, ray intersects triangle
    t = QVector3D::dotProduct(edge2, qvec) * inv_det;
    return t;
}

void Mesh::_updateVerticesNormalsBuffer()
{
    _buffer.resize(8 * (_vertices.size() / 3));
    int vId = 0, nId = 0, tId = 0;
    for (int i = 0; i < _buffer.size();) {
        _buffer[i++] = _vertices[vId++];
        _buffer[i++] = _vertices[vId++];
        _buffer[i++] = _vertices[vId++];
        if (nId + 2 < _normals.size()) {
            _buffer[i++] = _normals[nId++];
            _buffer[i++] = _normals[nId++];
            _buffer[i++] = _normals[nId++];
        } else {
            _buffer[i++] = 0;
            _buffer[i++] = 0;
            _buffer[i++] = 0;
        }
        if (tId + 1 < _texCoords.size()) {
            _buffer[i++] = _texCoords[tId++];
            _buffer[i++] = _texCoords[tId++];
        } else {
            _buffer[i++] = 0;
            _buffer[i++] = 0;
        }
    }
    _GLVertices->bind();
    _GLVertices->allocate(_buffer.data(), _buffer.size() * sizeof(float));
}
