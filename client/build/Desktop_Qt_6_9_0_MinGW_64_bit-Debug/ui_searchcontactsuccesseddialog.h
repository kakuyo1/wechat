/********************************************************************************
** Form generated from reading UI file 'searchcontactsuccesseddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHCONTACTSUCCESSEDDIALOG_H
#define UI_SEARCHCONTACTSUCCESSEDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customized_button.h>

QT_BEGIN_NAMESPACE

class Ui_searchContactSuccessedDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *contactInfoContainer;
    QHBoxLayout *horizontalLayout;
    QLabel *icon_label;
    QSpacerItem *horizontalSpacer_2;
    QLabel *name_label;
    QSpacerItem *horizontalSpacer;
    QWidget *buttons_container;
    QHBoxLayout *horizontalLayout_2;
    Customized_button *add_contact_btn;
    Customized_button *cancel_btn;

    void setupUi(QDialog *searchContactSuccessedDialog)
    {
        if (searchContactSuccessedDialog->objectName().isEmpty())
            searchContactSuccessedDialog->setObjectName("searchContactSuccessedDialog");
        searchContactSuccessedDialog->resize(280, 190);
        searchContactSuccessedDialog->setMinimumSize(QSize(280, 190));
        searchContactSuccessedDialog->setMaximumSize(QSize(280, 190));
        verticalLayout = new QVBoxLayout(searchContactSuccessedDialog);
        verticalLayout->setObjectName("verticalLayout");
        contactInfoContainer = new QWidget(searchContactSuccessedDialog);
        contactInfoContainer->setObjectName("contactInfoContainer");
        horizontalLayout = new QHBoxLayout(contactInfoContainer);
        horizontalLayout->setObjectName("horizontalLayout");
        icon_label = new QLabel(contactInfoContainer);
        icon_label->setObjectName("icon_label");
        icon_label->setMinimumSize(QSize(50, 50));
        icon_label->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(icon_label);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        name_label = new QLabel(contactInfoContainer);
        name_label->setObjectName("name_label");

        horizontalLayout->addWidget(name_label);

        horizontalSpacer = new QSpacerItem(137, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(contactInfoContainer);

        buttons_container = new QWidget(searchContactSuccessedDialog);
        buttons_container->setObjectName("buttons_container");
        horizontalLayout_2 = new QHBoxLayout(buttons_container);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        add_contact_btn = new Customized_button(buttons_container);
        add_contact_btn->setObjectName("add_contact_btn");
        add_contact_btn->setMinimumSize(QSize(110, 35));
        add_contact_btn->setMaximumSize(QSize(100, 35));

        horizontalLayout_2->addWidget(add_contact_btn);

        cancel_btn = new Customized_button(buttons_container);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(110, 35));
        cancel_btn->setMaximumSize(QSize(110, 35));

        horizontalLayout_2->addWidget(cancel_btn);


        verticalLayout->addWidget(buttons_container);


        retranslateUi(searchContactSuccessedDialog);

        QMetaObject::connectSlotsByName(searchContactSuccessedDialog);
    } // setupUi

    void retranslateUi(QDialog *searchContactSuccessedDialog)
    {
        searchContactSuccessedDialog->setWindowTitle(QCoreApplication::translate("searchContactSuccessedDialog", "Dialog", nullptr));
        icon_label->setText(QString());
        name_label->setText(QCoreApplication::translate("searchContactSuccessedDialog", "\345\217\266\350\220\214", nullptr));
        add_contact_btn->setText(QCoreApplication::translate("searchContactSuccessedDialog", "\346\267\273\345\212\240\345\210\260\351\200\232\350\256\257\345\275\225", nullptr));
        cancel_btn->setText(QCoreApplication::translate("searchContactSuccessedDialog", "\345\217\226\346\266\210\346\223\215\344\275\234", nullptr));
    } // retranslateUi

};

namespace Ui {
    class searchContactSuccessedDialog: public Ui_searchContactSuccessedDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHCONTACTSUCCESSEDDIALOG_H
