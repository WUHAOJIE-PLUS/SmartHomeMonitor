/****************************************************************************
** Meta object code from reading C++ file 'Login.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Login.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Login.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN8LoginTabE_t {};
} // unnamed namespace

template <> constexpr inline auto LoginTab::qt_create_metaobjectdata<qt_meta_tag_ZN8LoginTabE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "LoginTab",
        "Login_end",
        "",
        "UserRegistreSection",
        "UserLoginSection",
        "handleLogin1_OK",
        "data",
        "handleLogin1_ERROR",
        "handleLogin2_OK",
        "handleLogin2_ERROR",
        "handleRegistre1_OK",
        "handleRegistre1_ERROR",
        "handleRegistre2_OK",
        "handleRegistre2_ERROR"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'Login_end'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'UserRegistreSection'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'UserLoginSection'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleLogin1_OK'
        QtMocHelpers::SlotData<void(const QByteArray &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 6 },
        }}),
        // Slot 'handleLogin1_ERROR'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleLogin2_OK'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleLogin2_ERROR'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleRegistre1_OK'
        QtMocHelpers::SlotData<void(const QByteArray &)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QByteArray, 6 },
        }}),
        // Slot 'handleRegistre1_ERROR'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleRegistre2_OK'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleRegistre2_ERROR'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<LoginTab, qt_meta_tag_ZN8LoginTabE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject LoginTab::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8LoginTabE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8LoginTabE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN8LoginTabE_t>.metaTypes,
    nullptr
} };

void LoginTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<LoginTab *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->Login_end(); break;
        case 1: _t->UserRegistreSection(); break;
        case 2: _t->UserLoginSection(); break;
        case 3: _t->handleLogin1_OK((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 4: _t->handleLogin1_ERROR(); break;
        case 5: _t->handleLogin2_OK(); break;
        case 6: _t->handleLogin2_ERROR(); break;
        case 7: _t->handleRegistre1_OK((*reinterpret_cast< std::add_pointer_t<QByteArray>>(_a[1]))); break;
        case 8: _t->handleRegistre1_ERROR(); break;
        case 9: _t->handleRegistre2_OK(); break;
        case 10: _t->handleRegistre2_ERROR(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (LoginTab::*)()>(_a, &LoginTab::Login_end, 0))
            return;
    }
}

const QMetaObject *LoginTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginTab::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN8LoginTabE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LoginTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void LoginTab::Login_end()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
