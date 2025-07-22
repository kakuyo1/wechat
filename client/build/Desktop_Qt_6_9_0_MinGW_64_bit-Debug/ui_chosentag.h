/********************************************************************************
** Form generated from reading UI file 'chosentag.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOSENTAG_H
#define UI_CHOSENTAG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <customized_label.h>

QT_BEGIN_NAMESPACE

class Ui_ChosenTag
{
public:
    QHBoxLayout *horizontalLayout_2;
    QLabel *text_label;
    QWidget *remove_widget;
    QHBoxLayout *horizontalLayout;
    Customized_Label *remove_label;

    void setupUi(QWidget *ChosenTag)
    {
        if (ChosenTag->objectName().isEmpty())
            ChosenTag->setObjectName("ChosenTag");
        ChosenTag->resize(493, 43);
        ChosenTag->setMinimumSize(QSize(0, 43));
        ChosenTag->setMaximumSize(QSize(16777215, 43));
        ChosenTag->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout_2 = new QHBoxLayout(ChosenTag);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(5, 0, 0, 0);
        text_label = new QLabel(ChosenTag);
        text_label->setObjectName("text_label");
        text_label->setMinimumSize(QSize(0, 20));
        text_label->setMaximumSize(QSize(16777215, 20));

        horizontalLayout_2->addWidget(text_label);

        remove_widget = new QWidget(ChosenTag);
        remove_widget->setObjectName("remove_widget");
        remove_widget->setMinimumSize(QSize(25, 25));
        remove_widget->setMaximumSize(QSize(25, 25));
        horizontalLayout = new QHBoxLayout(remove_widget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        remove_label = new Customized_Label(remove_widget);
        remove_label->setObjectName("remove_label");
        remove_label->setMinimumSize(QSize(15, 15));
        remove_label->setMaximumSize(QSize(15, 15));
        remove_label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(remove_label);


        horizontalLayout_2->addWidget(remove_widget, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);


        retranslateUi(ChosenTag);

        QMetaObject::connectSlotsByName(ChosenTag);
    } // setupUi

    void retranslateUi(QWidget *ChosenTag)
    {
        ChosenTag->setWindowTitle(QCoreApplication::translate("ChosenTag", "Form", nullptr));
        text_label->setText(QCoreApplication::translate("ChosenTag", "TextLabel", nullptr));
        remove_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ChosenTag: public Ui_ChosenTag {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOSENTAG_H
