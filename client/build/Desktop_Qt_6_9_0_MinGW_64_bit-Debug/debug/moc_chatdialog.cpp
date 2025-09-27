/****************************************************************************
** Meta object code from reading C++ file 'chatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../chatdialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatdialog.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10ChatDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatDialog::qt_create_metaobjectdata<qt_meta_tag_ZN10ChatDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatDialog",
        "slot_load_more_sessionitems",
        "",
        "slot_load_more_contactitems",
        "slot_switchTo_contactInfoPage",
        "slot_append_chat_message",
        "std::shared_ptr<TextChatData>",
        "message",
        "slot_get_new_text_chat_batch",
        "std::shared_ptr<TextChatBatch>",
        "batch",
        "slot_sessionItem_clicked",
        "QListWidgetItem*",
        "item",
        "slot_switchTo_sessionItem_from_contactInfo",
        "std::shared_ptr<AuthResponse>",
        "contactInfo"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'slot_load_more_sessionitems'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_load_more_contactitems'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_switchTo_contactInfoPage'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_append_chat_message'
        QtMocHelpers::SlotData<void(std::shared_ptr<TextChatData>)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Slot 'slot_get_new_text_chat_batch'
        QtMocHelpers::SlotData<void(std::shared_ptr<TextChatBatch>)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'slot_sessionItem_clicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'slot_switchTo_sessionItem_from_contactInfo'
        QtMocHelpers::SlotData<void(std::shared_ptr<AuthResponse>)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 15, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChatDialog, qt_meta_tag_ZN10ChatDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10ChatDialogE_t>.metaTypes,
    nullptr
} };

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->slot_load_more_sessionitems(); break;
        case 1: _t->slot_load_more_contactitems(); break;
        case 2: _t->slot_switchTo_contactInfoPage(); break;
        case 3: _t->slot_append_chat_message((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatData>>>(_a[1]))); break;
        case 4: _t->slot_get_new_text_chat_batch((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<TextChatBatch>>>(_a[1]))); break;
        case 5: _t->slot_sessionItem_clicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 6: _t->slot_switchTo_sessionItem_from_contactInfo((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthResponse>>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10ChatDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
