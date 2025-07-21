/********************************************************************************
** Form generated from reading UI file 'sessionlistitem.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SESSIONLISTITEM_H
#define UI_SESSIONLISTITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SessionListItem
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *icon_widget;
    QLabel *icon_label;
    QLabel *redpoint_label;
    QWidget *user_info_label;
    QVBoxLayout *verticalLayout;
    QLabel *user_name_label;
    QLabel *user_chatinfo_label;
    QWidget *time_widget;
    QLabel *time_label;

    void setupUi(QWidget *SessionListItem)
    {
        if (SessionListItem->objectName().isEmpty())
            SessionListItem->setObjectName("SessionListItem");
        SessionListItem->resize(570, 75);
        SessionListItem->setMinimumSize(QSize(0, 75));
        SessionListItem->setMaximumSize(QSize(16777215, 75));
        horizontalLayout = new QHBoxLayout(SessionListItem);
        horizontalLayout->setSpacing(8);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(6, 2, 6, 2);
        icon_widget = new QWidget(SessionListItem);
        icon_widget->setObjectName("icon_widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(60);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(icon_widget->sizePolicy().hasHeightForWidth());
        icon_widget->setSizePolicy(sizePolicy);
        icon_widget->setMinimumSize(QSize(60, 60));
        icon_widget->setMaximumSize(QSize(60, 60));
        icon_label = new QLabel(icon_widget);
        icon_label->setObjectName("icon_label");
        icon_label->setGeometry(QRect(10, 10, 45, 45));
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(45);
        sizePolicy1.setVerticalStretch(45);
        sizePolicy1.setHeightForWidth(icon_label->sizePolicy().hasHeightForWidth());
        icon_label->setSizePolicy(sizePolicy1);
        redpoint_label = new QLabel(icon_widget);
        redpoint_label->setObjectName("redpoint_label");
        redpoint_label->setGeometry(QRect(30, 0, 30, 30));
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(30);
        sizePolicy2.setVerticalStretch(30);
        sizePolicy2.setHeightForWidth(redpoint_label->sizePolicy().hasHeightForWidth());
        redpoint_label->setSizePolicy(sizePolicy2);
        redpoint_label->setMinimumSize(QSize(30, 30));
        redpoint_label->setMaximumSize(QSize(30, 30));

        horizontalLayout->addWidget(icon_widget);

        user_info_label = new QWidget(SessionListItem);
        user_info_label->setObjectName("user_info_label");
        user_info_label->setMinimumSize(QSize(0, 0));
        verticalLayout = new QVBoxLayout(user_info_label);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(2, 5, 2, 5);
        user_name_label = new QLabel(user_info_label);
        user_name_label->setObjectName("user_name_label");
        user_name_label->setMinimumSize(QSize(0, 30));
        user_name_label->setMaximumSize(QSize(16777215, 30));

        verticalLayout->addWidget(user_name_label);

        user_chatinfo_label = new QLabel(user_info_label);
        user_chatinfo_label->setObjectName("user_chatinfo_label");
        user_chatinfo_label->setMinimumSize(QSize(0, 30));
        user_chatinfo_label->setMaximumSize(QSize(16777215, 30));

        verticalLayout->addWidget(user_chatinfo_label);


        horizontalLayout->addWidget(user_info_label);

        time_widget = new QWidget(SessionListItem);
        time_widget->setObjectName("time_widget");
        time_widget->setMinimumSize(QSize(50, 50));
        time_widget->setMaximumSize(QSize(50, 50));
        time_label = new QLabel(time_widget);
        time_label->setObjectName("time_label");
        time_label->setGeometry(QRect(0, 10, 70, 20));

        horizontalLayout->addWidget(time_widget);


        retranslateUi(SessionListItem);

        QMetaObject::connectSlotsByName(SessionListItem);
    } // setupUi

    void retranslateUi(QWidget *SessionListItem)
    {
        SessionListItem->setWindowTitle(QCoreApplication::translate("SessionListItem", "Dialog", nullptr));
        icon_label->setText(QString());
        redpoint_label->setText(QString());
        user_name_label->setText(QCoreApplication::translate("SessionListItem", "llfc", nullptr));
        user_chatinfo_label->setText(QCoreApplication::translate("SessionListItem", "hello world!", nullptr));
        time_label->setText(QCoreApplication::translate("SessionListItem", "13:54", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SessionListItem: public Ui_SessionListItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SESSIONLISTITEM_H
