#ifndef SCENE_H
#define SCENE_H

class Node;
class Camera;

class Scene
{
public:
    Scene();
    ~Scene();

    void    addNode(Node* child);
    void    update(float elasped);
    void    draw();

    void    setCamera(Camera* camera);
    Camera* camera() const;

    Node*   root() const;

    void    clear();

private:
    Node*   _root;
    Camera* _camera;
};

#endif // SCENE_H
