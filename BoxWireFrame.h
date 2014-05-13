#ifndef BOXWIREFRAME_H
#define BOXWIREFRAME_H

#include "Mesh.h"

class BoxWireFrame : public Mesh
{
public:
    BoxWireFrame(Node* parent);
    virtual ~BoxWireFrame();

    virtual void    init();
    void            setFromMinMax(QVector3D const& min, QVector3D const& max);
    void            setMin(QVector3D const& min);
    void            setMax(QVector3D const& max);
    QVector3D const& min() const;
    QVector3D const& max() const;

private:
    void            _setFromMinMax();

    QVector3D       _min;
    QVector3D       _max;
};

#endif // BOXWIREFRAME_H
