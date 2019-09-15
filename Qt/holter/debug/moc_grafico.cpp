/****************************************************************************
** Meta object code from reading C++ file 'grafico.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../grafico.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'grafico.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_grafico_t {
    QByteArrayData data[9];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_grafico_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_grafico_t qt_meta_stringdata_grafico = {
    {
QT_MOC_LITERAL(0, 0, 7), // "grafico"
QT_MOC_LITERAL(1, 8, 7), // "Cerrada"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 17), // "on_BSalir_clicked"
QT_MOC_LITERAL(4, 35, 20), // "on_BEncender_clicked"
QT_MOC_LITERAL(5, 56, 11), // "timerVencio"
QT_MOC_LITERAL(6, 68, 18), // "on_BGrabar_clicked"
QT_MOC_LITERAL(7, 87, 20), // "on_BAnterior_clicked"
QT_MOC_LITERAL(8, 108, 18) // "on_BBuscar_clicked"

    },
    "grafico\0Cerrada\0\0on_BSalir_clicked\0"
    "on_BEncender_clicked\0timerVencio\0"
    "on_BGrabar_clicked\0on_BAnterior_clicked\0"
    "on_BBuscar_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_grafico[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void grafico::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        grafico *_t = static_cast<grafico *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Cerrada(); break;
        case 1: _t->on_BSalir_clicked(); break;
        case 2: _t->on_BEncender_clicked(); break;
        case 3: _t->timerVencio(); break;
        case 4: _t->on_BGrabar_clicked(); break;
        case 5: _t->on_BAnterior_clicked(); break;
        case 6: _t->on_BBuscar_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (grafico::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&grafico::Cerrada)) {
                *result = 0;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject grafico::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_grafico.data,
      qt_meta_data_grafico,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *grafico::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *grafico::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_grafico.stringdata0))
        return static_cast<void*>(const_cast< grafico*>(this));
    return QDialog::qt_metacast(_clname);
}

int grafico::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void grafico::Cerrada()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
