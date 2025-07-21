/********************************************************************************
** Form generated from reading UI file 'resetpassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESETPASSWORDDIALOG_H
#define UI_RESETPASSWORDDIALOG_H

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

class Ui_ResetPasswordDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QWidget *errorTip_container;
    QVBoxLayout *verticalLayout;
    QLabel *error_tip;
    QHBoxLayout *horizontalLayout;
    QLabel *user_label;
    QLineEdit *user_lineEdit;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *email_label;
    QLineEdit *email_lineEdit;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_3;
    QLabel *verifyCode_label;
    QLineEdit *verifyCode_lineEdit;
    QPushButton *getVerifyCode_btn;
    QSpacerItem *verticalSpacer_6;
    QHBoxLayout *horizontalLayout_4;
    QLabel *newPassword_label;
    QLineEdit *newPassword_lineEdit;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer;
    QPushButton *confirm_btn;
    QPushButton *return_btn;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *ResetPasswordDialog)
    {
        if (ResetPasswordDialog->objectName().isEmpty())
            ResetPasswordDialog->setObjectName("ResetPasswordDialog");
        ResetPasswordDialog->resize(300, 500);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ResetPasswordDialog->sizePolicy().hasHeightForWidth());
        ResetPasswordDialog->setSizePolicy(sizePolicy);
        ResetPasswordDialog->setMinimumSize(QSize(300, 500));
        ResetPasswordDialog->setMaximumSize(QSize(300, 500));
        verticalLayout_3 = new QVBoxLayout(ResetPasswordDialog);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        errorTip_container = new QWidget(ResetPasswordDialog);
        errorTip_container->setObjectName("errorTip_container");
        verticalLayout = new QVBoxLayout(errorTip_container);
        verticalLayout->setObjectName("verticalLayout");
        error_tip = new QLabel(errorTip_container);
        error_tip->setObjectName("error_tip");

        verticalLayout->addWidget(error_tip, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);


        verticalLayout_2->addWidget(errorTip_container);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        user_label = new QLabel(ResetPasswordDialog);
        user_label->setObjectName("user_label");
        user_label->setMinimumSize(QSize(60, 30));
        user_label->setMaximumSize(QSize(60, 30));

        horizontalLayout->addWidget(user_label);

        user_lineEdit = new QLineEdit(ResetPasswordDialog);
        user_lineEdit->setObjectName("user_lineEdit");

        horizontalLayout->addWidget(user_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_4);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        email_label = new QLabel(ResetPasswordDialog);
        email_label->setObjectName("email_label");
        email_label->setMinimumSize(QSize(60, 30));
        email_label->setMaximumSize(QSize(60, 30));

        horizontalLayout_2->addWidget(email_label);

        email_lineEdit = new QLineEdit(ResetPasswordDialog);
        email_lineEdit->setObjectName("email_lineEdit");

        horizontalLayout_2->addWidget(email_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        verifyCode_label = new QLabel(ResetPasswordDialog);
        verifyCode_label->setObjectName("verifyCode_label");
        verifyCode_label->setMinimumSize(QSize(60, 30));
        verifyCode_label->setMaximumSize(QSize(60, 30));

        horizontalLayout_3->addWidget(verifyCode_label);

        verifyCode_lineEdit = new QLineEdit(ResetPasswordDialog);
        verifyCode_lineEdit->setObjectName("verifyCode_lineEdit");

        horizontalLayout_3->addWidget(verifyCode_lineEdit);

        getVerifyCode_btn = new QPushButton(ResetPasswordDialog);
        getVerifyCode_btn->setObjectName("getVerifyCode_btn");

        horizontalLayout_3->addWidget(getVerifyCode_btn);


        verticalLayout_2->addLayout(horizontalLayout_3);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        newPassword_label = new QLabel(ResetPasswordDialog);
        newPassword_label->setObjectName("newPassword_label");
        newPassword_label->setMinimumSize(QSize(60, 30));
        newPassword_label->setMaximumSize(QSize(60, 30));

        horizontalLayout_4->addWidget(newPassword_label);

        newPassword_lineEdit = new QLineEdit(ResetPasswordDialog);
        newPassword_lineEdit->setObjectName("newPassword_lineEdit");

        horizontalLayout_4->addWidget(newPassword_lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);

        confirm_btn = new QPushButton(ResetPasswordDialog);
        confirm_btn->setObjectName("confirm_btn");
        confirm_btn->setMinimumSize(QSize(100, 30));
        confirm_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_5->addWidget(confirm_btn);

        return_btn = new QPushButton(ResetPasswordDialog);
        return_btn->setObjectName("return_btn");
        return_btn->setMinimumSize(QSize(100, 30));
        return_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout_5->addWidget(return_btn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout_3->addLayout(verticalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        verticalLayout_3->setStretch(0, 2);
        verticalLayout_3->setStretch(1, 10);
        verticalLayout_3->setStretch(2, 2);

        retranslateUi(ResetPasswordDialog);

        QMetaObject::connectSlotsByName(ResetPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *ResetPasswordDialog)
    {
        ResetPasswordDialog->setWindowTitle(QCoreApplication::translate("ResetPasswordDialog", "Dialog", nullptr));
        error_tip->setText(QString());
        user_label->setText(QCoreApplication::translate("ResetPasswordDialog", "\347\224\250\346\210\267\345\220\215\357\274\232", nullptr));
        email_label->setText(QCoreApplication::translate("ResetPasswordDialog", "\351\202\256\347\256\261\357\274\232", nullptr));
        verifyCode_label->setText(QCoreApplication::translate("ResetPasswordDialog", "\351\252\214\350\257\201\347\240\201\357\274\232", nullptr));
        getVerifyCode_btn->setText(QCoreApplication::translate("ResetPasswordDialog", "\350\216\267\345\217\226", nullptr));
        newPassword_label->setText(QCoreApplication::translate("ResetPasswordDialog", "\346\226\260\345\257\206\347\240\201\357\274\232", nullptr));
        confirm_btn->setText(QCoreApplication::translate("ResetPasswordDialog", "\347\241\256\350\256\244", nullptr));
        return_btn->setText(QCoreApplication::translate("ResetPasswordDialog", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResetPasswordDialog: public Ui_ResetPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESETPASSWORDDIALOG_H
