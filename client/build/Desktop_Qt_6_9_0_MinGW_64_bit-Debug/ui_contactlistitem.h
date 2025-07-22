/********************************************************************************
** Form generated from reading UI file 'contactlistitem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTLISTITEM_H
#define UI_CONTACTLISTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *contact_icons_container;
    QLabel *contact_icon_label;
    QLabel *contact_redpoint_label;
    QLabel *contact_name_label;

    void setupUi(QWidget *ContactListItem)
    {
        if (ContactListItem->objectName().isEmpty())
            ContactListItem->setObjectName("ContactListItem");
        ContactListItem->resize(553, 70);
        ContactListItem->setMinimumSize(QSize(0, 70));
        ContactListItem->setMaximumSize(QSize(16777215, 70));
        horizontalLayout = new QHBoxLayout(ContactListItem);
        horizontalLayout->setSpacing(5);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        contact_icons_container = new QWidget(ContactListItem);
        contact_icons_container->setObjectName("contact_icons_container");
        contact_icons_container->setMinimumSize(QSize(60, 60));
        contact_icons_container->setMaximumSize(QSize(60, 60));
        contact_icon_label = new QLabel(contact_icons_container);
        contact_icon_label->setObjectName("contact_icon_label");
        contact_icon_label->setGeometry(QRect(10, 10, 45, 45));
        contact_icon_label->setMinimumSize(QSize(45, 45));
        contact_icon_label->setMaximumSize(QSize(45, 45));
        contact_redpoint_label = new QLabel(contact_icons_container);
        contact_redpoint_label->setObjectName("contact_redpoint_label");
        contact_redpoint_label->setGeometry(QRect(30, 0, 30, 30));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(30);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(contact_redpoint_label->sizePolicy().hasHeightForWidth());
        contact_redpoint_label->setSizePolicy(sizePolicy);
        contact_redpoint_label->setMinimumSize(QSize(30, 30));
        contact_redpoint_label->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(contact_icons_container);

        contact_name_label = new QLabel(ContactListItem);
        contact_name_label->setObjectName("contact_name_label");

        horizontalLayout->addWidget(contact_name_label);


        retranslateUi(ContactListItem);

        QMetaObject::connectSlotsByName(ContactListItem);
    } // setupUi

    void retranslateUi(QWidget *ContactListItem)
    {
        ContactListItem->setWindowTitle(QCoreApplication::translate("ContactListItem", "Form", nullptr));
        contact_icon_label->setText(QString());
        contact_redpoint_label->setText(QString());
        contact_name_label->setText(QCoreApplication::translate("ContactListItem", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContactListItem: public Ui_ContactListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTLISTITEM_H
