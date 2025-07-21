/********************************************************************************
** Form generated from reading UI file 'chatpage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATPAGE_H
#define UI_CHATPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <chatviewarea.h>
#include <customized_button.h>
#include <customized_label.h>
#include <customized_textedit.h>

QT_BEGIN_NAMESPACE

class Ui_ChatPage
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *title_widget;
    QVBoxLayout *verticalLayout;
    QLabel *title_label;
    ChatViewArea *chat_view;
    QWidget *tool_widget;
    QHBoxLayout *horizontalLayout;
    Customized_Label *emoji_label;
    Customized_Label *file_label;
    QSpacerItem *horizontalSpacer;
    Customized_TextEdit *chat_textEdit;
    QWidget *controlbtns_widget;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_2;
    Customized_button *receive_btn;
    Customized_button *send_btn;

    void setupUi(QDialog *ChatPage)
    {
        if (ChatPage->objectName().isEmpty())
            ChatPage->setObjectName("ChatPage");
        ChatPage->resize(560, 800);
        ChatPage->setMinimumSize(QSize(0, 0));
        verticalLayout_2 = new QVBoxLayout(ChatPage);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        title_widget = new QWidget(ChatPage);
        title_widget->setObjectName("title_widget");
        title_widget->setMinimumSize(QSize(0, 60));
        title_widget->setMaximumSize(QSize(16777215, 60));
        verticalLayout = new QVBoxLayout(title_widget);
        verticalLayout->setSpacing(7);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(-1, 2, 2, 8);
        title_label = new QLabel(title_widget);
        title_label->setObjectName("title_label");

        verticalLayout->addWidget(title_label);


        verticalLayout_2->addWidget(title_widget);

        chat_view = new ChatViewArea(ChatPage);
        chat_view->setObjectName("chat_view");

        verticalLayout_2->addWidget(chat_view);

        tool_widget = new QWidget(ChatPage);
        tool_widget->setObjectName("tool_widget");
        tool_widget->setMinimumSize(QSize(0, 40));
        tool_widget->setMaximumSize(QSize(16777215, 40));
        horizontalLayout = new QHBoxLayout(tool_widget);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(2, 2, 2, 2);
        emoji_label = new Customized_Label(tool_widget);
        emoji_label->setObjectName("emoji_label");
        emoji_label->setMinimumSize(QSize(25, 25));
        emoji_label->setMaximumSize(QSize(25, 25));

        horizontalLayout->addWidget(emoji_label);

        file_label = new Customized_Label(tool_widget);
        file_label->setObjectName("file_label");
        file_label->setMinimumSize(QSize(25, 25));
        file_label->setMaximumSize(QSize(25, 25));

        horizontalLayout->addWidget(file_label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_2->addWidget(tool_widget);

        chat_textEdit = new Customized_TextEdit(ChatPage);
        chat_textEdit->setObjectName("chat_textEdit");
        chat_textEdit->setMinimumSize(QSize(0, 150));
        chat_textEdit->setMaximumSize(QSize(16777215, 150));

        verticalLayout_2->addWidget(chat_textEdit);

        controlbtns_widget = new QWidget(ChatPage);
        controlbtns_widget->setObjectName("controlbtns_widget");
        controlbtns_widget->setMinimumSize(QSize(0, 40));
        controlbtns_widget->setMaximumSize(QSize(16777215, 40));
        horizontalLayout_2 = new QHBoxLayout(controlbtns_widget);
        horizontalLayout_2->setSpacing(15);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        receive_btn = new Customized_button(controlbtns_widget);
        receive_btn->setObjectName("receive_btn");
        receive_btn->setMinimumSize(QSize(80, 30));
        receive_btn->setMaximumSize(QSize(80, 30));

        horizontalLayout_2->addWidget(receive_btn);

        send_btn = new Customized_button(controlbtns_widget);
        send_btn->setObjectName("send_btn");
        send_btn->setMinimumSize(QSize(80, 30));
        send_btn->setMaximumSize(QSize(80, 30));

        horizontalLayout_2->addWidget(send_btn);


        verticalLayout_2->addWidget(controlbtns_widget);


        retranslateUi(ChatPage);

        QMetaObject::connectSlotsByName(ChatPage);
    } // setupUi

    void retranslateUi(QDialog *ChatPage)
    {
        ChatPage->setWindowTitle(QCoreApplication::translate("ChatPage", "Dialog", nullptr));
        title_label->setText(QCoreApplication::translate("ChatPage", "\347\207\203\347\203\247\347\232\204\350\203\270\346\257\233", nullptr));
        emoji_label->setText(QString());
        file_label->setText(QString());
        receive_btn->setText(QCoreApplication::translate("ChatPage", "\346\216\245\346\224\266", nullptr));
        send_btn->setText(QCoreApplication::translate("ChatPage", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatPage: public Ui_ChatPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATPAGE_H
