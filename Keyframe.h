#ifndef KEY_H
#define KEY_H

class Keyframe
{
public:
    enum TangentType {
        Smooth,
        Flat,
        Linear,
        Fixed
    };

    Keyframe();
    virtual ~Keyframe();

    float   time() const;
    void    setTime(float time);

    float   value() const;
    void    setValue(float value);

    TangentType tangentInType() const;
    void        setTangentInType(TangentType tangentInType);

    TangentType tangentOutType() const;
    void        setTangentOutType(TangentType tangentOutType);

    float       tangentInValue() const;
    void        setTangentInValue(float tangentInValue);

    float       tangentOutValue() const;
    void        setTangentOutValue(float tangentOutValue);

    float       a() const;
    void        setA(float a);

    float       b() const;
    void        setB(float b);

    float       c() const;
    void        setC(float c);

    float       d() const;
    void        setD(float d);

private:
    float       _time;
    float       _value;
    TangentType _tangentInType;
    TangentType _tangentOutType;
    float       _tangentInValue;
    float       _tangentOutValue;
    float       _a;
    float       _b;
    float       _c;
    float       _d;
};

#endif // KEY_H
