#ifndef BASICTOKENIZER_H
#define BASICTOKENIZER_H

#include <QVector3D>

class QIODevice;

class BasicTokenizer
{
public:
    BasicTokenizer(QIODevice* device);

    QString     nextToken();
    QVector3D   nextVector();
    float       nextFloat();
    int         nextInt();

private:
    bool    _isWhiteSpace(char c) const;

    QIODevice*   _stream;
};

#endif // BASICTOKENIZER_H
