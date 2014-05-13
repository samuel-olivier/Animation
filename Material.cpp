#include "Material.h"

#include <QDebug>

Material::Material(Node *parent) : Node(parent)
{
    setName("material");
    setType("Material");
    _texId = 0;
    _hasTexture = false;

    DOF* colorR = new DOF(DOF::ColorR, true);
    colorR->setMin(0);
    colorR->setMax(255);
    colorR->setValue(200);
    setDOF(DOF::ColorR, colorR);

    DOF* colorG = new DOF(DOF::ColorG, true);
    colorG->setMin(0);
    colorG->setMax(255);
    colorG->setValue(200);
    setDOF(DOF::ColorG, colorG);

    DOF* colorB = new DOF(DOF::ColorB, true);
    colorB->setMin(0);
    colorB->setMax(255);
    colorB->setValue(200);
    setDOF(DOF::ColorB, colorB);

    DOF* colorA = new DOF(DOF::ColorA, true);
    colorA->setMin(0);
    colorA->setMax(255);
    colorA->setValue(200);
    setDOF(DOF::ColorA, colorA);

    DOF* diffuse = new DOF(DOF::Diffuse, true);
    diffuse->setMin(0);
    diffuse->setMax(1);
    diffuse->setValue(1);
    setDOF(DOF::Diffuse, diffuse);

    DOF* specular = new DOF(DOF::Specular, true);
    specular->setMin(0);
    specular->setMax(1);
    specular->setValue(1);
    setDOF(DOF::Specular, specular);
}

Material::~Material()
{
}

void Material::setColor(const QColor &color)
{
    setDOFValue(DOF::ColorR, color.red());
    setDOFValue(DOF::ColorG, color.green());
    setDOFValue(DOF::ColorB, color.blue());
    setDOFValue(DOF::ColorA, color.alpha());
}

QColor Material::color() const
{
    return QColor(DOFValue(DOF::ColorR),
                  DOFValue(DOF::ColorG),
                  DOFValue(DOF::ColorB),
                  DOFValue(DOF::ColorA));
}

bool Material::hasTexture() const
{
    return _hasTexture;
}

void Material::setHasTexture(bool hasTexture)
{
    if (_texId > 0)
        _hasTexture = hasTexture;
    else
        _hasTexture = false;
}

GLuint Material::texture() const
{
    return _texId;
}

bool Material::loadTexture(const QString &filename)
{
    QImage img(filename);
    if (img.isNull())
        return false;
    img = QGLWidget::convertToGLFormat(img);
    glGenTextures(1, &_texId);
    glBindTexture(GL_TEXTURE_2D, _texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    setHasTexture(true);
    return hasTexture();
}

float Material::diffuseCoef() const
{
    return DOFValue(DOF::Diffuse);
}

void Material::setDiffuseCoef(float diffuseCoef)
{
    setDOFValue(DOF::Diffuse, diffuseCoef);
}

float Material::specularCoef() const
{
    return DOFValue(DOF::Specular);
}

void Material::setSpecularCoef(float specularCoef)
{
    setDOFValue(DOF::Specular, specularCoef);
}
