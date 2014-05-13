#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

#include "Camera.h"

#include <QVector3D>

class TrackBallCamera : public Camera
{
public:
    TrackBallCamera();
    virtual ~TrackBallCamera();

    virtual void    update(float elapsed);
    virtual void    updateMatrix();

    void            setTarget(QVector3D const& target);
    QVector3D const& target() const;

    void        mousePress(QPoint const& pos, Qt::KeyboardModifiers modifiers);
    void        mouseMove(QPoint const& pos, Qt::KeyboardModifiers modifiers);
    void        wheel(float delta);

private:
    float       _xAngle;
    float       _yAngle;
    float       _up;
    float       _right;
    float       _distance;
    QVector3D   _target;
    QPoint      _lastRotatePos;
    QPoint      _lastTranslatePos;
};

#endif // TRACKBALLCAMERA_H
