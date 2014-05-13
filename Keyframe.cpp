#include "Keyframe.h"

Keyframe::Keyframe()
{
    _time = 0;
    _value = 0;
    _tangentInType = Flat;
    _tangentOutType = Flat;
    _tangentInValue = 0;
    _tangentOutValue = 0;
}

Keyframe::~Keyframe()
{

}

float Keyframe::time() const
{
    return _time;
}

void Keyframe::setTime(float time)
{
    _time = time;
}

float Keyframe::value() const
{
    return _value;
}

void Keyframe::setValue(float value)
{
    _value = value;
}

Keyframe::TangentType Keyframe::tangentInType() const
{
    return _tangentInType;
}

void Keyframe::setTangentInType(Keyframe::TangentType tangentIn)
{
    _tangentInType = tangentIn;
}

Keyframe::TangentType Keyframe::tangentOutType() const
{
    return _tangentOutType;
}

void Keyframe::setTangentOutType(Keyframe::TangentType tangentOut)
{
    _tangentOutType = tangentOut;
}

float Keyframe::tangentInValue() const
{
    return _tangentInValue;
}

void Keyframe::setTangentInValue(float tangentInValue)
{
    _tangentInValue = tangentInValue;
}

float Keyframe::tangentOutValue() const
{
    return _tangentOutValue;
}

void Keyframe::setTangentOutValue(float tangentOutValue)
{
    _tangentOutValue = tangentOutValue;
}

float Keyframe::a() const
{
    return _a;
}

void Keyframe::setA(float a)
{
    _a = a;
}

float Keyframe::b() const
{
    return _b;
}

void Keyframe::setB(float b)
{
    _b = b;
}

float Keyframe::c() const
{
    return _c;
}

void Keyframe::setC(float c)
{
    _c = c;
}

float Keyframe::d() const
{
    return _d;
}

void Keyframe::setD(float d)
{
    _d = d;
}
