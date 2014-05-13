#include "Scene.h"

#include "Node.h"
#include "Camera.h"

Scene::Scene() : _root(new Node(NULL)), _camera(NULL)
{
    _root->init();
    _root->setName("Root");
}

Scene::~Scene()
{
}

void Scene::addNode(Node *child)
{
    child->setParent(_root);
}

void Scene::update(float elapsed)
{
    _root->update(elapsed);
}

void Scene::draw()
{
    _root->draw();
}

void Scene::setCamera(Camera *camera)
{
    _camera = camera;
}

Camera *Scene::camera() const
{
    return _camera;
}

Node *Scene::root() const
{
    return _root;
}

void Scene::clear()
{
    if (_camera) {
        _camera->setParent(NULL);
    }
    delete _root;
    _root = new Node(NULL);
    _root->setName("Root");
    _root->init();
    if (_camera) {
        _camera->setParent(_root);
    }
}
