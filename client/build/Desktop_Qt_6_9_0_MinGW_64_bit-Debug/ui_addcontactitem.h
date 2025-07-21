/********************************************************************************
** Form generated from reading UI file 'addcontactitem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDCONTACTITEM_H
#define UI_ADDCONTACTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddContactItem
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *addContact_icon;
    QLabel *tip_label;
    QLabel *rightToward_icon;

    void setupUi(QWidget *AddContactItem)
    {
        if (AddContactItem->objectName().isEmpty())
            AddContactItem->setObjectName("AddContactItem");
        AddContactItem->resize(250, 70);
        AddContactItem->setMinimumSize(QSize(250, 70));
        AddContactItem->setMaximumSize(QSize(254, 70));
        horizontalLayout = new QHBoxLayout(AddContactItem);
        horizontalLayout->setObjectName("horizontalLayout");
        addContact_icon = new QLabel(AddContactItem);
        addContact_icon->setObjectName("addContact_icon");
        addContact_icon->setMinimumSize(QSize(40, 40));
        addContact_icon->setMaximumSize(QSize(40, 40));

        horizontalLayout->addWidget(addContact_icon);

        tip_label = new QLabel(AddContactItem);
        tip_label->setObjectName("tip_label");

        horizontalLayout->addWidget(tip_label);

        rightToward_icon = new QLabel(AddContactItem);
        rightToward_icon->setObjectName("rightToward_icon");
        rightToward_icon->setMinimumSize(QSize(30, 30));
        rightToward_icon->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(rightToward_icon);


        retranslateUi(AddContactItem);

        QMetaObject::connectSlotsByName(AddContactItem);
    } // setupUi

    void retranslateUi(QWidget *AddContactItem)
    {
        AddContactItem->setWindowTitle(QCoreApplication::translate("AddContactItem", "Form", nullptr));
        addContact_icon->setText(QString());
        tip_label->setText(QCoreApplication::translate("AddContactItem", "\346\237\245\346\211\276 uid/name", nullptr));
        rightToward_icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AddContactItem: public Ui_AddContactItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDCONTACTITEM_H
