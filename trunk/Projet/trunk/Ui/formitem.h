#ifndef FORMITEM_H
#define FORMITEM_H

#include <QtGui/QListWidgetItem>

class FormItem: public QListWidgetItem{

public:
    enum ItemType{Type = QListWidgetItem::UserType + 1};

    FormItem(const QIcon& icon,
             const QString& text,
             const QString& path,
             QListWidget* parent = 0,
             int type = QListWidgetItem::UserType + 1);

    QString path(){return _path;}
    void setPath(const QString& path){_path = path;}

private:

    QString _path;
};

#endif // FORMITEM_H
