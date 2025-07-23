/****************************************************************************
** Meta object code from reading C++ file 'friendrequestlistitem.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../friendrequestlistitem.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'friendrequestlistitem.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN21FriendRequestListItemE_t {};
} // unnamed namespace

template <> constexpr inline auto FriendRequestListItem::qt_create_metaobjectdata<qt_meta_tag_ZN21FriendRequestListItemE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FriendRequestListItem",
        "signal_addFriendClicked",
        "",
        "std::shared_ptr<RequestInfo>",
        "requestInfo"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_addFriendClicked'
        QtMocHelpers::SignalData<void(std::shared_ptr<RequestInfo>)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FriendRequestListItem, qt_meta_tag_ZN21FriendRequestListItemE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FriendRequestListItem::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseListItem::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21FriendRequestListItemE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21FriendRequestListItemE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21FriendRequestListItemE_t>.metaTypes,
    nullptr
} };

void FriendRequestListItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FriendRequestListItem *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_addFriendClicked((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<RequestInfo>>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FriendRequestListItem::*)(std::shared_ptr<RequestInfo> )>(_a, &FriendRequestListItem::signal_addFriendClicked, 0))
            return;
    }
}

const QMetaObject *FriendRequestListItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FriendRequestListItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21FriendRequestListItemE_t>.strings))
        return static_cast<void*>(this);
    return BaseListItem::qt_metacast(_clname);
}

int FriendRequestListItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseListItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void FriendRequestListItem::signal_addFriendClicked(std::shared_ptr<RequestInfo> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}
QT_WARNING_POP
