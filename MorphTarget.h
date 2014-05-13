#ifndef MORPHTARGET_H
#define MORPHTARGET_H

#include "Node.h"

#include <QHash>
#include <QPair>
#include <QVector3D>

#include "BasicTokenizer.h"

class Skin;

class MorphTarget : public Node
{
public:
    MorphTarget(Node* parent);
    virtual ~MorphTarget();

    virtual bool    loadFromFile(QString const& filename);
    virtual void    update(float elapsed);
    virtual void    init();

    void    setSkin(Skin* newSkin);
    Skin*   skin() const;

    QPair<QVector3D, QVector3D> const&  delta(int i) const;

private:
    typedef bool    (MorphTarget::*loadFunction)(BasicTokenizer&);

    bool    _load(BasicTokenizer& tokenizer);

    bool    _loadPositions(BasicTokenizer& tokenizer);
    bool    _loadNormals(BasicTokenizer& tokenizer);

    QHash<int, QPair<QVector3D, QVector3D>> _delta;
    Skin*                                   _skin;

    static QHash<QString, loadFunction>     _loadingFunctions;};

#endif // MORPHTARGET_H
