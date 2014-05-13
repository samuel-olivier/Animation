#include "BasicTokenizer.h"

#include <QIODevice>

BasicTokenizer::BasicTokenizer(QIODevice *device) : _stream(device)
{
}

QString BasicTokenizer::nextToken()
{
    char c;
    QString token;
    bool filling = false;

    while (_stream->getChar(&c)) {
        bool isWhiteSpace = _isWhiteSpace(c);
        if (filling == false) {
            if (!isWhiteSpace) {
                filling = true;
            }
        }
        if (filling == true) {
            if (isWhiteSpace)
                return token;
            else
                token.append(c);
        }
    }
    return token;
}

QVector3D BasicTokenizer::nextVector()
{
    QVector3D res;
    res.setX(nextFloat());
    res.setY(nextFloat());
    res.setZ(nextFloat());
    return res;
}

float BasicTokenizer::nextFloat()
{
    return nextToken().toFloat();
}

int BasicTokenizer::nextInt()
{
    return nextToken().toInt();
}

bool BasicTokenizer::_isWhiteSpace(char c) const
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
