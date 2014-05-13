#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QTime>

class Scene;
class TrackBallCamera;
class Mesh;
class Node;

class BasicGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit BasicGLWidget(QWidget *parent = 0);
    ~BasicGLWidget();

    Scene*  scene() const;

signals:
    void    meshSelected(Mesh* mesh);
    void    initialized();
	
protected:
	virtual void initializeGL();
	virtual void paintGL();
    virtual void resizeGL(int width, int height);

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

    Mesh*   getNearestMesh(QVector3D const& pos, QVector3D const& direction) const;

private slots:
    void    updateScene();

private:
    Mesh*   _getNearestMesh(QVector3D const& pos, QVector3D const& direction, Node* current, float &k) const;

    Scene*  _scene;
    TrackBallCamera*    _camera;
    QTime   _loopTime;
};

#endif // MYGLWIDGET_H
