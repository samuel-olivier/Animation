#include "AnimationClip.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>

#include "Channel.h"

QHash<QString, AnimationClip::loadFunction> AnimationClip::_loadingFunctions = {
    {"range", &AnimationClip::_loadRange},
    {"numchannels", &AnimationClip::_loadNumChannels},
    {"channel", &AnimationClip::_loadChannel},
    {"extrapolate", &AnimationClip::_loadExtrapolate},
    {"keys", &AnimationClip::_loadKeys}
};

QHash<QString, Channel::ExtrapolateType>    AnimationClip::_channelExtrapolationTypes {
    {"constant", Channel::Constant},
    {"linear", Channel::Linear},
    {"cycle", Channel::Cycle},
    {"cycle_offset", Channel::CycleOffset},
    {"bounce", Channel::Bounce}
};

QHash<QString, Keyframe::TangentType>            AnimationClip::_keyTangentTypes = {
    {"smooth", Keyframe::Smooth},
    {"flat", Keyframe::Flat},
    {"linear", Keyframe::Linear}
};

QHash<Channel::ExtrapolateType, QString> AnimationClip::_channelExtrapolationNames;
QHash<Keyframe::TangentType, QString>    AnimationClip::_keyTangentNames;

AnimationClip::AnimationClip()
{
    if (_channelExtrapolationNames.isEmpty()) {
        QHashIterator<QString, Channel::ExtrapolateType> it(_channelExtrapolationTypes);

        while (it.hasNext()) {
            it.next();
            _channelExtrapolationNames[it.value()] = it.key();
        }
    }
    if (_keyTangentNames.isEmpty()) {
        QHashIterator<QString, Keyframe::TangentType> it(_keyTangentTypes);

        while (it.hasNext()) {
            it.next();
            _keyTangentNames[it.value()] = it.key();
        }
    }
}

AnimationClip::~AnimationClip()
{
    qDeleteAll(_channels);
}

bool AnimationClip::loadFromFile(const QString &filename)
{
    QFile   file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    BasicTokenizer  tokenizer(&file);
    if (!_loadAnimation(tokenizer)) {
//        deleteChildren();
        qDebug() << "Invalid file";
        file.close();
        return false;
    }
    QFileInfo info(filename);
    setName(info.baseName());
    _preCompute();
    file.close();
    return true;
}

const QString &AnimationClip::name() const
{
    return _name;
}

void AnimationClip::setName(const QString &name)
{
    _name = name;
}

float AnimationClip::startTime() const
{
    return _startTime;
}

float AnimationClip::endTime() const
{
    return _endTime;
}

const QVector<Channel *> &AnimationClip::channels() const
{
    return _channels;
}

void AnimationClip::setChannels(const QVector<Channel *> &channels)
{
    _channels = channels;
}

Channel *AnimationClip::channel(int idx) const
{
    return (idx < 0 || idx >= _channels.size()) ? NULL : _channels[idx];
}

void AnimationClip::evaluate(float t, Pose &pose)
{
    pose.resize(_channels.size());
    int i = 0;
    foreach (Channel* channel, _channels) {
        pose[i++] = channel ? channel->evaluate(t) : 0;
    }
}

void AnimationClip::saveToFile(const QString &filename)
{
    QFile   out(filename);
    out.open(QIODevice::WriteOnly);
    QTextStream stream(&out);
    stream << "animation {\n";
    stream << "\trange " << _startTime << " " << _endTime << "\n";
    stream << "\tnumchannels " << _channels.size() << "\n";
    foreach (Channel* channel, _channels) {
        QVector<Keyframe*> const& keys = channel->keys();
        stream << "\tchannel {\n";
        stream << "\t\textrapolate " << _channelExtrapolationNames[channel->extrapolationIn()] << " " << _channelExtrapolationNames[channel->extrapolationOut()] << "\n";
        stream << "\t\tkeys " << keys.size() << " {\n";
        foreach (Keyframe* key, keys) {
            stream << "\t\t\t" << key->time() << " " << key->value() << " ";
            if (key->tangentInType() == Keyframe::Fixed) {
                stream << key->tangentInValue();
            } else {
                stream << _keyTangentNames[key->tangentInType()];
            }
            stream << " ";
            if (key->tangentOutType() == Keyframe::Fixed) {
                stream << key->tangentOutValue();
            } else {
                stream << _keyTangentNames[key->tangentOutType()];
            }
            stream << "\n";
        }
        stream << "\t\t}\n";
        stream << "\t}\n";
    }
    stream << "}\n";
    out.close();
}

void AnimationClip::_preCompute()
{
    foreach(Channel* channel, _channels){
        if (channel) {
            channel->preCompute();
        }
    }
}

bool AnimationClip::_loadAnimation(BasicTokenizer &tokenizer)
{
    if (tokenizer.nextToken() != "animation")
        return false;
    if (tokenizer.nextToken() != "{")
        return false;
    QString token = tokenizer.nextToken();

    _currentParsingChannel = -1;
    while (!token.isEmpty()) {
        if (token == "}")
            return true;
        if (_loadingFunctions.contains(token) && (this->*_loadingFunctions[token])(tokenizer)) {
            token = tokenizer.nextToken();
            continue;
        }
        return false;
    }
    return true;
}

bool AnimationClip::_loadRange(BasicTokenizer &tokenizer)
{
    _startTime = tokenizer.nextFloat();
    _endTime = tokenizer.nextFloat();
    return true;
}

bool AnimationClip::_loadNumChannels(BasicTokenizer &tokenizer)
{
    _channels.resize(tokenizer.nextInt());
    _channels.fill(NULL);
    return true;
}

bool AnimationClip::_loadChannel(BasicTokenizer &tokenizer)
{
    if (tokenizer.nextToken() != "{")
        return false;
    _currentParsingChannel++;
    _channels[_currentParsingChannel] = new Channel();
    QString token = tokenizer.nextToken();

    while (!token.isEmpty()) {
        if (token == "}")
            return true;
        if (_loadingFunctions.contains(token) && (this->*_loadingFunctions[token])(tokenizer)) {
            token = tokenizer.nextToken();
            continue;
        }
        return false;
    }
    return true;
}

bool AnimationClip::_loadExtrapolate(BasicTokenizer &tokenizer)
{
    if (_currentParsingChannel < 0 || _currentParsingChannel >= _channels.size())
        return false;
    Channel* current = _channels[_currentParsingChannel];
    if (current == NULL)
        return false;
    QString in = tokenizer.nextToken();
    QString out = tokenizer.nextToken();

    if (_channelExtrapolationTypes.contains(in))
        current->setExtrapolationIn(_channelExtrapolationTypes[in]);
    else
        return false;
    if (_channelExtrapolationTypes.contains(out))
        current->setExtrapolationOut(_channelExtrapolationTypes[out]);
    else
        return false;
    return true;
}

bool AnimationClip::_loadKeys(BasicTokenizer &tokenizer)
{
    if (_currentParsingChannel < 0 || _currentParsingChannel >= _channels.size())
        return false;
    Channel* current = _channels[_currentParsingChannel];
    if (current == NULL)
        return false;
    int size = tokenizer.nextInt();
    if (tokenizer.nextToken() != "{")
        return false;
    QVector<Keyframe*>   keys;
    keys.resize(size);
    keys.fill(NULL);
    for (int i = 0; i < size; ++i) {
        Keyframe* key = new Keyframe();
        key->setTime(tokenizer.nextFloat());
        key->setValue(tokenizer.nextFloat());
        QString typeIn = tokenizer.nextToken();
        QString typeOut = tokenizer.nextToken();

        if (_keyTangentTypes.contains(typeIn)) {
            key->setTangentInType(_keyTangentTypes[typeIn]);
        } else {
            key->setTangentInType(Keyframe::Fixed);
            key->setTangentInValue(typeIn.toFloat());
        }
        if (_keyTangentTypes.contains(typeOut)) {
            key->setTangentOutType(_keyTangentTypes[typeOut]);
        } else {
            key->setTangentOutType(Keyframe::Fixed);
            key->setTangentOutValue(typeOut.toFloat());
        }
        keys[i] = key;
    }
    current->setKeys(keys);
    return tokenizer.nextToken() == "}";
}
