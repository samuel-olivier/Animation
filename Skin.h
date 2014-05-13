#ifndef SKIN_H
#define SKIN_H

#include "Mesh.h"

#include <QHash>
#include <QVector>
#include <QPair>
#include <QMatrix4x4>
#include <QDir>

#include "BasicTokenizer.h"

class Skeleton;

class Skin : public Mesh
{
public:
    Skin(Node* parent);
    virtual ~Skin();

    virtual bool    loadFromFile(QString const& filename);
    virtual void    update(float elapsed);
    virtual void    init();

    void            setSkeleton(Skeleton* newSkel);
    Skeleton*       skeleton() const;

    QVector3D       baseVertex(int idx) const;
    QVector3D       baseNormal(int idx) const;

private:
    typedef bool    (Skin::*loadFunction)(BasicTokenizer&);

    bool    _load(BasicTokenizer& tokenizer);
    bool    _loadFloat3Vector(QVector<float>& toFill, BasicTokenizer& tokenizer);
    bool    _loadInt3Vector(QVector<int> &toFill, BasicTokenizer& tokenizer);

    bool    _loadPositions(BasicTokenizer& tokenizer);
    bool    _loadNormals(BasicTokenizer& tokenizer);
    bool    _loadSkinWeights(BasicTokenizer& tokenizer);
    bool    _loadTriangles(BasicTokenizer& tokenizer);
    bool    _loadBindings(BasicTokenizer& tokenizer);
    bool    _loadTexCoords(BasicTokenizer& tokenizer);
    bool    _loadMaterial(BasicTokenizer& tokenizer);

    QVector<QVector<QPair<int, float>>>         _skinWeights;
    QVector<QMatrix4x4>                         _bindings;
    Skeleton*                                   _skeleton;
    QVector<float>                              _skinVertices;
    QVector<float>                              _skinNormals;
    QDir                                        _currentFileDir;

    static QHash<QString, loadFunction>         _loadingFunctions;
};

#endif // SKIN_H
