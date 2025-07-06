/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer;
    QWidget *errorTip_container;
    QVBoxLayout *verticalLayout;
    QLabel *error_tip;
    QHBoxLayout *horizontalLayout;
    QLabel *user_label;
    QLineEdit *user_lineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *email_label;
    QLineEdit *email_lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *password_label;
    QLineEdit *password_lineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *confirm_label;
    QLineEdit *confirm_lineEdit;
    QHBoxLayout *horizontalLayout_5;
    QLabel *verify_label;
    QLineEdit *verifyCode_lineEdit;
    QPushButton *getVerifyCode_btn;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *confirm_btn;
    QPushButton *cancel_btn;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName("RegisterDialog");
        RegisterDialog->resize(300, 500);
        RegisterDialog->setMinimumSize(QSize(300, 500));
        RegisterDialog->setMaximumSize(QSize(300, 500));
        horizontalLayout_7 = new QHBoxLayout(RegisterDialog);
        horizontalLayout_7->setObjectName("horizontalLayout_7");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        errorTip_container = new QWidget(RegisterDialog);
        errorTip_container->setObjectName("errorTip_container");
        verticalLayout = new QVBoxLayout(errorTip_container);
        verticalLayout->setObjectName("verticalLayout");
        error_tip = new QLabel(errorTip_container);
        error_tip->setObjectName("error_tip");
        error_tip->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(error_tip);


        verticalLayout_2->addWidget(errorTip_container);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        user_label = new QLabel(RegisterDialog);
        user_label->setObjectName("user_label");

        horizontalLayout->addWidget(user_label);

        user_lineEdit = new QLineEdit(RegisterDialog);
        user_lineEdit->setObjectName("user_lineEdit");

        horizontalLayout->addWidget(user_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(5, 5, 5, 5);
        email_label = new QLabel(RegisterDialog);
        email_label->setObjectName("email_label");

        horizontalLayout_2->addWidget(email_label);

        email_lineEdit = new QLineEdit(RegisterDialog);
        email_lineEdit->setObjectName("email_lineEdit");

        horizontalLayout_2->addWidget(email_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(5, 5, 5, 5);
        password_label = new QLabel(RegisterDialog);
        password_label->setObjectName("password_label");

        horizontalLayout_3->addWidget(password_label);

        password_lineEdit = new QLineEdit(RegisterDialog);
        password_lineEdit->setObjectName("password_lineEdit");

        horizontalLayout_3->addWidget(password_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(5, 5, 5, 5);
        confirm_label = new QLabel(RegisterDialog);
        confirm_label->setObjectName("confirm_label");

        horizontalLayout_4->addWidget(confirm_label);

        confirm_lineEdit = new QLineEdit(RegisterDialog);
        confirm_lineEdit->setObjectName("confirm_lineEdit");

        horizontalLayout_4->addWidget(confirm_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(5, 5, 5, 5);
        verify_label = new QLabel(RegisterDialog);
        verify_label->setObjectName("verify_label");

        horizontalLayout_5->addWidget(verify_label);

        verifyCode_lineEdit = new QLineEdit(RegisterDialog);
        verifyCode_lineEdit->setObjectName("verifyCode_lineEdit");

        horizontalLayout_5->addWidget(verifyCode_lineEdit);

        getVerifyCode_btn = new QPushButton(RegisterDialog);
        getVerifyCode_btn->setObjectName("getVerifyCode_btn");

        horizontalLayout_5->addWidget(getVerifyCode_btn);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        horizontalLayout_6->setContentsMargins(10, 5, 10, 5);
        confirm_btn = new QPushButton(RegisterDialog);
        confirm_btn->setObjectName("confirm_btn");
        confirm_btn->setMinimumSize(QSize(100, 0));
        confirm_btn->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(confirm_btn);

        cancel_btn = new QPushButton(RegisterDialog);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(100, 0));
        cancel_btn->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(cancel_btn);


        verticalLayout_2->addLayout(horizontalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);


        horizontalLayout_7->addLayout(verticalLayout_2);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        error_tip->setText(QCoreApplication::translate("RegisterDialog", "\351\224\231\350\257\257\346\217\220\347\244\272", nullptr));
        user_label->setText(QCoreApplication::translate("RegisterDialog", "\347\224\250\346\210\267\357\274\232", nullptr));
        email_label->setText(QCoreApplication::translate("RegisterDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        password_label->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        confirm_label->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\350\256\244\357\274\232", nullptr));
        verify_label->setText(QCoreApplication::translate("RegisterDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        getVerifyCode_btn->setText(QCoreApplication::translate("RegisterDialog", "\350\216\267\345\217\226", nullptr));
        confirm_btn->setText(QCoreApplication::translate("RegisterDialog", "\347\241\256\345\256\232", nullptr));
        cancel_btn->setText(QCoreApplication::translate("RegisterDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
