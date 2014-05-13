#ifndef ANIMATIONVIEWER_H
#define ANIMATIONVIEWER_H

#include <QGraphicsView>
#include <QHash>

#include "Channel.h"
#include "Keyframe.h"

namespace Ui {
class MainWindow;
}


class ChannelViewer : public QGraphicsView
{
    Q_OBJECT
public:
    ChannelViewer(QWidget *parent = 0);
    virtual ~ChannelViewer();

    void    setChannel(Channel* channel);
    void    initUi(Ui::MainWindow* ui);

protected:
    virtual void	mouseMoveEvent(QMouseEvent* event);
    virtual void	mousePressEvent(QMouseEvent* event);
    virtual void	mouseReleaseEvent(QMouseEvent* event);
    virtual void	keyReleaseEvent(QKeyEvent *event);

private slots:
    void    onSelectionChanged();
    void    onTangentTypeChanged(QString newType);
    void    onTangentValueChanged(double newValue);
    void    onKeyChanged(double newValue);
    void    onChannelChanged(QString newType);

private:
    struct KeyframeInfo {
        KeyframeInfo();
        void    reset();

        QGraphicsItemGroup* _group;
        QGraphicsRectItem*  _rect;
        QGraphicsLineItem*  _tangentIn;
        QGraphicsLineItem*  _tangentOut;
        Keyframe*           _keyframe;
    };

    struct World {
        float startTime;
        float endTime;
        float duration;
        int   numCyle;
        float worldWidth;
        float worldRight;
        float worldLeft;
        float worldHeight;
        float worldTop;
        float worldBottom;
        float heightOffset;
        float min;
        float max;
        float xSW;
        float ySW;
        float xWS;
        float yWS;
        float keySize;
        float keyWidth;
        float keyHeight;
        float tangentLength;
    };

    void    _clear();
    void    _createScene();
    void    _changeSelection(QGraphicsItem* newSelection);
    void    _setKeyUi();
    void    _updateKeys();
    void    _updateCurve();
    void    _addKey(QPointF const& pos);
    void    _createKey(Keyframe* key, bool addToScene = true);

    Ui::MainWindow*     _ui;
    Channel*            _channel;
    QGraphicsScene*     _scene;
    QGraphicsPathItem*  _innerCurve;
    QGraphicsPathItem*  _outerCurve;
    QGraphicsPathItem*  _mainGrid;
    QGraphicsPathItem*  _grid;
    QHash<QGraphicsItem*, KeyframeInfo>    _keys;
    QRectF              _world;
    QPoint              _originMousePos;
    KeyframeInfo        _selected;
    QGraphicsRectItem*  _selectionRect;
    bool                _isDragging;
    World               _info;

    static QHash<QString, Channel::ExtrapolateType>    _extrapolationNameType;
    static QHash<Channel::ExtrapolateType, QString>    _extrapolationTypeName;
    static QHash<QString, Keyframe::TangentType>       _tangentNameType;
    static QHash<Keyframe::TangentType, QString>       _tangentTypeName;
};

#endif // ANIMATIONVIEWER_H
