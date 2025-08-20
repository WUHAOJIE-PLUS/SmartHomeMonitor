/****************************************************************************
** Meta object code from reading C++ file 'TcpCon.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TcpCon.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpCon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13TCPConnectionE_t {};
} // unnamed namespace

template <> constexpr inline auto TCPConnection::qt_create_metaobjectdata<qt_meta_tag_ZN13TCPConnectionE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TCPConnection",
        "handleLogin1_OK",
        "",
        "data",
        "handleLogin1_ERROR",
        "handleLogin2_OK",
        "handleLogin2_ERROR",
        "handleRegistre1_OK",
        "handleRegistre1_ERROR",
        "handleRegistre2_OK",
        "handleRegistre2_ERROR",
        "parseIncomingData"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'handleLogin1_OK'
        QtMocHelpers::SignalData<void(const QByteArray &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Signal 'handleLogin1_ERROR'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handleLogin2_OK'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handleLogin2_ERROR'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handleRegistre1_OK'
        QtMocHelpers::SignalData<void(const QByteArray &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QByteArray, 3 },
        }}),
        // Signal 'handleRegistre1_ERROR'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handleRegistre2_OK'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'handleRegistre2_ERROR'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'parseIncomingData'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TCPConnection, qt_meta_tag_ZN13TCPConnectionE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TCPConnection::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCPConnectionE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCPConnectionE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13TCPConnectionE_t>.metaTypes,
    nullptr
} };

void TCPConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TCPConnection *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->handleLogin1_OK((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 1: _t->handleLogin1_ERROR(); break;
        case 2: _t->handleLogin2_OK(); break;
        case 3: _t->handleLogin2_ERROR(); break;
        case 4: _t->handleRegistre1_OK((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 5: _t->handleRegistre1_ERROR(); break;
        case 6: _t->handleRegistre2_OK(); break;
        case 7: _t->handleRegistre2_ERROR(); break;
        case 8: _t->parseIncomingData(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)(const QByteArray & )>(_a, &TCPConnection::handleLogin1_OK, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleLogin1_ERROR, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleLogin2_OK, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleLogin2_ERROR, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)(const QByteArray & )>(_a, &TCPConnection::handleRegistre1_OK, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleRegistre1_ERROR, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleRegistre2_OK, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TCPConnection::*)()>(_a, &TCPConnection::handleRegistre2_ERROR, 7))
            return;
    }
}

const QMetaObject *TCPConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13TCPConnectionE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int TCPConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void TCPConnection::handleLogin1_OK(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TCPConnection::handleLogin1_ERROR()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TCPConnection::handleLogin2_OK()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TCPConnection::handleLogin2_ERROR()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TCPConnection::handleRegistre1_OK(const QByteArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void TCPConnection::handleRegistre1_ERROR()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TCPConnection::handleRegistre2_OK()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TCPConnection::handleRegistre2_ERROR()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
