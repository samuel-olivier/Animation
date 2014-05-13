#ifndef CHANNEL_H
#define CHANNEL_H

#include <QVector>

class Keyframe;

class Channel
{
public:
    enum ExtrapolateType {
        Constant,
        Linear,
        Cycle,
        CycleOffset,
        Bounce
    };

    Channel();
    virtual ~Channel();

    QVector<Keyframe*> const&    keys() const;
    void                    setKeys(QVector<Keyframe*> const& keys);

    Keyframe*                    key(int idx) const;
    void                    setKey(int idx, Keyframe* key);

    ExtrapolateType         extrapolationIn() const;
    void                    setExtrapolationIn(ExtrapolateType extrapolationIn);

    ExtrapolateType         extrapolationOut() const;
    void                    setExtrapolationOut(ExtrapolateType extrapolationOut);

    void                    preCompute();
    float evaluate(float t);

    float   startTime() const;
    float   endTime() const;

    float   minValue() const;
    float   maxValue() const;

    void    sortKeys();
    void    addKey(Keyframe* key);
    void    deleteKey(Keyframe* key);

private:
    QVector<Keyframe*>   _keys;
    ExtrapolateType _extrapolationIn;
    ExtrapolateType _extrapolationOut;
};

#endif // CHANNEL_H
