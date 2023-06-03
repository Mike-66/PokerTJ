/****************************************************************************
** Meta object code from reading C++ file 'ws_server.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ws_server/ws_server.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ws_server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TWS_Server_t {
    QByteArrayData data[13];
    char stringdata0[179];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TWS_Server_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TWS_Server_t qt_meta_stringdata_TWS_Server = {
    {
QT_MOC_LITERAL(0, 0, 10), // "TWS_Server"
QT_MOC_LITERAL(1, 11, 6), // "closed"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 15), // "MessageReceived"
QT_MOC_LITERAL(4, 35, 7), // "message"
QT_MOC_LITERAL(5, 43, 21), // "BinaryMessageReceived"
QT_MOC_LITERAL(6, 65, 11), // "QWebSocket*"
QT_MOC_LITERAL(7, 77, 7), // "pClient"
QT_MOC_LITERAL(8, 85, 18), // "SocketDisconnected"
QT_MOC_LITERAL(9, 104, 15), // "onNewConnection"
QT_MOC_LITERAL(10, 120, 18), // "processTextMessage"
QT_MOC_LITERAL(11, 139, 20), // "processBinaryMessage"
QT_MOC_LITERAL(12, 160, 18) // "socketDisconnected"

    },
    "TWS_Server\0closed\0\0MessageReceived\0"
    "message\0BinaryMessageReceived\0QWebSocket*\0"
    "pClient\0SocketDisconnected\0onNewConnection\0"
    "processTextMessage\0processBinaryMessage\0"
    "socketDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TWS_Server[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       5,    2,   58,    2, 0x06 /* Public */,
       8,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   66,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,
      11,    1,   70,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 6, QMetaType::QByteArray,    7,    4,
    QMetaType::Void, 0x80000000 | 6,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QByteArray,    4,
    QMetaType::Void,

       0        // eod
};

void TWS_Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TWS_Server *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->MessageReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->BinaryMessageReceived((*reinterpret_cast< QWebSocket*(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 3: _t->SocketDisconnected((*reinterpret_cast< QWebSocket*(*)>(_a[1]))); break;
        case 4: _t->onNewConnection(); break;
        case 5: _t->processTextMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->processBinaryMessage((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->socketDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TWS_Server::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TWS_Server::closed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TWS_Server::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TWS_Server::MessageReceived)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TWS_Server::*)(QWebSocket * , QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TWS_Server::BinaryMessageReceived)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TWS_Server::*)(QWebSocket * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TWS_Server::SocketDisconnected)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TWS_Server::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TWS_Server.data,
    qt_meta_data_TWS_Server,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TWS_Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TWS_Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TWS_Server.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TWS_Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TWS_Server::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TWS_Server::MessageReceived(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TWS_Server::BinaryMessageReceived(QWebSocket * _t1, QByteArray _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TWS_Server::SocketDisconnected(QWebSocket * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
