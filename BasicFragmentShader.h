#ifndef BASICFRAGMENTSHADER_H
#define BASICFRAGMENTSHADER_H

#include <QGLShader>

class BasicFragmentShader : public QGLShader
{
    Q_OBJECT
public:
    BasicFragmentShader(QObject *parent = 0);
    virtual ~BasicFragmentShader();
};

#endif // BASICFRAGMENTSHADER_H
