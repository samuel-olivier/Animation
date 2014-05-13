#ifndef RENDERER_H
#define RENDERER_H

#include <QObject>

class BasicShaderProgram;
class Mesh;
class Scene;
class Camera;

#define renderer Renderer::instance()

class Renderer
{
public:
    static Renderer*    instance();
    static const float  Epsilon;

    void    draw(Scene* scene);
    void    draw(Mesh* mesh);

    void    setCurrentMesh(Mesh* mesh);
    Mesh*   currentMesh() const;

private:
    Renderer();
    virtual ~Renderer();

    static Renderer*    _instance;

    BasicShaderProgram* _shaderProgram;
    Mesh*               _currentMesh;
    Camera*             _currentCamera;
};

#endif // RENDERER_H
