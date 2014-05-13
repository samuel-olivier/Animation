#ifndef MATERIAL_H
#define MATERIAL_H

#include "Node.h"

#include <QColor>
#include <QString>
#include <qgl.h>

class Material : public Node
{
public:
    Material(Node* parent);
    virtual ~Material();

    void    setColor(QColor const& color);
    QColor  color() const;

    bool    hasTexture() const;
    void    setHasTexture(bool hasTexture);

    GLuint  texture() const;
    bool loadTexture(QString const& filename);

    float   diffuseCoef() const;
    void    setDiffuseCoef(float diffuseCoef);

    float   specularCoef() const;
    void    setSpecularCoef(float specularCoef);

private:
    bool    _hasTexture;
    GLuint  _texId;
};

#endif // MATERIAL_H
