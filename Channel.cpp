#include "Channel.h"

#include <QDebug>

#include "Keyframe.h"

Channel::Channel()
{
}

Channel::~Channel()
{
    qDeleteAll(_keys);
}

const QVector<Keyframe*> &Channel::keys() const
{
    return _keys;
}

void Channel::setKeys(const QVector<Keyframe*> &keys)
{
    _keys = keys;
}

Keyframe* Channel::key(int idx) const
{
    return (idx >= 0 && idx < _keys.size()) ? _keys[idx] : NULL;
}

void Channel::setKey(int idx, Keyframe* key)
{
    if (idx >= 0 && idx < _keys.size())
        _keys[idx] = key;
}

Channel::ExtrapolateType Channel::extrapolationIn() const
{
    return _extrapolationIn;
}

void Channel::setExtrapolationIn(Channel::ExtrapolateType extrapolationIn)
{
    _extrapolationIn = extrapolationIn;
}

Channel::ExtrapolateType Channel::extrapolationOut() const
{
    return _extrapolationOut;
}

void Channel::setExtrapolationOut(Channel::ExtrapolateType extrapolationOut)
{
    _extrapolationOut = extrapolationOut;
}

void Channel::preCompute()
{
    for (int i = 0; i < _keys.size(); ++i) {
        Keyframe* current = _keys[i];
        Keyframe* previous = (i <= 0) ? current : _keys[i - 1];
        Keyframe* next = i >= (_keys.size() - 1) ? current : _keys[i + 1];

        if (!current || !previous || !next)
            continue;

        if (current != previous && current->tangentInType() == Keyframe::Linear) {
            if (current->time() == previous->time()) {
                current->setTangentInValue(0);
            } else {
                current->setTangentInValue((current->value() - previous->value()) / (current->time() - previous->time()));
            }
        } else if (current != previous && current->tangentInType() == Keyframe::Smooth) {
            if (next->time() == previous->time()) {
                current->setTangentInValue(0);
            } else {
                current->setTangentInValue((next->value() - previous->value()) / (next->time() - previous->time()));
            }
        } else if (current->tangentInType() == Keyframe::Flat) {
            current->setTangentInValue(0);
        }
        if (current == previous && current->tangentInType() != Keyframe::Fixed) {
            current->setTangentInValue(current->tangentOutValue());
        }
        if (current != next && current->tangentOutType() == Keyframe::Linear) {
            if (current->time() == next->time()) {
                current->setTangentOutValue(0);
            } else {
                current->setTangentOutValue((next->value() - current->value()) / (next->time() - current->time()));
            }
        } else if (current != next && current->tangentOutType() == Keyframe::Smooth) {
            if (previous->time() == next->time()) {
                current->setTangentOutValue(0);
            } else {
                current->setTangentOutValue((next->value() - previous->value()) / (next->time() - previous->time()));
            }
        } else if (current->tangentOutType() == Keyframe::Flat) {
            current->setTangentOutValue(0);
        }
        if (current == previous && current->tangentInType() != Keyframe::Fixed) {
            current->setTangentInValue(current->tangentOutValue());
        }
        if (current == next && current->tangentOutType() != Keyframe::Fixed) {
            current->setTangentOutValue(current->tangentInValue());
        }
    }
    for (int i = 0; i < _keys.size() - 1; ++i) {
        Keyframe* k0 = _keys[i];
        Keyframe* k1 = _keys[i + 1];

        if (!k0 || !k1)
            continue;
        float tIn = k1->tangentInValue() * (k1->time() - k0->time());
        float tOut = k0->tangentOutValue() * (k1->time() - k0->time());
        float a = 2 * k0->value() - 2 * k1->value() + tOut + tIn,
                b = -3 * k0->value() + 3 * k1->value() - 2 * tOut - 1 * tIn,
                c = tOut,
                d = k0->value();
        k0->setA(a);
        k0->setB(b);
        k0->setC(c);
        k0->setD(d);
    }
}

float Channel::evaluate(float t)
{
    if (_keys.size() == 0)
        return 0;
    Keyframe* f = _keys[0];
    Keyframe* l = _keys[_keys.size() - 1];
    if (!l || !f)
        return 0;
    float start = f->time(),
            end = l->time();
    float duration = end - start,
            offset = 0;
    int occured = qRound((t - start) / duration - 0.5);
    if (t < start) {
        if (extrapolationIn() == ExtrapolateType::Constant) {
            return f->value();
        } else if (extrapolationIn() == ExtrapolateType::Linear) {
            return f->tangentOutValue() * (t - start) + f->value();
        } else if (extrapolationIn() == ExtrapolateType::Cycle) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
        } else if (extrapolationIn() == ExtrapolateType::CycleOffset) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
            offset = occured * (l->value() - f->value());
        } else if (extrapolationIn() == ExtrapolateType::Bounce) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
            if (abs(occured) % 2 == 1) {
                t = end - fmod(t - start + duration * abs(occured), duration);
            }
        }
    } else if (t > end) {
        if (extrapolationOut() == ExtrapolateType::Constant) {
            return l->value();
        } else if (extrapolationOut() == ExtrapolateType::Linear) {
            return l->tangentInValue() * (t - end) + l->value();
        } else if (extrapolationOut() == ExtrapolateType::Cycle) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
        } else if (extrapolationOut() == ExtrapolateType::CycleOffset) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
            offset = occured * (l->value() - f->value());
        } else if (extrapolationOut() == ExtrapolateType::Bounce) {
            t = fmod(t - start + duration * abs(occured), duration) + start;
            if (occured % 2 == 1) {
                t = end - fmod(t - start + duration * abs(occured), duration);
            }
        }
    }
    for (int i = 0; i < _keys.size(); ++i) {
        Keyframe* k0 = _keys[i];
        Keyframe* k1 = (i == _keys.size() - 1) ? k0 : _keys[i + 1];

        if (!k0 || !k1)
            return 0;
        else if (k0 == k1)
            return k0->value();
        if (t < k0->time() || t >= k1->time())
            continue ;
        float u = 0;
        if (k1->time() != k0->time()) {
            u = (t - k0->time()) / (k1->time() - k0->time());
        }
        return offset + k0->d() + u * (k0->c() + u * (k0->b() + u * k0->a()));
    }
    return 0;
}

float Channel::startTime() const
{
    if (_keys.size() > 0) {
        Keyframe* first = _keys.first();
        return first ? first->time() : 0;
    }
    return 0;
}

float Channel::endTime() const
{
    if (_keys.size() > 0) {
        Keyframe* last = _keys.last();
        return last ? last->time() : 0;
    }
    return 0;
}

float Channel::minValue() const
{
    float res = 0;
    bool set = false;

    foreach (Keyframe* key, _keys) {
        if (!key)
            continue;
        if (set == false) {
            set = true;
            res = key->value();
        } else {
            res = qMin(res, key->value());
        }
    }
    return res;
}

float Channel::maxValue() const
{
    float res = 0;
    bool set = false;

    foreach (Keyframe* key, _keys) {
        if (!key)
            continue;
        if (set == false) {
            set = true;
            res = key->value();
        } else {
            res = qMax(res, key->value());
        }
    }
    return res;
}

void Channel::sortKeys()
{
    qSort(_keys.begin(), _keys.end(), [] (Keyframe* l, Keyframe* r) {return l->time() < r->time();});
}

void Channel::addKey(Keyframe *key)
{
    _keys << key;
    sortKeys();
    preCompute();
}

void Channel::deleteKey(Keyframe *key)
{
    _keys.remove(_keys.indexOf(key));
    preCompute();
}
