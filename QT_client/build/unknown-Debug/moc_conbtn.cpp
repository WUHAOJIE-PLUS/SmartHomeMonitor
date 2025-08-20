/****************************************************************************
** Meta object code from reading C++ file 'conbtn.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../conbtn.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'conbtn.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6ConBtnE_t {};
} // unnamed namespace

template <> constexpr inline auto ConBtn::qt_create_metaobjectdata<qt_meta_tag_ZN6ConBtnE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ConBtn",
        "sendInfo",
        "",
        "voiceChange_mute",
        "voiceChange",
        "onUpClicked",
        "onDownClicked",
        "onLeftClicked",
        "onRightClicked",
        "onUpLeftClicked",
        "onUpRightClicked",
        "onDownLeftClicked",
        "onDownRightClicked",
        "onCenterClicked",
        "onZoomClicked_add",
        "onZoomClicked_muins",
        "onFocusClicked_add",
        "onFocusClicked_muins",
        "onLightClicked_add",
        "onLightClicked_muins",
        "change_voice_icon"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sendInfo'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 2 },
        }}),
        // Signal 'voiceChange_mute'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'voiceChange'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onUpClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDownClicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLeftClicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRightClicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUpLeftClicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUpRightClicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDownLeftClicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDownRightClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCenterClicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZoomClicked_add'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onZoomClicked_muins'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFocusClicked_add'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFocusClicked_muins'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLightClicked_add'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLightClicked_muins'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'change_voice_icon'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ConBtn, qt_meta_tag_ZN6ConBtnE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ConBtn::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConBtnE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConBtnE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6ConBtnE_t>.metaTypes,
    nullptr
} };

void ConBtn::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ConBtn *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sendInfo((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->voiceChange_mute(); break;
        case 2: _t->voiceChange(); break;
        case 3: _t->onUpClicked(); break;
        case 4: _t->onDownClicked(); break;
        case 5: _t->onLeftClicked(); break;
        case 6: _t->onRightClicked(); break;
        case 7: _t->onUpLeftClicked(); break;
        case 8: _t->onUpRightClicked(); break;
        case 9: _t->onDownLeftClicked(); break;
        case 10: _t->onDownRightClicked(); break;
        case 11: _t->onCenterClicked(); break;
        case 12: _t->onZoomClicked_add(); break;
        case 13: _t->onZoomClicked_muins(); break;
        case 14: _t->onFocusClicked_add(); break;
        case 15: _t->onFocusClicked_muins(); break;
        case 16: _t->onLightClicked_add(); break;
        case 17: _t->onLightClicked_muins(); break;
        case 18: _t->change_voice_icon(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ConBtn::*)(const QString & )>(_a, &ConBtn::sendInfo, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConBtn::*)()>(_a, &ConBtn::voiceChange_mute, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConBtn::*)()>(_a, &ConBtn::voiceChange, 2))
            return;
    }
}

const QMetaObject *ConBtn::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConBtn::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ConBtnE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ConBtn::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void ConBtn::sendInfo(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void ConBtn::voiceChange_mute()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConBtn::voiceChange()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
