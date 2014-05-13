#ifndef CAMERA_H
#define CAMERA_H

#include "Node.h"

class Camera : public Node
{
public:
    Camera(Node* parent);
    virtual ~Camera();

    QMatrix4x4& projection();

    virtual void    update(float elapsed);

    void    setAspectRatio(float aspectRatio);
    float   aspectRatio() const;

    void    setViewportSize(QSize const& viewportSize);
    void    setViewportSize(float width, float height);
    QSize const& viewportSIze() const;

    QVector3D mapPoint(QPoint const screenPos) const;

private:
    float       _aspectRatio;
    QSize       _viewportSize;
    QMatrix4x4  _projection;
};

#endif // CAMERA_H
