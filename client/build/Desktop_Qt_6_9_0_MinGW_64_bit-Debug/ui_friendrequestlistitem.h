/********************************************************************************
** Form generated from reading UI file 'friendrequestlistitem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDREQUESTLISTITEM_H
#define UI_FRIENDREQUESTLISTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customized_button.h>

QT_BEGIN_NAMESPACE

class Ui_FriendRequestListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *FR_icon_label;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;
    QLabel *friend_name_label;
    QLabel *friend_message_label;
    Customized_button *FR_add_btn;
    QLabel *already_add_label;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *FriendRequestListItem)
    {
        if (FriendRequestListItem->objectName().isEmpty())
            FriendRequestListItem->setObjectName("FriendRequestListItem");
        FriendRequestListItem->resize(457, 80);
        FriendRequestListItem->setMinimumSize(QSize(0, 80));
        FriendRequestListItem->setMaximumSize(QSize(16777215, 80));
        horizontalLayout = new QHBoxLayout(FriendRequestListItem);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(9, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        FR_icon_label = new QLabel(FriendRequestListItem);
        FR_icon_label->setObjectName("FR_icon_label");
        FR_icon_label->setMinimumSize(QSize(45, 45));
        FR_icon_label->setMaximumSize(QSize(45, 45));

        horizontalLayout->addWidget(FR_icon_label);

        widget = new QWidget(FriendRequestListItem);
        widget->setObjectName("widget");
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(-1, 2, -1, 2);
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        verticalLayout = new QVBoxLayout(widget_2);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, 2, -1, 2);
        friend_name_label = new QLabel(widget_2);
        friend_name_label->setObjectName("friend_name_label");
        friend_name_label->setMinimumSize(QSize(100, 30));
        friend_name_label->setMaximumSize(QSize(200, 30));

        verticalLayout->addWidget(friend_name_label);

        friend_message_label = new QLabel(widget_2);
        friend_message_label->setObjectName("friend_message_label");
        friend_message_label->setMinimumSize(QSize(100, 30));
        friend_message_label->setMaximumSize(QSize(500, 30));

        verticalLayout->addWidget(friend_message_label);


        horizontalLayout_2->addWidget(widget_2);

        FR_add_btn = new Customized_button(widget);
        FR_add_btn->setObjectName("FR_add_btn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(80);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(FR_add_btn->sizePolicy().hasHeightForWidth());
        FR_add_btn->setSizePolicy(sizePolicy);
        FR_add_btn->setMinimumSize(QSize(80, 30));
        FR_add_btn->setMaximumSize(QSize(80, 30));

        horizontalLayout_2->addWidget(FR_add_btn);

        already_add_label = new QLabel(widget);
        already_add_label->setObjectName("already_add_label");
        already_add_label->setMinimumSize(QSize(40, 0));
        already_add_label->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(already_add_label);

        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        horizontalLayout->addWidget(widget);


        retranslateUi(FriendRequestListItem);

        QMetaObject::connectSlotsByName(FriendRequestListItem);
    } // setupUi

    void retranslateUi(QWidget *FriendRequestListItem)
    {
        FriendRequestListItem->setWindowTitle(QCoreApplication::translate("FriendRequestListItem", "Form", nullptr));
        FR_icon_label->setText(QString());
        friend_name_label->setText(QString());
        friend_message_label->setText(QString());
        FR_add_btn->setText(QCoreApplication::translate("FriendRequestListItem", "\346\267\273\345\212\240", nullptr));
        already_add_label->setText(QCoreApplication::translate("FriendRequestListItem", "\345\267\262\346\267\273\345\212\240", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendRequestListItem: public Ui_FriendRequestListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDREQUESTLISTITEM_H
