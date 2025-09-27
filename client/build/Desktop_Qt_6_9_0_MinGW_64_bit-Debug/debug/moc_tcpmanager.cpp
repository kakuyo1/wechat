/****************************************************************************
** Meta object code from reading C++ file 'tcpmanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../tcpmanager.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpmanager.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10TcpManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto TcpManager::qt_create_metaobjectdata<qt_meta_tag_ZN10TcpManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TcpManager",
        "signal_connect_to_chatserver_success",
        "",
        "success",
        "signal_login_failed",
        "signal_switchto_chatdialog",
        "signal_send_data",
        "RequestType",
        "type",
        "jsondata",
        "signal_login_failed_online_already",
        "signal_search_user_success",
        "std::shared_ptr<SearchInfo>",
        "contactInfo",
        "signal_search_user_failed",
        "errorMessage",
        "signal_add_newFriendListItem",
        "std::shared_ptr<FriendListItemInfo>",
        "itemInfo",
        "signal_add_contact_request_success",
        "std::shared_ptr<AddContactResponse>",
        "response",
        "signal_addcontactlistitem_showRedPoint",
        "signal_getACK_auth_friend_request_success_handlerequestItem",
        "std::shared_ptr<AuthResponse>",
        "signal_getACK_auth_friend_request_success_addNewItem",
        "signal_getPush_auth_friend_request_success",
        "signal_receive_chat_text_message",
        "std::shared_ptr<TextChatBatch>",
        "messageBatch",
        "slot_connect_to_chatserver",
        "serverInfo",
        "info",
        "slot_send_data"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_connect_to_chatserver_success'
        QtMocHelpers::SignalData<void(bool)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 3 },
        }}),
        // Signal 'signal_login_failed'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_switchto_chatdialog'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_send_data'
        QtMocHelpers::SignalData<void(RequestType, const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 }, { QMetaType::QString, 9 },
        }}),
        // Signal 'signal_login_failed_online_already'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_search_user_success'
        QtMocHelpers::SignalData<void(std::shared_ptr<SearchInfo>)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Signal 'signal_search_user_failed'
        QtMocHelpers::SignalData<void(QString)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 15 },
        }}),
        // Signal 'signal_add_newFriendListItem'
        QtMocHelpers::SignalData<void(std::shared_ptr<FriendListItemInfo>)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 17, 18 },
        }}),
        // Signal 'signal_add_contact_request_success'
        QtMocHelpers::SignalData<void(std::shared_ptr<AddContactResponse>)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 20, 21 },
        }}),
        // Signal 'signal_addcontactlistitem_showRedPoint'
        QtMocHelpers::SignalData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_getACK_auth_friend_request_success_handlerequestItem'
        QtMocHelpers::SignalData<void(std::shared_ptr<AuthResponse>)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 21 },
        }}),
        // Signal 'signal_getACK_auth_friend_request_success_addNewItem'
        QtMocHelpers::SignalData<void(std::shared_ptr<AuthResponse>)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 21 },
        }}),
        // Signal 'signal_getPush_auth_friend_request_success'
        QtMocHelpers::SignalData<void(std::shared_ptr<AuthResponse>)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 24, 21 },
        }}),
        // Signal 'signal_receive_chat_text_message'
        QtMocHelpers::SignalData<void(std::shared_ptr<TextChatBatch>)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 28, 29 },
        }}),
        // Slot 'slot_connect_to_chatserver'
        QtMocHelpers::SlotData<void(serverInfo)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 31, 32 },
        }}),
        // Slot 'slot_send_data'
        QtMocHelpers::SlotData<void(RequestType, const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 }, { QMetaType::QString, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TcpManager, qt_meta_tag_ZN10TcpManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TcpManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TcpManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TcpManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10TcpManagerE_t>.metaTypes,
    nullptr
} };

void TcpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TcpManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_connect_to_chatserver_success((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 1: _t->signal_login_failed(); break;
        case 2: _t->signal_switchto_chatdialog(); break;
        case 3: _t->signal_send_data((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->signal_login_failed_online_already(); break;
        case 5: _t->signal_search_user_success((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<SearchInfo>>>(_a[1]))); break;
        case 6: _t->signal_search_user_failed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->signal_add_newFriendListItem((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<FriendListItemInfo>>>(_a[1]))); break;
        case 8: _t->signal_add_contact_request_success((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AddContactResponse>>>(_a[1]))); break;
        case 9: _t->signal_addcontactlistitem_showRedPoint(); break;
        case 10: _t->signal_getACK_auth_friend_request_success_handlerequestItem((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthResponse>>>(_a[1]))); break;
        case 11: _t->signal_getACK_auth_friend_request_success_addNewItem((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthResponse>>>(_a[1]))); break;
        case 12: _t->signal_getPush_auth_friend_request_success((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthResponse>>>(_a[1]))); break;
        case 13: _t->signal_receive_chat_text_message((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatBatch>>>(_a[1]))); break;
        case 14: _t->slot_connect_to_chatserver((*reinterpret_cast< std::add_pointer_t<serverInfo>>(_a[1]))); break;
        case 15: _t->slot_send_data((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(bool )>(_a, &TcpManager::signal_connect_to_chatserver_success, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)()>(_a, &TcpManager::signal_login_failed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)()>(_a, &TcpManager::signal_switchto_chatdialog, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(RequestType , const QString & )>(_a, &TcpManager::signal_send_data, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)()>(_a, &TcpManager::signal_login_failed_online_already, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<SearchInfo> )>(_a, &TcpManager::signal_search_user_success, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(QString )>(_a, &TcpManager::signal_search_user_failed, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<FriendListItemInfo> )>(_a, &TcpManager::signal_add_newFriendListItem, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<AddContactResponse> )>(_a, &TcpManager::signal_add_contact_request_success, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)()>(_a, &TcpManager::signal_addcontactlistitem_showRedPoint, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<AuthResponse> )>(_a, &TcpManager::signal_getACK_auth_friend_request_success_handlerequestItem, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<AuthResponse> )>(_a, &TcpManager::signal_getACK_auth_friend_request_success_addNewItem, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<AuthResponse> )>(_a, &TcpManager::signal_getPush_auth_friend_request_success, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (TcpManager::*)(std::shared_ptr<TextChatBatch> )>(_a, &TcpManager::signal_receive_chat_text_message, 13))
            return;
    }
}

const QMetaObject *TcpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10TcpManagerE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Singleton<TcpManager>"))
        return static_cast< Singleton<TcpManager>*>(this);
    if (!strcmp(_clname, "std::enable_shared_from_this<TcpManager>"))
        return static_cast< std::enable_shared_from_this<TcpManager>*>(this);
    return QObject::qt_metacast(_clname);
}

int TcpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void TcpManager::signal_connect_to_chatserver_success(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void TcpManager::signal_login_failed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TcpManager::signal_switchto_chatdialog()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TcpManager::signal_send_data(RequestType _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void TcpManager::signal_login_failed_online_already()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TcpManager::signal_search_user_success(std::shared_ptr<SearchInfo> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void TcpManager::signal_search_user_failed(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void TcpManager::signal_add_newFriendListItem(std::shared_ptr<FriendListItemInfo> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void TcpManager::signal_add_contact_request_success(std::shared_ptr<AddContactResponse> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void TcpManager::signal_addcontactlistitem_showRedPoint()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TcpManager::signal_getACK_auth_friend_request_success_handlerequestItem(std::shared_ptr<AuthResponse> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void TcpManager::signal_getACK_auth_friend_request_success_addNewItem(std::shared_ptr<AuthResponse> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}

// SIGNAL 12
void TcpManager::signal_getPush_auth_friend_request_success(std::shared_ptr<AuthResponse> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1);
}

// SIGNAL 13
void TcpManager::signal_receive_chat_text_message(std::shared_ptr<TextChatBatch> _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}
QT_WARNING_POP
