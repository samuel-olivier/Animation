#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "Skeleton.h"
#include "Skin.h"
#include "Scene.h"
#include "TreeItem.h"
#include "Joint.h"
#include "BallJoint.h"
#include "TranslationalJoint.h"
#include "DoubleSpinBox.h"
#include "Renderer.h"
#include "Mesh.h"
#include "MorphTarget.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "SkeletonRig.h"

#define ROOT_PATH "C:/Users/samuel/Documents/Dev/Animation/Files"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _skel = NULL;
    _skin = NULL;
    _animPlayer = NULL;
    ui->setupUi(this);
    connect(ui->glView, SIGNAL(initialized()), SLOT(onInitialized()));
    connect(ui->action_Open_Skel, SIGNAL(triggered()), SLOT(onOpenSkel()));
    connect(ui->action_Open_Skin, SIGNAL(triggered()), SLOT(onOpenSkin()));
    connect(ui->action_Open_Morph_Target, SIGNAL(triggered()), SLOT(onOpenMorphTarget()));
    connect(ui->action_Open_Animation, SIGNAL(triggered()), SLOT(onOpenAnimation()));
    connect(ui->action_Save_Animation, SIGNAL(triggered()), SLOT(onSaveAnimation()));
    connect(ui->action_Clear, SIGNAL(triggered()), SLOT(onClear()));
    connect(ui->action_Toggle_Skin_Skeleton, SIGNAL(triggered()), SLOT(onToggleSkinSkeleton()));
    connect(ui->searchEdit, SIGNAL(returnPressed()), SLOT(onSearch()));
    connect(ui->skeletonTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->glView, SIGNAL(meshSelected(Mesh*)), SLOT(onCurrentMeshChanged(Mesh*)));
    ui->skeletonTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->channelView->initUi(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onInitialized()
{
    _loadSkel(ROOT_PATH "/wasp.skel");
    _loadSkin(ROOT_PATH "/wasp.skin");
    _loadAnim(ROOT_PATH "/wasp_walk.anim");
}

void MainWindow::onOpenSkel()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a Skeleton", ROOT_PATH, "Skel (*.skel);;All (*.*)");

    if (filename.isEmpty()) {
        _loadSkel(ROOT_PATH "/tube.skel");
    } else {
        _loadSkel(filename);
    }
}

void MainWindow::onOpenSkin()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open a Skin", ROOT_PATH, "Skin (*.skin);;All (*.*)");

    if (filename.isEmpty()) {
        _loadSkin(ROOT_PATH "/tube_smooth.skin");
    } else {
        _loadSkin(filename);
    }
}

void MainWindow::onOpenMorphTarget()
{
    if (!_skin) {
        if (QMessageBox::question(this, "Skin required", "You have to load a skin before loading a morph target. Do you want to load a skin now?") == QMessageBox::StandardButton::No)
            return ;
        onOpenSkin();
        if (!_skin)
            return ;
    }
    QStringList filenames = QFileDialog::getOpenFileNames(this, "Open Morph Targets", ROOT_PATH, "Morph Targets (*.morph);;All (*.*)");

    foreach (QString filename, filenames) {
        _loadMorphTarget(filename);
    }
}

void MainWindow::onOpenAnimation()
{
    if (!_skel) {
        if (QMessageBox::question(this, "Skeleton required", "You have to load a skeleton before loading an animation. Do you want to load a skeleton now?") == QMessageBox::StandardButton::No)
            return ;
        onOpenSkel();
        if (!_skel)
            return ;
    }
    QString filename = QFileDialog::getOpenFileName(this, "Open a Anim", ROOT_PATH, "Anim (*.anim);;All (*.*)");

    if (!filename.isEmpty()) {
        _loadAnim(filename);
    }
}

void MainWindow::onClear()
{
    ui->skeletonTree->clear();
    renderer->setCurrentMesh(NULL);
    delete _skel;
    _skel = NULL;
    delete _skin;
    _skin = NULL;
}

void MainWindow::onToggleSkinSkeleton()
{
    if (_skin && _skel) {
        _skel->setBoxesVisible(_skin->visible());
        _skin->setVisible(!_skin->visible());
    }
}

void MainWindow::onSaveAnimation()
{
    if (_animPlayer) {
        QString filename = QFileDialog::getSaveFileName(this, "Save an Anim", ROOT_PATH, "Anim (*.anim);;All (*.*)");

        if (!filename.isEmpty()) {
            _animPlayer->saveToFile(filename);
        }
    }
}

void MainWindow::onValueChanged(double value)
{
    DoubleSpinBox<DOF*>*   current = dynamic_cast<DoubleSpinBox<DOF*>*>(sender());

    if (!current)
        return;
    DOF*   DOF = current->userData();
    if (!DOF)
        return ;
    DOF->setValue(value);
}

void MainWindow::onSearch()
{
    QString search = ui->searchEdit->text();
    QTreeWidgetItemIterator it(ui->skeletonTree);
    while (*it) {
        if ((*it)->text(0).contains(search)) {
            ui->skeletonTree->setCurrentItem(*it);
            return ;
        }
        ++it;
    }
}

void MainWindow::onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    TreeItem<Joint*>* itemJoint = dynamic_cast<TreeItem<Joint*>*>(current);
    TreeItem<DOF*>* itemDOF = dynamic_cast<TreeItem<DOF*>*>(current);
    Joint* joint = NULL;
    ui->channelView->setChannel(NULL);
    if (itemJoint) {
        joint = itemJoint->userData();
        if (!joint)
            return ;
        Node* box = joint->children().size() == 0 ? NULL : *(joint->children().begin());
        renderer->setCurrentMesh(dynamic_cast<Mesh*>(box));
    } else if (itemDOF) {
        DOF* currentDOF = itemDOF->userData();
        if (!currentDOF || !_animPlayer)
            return ;
        ui->channelView->setChannel(_animPlayer->channel(currentDOF));
    }
}

void MainWindow::onCurrentMeshChanged(Mesh *mesh)
{
    if (!mesh)
        return ;
    Node* toSearch = mesh;
    Joint* joint = dynamic_cast<Joint*>(mesh->parent());
    if (joint && mesh->name() == "Box")
        toSearch = joint;
    QTreeWidgetItemIterator it(ui->skeletonTree);
    while (*it) {
        TreeItem<Node*>* current = dynamic_cast<TreeItem<Node*>*>(*it);
        if (current && current->userData() == toSearch) {
            QTreeWidgetItem* toSelect = *it;
            ui->skeletonTree->blockSignals(true);
            ui->skeletonTree->setCurrentItem(toSelect);
            ui->skeletonTree->blockSignals(false);
            renderer->setCurrentMesh(mesh);
            return ;
        }
        ++it;
    }
}

void MainWindow::_loadSkel(const QString &filename)
{
    delete _skel;
    ui->skeletonTree->clear();
    renderer->setCurrentMesh(NULL);
    _skel = filename.isEmpty() ? NULL : new Skeleton(NULL);
    if (_skel) {
        _skel->init();
        if (_skel->loadFromFile(filename)) {
            ui->glView->scene()->addNode(_skel);
        } else {
            delete _skel;
            _skel = NULL;
        }
    }
    if (_skel && _skin) {
        _skel->setBoxesVisible(false);
        _skin->setSkeleton(_skel);
    } else if (_skel) {
        _skel->setBoxesVisible(true);
    } else if (_skin) {
        _skin->setSkeleton(NULL);
    }
    _initEmptyAnim();
}

void MainWindow::_loadSkin(const QString &filename)
{
    delete _skin;
    ui->skeletonTree->clear();
    renderer->setCurrentMesh(NULL);
    _skin = filename.isEmpty() ? NULL : new Skin(NULL);
    if (_skin) {
        _skin->init();
        if (_skin->loadFromFile(filename)) {
            ui->glView->scene()->addNode(_skin);
        } else {
            delete _skin;
            _skin = NULL;
        }
    }
    if (_skel && _skin) {
        _skel->setBoxesVisible(false);
        _skin->setSkeleton(_skel);
    } else if (_skel) {
        _skel->setBoxesVisible(true);
    } else if (_skin) {
        _skin->setSkeleton(NULL);
    }
    _loadSceneTree();
}

void MainWindow::_loadMorphTarget(const QString &filename)
{
    if (!_skin)
        return ;
    ui->skeletonTree->clear();
    renderer->setCurrentMesh(NULL);
    MorphTarget* morphTarget = filename.isEmpty() ? NULL : new MorphTarget(_skin);
    if (morphTarget) {
        morphTarget->init();
        morphTarget->setSkin(_skin);
        if (!morphTarget->loadFromFile(filename)) {
            delete morphTarget;
            return ;
        }
    }
    _loadSceneTree();
}

void MainWindow::_loadAnim(const QString &filename)
{
    ui->channelView->setChannel(NULL);
    delete _animPlayer;
    _animPlayer = NULL;
    if (!_skel)
        return ;
    AnimationClip* anim = new AnimationClip();

    if (!anim->loadFromFile(filename)) {
        delete anim;
        return ;
    }
    _animPlayer = new AnimationPlayer(NULL);
    SkeletonRig* rig = new SkeletonRig();
    rig->setSkeleton(_skel);
    _animPlayer->setAnimation(anim);
    _animPlayer->setRig(rig);
    _animPlayer->setName(anim->name());
    ui->glView->scene()->addNode(_animPlayer);
    ui->skeletonTree->clear();
    _loadSceneTree();
}

void MainWindow::_loadSceneTree()
{
    Scene* scene = ui->glView->scene();
    TreeItem<Scene*>* root = new TreeItem<Scene*>(ui->skeletonTree);
    root->setUserData(scene);
    root->setText(0, "Scene");
    _loadNode(root, scene->root());
}

void MainWindow::_loadNode(QTreeWidgetItem *parent, Node *current)
{
    TreeItem<Node*>* currentItem = new TreeItem<Node*>(parent);
    currentItem->setUserData(current);
    currentItem->setText(0, current->name());
    currentItem->setText(1, current->type());
    QList<DOF*> const& DOFs = current->DOFs();
    if (DOFs.size() > 0) {
        TreeItem<Node*>* DOFsItem = new TreeItem<Node*>(currentItem);
        DOFsItem->setUserData(current);
        DOFsItem->setText(0, "DOFs");
        foreach (DOF* currentDOF, DOFs) {
            TreeItem<DOF*>* DOFItem = new TreeItem<DOF*>(DOFsItem);
            DOFItem->setUserData(currentDOF);
            DOFItem->setText(0, currentDOF->name());

            TreeItem<DOF*>* DOFValueItem = new TreeItem<DOF*>(DOFItem);
            DOFValueItem->setUserData(currentDOF);
            DOFValueItem->setText(0, "Value");
            if (currentDOF->isEnabled()) {
                DoubleSpinBox<DOF*>*    spinValue = new DoubleSpinBox<DOF*>(ui->skeletonTree);
                spinValue->setUserData(currentDOF);
                spinValue->setSingleStep(0.01);
                ui->skeletonTree->setItemWidget(DOFValueItem, 1, spinValue);
                if (currentDOF->hasMin())
                    spinValue->setMinimum(currentDOF->min());
                else
                    spinValue->setMinimum(-1000000);
                if (currentDOF->hasMax())
                    spinValue->setMaximum(currentDOF->max());
                else
                    spinValue->setMaximum(1000000);
                spinValue->setValue(currentDOF->value());
                connect(spinValue, SIGNAL(valueChanged(double)), SLOT(onValueChanged(double)));
            } else {
                DOFValueItem->setText(1, QString::number(currentDOF->value()));
            }

            if (currentDOF->hasMin()) {
                TreeItem<DOF*>* DOFMinItem = new TreeItem<DOF*>(DOFItem);
                DOFMinItem->setUserData(currentDOF);
                DOFMinItem->setText(0, "Min");
                DOFMinItem->setText(1, QString::number(currentDOF->min()));
            }

            if (currentDOF->hasMax()) {
                TreeItem<DOF*>* DOFMaxItem = new TreeItem<DOF*>(DOFItem);
                DOFMaxItem->setUserData(currentDOF);
                DOFMaxItem->setText(0, "Max");
                DOFMaxItem->setText(1, QString::number(currentDOF->max()));
            }
        }
    }
    foreach (Node* currentChild, current->children()) {
        if (currentChild->visible())
            _loadNode(currentItem, currentChild);
    }
}

void MainWindow::_initEmptyAnim()
{
    ui->channelView->setChannel(NULL);
    delete _animPlayer;
    _animPlayer = NULL;
    if (!_skel)
        return ;
    AnimationClip* anim = new AnimationClip();
    _animPlayer = new AnimationPlayer(NULL);
    SkeletonRig* rig = new SkeletonRig();
    rig->setSkeleton(_skel);
    _animPlayer->setRig(rig);
    anim->setName("Empty Anim");

    QVector<Channel*> channels;
    QVector<DOF*> DOFs = rig->DOFs();
    channels.resize(DOFs.size());
    for (int i = 0; i < DOFs.size(); ++i) {
        Channel* channel = new Channel();
        QVector<Keyframe*> keys;
        Keyframe* key = new Keyframe();
        key->setValue(DOFs[i]->value());
        keys.append(key);
        channel->setKeys(keys);
        channel->preCompute();
        channels[i] = channel;
    }
    anim->setChannels(channels);
    _animPlayer->setAnimation(anim);
    _animPlayer->setName(anim->name());
    ui->glView->scene()->addNode(_animPlayer);
    ui->skeletonTree->clear();
    _loadSceneTree();

}
