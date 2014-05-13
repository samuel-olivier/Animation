#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QColor>

#include "Node.h"
#include "Material.h"

class QGLBuffer;

class Mesh : public Node
{
public:
    enum DrawMode{
        Fill,
        Line
    };

    Mesh(Node* parent);
    virtual ~Mesh();
	
    virtual void	init();
    virtual void    draw();

    void			setVertices(const QVector<float>& vertices);
    const QVector<float>	vertices() const;
    float			vertex(int i) const;

    void			setNormals(const QVector<float>& normals);
    const QVector<float>	normals() const;
    float			normal(int i) const;

    void			setTexCoords(const QVector<float>& texCoords);
    const QVector<float>	texCoords() const;
    float			texCoord(int i) const;

    QGLBuffer*		GLVertices() const;

	void			setFaces(const QVector<int>& faces);
	const QVector<int>	faces() const;	
	int				face(int i) const;
	QGLBuffer*		GLFaces() const;

    void            setDrawMode(DrawMode mode);
    DrawMode        drawMode() const;

    bool            isWireFrame() const;

    Material*       material() const;

    float           intersects(QVector3D const& pos, QVector3D const& direction) const;

protected:
    float           _triangleIntersection(QVector3D const& pos, QVector3D const& direction, QVector3D const& p0, QVector3D const& p1, QVector3D const& p2) const;
    void            _updateVerticesNormalsBuffer();

    QVector<float>	_vertices;
    QVector<float>	_normals;
    QVector<float>	_texCoords;
    QVector<float>	_buffer;
    QGLBuffer*		_GLVertices;
	QVector<int>	_faces;
	QGLBuffer*		_GLFaces;
    DrawMode        _drawMode;
    bool            _isWireFrame;
    Material*       _material;
};

#endif // MESH_H
