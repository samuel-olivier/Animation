#ifndef DOF_H
#define DOF_H

#include <QString>
#include <QHash>

class DOF
{
public:
    enum Type {
        RotationX,
        RotationY,
        RotationZ,
        TranslationX,
        TranslationY,
        TranslationZ,
        ColorR,
        ColorG,
        ColorB,
        ColorA,
        Diffuse,
        Specular,
        MorphTarget,
        AnimSpeed,
        None
    };

    DOF(Type type = None, bool enabled = false);
    virtual ~DOF();

    float   value() const;
    void    setValue(float val);

    bool    hasMin() const;
    void    setHasMin(bool hasMin);

    float   min() const;
    void    setMin(float val);

    bool    hasMax() const;
    void    setHasMax(bool hasMax);

    float   max() const;
    void    setMax(float val);

    bool    isEnabled() const;
    void    setEnabled(bool enabled);

    QString const&  name() const;
    void            setName(QString const& name);

    Type    type() const;
    void    setType(Type type);
private:
    bool    _hasMin;
    float   _min;
    bool    _hasMax;
    float   _max;
    float   _value;
    QString _name;
    bool    _enabled;
    Type    _type;

    static QHash<Type, QString>  _names;
};

#endif // DOF_H
