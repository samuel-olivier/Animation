#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H

#include <QHash>
#include <QVector>
#include <QString>

#include "BasicTokenizer.h"
#include "Channel.h"
#include "Keyframe.h"
#include "Pose.h"

class AnimationClip
{
public:
    AnimationClip();
    virtual ~AnimationClip();

    virtual bool    loadFromFile(QString const& filename);

    QString const&  name() const;
    void            setName(QString const& name);

    float   startTime() const;
    float   endTime() const;

    QVector<Channel*> const&    channels() const;
    void                        setChannels(QVector<Channel*> const& channels);

    Channel*        channel(int idx) const;

    void            evaluate(float t, Pose& pose);
    void            saveToFile(QString const& filename);

private:
    typedef bool    (AnimationClip::*loadFunction)(BasicTokenizer&);

    void    _preCompute();

    bool    _loadAnimation(BasicTokenizer& tokenizer);
    bool    _loadRange(BasicTokenizer& tokenizer);
    bool    _loadNumChannels(BasicTokenizer& tokenizer);
    bool    _loadChannel(BasicTokenizer& tokenizer);
    bool    _loadExtrapolate(BasicTokenizer& tokenizer);
    bool    _loadKeys(BasicTokenizer& tokenizer);

    int                 _currentParsingChannel;
    float               _startTime;
    float               _endTime;
    QVector<Channel*>   _channels;
    QString             _name;

    static QHash<QString, loadFunction>             _loadingFunctions;
    static QHash<QString, Channel::ExtrapolateType> _channelExtrapolationTypes;
    static QHash<QString, Keyframe::TangentType>    _keyTangentTypes;
    static QHash<Channel::ExtrapolateType, QString> _channelExtrapolationNames;
    static QHash<Keyframe::TangentType, QString>    _keyTangentNames;
};

#endif // ANIMATIONCLIP_H
