/********************************************************************************
** Form generated from reading UI file 'contactgrouptipitem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTGROUPTIPITEM_H
#define UI_CONTACTGROUPTIPITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactGroupTipItem
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *contactGroupTip_label;

    void setupUi(QWidget *ContactGroupTipItem)
    {
        if (ContactGroupTipItem->objectName().isEmpty())
            ContactGroupTipItem->setObjectName("ContactGroupTipItem");
        ContactGroupTipItem->resize(250, 42);
        ContactGroupTipItem->setMinimumSize(QSize(250, 25));
        ContactGroupTipItem->setMaximumSize(QSize(250, 42));
        verticalLayout = new QVBoxLayout(ContactGroupTipItem);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(20, 0, 0, 0);
        contactGroupTip_label = new QLabel(ContactGroupTipItem);
        contactGroupTip_label->setObjectName("contactGroupTip_label");
        contactGroupTip_label->setMinimumSize(QSize(0, 16));
        contactGroupTip_label->setMaximumSize(QSize(16777215, 16));

        verticalLayout->addWidget(contactGroupTip_label);


        retranslateUi(ContactGroupTipItem);

        QMetaObject::connectSlotsByName(ContactGroupTipItem);
    } // setupUi

    void retranslateUi(QWidget *ContactGroupTipItem)
    {
        ContactGroupTipItem->setWindowTitle(QCoreApplication::translate("ContactGroupTipItem", "Form", nullptr));
        contactGroupTip_label->setText(QCoreApplication::translate("ContactGroupTipItem", "\346\226\260\347\232\204\346\234\213\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContactGroupTipItem: public Ui_ContactGroupTipItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTGROUPTIPITEM_H
