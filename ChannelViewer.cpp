#include "ChannelViewer.h"

#include <QGraphicsItem>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

#include "ui_MainWindow.h"

QHash<QString, Channel::ExtrapolateType>    ChannelViewer::_extrapolationNameType = {
    {"Cycle", Channel::Cycle},
    {"Cycle Offset", Channel::CycleOffset},
    {"Linear", Channel::Linear},
    {"Constant", Channel::Constant},
    {"Bounce", Channel::Bounce}
};

QHash<QString, Keyframe::TangentType>       ChannelViewer::_tangentNameType = {
    {"Linear", Keyframe::Linear},
    {"Smooth", Keyframe::Smooth},
    {"Flat", Keyframe::Flat},
    {"Fixed", Keyframe::Fixed}
};

QHash<Channel::ExtrapolateType, QString>    ChannelViewer::_extrapolationTypeName;
QHash<Keyframe::TangentType, QString>       ChannelViewer::_tangentTypeName;

ChannelViewer::ChannelViewer(QWidget *parent) :
    QGraphicsView(parent)
{
    if (_extrapolationTypeName.size() == 0) {
        QHashIterator<QString, Channel::ExtrapolateType> it(_extrapolationNameType);
        while (it.hasNext()) {
            it.next();
            _extrapolationTypeName[it.value()] = it.key();
        }
    }
    if (_tangentTypeName.size() == 0) {
        QHashIterator<QString, Keyframe::TangentType> it(_tangentNameType);
        while (it.hasNext()) {
            it.next();
            _tangentTypeName[it.value()] = it.key();
        }
    }
    _ui = NULL;
    _scene = new QGraphicsScene(this);
    _scene->setBackgroundBrush(QBrush(QColor(69, 69, 61)));
    setScene(_scene);
    _innerCurve = NULL;
    _outerCurve = NULL;
    _mainGrid = NULL;
    _grid = NULL;
    setRenderHint(QPainter::Antialiasing);
    _isDragging = false;
    _selectionRect = NULL;
}

ChannelViewer::~ChannelViewer()
{
}

void ChannelViewer::setChannel(Channel* channel)
{
    _channel = channel;
    if (!channel) {
        _clear();
        return ;
    }
    _ui->extrapolationInType->blockSignals(true);
    _ui->extrapolationOutType->blockSignals(true);

    _ui->extrapolationInType->setCurrentText(_extrapolationTypeName[_channel->extrapolationIn()]);
    _ui->extrapolationOutType->setCurrentText(_extrapolationTypeName[_channel->extrapolationOut()]);

    _ui->extrapolationInType->blockSignals(false);
    _ui->extrapolationOutType->blockSignals(false);
    _createScene();
}

void ChannelViewer::initUi(Ui::MainWindow *ui)
{
    if (_ui == NULL) {
        _ui = ui;
        connect(ui->tangentInType, SIGNAL(currentIndexChanged(QString)), SLOT(onTangentTypeChanged(QString)));
        connect(ui->tangentInValue, SIGNAL(valueChanged(double)), SLOT(onTangentValueChanged(double)));
        connect(ui->tangentOutType, SIGNAL(currentIndexChanged(QString)), SLOT(onTangentTypeChanged(QString)));
        connect(ui->tangentOutValue, SIGNAL(valueChanged(double)), SLOT(onTangentValueChanged(double)));
        connect(ui->extrapolationInType, SIGNAL(currentIndexChanged(QString)), SLOT(onChannelChanged(QString)));
        connect(ui->extrapolationOutType, SIGNAL(currentIndexChanged(QString)), SLOT(onChannelChanged(QString)));
        connect(ui->keyTime, SIGNAL(valueChanged(double)), SLOT(onKeyChanged(double)));
        connect(ui->keyValue, SIGNAL(valueChanged(double)), SLOT(onKeyChanged(double)));
    }
}

void ChannelViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (_channel) {
        if (event->buttons() & Qt::LeftButton) {
            if ((event->pos() - _originMousePos).manhattanLength() >= QApplication::startDragDistance()) {
                _isDragging = true;
            }
        }
        if (_isDragging && _selected._keyframe != NULL) {
            QPointF pos = mapToScene(event->pos());
            _selected._keyframe->setTime(pos.x());
            _selected._keyframe->setValue(pos.y());
            onKeyChanged(0);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void ChannelViewer::mousePressEvent(QMouseEvent *event)
{
    if (_channel) {
        if (event->button() == Qt::LeftButton) {
            _originMousePos = event->pos();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void ChannelViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (_channel) {
        if (event->button() == Qt::LeftButton) {
            if (_isDragging == false) {
                if (event->modifiers() & Qt::ControlModifier) {
                    _addKey(mapToScene(event->pos()));
                } else {
                    QGraphicsItem* current = itemAt(_originMousePos);

                    _changeSelection(NULL);
                    while (current) {
                        if (current->type() == QGraphicsItemGroup::Type) {
                            QGraphicsItemGroup* keyGroup = dynamic_cast<QGraphicsItemGroup*>(current);
                            if (keyGroup && _keys.contains(keyGroup)) {
                                _changeSelection(keyGroup);
                            }
                        }
                        current = current->parentItem();
                    }
                }
            } else {
                _isDragging = false;
            }
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void ChannelViewer::keyReleaseEvent(QKeyEvent *event)
{
    if (_channel) {
        if (event->key() == Qt::Key_Delete && _selected._keyframe) {
            KeyframeInfo current = _selected;
            _changeSelection(NULL);
            _keys.remove(current._group);
            delete current._rect;
            delete current._tangentOut;
            delete current._tangentIn;
            delete current._group;
            _channel->deleteKey(current._keyframe);
            _updateCurve();
        }
    }
}

void ChannelViewer::onSelectionChanged()
{
    QList<QGraphicsItem*> items = _scene->selectedItems();
    if (items.isEmpty())
        return ;

    QGraphicsItem* first = items.first();
    if (!first)
        return ;

    if (first->type() == QGraphicsItemGroup::Type && _keys.contains(first)) {
        _selected = _keys[first];
    }
}

void ChannelViewer::onTangentTypeChanged(QString newType)
{
    Keyframe* key = _selected._keyframe;
    if (!key || !_channel)
        return ;
    if (sender() == _ui->tangentInType) {
        key->setTangentInType(_tangentNameType.value(newType, key->tangentInType()));
    } else if (sender() == _ui->tangentOutType) {
        key->setTangentOutType(_tangentNameType.value(newType, key->tangentOutType()));
    }
    _channel->preCompute();
    _updateKeys();
    _updateCurve();
    _setKeyUi();
}

void ChannelViewer::onTangentValueChanged(double newValue)
{
    Keyframe* key = _selected._keyframe;
    if (!key || !_channel)
        return ;
    if (sender() == _ui->tangentInValue) {
        key->setTangentInValue(newValue);
        key->setTangentInType(Keyframe::Fixed);
    } else if (sender() == _ui->tangentOutValue) {
        key->setTangentOutValue(newValue);
        key->setTangentOutType(Keyframe::Fixed);
    }
    _channel->preCompute();
    _updateKeys();
    _updateCurve();
    _setKeyUi();
}

void ChannelViewer::onKeyChanged(double newValue)
{
    Keyframe* key = _selected._keyframe;
    if (!key || !_channel)
        return ;
    if (sender() == _ui->keyTime) {
        key->setTime(newValue);
    } else if (sender() == _ui->keyValue) {
        key->setValue(newValue);
    }
    _channel->sortKeys();
    _channel->preCompute();
    _updateKeys();
    _updateCurve();
    _setKeyUi();
}

void ChannelViewer::onChannelChanged(QString newType)
{
    if (!_channel)
        return ;
    if (sender() == _ui->extrapolationInType) {
        _channel->setExtrapolationIn(_extrapolationNameType.value(newType, _channel->extrapolationIn()));
    } else if (sender() == _ui->extrapolationOutType) {
        _channel->setExtrapolationOut(_extrapolationNameType.value(newType, _channel->extrapolationOut()));
    }
    _updateCurve();
}

void ChannelViewer::_clear()
{
    delete _innerCurve;
    delete _outerCurve;
    delete _mainGrid;
    delete _grid;
    delete _selectionRect;
    QHashIterator<QGraphicsItem*, KeyframeInfo>  it(_keys);
    while (it.hasNext()) {
        it.next();
        delete it.value()._rect;
        delete it.value()._tangentIn;
        delete it.value()._tangentOut;
        delete it.value()._group;
    }
    _innerCurve = NULL;
    _outerCurve = NULL;
    _mainGrid = NULL;
    _grid = NULL;
    _selectionRect = NULL;
    _keys.clear();
    _scene->clear();
}

void ChannelViewer::_createScene()
{
    _clear();
    _info.startTime = _channel->startTime();
    _info.endTime = _channel->endTime();
    _info.duration = _info.endTime - _info.startTime;
    if (_info.duration == 0)
        _info.duration = 1;
    _info.numCyle = 5;
    _info.worldWidth = _info.duration * _info.numCyle;
    _info.worldLeft = _info.startTime - (_info.worldWidth - (_info.endTime - _info.startTime)) / 2;
    _info.worldRight = _info.worldLeft + _info.worldWidth;
    _info.max = 0;
    _info.min = 0;
    QPointF previous;
    QPen selectionPen(QColor(90, 255, 48));
    selectionPen.setWidthF(0);
    QPen innerPen(QColor(255, 184, 32));
    innerPen.setWidthF(0);
    QPen outerPen(innerPen);
    outerPen.setColor(outerPen.color().darker(150));
    QPen mainGridPen(QColor(150, 150, 150));
    mainGridPen.setWidthF(0);
    QPen gridPen(QColor(115, 115, 115));
    gridPen.setWidthF(0);
    QPen axisPen(QColor(186, 186, 186));
    axisPen.setWidthF(0);
    QPainterPath innerPath;
    QPainterPath outerPath;
    QPainterPath mainGridPath;
    QPainterPath gridPath;
    bool minMaxSet = false;
    for (int i = 0; i < width(); ++i) {
        QPointF pos;
        pos.setX(_info.worldLeft + _info.worldWidth * ((float)i / width()));
        pos.setY(_channel->evaluate(pos.x()));
        if (i > 0) {
            if (pos.x() >= _info.startTime && pos.x() <= _info.endTime) {
                if (minMaxSet == false) {
                    minMaxSet = true;
                    _info.min = pos.y();
                    _info.max = _info.min;
                } else {
                    _info.min = qMin(_info.min, (float)pos.y());
                    _info.max = qMax(_info.max, (float)pos.y());
                }
                innerPath.moveTo(previous);
                innerPath.lineTo(pos);
            } else {
                outerPath.moveTo(previous);
                outerPath.lineTo(pos);
            }
        }
        previous = pos;
    }
    _info.worldHeight = _info.max - _info.min;
    if (_info.worldHeight == 0) {
        _info.worldHeight = 1;
        _info.min -= 0.5;
        _info.max += 0.5;
    }
    _info.heightOffset = (_info.worldHeight * 1.1 - _info.worldHeight) / 2;
    _info.worldHeight *= 1.1;
    _info.worldTop = _info.max + _info.heightOffset;
    _info.worldBottom = _info.min - _info.heightOffset;
    _info.xSW = _info.worldWidth / width();
    _info.ySW = _info.worldHeight / height();
    _info.xWS = (float)width() / _info.worldWidth;
    _info.yWS = (float)height() / _info.worldHeight;
    _info.keySize = 5;
    _info.keyWidth = _info.keySize * _info.xSW;
    _info.keyHeight = _info.keySize * _info.ySW;
    _info.tangentLength = 32;
    QVector<Keyframe*> const& keys = _channel->keys();
    foreach (Keyframe* key, keys) {
        if (!key)
            continue;
        _createKey(key, false);
    }
    for (int i = _info.worldLeft; i < _info.worldRight; ++i) {
        mainGridPath.moveTo(i, _info.worldTop);
        mainGridPath.lineTo(i, _info.worldBottom);
        if (i + 0.5 < _info.worldRight) {
            gridPath.moveTo(i + 0.5, _info.worldTop);
            gridPath.lineTo(i + 0.5, _info.worldBottom);
        }
    }
    for (float i = _info.worldBottom; i < _info.worldTop; i += 1) {
        mainGridPath.moveTo(_info.worldLeft, i);
        mainGridPath.lineTo(_info.worldRight, i);
        if (i + 0.5 < _info.worldTop) {
            gridPath.moveTo(_info.worldLeft, i + 0.5);
            gridPath.lineTo(_info.worldRight, i + 0.5);
        }
    }
    _selectionRect = new QGraphicsRectItem();
    _selectionRect->setPen(selectionPen);
    _selectionRect->setVisible(false);
    _innerCurve = new QGraphicsPathItem(innerPath);
    _innerCurve->setPen(innerPen);
    _outerCurve = new QGraphicsPathItem(outerPath);
    _outerCurve->setPen(outerPen);
    _mainGrid = new QGraphicsPathItem(mainGridPath);
    _mainGrid->setPen(mainGridPen);
    _grid = new QGraphicsPathItem(gridPath);
    _grid->setPen(gridPen);
    _scene->addItem(_grid);
    _scene->addItem(_mainGrid);
    _scene->addItem(_innerCurve);
    _scene->addItem(_outerCurve);
    _scene->addItem(_selectionRect);
    QHashIterator<QGraphicsItem*, KeyframeInfo>  it(_keys);
    while (it.hasNext()) {
        it.next();
        _scene->addItem(it.key());
    }
    QTransform sym;
    sym.scale(1, -1);
    setTransform(sym);
    _world.setRect(_info.worldLeft, _info.worldTop, _info.worldWidth, _info.worldHeight);
    fitInView(_world);
    centerOn(_world.center());
    ensureVisible(_world);
}

void ChannelViewer::_changeSelection(QGraphicsItem *newSelection)
{
    _selected.reset();
    if (_keys.contains(newSelection)) {
        _selected = _keys[newSelection];
        _selectionRect->setVisible(true);
        _ui->keyGroup->setEnabled(true);
        _setKeyUi();
    } else {
        _selectionRect->setVisible(false);
        _ui->keyGroup->setEnabled(false);
    }
}

void ChannelViewer::_setKeyUi()
{
    _selectionRect->setRect(_selected._group->sceneBoundingRect());
    _ui->tangentInType->blockSignals(true);
    _ui->tangentInValue->blockSignals(true);
    _ui->tangentOutType->blockSignals(true);
    _ui->tangentOutValue->blockSignals(true);
    _ui->keyTime->blockSignals(true);
    _ui->keyValue->blockSignals(true);

    _ui->tangentInType->setCurrentText(_tangentTypeName[_selected._keyframe->tangentInType()]);
    _ui->tangentInValue->setValue(_selected._keyframe->tangentInValue());
    _ui->tangentOutType->setCurrentText(_tangentTypeName[_selected._keyframe->tangentOutType()]);
    _ui->tangentOutValue->setValue(_selected._keyframe->tangentOutValue());
    _ui->keyTime->setValue(_selected._keyframe->time());
    _ui->keyValue->setValue(_selected._keyframe->value());

    _ui->tangentInType->blockSignals(false);
    _ui->tangentInValue->blockSignals(false);
    _ui->tangentOutType->blockSignals(false);
    _ui->tangentOutValue->blockSignals(false);
    _ui->keyTime->blockSignals(false);
    _ui->keyValue->blockSignals(false);
}

void ChannelViewer::_updateKeys()
{
    QHashIterator<QGraphicsItem*, KeyframeInfo> it(_keys);

    while (it.hasNext()) {
        it.next();
        Keyframe* key = it.value()._keyframe;
        QLineF line(key->time(), key->value(), key->time() + 1, key->value() + key->tangentOutValue());
        line.setLine(line.x1() * _info.xWS, line.y1() * _info.yWS, line.x2() * _info.xWS, line.y2() * _info.yWS);
        line.setLength(_info.tangentLength);
        line.setLine(line.x1() * _info.xSW, line.y1() * _info.ySW, line.x2() * _info.xSW, line.y2() * _info.ySW);
        it.value()._tangentOut->setLine(line);

        line.setLine(key->time(), key->value(), key->time() - 1, key->value() - key->tangentInValue());
        line.setLine(line.x1() * _info.xWS, line.y1() * _info.yWS, line.x2() * _info.xWS, line.y2() * _info.yWS);
        line.setLength(_info.tangentLength);
        line.setLine(line.x1() * _info.xSW, line.y1() * _info.ySW, line.x2() * _info.xSW, line.y2() * _info.ySW);
        it.value()._tangentIn->setLine(line);

        it.value()._rect->setRect(key->time() - _info.keyWidth / 2, key->value() - _info.keyHeight / 2, _info.keyWidth, _info.keyHeight);
        it.value()._group->removeFromGroup(it.value()._rect);
        it.value()._group->addToGroup(it.value()._rect);
    }
}

void ChannelViewer::_updateCurve()
{
    _info.startTime = _channel->startTime();
    _info.endTime = _channel->endTime();
    QPainterPath innerPath;
    QPainterPath outerPath;
    QPointF previous;
    for (int i = 0; i < width(); ++i) {
        QPointF pos;
        pos.setX(_info.worldLeft + _info.worldWidth * ((float)i / width()));
        pos.setY(_channel->evaluate(pos.x()));
        if (i > 0) {
            if (pos.x() >= _info.startTime && pos.x() <= _info.endTime) {
                innerPath.moveTo(previous);
                innerPath.lineTo(pos);
            } else {
                outerPath.moveTo(previous);
                outerPath.lineTo(pos);
            }
        }
        previous = pos;
    }
    _innerCurve->setPath(innerPath);
    _outerCurve->setPath(outerPath);
}

void ChannelViewer::_addKey(const QPointF &pos)
{
    Keyframe* newKey = new Keyframe();
    newKey->setTime(pos.x());
    newKey->setValue(pos.y());
    _channel->addKey(newKey);
    _createKey(newKey);
    _updateCurve();
}

void ChannelViewer::_createKey(Keyframe *key, bool addToScene)
{
    QPen keysPen(QColor(90, 255, 48));
    keysPen.setWidthF(0);
    QBrush keysBrush(keysPen.color());
    QPen tangentsPen(QColor(90, 255, 48));
    tangentsPen.setWidthF(0);
    QGraphicsItemGroup* keyGroup = new QGraphicsItemGroup(NULL);
    QGraphicsLineItem* tangentOut = new QGraphicsLineItem(keyGroup);
    tangentOut->setPen(tangentsPen);
    QGraphicsLineItem* tangentIn = new QGraphicsLineItem(keyGroup);
    tangentIn->setPen(tangentsPen);
    QGraphicsRectItem* rect = new QGraphicsRectItem(keyGroup);
    rect->setPen(keysPen);
    rect->setBrush(keysBrush);

    keyGroup->addToGroup(rect);
    keyGroup->addToGroup(tangentIn);
    keyGroup->addToGroup(tangentOut);
    KeyframeInfo value;
    value._group = keyGroup;
    value._rect = rect;
    value._tangentIn = tangentIn;
    value._tangentOut = tangentOut;
    value._keyframe = key;
    _keys[keyGroup] = value;
    _updateKeys();
    if (addToScene)
        _scene->addItem(keyGroup);
}

ChannelViewer::KeyframeInfo::KeyframeInfo()
{
    reset();
}

void ChannelViewer::KeyframeInfo::reset()
{
    _rect = NULL;
    _tangentIn = NULL;
    _tangentOut = NULL;
    _group = NULL;
    _keyframe = NULL;
}
