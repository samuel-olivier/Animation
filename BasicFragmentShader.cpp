#include "BasicFragmentShader.h"

BasicFragmentShader::BasicFragmentShader(QObject *parent) :
    QGLShader(QGLShader::Fragment, parent)
{
    compileSourceCode(
                "uniform vec4 color;\n"
                "uniform bool hasTexture;\n"
                "uniform sampler2D texSampler;\n"
                "uniform float diffuseCoef;\n"
                "uniform float specularCoef;\n"
                "varying vec3 vNormal;\n"
                "varying vec3 vLight1Dir;\n"
                "varying vec3 vLight2Dir;\n"
                "varying vec3 vEyeDir;\n"
                "varying vec2 texCoords;\n"
                "void main() {\n"
                "vec4 newColor = color;\n"
                "if (hasTexture) {\n"
                "newColor = texture2D(texSampler, texCoords);\n"
                "}\n"
                "vec4 light1Color = vec4(0.8, 1.0, 1.0, 1.0);\n"
                "vec4 light2Color = vec4(1.0, 1.0, 0.8, 1.0);\n"
                "vec3 n = normalize(vNormal);\n"
                "vec3 l1 = normalize(vLight1Dir);\n"
                "vec3 l2 = normalize(vLight2Dir);\n"
                "vec3 e = normalize(vEyeDir);\n"
                "vec3 r1 = reflect(-l1, n);\n"
                "vec3 r2 = reflect(-l2, n);\n"
                "float cosAlpha1 = clamp(dot(e, r1), 0.0, 1.0);\n"
                "float light1Power = 1.0;\n"
                "float cosTheta1 = clamp(dot(n, l1), 0.0, 1.0);\n"
                "float cosAlpha2 = clamp(dot(e, r2), 0.0, 1.0);\n"
                "float light2Power = 1.0;\n"
                "float cosTheta2 = clamp(dot(n, l2), 0.0, 1.0);\n"
                "vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0) * newColor;\n"
                "vec4 diffuseColor = newColor * light1Color * light1Power * cosTheta1 + newColor * light2Color * light2Power * cosTheta2;\n"
                "vec4 specularColor = light1Color * light1Power * pow(cosAlpha1, 25.0) + light2Color * light2Power * pow(cosAlpha2, 25.0);\n"
                "gl_FragColor = ambientColor + diffuseColor * diffuseCoef + specularColor * specularCoef;\n"
                "}"
                );
}

BasicFragmentShader::~BasicFragmentShader()
{
}
