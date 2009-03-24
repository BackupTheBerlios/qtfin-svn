/****************************************************************************
** Meta object code from reading C++ file 'paintingscene.h'
**
** Created: Mon 23. Mar 20:28:14 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../paintingscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paintingscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaintingScene[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      36,   34,   14,   14, 0x0a,
      61,   34,   14,   14, 0x0a,
      84,   34,   14,   14, 0x0a,
     109,   14,   14,   14, 0x0a,
     125,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PaintingScene[] = {
    "PaintingScene\0\0lineFinished(bool)\0a\0"
    "activateAddControl(bool)\0"
    "activateAddPoint(bool)\0activateCreateLine(bool)\0"
    "alignTangents()\0showCoords()\0"
};

const QMetaObject PaintingScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_PaintingScene,
      qt_meta_data_PaintingScene, 0 }
};

const QMetaObject *PaintingScene::metaObject() const
{
    return &staticMetaObject;
}

void *PaintingScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintingScene))
        return static_cast<void*>(const_cast< PaintingScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int PaintingScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: lineFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: activateAddControl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: activateAddPoint((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: activateCreateLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: alignTangents(); break;
        case 5: showCoords(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void PaintingScene::lineFinished(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
