#include "Skin.h"

#include <QFile>
#include <QFileInfo>

#include "Skeleton.h"
#include "MorphTarget.h"

QHash<QString, Skin::loadFunction>    Skin::_loadingFunctions = {
    {"positions", &Skin::_loadPositions},
    {"normals", &Skin::_loadNormals},
    {"skinweights", &Skin::_loadSkinWeights},
    {"triangles", &Skin::_loadTriangles},
    {"bindings", &Skin::_loadBindings},
    {"texcoords", &Skin::_loadTexCoords},
    {"material", &Skin::_loadMaterial}
};

Skin::Skin(Node *parent) : Mesh(parent), _skeleton(NULL)
{
    setName("Skin");
    setType("Skin");
}

Skin::~Skin()
{
}

bool Skin::loadFromFile(const QString &filename )
{
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QFileInfo fileInfo(filename);
    _currentFileDir = fileInfo.dir();
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

void Skin::update(float elapsed)
{
    if (_skeleton)
        _skeleton->update(0);
    QVector<float> positions = _skinVertices;
    QVector<float> normals = _skinNormals;

    QVector<QMatrix4x4> cacheValues;
    if (_skeleton) {
        cacheValues.resize(qMax(_skeleton->jointNumber(), _bindings.size()));
        for (int i = 0; i < cacheValues.size(); ++i) {
            if (i < _skeleton->jointNumber())
                cacheValues[i] = _skeleton->jointWorld(i);
            if (i < _bindings.size())
                cacheValues[i] *= _bindings[i];
        }
    }
    for (int i = 0; i + 2 < positions.size() && i + 2 < normals.size(); i += 3) {
        QVector4D v(positions[i], positions[i + 1], positions[i + 2], 1),
                n(normals[i], normals[i + 1], normals[i + 2], 0),
                newV(0, 0, 0, 0), newN(0, 0, 0, 0);
        int idx = i / 3;
        if (_skeleton) {
            QVector<QPair<int, float>>& weights = _skinWeights[idx];
            for (int j = 0; j < weights.size(); ++j) {
                int jointId = weights[j].first;
                float weight = weights[j].second;
                newV += weight * cacheValues[jointId] * v;
                newN += weight * cacheValues[jointId] * n;
            }
        } else {
            newV = v;
            newN = n;
        }
        foreach (Node* child, children()) {
            MorphTarget* morphTarget = dynamic_cast<MorphTarget*>(child);
            if (morphTarget) {
                QPair<QVector3D, QVector3D> delta = morphTarget->delta(idx);
                float DOFValue = morphTarget->DOFValue(DOF::MorphTarget);

                newV += DOFValue * QVector4D(delta.first, 0);
                newN += DOFValue * QVector4D(delta.second, 0);
            }
        }
        newN.normalize();
        positions[i] = newV.x();
        positions[i + 1] = newV.y();
        positions[i + 2] = newV.z();
        normals[i] = newN.x();
        normals[i + 1] = newN.y();
        normals[i + 2] = newN.z();
    }
    setVertices(positions);
    setNormals(normals);
    Mesh::update(elapsed);
}

void Skin::init()
{
    Mesh::init();
}

void Skin::setSkeleton(Skeleton *newSkel)
{
    _skeleton = newSkel;
}

Skeleton *Skin::skeleton() const
{
    return _skeleton;
}

QVector3D Skin::baseVertex(int idx) const
{
    idx *= 3;
    return (idx + 2 < _skinVertices.size()) ?
                QVector3D(_skinVertices[idx],
                          _skinVertices[idx + 1],
            _skinVertices[idx + 2]) :
            QVector3D();
}

QVector3D Skin::baseNormal(int idx) const
{
    idx *= 3;
    return (idx + 2 < _skinNormals.size()) ?
                QVector3D(_skinNormals[idx],
                          _skinNormals[idx + 1],
            _skinNormals[idx + 2]) :
            QVector3D();
}

bool Skin::_load(BasicTokenizer &tokenizer)
{
    QString token = tokenizer.nextToken();

    while (!token.isEmpty()) {
        if (_loadingFunctions.contains(token) && (this->*_loadingFunctions[token])(tokenizer)) {
            token = tokenizer.nextToken();
            continue;
        }
        return false;
    }
    return true;
}

bool Skin::_loadFloat3Vector(QVector<float> &toFill, BasicTokenizer &tokenizer)
{
    int size = tokenizer.nextInt() * 3;
    if (tokenizer.nextToken() != "{")
        return false;
    toFill.resize(size);
    for (int i = 0; i < size; ++i) {
        toFill[i] = tokenizer.nextFloat();
    }
    return tokenizer.nextToken() == "}";
}

bool Skin::_loadInt3Vector(QVector<int> &toFill, BasicTokenizer &tokenizer)
{
    int size = tokenizer.nextInt() * 3;
    if (tokenizer.nextToken() != "{")
        return false;
    toFill.resize(size);
    for (int i = 0; i < size; ++i) {
        toFill[i] = tokenizer.nextInt();
    }
    return tokenizer.nextToken() == "}";
}

bool Skin::_loadPositions(BasicTokenizer &tokenizer)
{
    if (!_loadFloat3Vector(_skinVertices, tokenizer))
        return false;
    setVertices(_skinVertices);
    return true;
}

bool Skin::_loadNormals(BasicTokenizer &tokenizer)
{
    if (!_loadFloat3Vector(_skinNormals, tokenizer))
        return false;
    setNormals(_skinNormals);
    return true;
}

bool Skin::_loadSkinWeights(BasicTokenizer &tokenizer)
{
    int size = tokenizer.nextInt();
    if (tokenizer.nextToken() != "{")
        return false;
    _skinWeights.resize(size);
    for (int i = 0; i < size; ++i) {
        int numBinds = tokenizer.nextInt();
        _skinWeights[i].resize(numBinds);
        for (int j  = 0; j < numBinds; ++j) {
            int id = tokenizer.nextInt();
            float value = tokenizer.nextFloat();
            _skinWeights[i][j] = QPair<int, float>(id, value);
        }
    }
    return tokenizer.nextToken() == "}";
}

bool Skin::_loadTriangles(BasicTokenizer &tokenizer)
{
    QVector<int> faces;
    if (!_loadInt3Vector(faces, tokenizer))
        return false;
    setFaces(faces);
    return true;
}

bool Skin::_loadBindings(BasicTokenizer &tokenizer)
{
    int size = tokenizer.nextInt();
    if (tokenizer.nextToken() != "{")
        return false;
    _bindings.resize(size);
    for (int i = 0; i < size; ++i) {
        if (tokenizer.nextToken() != "matrix")
            return false;
        if (tokenizer.nextToken() != "{")
            return false;
        for (int j = 0; j < 4; ++j) {
            QVector3D current = tokenizer.nextVector();
            QVector4D column(current, j == 3 ? 1 : 0);
            _bindings[i].setColumn(j, column);
        }
        _bindings[i] = _bindings[i].inverted();
        if (tokenizer.nextToken() != "}")
            return false;
    }
    return tokenizer.nextToken() == "}";
}

bool Skin::_loadTexCoords(BasicTokenizer &tokenizer)
{
    int size = tokenizer.nextInt() * 2;
    if (tokenizer.nextToken() != "{")
        return false;
    QVector<float> coords;
    coords.resize(size);
    for (int i = 0; i < size; ++i) {
        coords[i] = tokenizer.nextFloat();
    }
    setTexCoords(coords);
    return tokenizer.nextToken() == "}";
}

bool Skin::_loadMaterial(BasicTokenizer &tokenizer)
{
    QString name = tokenizer.nextToken();

    if (name.isEmpty())
        return false;
    if (tokenizer.nextToken() != "{")
        return false;
    if (tokenizer.nextToken() != "texture")
        return false;
    QString textureName = tokenizer.nextToken();
    if (textureName.isEmpty())
        return false;
    textureName = _currentFileDir.absoluteFilePath(textureName);
    if (tokenizer.nextToken() != "}")
        return false;
    return material()->loadTexture(textureName);
}

