#include "DOF.h"

QHash<DOF::Type, QString>  DOF::_names = {
    {DOF::RotationX, "Rotation X"},
    {DOF::RotationY, "Rotation Y"},
    {DOF::RotationZ, "Rotation Z"},
    {DOF::TranslationX, "Translation X"},
    {DOF::TranslationY, "Translation Y"},
    {DOF::TranslationZ, "Translation Z"},
    {DOF::ColorR, "Color R"},
    {DOF::ColorG, "Color G"},
    {DOF::ColorB, "Color B"},
    {DOF::ColorA, "Color A"},
    {DOF::Diffuse, "Diffuse"},
    {DOF::Specular, "Specular"},
    {DOF::MorphTarget, "Level"},
    {DOF::AnimSpeed, "Anim Speed"}
};


DOF::DOF(Type type, bool enabled)
{
    _hasMin = false;
    _min = 0;
    _hasMax = false;
    _max = 0;
    _value = 0;
    _enabled = enabled;
    _type = type;
    _name = _names[type];
}

DOF::~DOF()
{
}

float DOF::value() const
{
    return _value;
}

void DOF::setValue(float val)
{
    if (_hasMin && val < _min)
        val = _min;
    else if (_hasMax && val > _max)
        val = _max;
    _value = val;
}

bool DOF::hasMin() const
{
    return _hasMin;
}

void DOF::setHasMin(bool hasMin)
{
    _hasMin = hasMin;
}

float DOF::min() const
{
    return _min;
}

void DOF::setMin(float val)
{
    _min = val;
    if (_value < _min)
        _value = _min;
    _hasMin = true;
}

bool DOF::hasMax() const
{
    return _hasMax;
}

void DOF::setHasMax(bool hasMax)
{
    _hasMax = hasMax;
}

float DOF::max() const
{
    return _max;
}

void DOF::setMax(float val)
{
    _max = val;
    if (_value > _max)
        _value = _max;
    _hasMax = true;
}

bool DOF::isEnabled() const
{
    return _enabled;
}

void DOF::setEnabled(bool enabled)
{
    _enabled = enabled;
}

const QString &DOF::name() const
{
    return _name;
}

void DOF::setName(const QString &name)
{
    _name = name;
}

DOF::Type DOF::type() const
{
    return _type;
}

void DOF::setType(DOF::Type type)
{
    _type = type;
}
