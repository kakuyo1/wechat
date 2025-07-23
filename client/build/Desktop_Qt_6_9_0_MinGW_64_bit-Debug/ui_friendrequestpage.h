/********************************************************************************
** Form generated from reading UI file 'friendrequestpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDREQUESTPAGE_H
#define UI_FRIENDREQUESTPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <friendrequestlist.h>

QT_BEGIN_NAMESPACE

class Ui_FriendRequestPage
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *FR_title_widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *FR_title_label;
    QWidget *FR_list_widget;
    QVBoxLayout *verticalLayout_2;
    FriendRequestList *FR_list;

    void setupUi(QWidget *FriendRequestPage)
    {
        if (FriendRequestPage->objectName().isEmpty())
            FriendRequestPage->setObjectName("FriendRequestPage");
        FriendRequestPage->resize(533, 421);
        verticalLayout = new QVBoxLayout(FriendRequestPage);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        FR_title_widget = new QWidget(FriendRequestPage);
        FR_title_widget->setObjectName("FR_title_widget");
        horizontalLayout = new QHBoxLayout(FR_title_widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(10, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        FR_title_label = new QLabel(FR_title_widget);
        FR_title_label->setObjectName("FR_title_label");
        FR_title_label->setMinimumSize(QSize(0, 40));
        FR_title_label->setMaximumSize(QSize(16777215, 40));

        horizontalLayout->addWidget(FR_title_label);


        verticalLayout->addWidget(FR_title_widget);

        FR_list_widget = new QWidget(FriendRequestPage);
        FR_list_widget->setObjectName("FR_list_widget");
        verticalLayout_2 = new QVBoxLayout(FR_list_widget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        FR_list = new FriendRequestList(FR_list_widget);
        FR_list->setObjectName("FR_list");

        verticalLayout_2->addWidget(FR_list);


        verticalLayout->addWidget(FR_list_widget);


        retranslateUi(FriendRequestPage);

        QMetaObject::connectSlotsByName(FriendRequestPage);
    } // setupUi

    void retranslateUi(QWidget *FriendRequestPage)
    {
        FriendRequestPage->setWindowTitle(QCoreApplication::translate("FriendRequestPage", "Form", nullptr));
        FR_title_label->setText(QCoreApplication::translate("FriendRequestPage", "\346\226\260\347\232\204\346\234\213\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendRequestPage: public Ui_FriendRequestPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDREQUESTPAGE_H
