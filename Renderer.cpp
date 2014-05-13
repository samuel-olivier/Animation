#include "Renderer.h"

#include <QGLBuffer>
#include <qgl.h>

#include "BasicShaderProgram.h"
#include "Scene.h"
#include "Mesh.h"
#include "Camera.h"

Renderer*    Renderer::_instance = NULL;
const float  Renderer::Epsilon = 0.0001;

Renderer *Renderer::instance()
{
    if (_instance == NULL)
        _instance = new Renderer();
    return _instance;
}

void Renderer::draw(Scene *scene)
{
    _shaderProgram->bind();
    _currentCamera = scene->camera();
    scene->draw();
}

void Renderer::draw(Mesh *mesh)
{
    QGLBuffer* vertexBuffer = mesh->GLVertices();
    vertexBuffer->bind();

    _shaderProgram->setAttributeBuffer(_shaderProgram->vertexAttributeLocation(), GL_FLOAT, 0, 3, 8 * sizeof(float));
    _shaderProgram->setAttributeBuffer(_shaderProgram->normalAttributeLocation(), GL_FLOAT, 3 * sizeof(float), 3, 8 * sizeof(float));
    _shaderProgram->setAttributeBuffer(_shaderProgram->texCoordsAttributeLocation(), GL_FLOAT, 6 * sizeof(float), 2, 8 * sizeof(float));
    _shaderProgram->setUniformValue(_shaderProgram->mvpUniformLocation(), _currentCamera->projection() * _currentCamera->world() * mesh->world());
    _shaderProgram->setUniformValue(_shaderProgram->mUniformLocation(), mesh->world());
    _shaderProgram->setUniformValue(_shaderProgram->vUniformLocation(), _currentCamera->world());
    _shaderProgram->setUniformValue(_shaderProgram->colorUniformLocation(), mesh == _currentMesh ? QColor(Qt::red) : mesh->material()->color());
    _shaderProgram->setUniformValue(_shaderProgram->hasTextureUniformLocation(), mesh->material()->hasTexture());
    _shaderProgram->setUniformValue(_shaderProgram->diffuseCoefUniformLocation(), mesh->material()->diffuseCoef());
    _shaderProgram->setUniformValue(_shaderProgram->specularCoefUniformLocation(), mesh->material()->specularCoef());

    if (mesh->material()->hasTexture()) {
        glBindTexture(GL_TEXTURE_2D, mesh->material()->texture());
    }
    QGLBuffer* faceBuffer = mesh->GLFaces();
    faceBuffer->bind();
    if (!mesh->isWireFrame())
        glPolygonMode(GL_FRONT_AND_BACK, mesh->drawMode() == Mesh::Line ? GL_LINE : GL_FILL);
    glDrawElements(mesh->isWireFrame() ? GL_LINES : GL_TRIANGLES, faceBuffer->size(), GL_UNSIGNED_INT, (GLvoid*)0);
}

void Renderer::setCurrentMesh(Mesh *mesh)
{
    _currentMesh = mesh;
}

Mesh *Renderer::currentMesh() const
{
    return _currentMesh;
}

Renderer::Renderer()
{
    _shaderProgram = new BasicShaderProgram();
    _currentMesh = NULL;
    _currentCamera = NULL;
}

Renderer::~Renderer()
{
    delete _shaderProgram;
}
