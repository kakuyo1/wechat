/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <chatpage.h>
#include <customized_button.h>
#include <customized_editline.h>
#include <searchlist.h>
#include <sessionlist.h>
#include <statewidget.h>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *side_bar;
    QVBoxLayout *verticalLayout;
    QLabel *side_icon_label;
    QSpacerItem *verticalSpacer_2;
    StateWidget *side_chat_label;
    StateWidget *side_contact_label;
    QSpacerItem *verticalSpacer;
    QWidget *content_panal;
    QVBoxLayout *verticalLayout_2;
    QWidget *search_area;
    QHBoxLayout *horizontalLayout_2;
    Customized_EditLine *search_lineEdit;
    Customized_button *add_btn;
    SessionList *chat_list;
    QListWidget *contact_list;
    SearchList *search_list;
    QStackedWidget *stackedWidget;
    ChatPage *chat_page;
    QWidget *friend_apply_page;
    QWidget *contact_info_page;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName("ChatDialog");
        ChatDialog->resize(800, 600);
        ChatDialog->setMinimumSize(QSize(800, 600));
        horizontalLayout = new QHBoxLayout(ChatDialog);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        side_bar = new QWidget(ChatDialog);
        side_bar->setObjectName("side_bar");
        side_bar->setMinimumSize(QSize(56, 0));
        side_bar->setMaximumSize(QSize(56, 16777215));
        verticalLayout = new QVBoxLayout(side_bar);
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 40, 0, 0);
        side_icon_label = new QLabel(side_bar);
        side_icon_label->setObjectName("side_icon_label");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(30);
        sizePolicy.setVerticalStretch(30);
        sizePolicy.setHeightForWidth(side_icon_label->sizePolicy().hasHeightForWidth());
        side_icon_label->setSizePolicy(sizePolicy);
        side_icon_label->setMinimumSize(QSize(35, 35));
        side_icon_label->setMaximumSize(QSize(35, 35));

        verticalLayout->addWidget(side_icon_label, 0, Qt::AlignmentFlag::AlignHCenter);

        verticalSpacer_2 = new QSpacerItem(20, 30, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout->addItem(verticalSpacer_2);

        side_chat_label = new StateWidget(side_bar);
        side_chat_label->setObjectName("side_chat_label");
        side_chat_label->setMinimumSize(QSize(35, 35));
        side_chat_label->setMaximumSize(QSize(35, 35));

        verticalLayout->addWidget(side_chat_label, 0, Qt::AlignmentFlag::AlignHCenter);

        side_contact_label = new StateWidget(side_bar);
        side_contact_label->setObjectName("side_contact_label");
        side_contact_label->setMinimumSize(QSize(35, 35));
        side_contact_label->setMaximumSize(QSize(35, 35));

        verticalLayout->addWidget(side_contact_label, 0, Qt::AlignmentFlag::AlignHCenter);

        verticalSpacer = new QSpacerItem(20, 432, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(side_bar);

        content_panal = new QWidget(ChatDialog);
        content_panal->setObjectName("content_panal");
        content_panal->setMinimumSize(QSize(250, 0));
        content_panal->setMaximumSize(QSize(250, 16777215));
        verticalLayout_2 = new QVBoxLayout(content_panal);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 15, 0, 0);
        search_area = new QWidget(content_panal);
        search_area->setObjectName("search_area");
        horizontalLayout_2 = new QHBoxLayout(search_area);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(5, 0, 0, 10);
        search_lineEdit = new Customized_EditLine(search_area);
        search_lineEdit->setObjectName("search_lineEdit");
        search_lineEdit->setMinimumSize(QSize(0, 24));
        search_lineEdit->setMaximumSize(QSize(16777215, 24));

        horizontalLayout_2->addWidget(search_lineEdit);

        add_btn = new Customized_button(search_area);
        add_btn->setObjectName("add_btn");
        add_btn->setMinimumSize(QSize(24, 24));
        add_btn->setMaximumSize(QSize(24, 24));

        horizontalLayout_2->addWidget(add_btn);


        verticalLayout_2->addWidget(search_area);

        chat_list = new SessionList(content_panal);
        chat_list->setObjectName("chat_list");
        chat_list->setMinimumSize(QSize(250, 0));
        chat_list->setMaximumSize(QSize(250, 16777215));
        chat_list->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(chat_list);

        contact_list = new QListWidget(content_panal);
        contact_list->setObjectName("contact_list");
        contact_list->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 255, 127);"));

        verticalLayout_2->addWidget(contact_list);

        search_list = new SearchList(content_panal);
        search_list->setObjectName("search_list");
        search_list->setStyleSheet(QString::fromUtf8(""));

        verticalLayout_2->addWidget(search_list);


        horizontalLayout->addWidget(content_panal);

        stackedWidget = new QStackedWidget(ChatDialog);
        stackedWidget->setObjectName("stackedWidget");
        chat_page = new ChatPage();
        chat_page->setObjectName("chat_page");
        stackedWidget->addWidget(chat_page);
        friend_apply_page = new QWidget();
        friend_apply_page->setObjectName("friend_apply_page");
        stackedWidget->addWidget(friend_apply_page);
        contact_info_page = new QWidget();
        contact_info_page->setObjectName("contact_info_page");
        stackedWidget->addWidget(contact_info_page);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QCoreApplication::translate("ChatDialog", "Dialog", nullptr));
        side_icon_label->setText(QString());
        add_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
