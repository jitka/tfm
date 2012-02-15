/****************************************************************************
** Meta object code from reading C++ file 'tab.h'
**
** Created: Wed Feb 15 23:43:03 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PartMassage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      22,   13,   12,   12, 0x08,
      42,   36,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PartMassage[] = {
    "PartMassage\0\0newValue\0onChange(int)\0"
    "state\0onCheck(int)\0"
};

void PartMassage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PartMassage *_t = static_cast<PartMassage *>(_o);
        switch (_id) {
        case 0: _t->onChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onCheck((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PartMassage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PartMassage::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PartMassage,
      qt_meta_data_PartMassage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PartMassage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PartMassage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PartMassage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PartMassage))
        return static_cast<void*>(const_cast< PartMassage*>(this));
    return QObject::qt_metacast(_clname);
}

int PartMassage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_Tab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x08,
      12,    4,    4,    4, 0x08,
      31,   22,    4,    4, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Tab[] = {
    "Tab\0\0onOk()\0onReset()\0newValue\0"
    "onSumChange(int)\0"
};

void Tab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Tab *_t = static_cast<Tab *>(_o);
        switch (_id) {
        case 0: _t->onOk(); break;
        case 1: _t->onReset(); break;
        case 2: _t->onSumChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Tab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Tab::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Tab,
      qt_meta_data_Tab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Tab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Tab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Tab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Tab))
        return static_cast<void*>(const_cast< Tab*>(this));
    return QWidget::qt_metacast(_clname);
}

int Tab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
