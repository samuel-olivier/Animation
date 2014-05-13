#include "BasicVertexShader.h"

BasicVertexShader::BasicVertexShader(QObject *parent) :
    QGLShader(QGLShader::Vertex, parent)
{
    compileSourceCode(
                "attribute vec3 vertex;\n"
                "attribute vec3 normal;\n"
                "attribute vec2 texCoordsAttr;\n"
                "uniform mat4 mvp;\n"
                "uniform mat4 v;\n"
                "uniform mat4 m;\n"
                "varying vec3 vEyeDir;\n"
                "varying vec3 vNormal;\n"
                "varying vec3 vLight1Dir;\n"
                "varying vec3 vLight2Dir;\n"
                "varying vec2 texCoords;\n"
                "void main() {\n"
                "texCoords = texCoordsAttr;\n"
                "gl_Position = mvp * vec4(vertex, 1.0);\n"
                "vec3 mPosition = (m * vec4(vertex, 1.0)).xyz;\n"
                "vec3 vmPosition = (v * m * vec4(vertex, 1.0)).xyz;\n"
                "vEyeDir = vec3(0.0, 0.0, 0.0) - vmPosition;\n"
                "vec3 mLight1 = vec3(50.0, 50.0, 5.0);\n"
                "vec3 vLight1 = (v * vec4(mLight1, 1.0)).xyz;\n"
                "vec3 mLight2 = vec3(-50.0, 20.0, 5.0);\n"
                "vec3 vLight2 = (v * vec4(mLight2, 1.0)).xyz;\n"
                "vLight1Dir = vLight1 + vEyeDir;\n"
                "vLight2Dir = vLight2 + vEyeDir;\n"
                "vNormal = (v * m * vec4(normal, 0.0)).xyz;\n"
                "}\n"
                );
}

BasicVertexShader::~BasicVertexShader()
{

}
