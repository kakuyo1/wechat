/****************************************************************************
** Meta object code from reading C++ file 'httpmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../httpmanager.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httpmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
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
struct qt_meta_tag_ZN11HttpManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto HttpManager::qt_create_metaobjectdata<qt_meta_tag_ZN11HttpManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HttpManager",
        "signal_http_request_finished",
        "",
        "RequestType",
        "type",
        "response",
        "ErrorCode",
        "error",
        "Module",
        "module",
        "signal_register_module_finished",
        "slot_http_request_finished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_http_request_finished'
        QtMocHelpers::SignalData<void(RequestType, QString, ErrorCode, Module)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Signal 'signal_register_module_finished'
        QtMocHelpers::SignalData<void(RequestType, QString, ErrorCode)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 },
        }}),
        // Slot 'slot_http_request_finished'
        QtMocHelpers::SlotData<void(RequestType, QString, ErrorCode, Module)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 }, { 0x80000000 | 8, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HttpManager, qt_meta_tag_ZN11HttpManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HttpManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11HttpManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11HttpManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11HttpManagerE_t>.metaTypes,
    nullptr
} };

void HttpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HttpManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_http_request_finished((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ErrorCode>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<Module>>(_a[4]))); break;
        case 1: _t->signal_register_module_finished((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ErrorCode>>(_a[3]))); break;
        case 2: _t->slot_http_request_finished((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ErrorCode>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<Module>>(_a[4]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HttpManager::*)(RequestType , QString , ErrorCode , Module )>(_a, &HttpManager::signal_http_request_finished, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HttpManager::*)(RequestType , QString , ErrorCode )>(_a, &HttpManager::signal_register_module_finished, 1))
            return;
    }
}

const QMetaObject *HttpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HttpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11HttpManagerE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Singleton<HttpManager>"))
        return static_cast< Singleton<HttpManager>*>(this);
    if (!strcmp(_clname, "std::enable_shared_from_this<HttpManager>"))
        return static_cast< std::enable_shared_from_this<HttpManager>*>(this);
    return QObject::qt_metacast(_clname);
}

int HttpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void HttpManager::signal_http_request_finished(RequestType _t1, QString _t2, ErrorCode _t3, Module _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void HttpManager::signal_register_module_finished(RequestType _t1, QString _t2, ErrorCode _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3);
}
QT_WARNING_POP
