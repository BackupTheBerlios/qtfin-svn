#include "formitem.h"

FormItem::FormItem(const QIcon& icon, const QString& text, const QString& path, QListWidget* parent, int type)
        :QListWidgetItem(icon, text, parent, type), _path(path){

}
