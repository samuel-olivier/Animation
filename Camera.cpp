#include "Camera.h"

Camera::Camera(Node *parent) : Node(parent)
{
    _aspectRatio = 45;
    setName("Camera");
    setType("Camera");
}

Camera::~Camera() {
}

QMatrix4x4 &Camera::projection()
{
    return _projection;
}

void Camera::update(float elapsed)
{
    _projection.setToIdentity();
    _projection.perspective(_aspectRatio, (float)_viewportSize.width() / (float)_viewportSize.height(), 0.001, 1000.0);
    Node::update(elapsed);
}

void Camera::setAspectRatio(float aspectRatio)
{
    _aspectRatio = aspectRatio;
}

float Camera::aspectRatio() const
{
    return _aspectRatio;
}

void Camera::setViewportSize(const QSize &viewportSize)
{
    _viewportSize = viewportSize;
}

void Camera::setViewportSize(float width, float height)
{
    _viewportSize.setWidth(width);
    _viewportSize.setHeight(height);
}

const QSize &Camera::viewportSIze() const
{
    return _viewportSize;
}

QVector3D Camera::mapPoint(const QPoint screenPos) const
{
    float x = screenPos.x(),
        y = screenPos.y(),
        width = _viewportSize.width(),
        height = _viewportSize.height(),
        xrel = (x * 2 - width) / width,
        yrel = -(y * 2 - height) / height;

    // Reverse the projection and return the point in world co-ordinates.
    QMatrix4x4 m = _world.inverted() * _projection.inverted();
    QMatrix4x4 invm = m;
    return invm.map(QVector3D(xrel, yrel, -1.0f));
}
