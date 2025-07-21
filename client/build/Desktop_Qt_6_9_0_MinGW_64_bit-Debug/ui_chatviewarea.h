/********************************************************************************
** Form generated from reading UI file 'chatviewarea.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATVIEWAREA_H
#define UI_CHATVIEWAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatViewArea
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *ChatViewArea)
    {
        if (ChatViewArea->objectName().isEmpty())
            ChatViewArea->setObjectName("ChatViewArea");
        ChatViewArea->resize(526, 308);
        verticalLayout = new QVBoxLayout(ChatViewArea);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(ChatViewArea);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 524, 306));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(ChatViewArea);

        QMetaObject::connectSlotsByName(ChatViewArea);
    } // setupUi

    void retranslateUi(QWidget *ChatViewArea)
    {
        ChatViewArea->setWindowTitle(QCoreApplication::translate("ChatViewArea", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatViewArea: public Ui_ChatViewArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATVIEWAREA_H
