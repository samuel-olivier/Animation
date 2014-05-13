#include <QDebug>

#include "TrackBallCamera.h"

TrackBallCamera::TrackBallCamera() : Camera(NULL)
{
    _xAngle = 0;
    _yAngle = 0;
    _up = 0;
    _right = 0;
    _distance = 60;
    setType("TrackBall");
}

TrackBallCamera::~TrackBallCamera()
{
}

void TrackBallCamera::update(float elapsed)
{
    Camera::update(elapsed);
}

void TrackBallCamera::updateMatrix()
{
    QVector3D up(0, 1, 0);
    Node::updateMatrix();
    QMatrix4x4  directionMatrix;
    directionMatrix.rotate(_yAngle, 0, 1, 0);
    directionMatrix.rotate(_xAngle, 1, 0, 0);
    QVector3D direction(0, 0, 1);
    direction = directionMatrix * direction;
    direction *= _distance;
    QVector3D right = QVector3D::crossProduct(direction, up).normalized();
    QVector3D realUp = QVector3D::crossProduct(direction, right).normalized();
    QVector3D newTarget = _target + right * _right + realUp * _up;
    _local.lookAt(direction + newTarget, newTarget, up);
}

void TrackBallCamera::setTarget(const QVector3D &target)
{
    _target = target;
}

const QVector3D &TrackBallCamera::target() const
{
    return _target;
}

void TrackBallCamera::mousePress(const QPoint &pos, Qt::KeyboardModifiers modifiers)
{
    if (modifiers & Qt::ControlModifier) {
        _lastTranslatePos = pos;
    } else {
        _lastRotatePos = pos;
    }
}

void TrackBallCamera::mouseMove(const QPoint &pos, Qt::KeyboardModifiers modifiers)
{
    if (modifiers & Qt::ControlModifier) {
        _up += (pos.y() - _lastTranslatePos.y()) * (-0.08);
        _right += (pos.x() - _lastTranslatePos.x()) * 0.08;
        _lastTranslatePos = pos;
    } else {
        float diffX = pos.x() - _lastRotatePos.x();
        float diffY = pos.y() - _lastRotatePos.y();
        _lastRotatePos = pos;
        _yAngle -= diffX;
        _xAngle -= diffY;
        if (_xAngle > 89)
            _xAngle = 89;
        else if (_xAngle < -89)
            _xAngle = -89;
    }
}

void TrackBallCamera::wheel(float delta)
{
    _distance -= delta / 5;
    if (_distance < 0)
        _distance = 0;
}

