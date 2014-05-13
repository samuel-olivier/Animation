#include "BasicGLWidget.h"

#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "Renderer.h"
#include "Scene.h"
#include "Box.h"
#include "BoxWireFrame.h"
#include "Skeleton.h"
#include "TrackBallCamera.h"

BasicGLWidget::BasicGLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::SampleBuffers));
}

BasicGLWidget::~BasicGLWidget()
{
}

Scene *BasicGLWidget::scene() const
{
    return _scene;
}

void BasicGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.125, 0.125, 0.125, 1.0);
    _scene = new Scene();
    _camera = new TrackBallCamera();
    _scene->addNode(_camera);
    _scene->setCamera(_camera);

    QTimer* updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), SLOT(updateScene()));
    updateTimer->start(1000 / 60);
    _loopTime.restart();
    emit initialized();
}

void BasicGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderer->draw(_scene);
}

void BasicGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    _camera->setViewportSize(width, height);
    updateScene();
}

void BasicGLWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        _camera->mousePress(e->pos(), e->modifiers());
    } else if (e->button() == Qt::RightButton) {
        QVector3D clickPos(_camera->mapPoint(e->pos())),
                eyePos(_camera->world().inverted().column(3)),
                direction = (clickPos - eyePos).normalized();
        Mesh* selected = getNearestMesh(eyePos, direction);
        renderer->setCurrentMesh(selected);
        if (selected)
           emit meshSelected(selected);
    }
}

void BasicGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        _camera->mouseMove(e->pos(), e->modifiers());
    }
}

void BasicGLWidget::wheelEvent(QWheelEvent *e)
{
    _camera->wheel(e->delta());
}

void BasicGLWidget::updateScene()
{
    float msecElapsed = _loopTime.elapsed();
    float elapsed = msecElapsed / 1000.0;
    _loopTime.restart();
    _scene->update(elapsed);
    update();
}

Mesh *BasicGLWidget::_getNearestMesh(const QVector3D &pos, const QVector3D &direction, Node *current, float& k) const
{
    Mesh* res = NULL;
    Mesh* testing = dynamic_cast<Mesh*>(current);
    if (testing && testing->visible()) {
        float newK = testing->intersects(pos, direction);
        if (newK > 0 && (k == -1 || newK < k)) {
            k = newK;
            res = testing;
        }
    }
    QSet<Node*> const& children = current->children();
    foreach (Node* child, children) {
        Mesh* found = _getNearestMesh(pos, direction, child, k);
        if (found)
            res = found;
    }
    return res;
}

Mesh *BasicGLWidget::getNearestMesh(const QVector3D &pos, const QVector3D &direction) const
{
    float k = -1;
    return _getNearestMesh(pos, direction, _scene->root(), k);
}
