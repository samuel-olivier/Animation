#include "BasicShaderProgram.h"

#include "BasicVertexShader.h"
#include "BasicFragmentShader.h"

BasicShaderProgram::BasicShaderProgram(QObject *parent) :
    QGLShaderProgram(parent)
{
    _vertexShader = new BasicVertexShader();
    _fragmentShader = new BasicFragmentShader();
    addShader(_vertexShader);
    addShader(_fragmentShader);
    link();
    _vertexAttributeLocation = attributeLocation("vertex");
    enableAttributeArray(_vertexAttributeLocation);
    _normalAttributeLocation = attributeLocation("normal");
    enableAttributeArray(_normalAttributeLocation);
    _texCoordsAttributeLocation = attributeLocation("texCoordsAttr");
    enableAttributeArray(_texCoordsAttributeLocation);
    _mvpUniformLocation = uniformLocation("mvp");
    _mUniformLocation = uniformLocation("m");
    _vUniformLocation = uniformLocation("v");
    _colorUniformLocation = uniformLocation("color");
    _hasTextureUniformLocation = uniformLocation("hasTexture");
    _diffuseCoefUniformLocation = uniformLocation("diffuseCoef");
    _specularCoefUniformLocation = uniformLocation("specularCoef");
}

BasicShaderProgram::~BasicShaderProgram()
{
    delete _vertexShader;
    delete _fragmentShader;
}

int BasicShaderProgram::vertexAttributeLocation() const
{
    return _vertexAttributeLocation;
}

int BasicShaderProgram::normalAttributeLocation() const
{
    return _normalAttributeLocation;
}

int BasicShaderProgram::texCoordsAttributeLocation() const
{
    return _texCoordsAttributeLocation;
}

int BasicShaderProgram::mvpUniformLocation() const
{
    return _mvpUniformLocation;
}

int BasicShaderProgram::mUniformLocation() const
{
    return _mUniformLocation;
}

int BasicShaderProgram::vUniformLocation() const
{
    return _vUniformLocation;
}

int BasicShaderProgram::colorUniformLocation() const
{
    return _colorUniformLocation;
}

int BasicShaderProgram::hasTextureUniformLocation() const
{
    return _hasTextureUniformLocation;
}

int BasicShaderProgram::diffuseCoefUniformLocation() const
{
    return _diffuseCoefUniformLocation;
}

int BasicShaderProgram::specularCoefUniformLocation() const
{
    return _specularCoefUniformLocation;
}
