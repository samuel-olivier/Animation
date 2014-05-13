#include "Skeleton.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "BallJoint.h"
#include "TranslationalJoint.h"
#include "FixedJoint.h"
#include "PrismaticJoint.h"
#include "HingeJoint.h"
#include "FreeJoint.h"
#include "Box.h"

QHash<QString, Skeleton::loadFunction>    Skeleton::_loadingFunctions = {
    {"offset", &Skeleton::_loadOffset},
    {"boxmin", &Skeleton::_loadBoxMin},
    {"boxmax", &Skeleton::_loadBoxMax},
    {"pose", &Skeleton::_loadRot},
    {"trans", &Skeleton::_loadTrans},
    {"rotxlimit", &Skeleton::_loadRotX},
    {"rotylimit", &Skeleton::_loadRotY},
    {"rotzlimit", &Skeleton::_loadRotZ},
    {"transxlimit", &Skeleton::_loadTransX},
    {"transylimit", &Skeleton::_loadTransY},
    {"transzlimit", &Skeleton::_loadTransZ},
    {"activateDOF", &Skeleton::_activateDOF}
};
QHash<QString, Skeleton::createJointFunction>  Skeleton::_createJointFunctions = {
    {"balljoint", &Skeleton::_createBallJoint},
    {"translationaljoint", &Skeleton::_createTranslationalJoint},
    {"fixedjoint", &Skeleton::_createFixedJoint},
    {"prismaticjoint", &Skeleton::_createPrismaticJoint},
    {"hingejoint", &Skeleton::_createHingeJoint},
    {"freejoint", &Skeleton::_createFreeJoint}
};

QHash<QString, DOF::Type>            Skeleton::_dofNames = {
    {"RotX", DOF::RotationX},
    {"RotY", DOF::RotationY},
    {"RotZ", DOF::RotationZ},
    {"TransX", DOF::TranslationX},
    {"TransY", DOF::TranslationY},
    {"TransZ", DOF::TranslationZ}
};


Skeleton::Skeleton(Node *parent) : Node(parent)
{
    setName("Skeleton");
    setType("Skeleton");
}

Skeleton::~Skeleton()
{
}

void    Skeleton::init()
{
}

QMatrix4x4 Skeleton::jointWorld(int i) const
{
    return i >= _joints.size() ? QMatrix4x4() : _joints[i]->world();
}

int Skeleton::jointNumber() const
{
    return _joints.size();
}

const QList<Joint *> &Skeleton::joints() const
{
    return _joints;
}

void Skeleton::setBoxesVisible(bool visible)
{
    _setBoxesVisible(this, visible);
}

void Skeleton::_setBoxesVisible(Node *current, bool visible)
{
    if (dynamic_cast<Box*>(current))
        current->setVisible(visible);
    foreach (Node* child, current->children()) {
        _setBoxesVisible(child, visible);
    }
}

bool Skeleton::_load(Node *parent, BasicTokenizer &tokenizer, const QString &currentToken)
{
    QString token = currentToken.isEmpty() ? tokenizer.nextToken() : currentToken;

    if (token.isEmpty())
        return false;
    else if (_createJointFunctions.contains(token))
        return _loadJoint(parent, token, tokenizer);
    else if (!token.isEmpty() && _loadingFunctions.contains(token))
        return (this->*_loadingFunctions[token])(parent, tokenizer);
    return false;
}

bool Skeleton::_loadJoint(Node *parent, const QString &type, BasicTokenizer &tokenizer)
{
    QString name = tokenizer.nextToken(),
            bracket = tokenizer.nextToken();
    if (name.isEmpty() || bracket != "{")
        return false;
    Joint*  current = (this->*_createJointFunctions[type])(parent);
    _joints.append(current);
    current->setName(name);
    current->init();
    QString token = tokenizer.nextToken();
    while (!token.isEmpty() && token != "}") {
        if (!_load(current, tokenizer, token))
            return false;
        token = tokenizer.nextToken();
    }
    return token == "}";
}

Joint *Skeleton::_createBallJoint(Node *parent)
{
    return new BallJoint(parent);
}

Joint *Skeleton::_createTranslationalJoint(Node *parent)
{
    return new TranslationalJoint(parent);
}

Joint *Skeleton::_createFixedJoint(Node *parent)
{
    return new FixedJoint(parent);
}

Joint *Skeleton::_createPrismaticJoint(Node *parent)
{
    return new PrismaticJoint(parent);
}

Joint *Skeleton::_createHingeJoint(Node *parent)
{
    return new HingeJoint(parent);
}

Joint *Skeleton::_createFreeJoint(Node *parent)
{
    return new FreeJoint(parent);
}

bool Skeleton::_loadOffset(Node *parent, BasicTokenizer &tokenizer)
{
    QVector3D value = tokenizer.nextVector();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (current) {
        current->setOffset(value);
        return true;
    }
    return false;}

bool Skeleton::_loadBoxMin(Node *parent, BasicTokenizer &tokenizer)
{
    QVector3D value = tokenizer.nextVector();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (current) {
        current->setMinBox(value);
        return true;
    }
    return false;
}

bool Skeleton::_loadBoxMax(Node *parent, BasicTokenizer &tokenizer)
{
    QVector3D value = tokenizer.nextVector();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (current) {
        current->setMaxBox(value);
        return true;
    }
    return false;
}

bool Skeleton::_loadRot(Node *parent, BasicTokenizer &tokenizer)
{
    QVector3D value = tokenizer.nextVector();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFValue(DOF::RotationX, value.x());
    current->setDOFValue(DOF::RotationY, value.y());
    current->setDOFValue(DOF::RotationZ, value.z());
    return true;
}

bool Skeleton::_loadTrans(Node *parent, BasicTokenizer &tokenizer)
{
    QVector3D value = tokenizer.nextVector();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFValue(DOF::TranslationX, value.x());
    current->setDOFValue(DOF::TranslationY, value.y());
    current->setDOFValue(DOF::TranslationZ, value.z());
    return true;
}

bool Skeleton::_loadRotX(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::RotationX, min);
    current->setDOFMax(DOF::RotationX, max);
    return true;
}

bool Skeleton::_loadRotY(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::RotationY, min);
    current->setDOFMax(DOF::RotationY, max);
    return true;
}

bool Skeleton::_loadRotZ(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::RotationZ, min);
    current->setDOFMax(DOF::RotationZ, max);
    return true;
}

bool Skeleton::_loadTransX(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::TranslationX, min);
    current->setDOFMax(DOF::TranslationX, max);
    return true;
}

bool Skeleton::_loadTransY(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::TranslationY, min);
    current->setDOFMax(DOF::TranslationY, max);
    return true;
}

bool Skeleton::_loadTransZ(Node *parent, BasicTokenizer &tokenizer)
{
    float min = tokenizer.nextFloat(), max = tokenizer.nextFloat();
    Joint* current = dynamic_cast<Joint*>(parent);
    if (!current)
        return false;
    current->setDOFMin(DOF::TranslationZ, min);
    current->setDOFMax(DOF::TranslationZ, max);
    return true;
}

bool Skeleton::_activateDOF(Node *parent, BasicTokenizer &tokenizer)
{
    QString dofType = tokenizer.nextToken();

    if (_dofNames.contains(dofType))
        parent->setDOFEnabled(_dofNames[dofType], true);
    return true;
}

bool Skeleton::loadFromFile(const QString &filename)
{
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    BasicTokenizer  tokenizer(&file);
    if (!_load(this, tokenizer)) {
        deleteChildren();
        file.close();
        qDebug() << "Invalid file";
        return false;
    }
    QFileInfo info(filename);
    setName(info.baseName());
    file.close();
    return true;
}

void Skeleton::update(float elapsed)
{
     Node::update(elapsed);
}
