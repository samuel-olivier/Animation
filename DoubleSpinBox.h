#ifndef DOUBLESPINBOX_H
#define DOUBLESPINBOX_H

#include <QDoubleSpinBox>

template <class T>
class DoubleSpinBox : public QDoubleSpinBox
{
public:
    DoubleSpinBox(QWidget *parent = 0) : QDoubleSpinBox(parent)
    {
    }

    void    setUserData(T& data)
    {
        _data = data;
    }

    T&      userData()
    {
        return _data;
    }

private:
    T   _data;
};

#endif // DOUBLESPINBOX_H
