/****************************************************************************
** Meta object code from reading C++ file 'contactlist.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../contactlist.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contactlist.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN11ContactListE_t {};
} // unnamed namespace

template <> constexpr inline auto ContactList::qt_create_metaobjectdata<qt_meta_tag_ZN11ContactListE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ContactList",
        "signal_load_contacts",
        "",
        "signal_switchTo_contactInfoPage",
        "signal_switchTo_friendRequestPage",
        "slot_AfterACK_addNewContactItem",
        "std::shared_ptr<AuthResponse>",
        "slot_contactItem_clicked",
        "QListWidgetItem*",
        "item"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_load_contacts'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_switchTo_contactInfoPage'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'signal_switchTo_friendRequestPage'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'slot_AfterACK_addNewContactItem'
        QtMocHelpers::SlotData<void(std::shared_ptr<AuthResponse>)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 6, 2 },
        }}),
        // Slot 'slot_contactItem_clicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ContactList, qt_meta_tag_ZN11ContactListE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ContactList::staticMetaObject = { {
    QMetaObject::SuperData::link<QListWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ContactListE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ContactListE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11ContactListE_t>.metaTypes,
    nullptr
} };

void ContactList::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ContactList *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_load_contacts(); break;
        case 1: _t->signal_switchTo_contactInfoPage(); break;
        case 2: _t->signal_switchTo_friendRequestPage(); break;
        case 3: _t->slot_AfterACK_addNewContactItem((*reinterpret_cast< std::add_pointer_t<std::shared_ptr<AuthResponse>>>(_a[1]))); break;
        case 4: _t->slot_contactItem_clicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ContactList::*)()>(_a, &ContactList::signal_load_contacts, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ContactList::*)()>(_a, &ContactList::signal_switchTo_contactInfoPage, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ContactList::*)()>(_a, &ContactList::signal_switchTo_friendRequestPage, 2))
            return;
    }
}

const QMetaObject *ContactList::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContactList::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11ContactListE_t>.strings))
        return static_cast<void*>(this);
    return QListWidget::qt_metacast(_clname);
}

int ContactList::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QListWidget::qt_metacall(_c, _id, _a);
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
void ContactList::signal_load_contacts()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ContactList::signal_switchTo_contactInfoPage()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ContactList::signal_switchTo_friendRequestPage()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
