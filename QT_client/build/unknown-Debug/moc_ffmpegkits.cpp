/****************************************************************************
** Meta object code from reading C++ file 'ffmpegkits.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ffmpegkits.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ffmpegkits.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10FFmpegKitsE_t {};
} // unnamed namespace

template <> constexpr inline auto FFmpegKits::qt_create_metaobjectdata<qt_meta_tag_ZN10FFmpegKitsE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FFmpegKits",
        "sigGetOneFrame",
        "",
        "image",
        "sigError",
        "message",
        "sigInitAudioDevice",
        "sampleRate",
        "channels",
        "End",
        "AVFormatContext*",
        "onInitAudioDevice"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sigGetOneFrame'
        QtMocHelpers::SignalData<void(QImage)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QImage, 3 },
        }}),
        // Signal 'sigError'
        QtMocHelpers::SignalData<void(QString)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'sigInitAudioDevice'
        QtMocHelpers::SignalData<void(int, int)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 },
        }}),
        // Signal 'End'
        QtMocHelpers::SignalData<void(AVFormatContext *)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 10, 2 },
        }}),
        // Slot 'onInitAudioDevice'
        QtMocHelpers::SlotData<void(int, int)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FFmpegKits, qt_meta_tag_ZN10FFmpegKitsE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FFmpegKits::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10FFmpegKitsE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10FFmpegKitsE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10FFmpegKitsE_t>.metaTypes,
    nullptr
} };

void FFmpegKits::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FFmpegKits *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sigGetOneFrame((*reinterpret_cast< std::add_pointer_t<QImage>>(_a[1]))); break;
        case 1: _t->sigError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->sigInitAudioDevice((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 3: _t->End((*reinterpret_cast< std::add_pointer_t<AVFormatContext*>>(_a[1]))); break;
        case 4: _t->onInitAudioDevice((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FFmpegKits::*)(QImage )>(_a, &FFmpegKits::sigGetOneFrame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FFmpegKits::*)(QString )>(_a, &FFmpegKits::sigError, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FFmpegKits::*)(int , int )>(_a, &FFmpegKits::sigInitAudioDevice, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FFmpegKits::*)(AVFormatContext * )>(_a, &FFmpegKits::End, 3))
            return;
    }
}

const QMetaObject *FFmpegKits::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FFmpegKits::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10FFmpegKitsE_t>.strings))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int FFmpegKits::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FFmpegKits::sigGetOneFrame(QImage _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void FFmpegKits::sigError(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void FFmpegKits::sigInitAudioDevice(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void FFmpegKits::End(AVFormatContext * _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
