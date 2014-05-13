#ifndef NODE_H
#define NODE_H

#include <QSet>
#include <QMatrix4x4>
#include <QHash>

#include "DOF.h"

class Node
{
public:
    Node(Node* parent);
    virtual ~Node();

    virtual void    init();

    void    addChild(Node* child);
    void    removeChild(Node* child);
    QSet<Node*> const& children() const;
    void    deleteChildren();

    void            setParent(Node* parent);
    Node*           parent();

    QString const&  name();
    void            setName(QString const& newName);
    Node*           childByName(QString const& childName);

    QString const&  type();
    void            setType(QString const& type);

    bool            visible() const;
    void            setVisible(bool isVisible);

    virtual void    update(float elapsed);
    virtual void    draw();

    void                setLocal(QMatrix4x4 const& matrix);
    QMatrix4x4&         local();
    QMatrix4x4 const&   world() const;

    const QList<DOF *> &DOFs() const;
    DOF*                getDOF(DOF::Type type) const;

    virtual float       DOFValue(DOF::Type type) const;
    virtual void        setDOFValue(DOF::Type type, float value);

    virtual float       DOFMin(DOF::Type type) const;
    virtual void        setDOFMin(DOF::Type type, float min);

    virtual float       DOFMax(DOF::Type type) const;
    virtual void        setDOFMax(DOF::Type type, float max);

    virtual bool        DOFHasMin(DOF::Type type) const;
    virtual void        setDOFHasMin(DOF::Type type, bool hasMin);

    virtual bool        DOFHasMax(DOF::Type type) const;
    virtual void        setDOFHasMax(DOF::Type type, bool hasMax);

    virtual bool        DOFIsEnabled(DOF::Type type) const;
    virtual void        setDOFEnabled(DOF::Type type, bool enabled);

    virtual QString     DOFName(DOF::Type type) const;
    virtual void        setDOFName(DOF::Type type, QString const& name);

protected:
    virtual void    updateMatrix();
    virtual void    resetLocal();
    void            setDOF(DOF::Type type, DOF* dof);

    QSet<Node*>     _children;
    Node*           _parent;
    QString         _name;
    QString         _type;
    bool            _visible;
    QMatrix4x4      _local;
    QMatrix4x4      _world;
    QHash<DOF::Type, DOF*>  _DOFs;
    QList<DOF*>     _DOFList;
};

#endif // NODE_H
