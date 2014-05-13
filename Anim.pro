#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T17:22:14
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Anim
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Mesh.cpp \
    Scene.cpp \
    Node.cpp \
    Renderer.cpp \
    BasicFragmentShader.cpp \
    BasicShaderProgram.cpp \
    BasicGLWidget.cpp \
    BasicVertexShader.cpp \
    Box.cpp \
    BoxWireFrame.cpp \
    Joint.cpp \
    DOF.cpp \
    BallJoint.cpp \
    Skeleton.cpp \
    BasicTokenizer.cpp \
    Camera.cpp \
    TrackBallCamera.cpp \
    TranslationalJoint.cpp \
    FixedJoint.cpp \
    PrismaticJoint.cpp \
    HingeJoint.cpp \
    Skin.cpp \
    Material.cpp \
    MorphTarget.cpp \
    AnimationClip.cpp \
    Channel.cpp \
    Keyframe.cpp \
    Pose.cpp \
    ChannelViewer.cpp \
    Rig.cpp \
    SkeletonRig.cpp \
    AnimationPlayer.cpp \
    FreeJoint.cpp

HEADERS  += MainWindow.h \
    Mesh.h \
    Scene.h \
    Node.h \
    Renderer.h \
    BasicFragmentShader.h \
    BasicShaderProgram.h \
    BasicGLWidget.h \
    BasicVertexShader.h \
    Box.h \
    BoxWireFrame.h \
    Joint.h \
    DOF.h \
    BallJoint.h \
    Skeleton.h \
    BasicTokenizer.h \
    TreeItem.h \
    DoubleSpinBox.h \
    Camera.h \
    TrackBallCamera.h \
    TranslationalJoint.h \
    FixedJoint.h \
    PrismaticJoint.h \
    HingeJoint.h \
    Skin.h \
    Material.h \
    MorphTarget.h \
    AnimationClip.h \
    Channel.h \
    Keyframe.h \
    Pose.h \
    ChannelViewer.h \
    Rig.h \
    SkeletonRig.h \
    AnimationPlayer.h \
    FreeJoint.h

FORMS    += MainWindow.ui

QMAKE_CXXFLAGS += -std=c++0x
