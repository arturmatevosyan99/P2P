/****************************************************************************
** Meta object code from reading C++ file 'chatclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/chatclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ChatClient_t {
    QByteArrayData data[26];
    char stringdata0[280];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatClient_t qt_meta_stringdata_ChatClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ChatClient"
QT_MOC_LITERAL(1, 11, 9), // "connected"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 8), // "loggedIn"
QT_MOC_LITERAL(4, 31, 10), // "loginError"
QT_MOC_LITERAL(5, 42, 7), // "sReason"
QT_MOC_LITERAL(6, 50, 12), // "disconnected"
QT_MOC_LITERAL(7, 63, 15), // "messageReceived"
QT_MOC_LITERAL(8, 79, 7), // "sSender"
QT_MOC_LITERAL(9, 87, 5), // "sText"
QT_MOC_LITERAL(10, 93, 5), // "error"
QT_MOC_LITERAL(11, 99, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(12, 128, 11), // "socketError"
QT_MOC_LITERAL(13, 140, 10), // "userJoined"
QT_MOC_LITERAL(14, 151, 9), // "sUserName"
QT_MOC_LITERAL(15, 161, 8), // "userLeft"
QT_MOC_LITERAL(16, 170, 15), // "connectToServer"
QT_MOC_LITERAL(17, 186, 12), // "QHostAddress"
QT_MOC_LITERAL(18, 199, 7), // "address"
QT_MOC_LITERAL(19, 207, 4), // "port"
QT_MOC_LITERAL(20, 212, 5), // "login"
QT_MOC_LITERAL(21, 218, 8), // "userName"
QT_MOC_LITERAL(22, 227, 11), // "sendMessage"
QT_MOC_LITERAL(23, 239, 9), // "sReceiver"
QT_MOC_LITERAL(24, 249, 18), // "disconnectFromHost"
QT_MOC_LITERAL(25, 268, 11) // "onReadyRead"

    },
    "ChatClient\0connected\0\0loggedIn\0"
    "loginError\0sReason\0disconnected\0"
    "messageReceived\0sSender\0sText\0error\0"
    "QAbstractSocket::SocketError\0socketError\0"
    "userJoined\0sUserName\0userLeft\0"
    "connectToServer\0QHostAddress\0address\0"
    "port\0login\0userName\0sendMessage\0"
    "sReceiver\0disconnectFromHost\0onReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,
       4,    1,   81,    2, 0x06 /* Public */,
       6,    0,   84,    2, 0x06 /* Public */,
       7,    2,   85,    2, 0x06 /* Public */,
      10,    1,   90,    2, 0x06 /* Public */,
      13,    1,   93,    2, 0x06 /* Public */,
      15,    1,   96,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    2,   99,    2, 0x0a /* Public */,
      20,    1,  104,    2, 0x0a /* Public */,
      22,    2,  107,    2, 0x0a /* Public */,
      24,    0,  112,    2, 0x0a /* Public */,
      25,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 17, QMetaType::UShort,   18,   19,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   23,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ChatClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChatClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->loggedIn(); break;
        case 2: _t->loginError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->messageReceived((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->userJoined((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->userLeft((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->connectToServer((*reinterpret_cast< const QHostAddress(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2]))); break;
        case 9: _t->login((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->sendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->disconnectFromHost(); break;
        case 12: _t->onReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChatClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::loggedIn)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)(QString const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::loginError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::disconnected)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)(QString const & , QString const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::messageReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)(QAbstractSocket::SocketError );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::error)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)(QString const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::userJoined)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ChatClient::*)(QString const & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ChatClient::userLeft)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ChatClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ChatClient.data,
    qt_meta_data_ChatClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ChatClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ChatClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ChatClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void ChatClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChatClient::loggedIn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ChatClient::loginError(QString const & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ChatClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ChatClient::messageReceived(QString const & _t1, QString const & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ChatClient::error(QAbstractSocket::SocketError _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ChatClient::userJoined(QString const & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void ChatClient::userLeft(QString const & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
