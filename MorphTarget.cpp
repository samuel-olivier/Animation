#include "MorphTarget.h"

#include <QFile>

#include "Skin.h"

QHash<QString, MorphTarget::loadFunction>   MorphTarget::_loadingFunctions = {
    {"positions", &MorphTarget::_loadPositions},
    {"normals", &MorphTarget::_loadNormals}
};


MorphTarget::MorphTarget(Node *parent) : Node(parent)
{
    setName("Morph Target");
    setType("Morph Target");
    DOF* state = new DOF(DOF::MorphTarget, true);
    state->setMin(-2);
    state->setMax(2);
    state->setValue(0);
    setDOF(DOF::MorphTarget, state);
}

MorphTarget::~MorphTarget()
{
}

bool MorphTarget::loadFromFile(const QString &filename)
{
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QFileInfo fileInfo(filename);
    BasicTokenizer  tokenizer(&file);
    if (!_load(tokenizer)) {
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

void MorphTarget::update(float elapsed)
{

}

void MorphTarget::init()
{
    Node::init();
}

void MorphTarget::setSkin(Skin *newSkin)
{
    _skin = newSkin;
}

Skin *MorphTarget::skin() const
{
    return _skin;
}

const QPair<QVector3D, QVector3D> &MorphTarget::delta(int i) const
{
    return _delta.value(i);
}

bool MorphTarget::_load(BasicTokenizer &tokenizer)
{
    QString token = tokenizer.nextToken();

    while (!token.isEmpty()) {
        if (_loadingFunctions.contains(token) && (this->*_loadingFunctions[token])(tokenizer)) {
            token = tokenizer.nextToken();
            continue;
        }
        return false;
    }
    return true;}

bool MorphTarget::_loadPositions(BasicTokenizer &tokenizer)
{
    if (!_skin)
        return false;
    int size = tokenizer.nextInt();
    if (tokenizer.nextToken() != "{")
        return false;
    for (int i = 0; i < size; ++i) {
        int id = tokenizer.nextInt();
        QVector3D vector = tokenizer.nextVector();
        _delta[id].first = vector - _skin->baseVertex(id);
    }
    return tokenizer.nextToken() == "}";
}

bool MorphTarget::_loadNormals(BasicTokenizer &tokenizer)
{
    if (!_skin)
        return false;
    int size = tokenizer.nextInt();
    if (tokenizer.nextToken() != "{")
        return false;
    for (int i = 0; i < size; ++i) {
        int id = tokenizer.nextInt();
        QVector3D vector = tokenizer.nextVector();
        _delta[id].second = vector - _skin->baseNormal(id);
    }
    return tokenizer.nextToken() == "}";
}
