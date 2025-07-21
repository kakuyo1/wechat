/****************************************************************************
** Meta object code from reading C++ file 'resetpassworddialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../resetpassworddialog.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resetpassworddialog.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19ResetPasswordDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto ResetPasswordDialog::qt_create_metaobjectdata<qt_meta_tag_ZN19ResetPasswordDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ResetPasswordDialog",
        "signal_switchto_login",
        "",
        "checkUserValidation",
        "text",
        "checkEmailValidation",
        "checkNewPasswordValidation",
        "checkVerifyCodeValidation",
        "on_return_btn_clicked",
        "on_getVerifyCode_btn_clicked",
        "on_confirm_btn_clicked",
        "slot_reset_password_handle",
        "RequestType",
        "type",
        "response",
        "ErrorCode",
        "error"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'signal_switchto_login'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'checkUserValidation'
        QtMocHelpers::SlotData<bool(const QString &)>(3, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'checkEmailValidation'
        QtMocHelpers::SlotData<bool(const QString &)>(5, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'checkNewPasswordValidation'
        QtMocHelpers::SlotData<bool(const QString &)>(6, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'checkVerifyCodeValidation'
        QtMocHelpers::SlotData<bool(const QString &)>(7, 2, QMC::AccessPrivate, QMetaType::Bool, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'on_return_btn_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_getVerifyCode_btn_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_confirm_btn_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'slot_reset_password_handle'
        QtMocHelpers::SlotData<void(RequestType, QString, ErrorCode)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 12, 13 }, { QMetaType::QString, 14 }, { 0x80000000 | 15, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ResetPasswordDialog, qt_meta_tag_ZN19ResetPasswordDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ResetPasswordDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResetPasswordDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResetPasswordDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19ResetPasswordDialogE_t>.metaTypes,
    nullptr
} };

void ResetPasswordDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ResetPasswordDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->signal_switchto_login(); break;
        case 1: { bool _r = _t->checkUserValidation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->checkEmailValidation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->checkNewPasswordValidation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->checkVerifyCodeValidation((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: _t->on_return_btn_clicked(); break;
        case 6: _t->on_getVerifyCode_btn_clicked(); break;
        case 7: _t->on_confirm_btn_clicked(); break;
        case 8: _t->slot_reset_password_handle((*reinterpret_cast< std::add_pointer_t<RequestType>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<ErrorCode>>(_a[3]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ResetPasswordDialog::*)()>(_a, &ResetPasswordDialog::signal_switchto_login, 0))
            return;
    }
}

const QMetaObject *ResetPasswordDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResetPasswordDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19ResetPasswordDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ResetPasswordDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void ResetPasswordDialog::signal_switchto_login()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
