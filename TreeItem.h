#ifndef TREEITEM_H
#define TREEITEM_H

#include <QTreeWidgetItem>

template <class T>
class TreeItem : public QTreeWidgetItem
{
public:
    TreeItem(QTreeWidget *parent = 0) : QTreeWidgetItem(parent)
    {
    }

    TreeItem(QTreeWidgetItem *parent = 0) : QTreeWidgetItem(parent)
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

#endif // TREEITEM_H
