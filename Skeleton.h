#ifndef SKELETON_H
#define SKELETON_H

#include "Node.h"

#include "BasicTokenizer.h"

class Joint;

class Skeleton : public Node
{    
public:
    Skeleton(Node* parent);
    virtual ~Skeleton();

    virtual bool    loadFromFile(QString const& filename);
    virtual void    update(float elapsed);
    void init();

    QMatrix4x4      jointWorld(int i) const;
    int             jointNumber() const;
    QList<Joint*> const&   joints() const;
    void            setBoxesVisible(bool visible);

private:
    typedef bool    (Skeleton::*loadFunction)(Node*, BasicTokenizer&);
    typedef Joint*    (Skeleton::*createJointFunction)(Node*);

    void            _setBoxesVisible(Node* current, bool visible);

    bool    _load(Node* parent, BasicTokenizer& tokenizer, QString const& currentToken = "");
    bool    _loadJoint(Node* parent, QString const& type, BasicTokenizer& tokenizer);

    Joint*  _createBallJoint(Node* parent);
    Joint*  _createTranslationalJoint(Node* parent);
    Joint*  _createFixedJoint(Node* parent);
    Joint*  _createPrismaticJoint(Node* parent);
    Joint*  _createHingeJoint(Node* parent);
    Joint*  _createFreeJoint(Node* parent);

    bool    _loadOffset(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadBoxMin(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadBoxMax(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadRot(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadTrans(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadRotX(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadRotY(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadRotZ(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadTransX(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadTransY(Node* parent, BasicTokenizer& tokenizer);
    bool    _loadTransZ(Node* parent, BasicTokenizer& tokenizer);
    bool    _activateDOF(Node* parent, BasicTokenizer& tokenizer);

    QList<Joint*>   _joints;

    static QHash<QString, loadFunction>         _loadingFunctions;
    static QHash<QString, createJointFunction>  _createJointFunctions;
    static QHash<QString, DOF::Type>            _dofNames;
};

#endif // SKELETON_H
