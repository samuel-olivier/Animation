#ifndef MYSHADERPROGRAM_H
#define MYSHADERPROGRAM_H

#include <QGLShaderProgram>

class BasicVertexShader;
class BasicFragmentShader;

class BasicShaderProgram : public QGLShaderProgram
{
    Q_OBJECT
public:
    BasicShaderProgram(QObject *parent = 0);
    virtual ~BasicShaderProgram();

    int     vertexAttributeLocation() const;
    int     normalAttributeLocation() const;
    int     texCoordsAttributeLocation() const;
    int     mvpUniformLocation() const;
    int     mUniformLocation() const;
    int     vUniformLocation() const;
    int     colorUniformLocation() const;
    int     hasTextureUniformLocation() const;
    int     diffuseCoefUniformLocation() const;
    int     specularCoefUniformLocation() const;

private:
    BasicVertexShader*      _vertexShader;
    BasicFragmentShader*    _fragmentShader;
    int                     _vertexAttributeLocation;
    int                     _normalAttributeLocation;
    int                     _texCoordsAttributeLocation;
    int                     _mvpUniformLocation;
    int                     _mUniformLocation;
    int                     _vUniformLocation;
    int                     _colorUniformLocation;
    int                     _hasTextureUniformLocation;
    int                     _specularCoefUniformLocation;
    int                     _diffuseCoefUniformLocation;
};

#endif // MYSHADERPROGRAM_H
