#ifndef BASICVERTEXSHADER_H
#define BASICVERTEXSHADER_H

#include <QGLShader>

class BasicVertexShader : public QGLShader
{
    Q_OBJECT
public:
    BasicVertexShader(QObject *parent = 0);
    virtual ~BasicVertexShader();

};

#endif // BASICVERTEXSHADER_H
