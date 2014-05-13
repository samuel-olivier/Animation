#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariant>

namespace Ui {
class MainWindow;
}

#include "Joint.h"

class QTreeWidgetItem;
class Skeleton;
class Skin;
class Mesh;
class AnimationPlayer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void    onInitialized();
    void    onOpenSkel();
    void    onOpenSkin();
    void    onOpenMorphTarget();
    void    onOpenAnimation();
    void    onClear();
    void    onToggleSkinSkeleton();
    void    onSaveAnimation();
    void    onValueChanged(double value);
    void    onSearch();
    void    onCurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void    onCurrentMeshChanged(Mesh* mesh);

private:
    typedef void    (Joint::*jointSetter)(float);

    void    _loadSkel(QString const& filename);
    void    _loadSkin(QString const& filename);
    void    _loadMorphTarget(QString const& filename);
    void    _loadAnim(QString const& filename);
    void    _loadSceneTree();
    void    _loadNode(QTreeWidgetItem* parent, Node *current);
    void    _initEmptyAnim();

    void    _displayBallJoint(Joint* joint, QTreeWidgetItem* parent);
    void    _displayTranslationalJoint(Joint* joint, QTreeWidgetItem* parent);

    Ui::MainWindow*     ui;
    Skeleton*           _skel;
    Skin*               _skin;
    AnimationPlayer*    _animPlayer;
};

#endif // MAINWINDOW_H
