/********************************************************************************
** Form generated from reading UI file 'contactinfopage.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTINFOPAGE_H
#define UI_CONTACTINFOPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customized_button.h>

QT_BEGIN_NAMESPACE

class Ui_ContactInfoPage
{
public:
    QVBoxLayout *verticalLayout_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_5;
    QLabel *icon_label;
    QSpacerItem *horizontalSpacer_7;
    QWidget *widget_6;
    QVBoxLayout *verticalLayout;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *nameTip_label;
    QLabel *name_label;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *backupTip_label;
    QLabel *backupName_label;
    QWidget *widget_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *genderTip_label;
    QLabel *gender_label;
    QSpacerItem *horizontalSpacer_6;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    Customized_button *chat_btn;
    QSpacerItem *horizontalSpacer_2;
    Customized_button *audio_btn;
    QSpacerItem *horizontalSpacer_3;
    Customized_button *video_btn;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ContactInfoPage)
    {
        if (ContactInfoPage->objectName().isEmpty())
            ContactInfoPage->setObjectName("ContactInfoPage");
        ContactInfoPage->resize(789, 511);
        verticalLayout_2 = new QVBoxLayout(ContactInfoPage);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(120, 120, 120, 120);
        widget = new QWidget(ContactInfoPage);
        widget->setObjectName("widget");
        horizontalLayout_5 = new QHBoxLayout(widget);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        icon_label = new QLabel(widget);
        icon_label->setObjectName("icon_label");
        icon_label->setMinimumSize(QSize(130, 130));
        icon_label->setMaximumSize(QSize(130, 130));

        horizontalLayout_5->addWidget(icon_label);

        horizontalSpacer_7 = new QSpacerItem(20, 20, QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        widget_6 = new QWidget(widget);
        widget_6->setObjectName("widget_6");
        verticalLayout = new QVBoxLayout(widget_6);
        verticalLayout->setObjectName("verticalLayout");
        widget_3 = new QWidget(widget_6);
        widget_3->setObjectName("widget_3");
        horizontalLayout_4 = new QHBoxLayout(widget_3);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        nameTip_label = new QLabel(widget_3);
        nameTip_label->setObjectName("nameTip_label");
        nameTip_label->setMinimumSize(QSize(60, 40));
        nameTip_label->setMaximumSize(QSize(60, 40));

        horizontalLayout_4->addWidget(nameTip_label);

        name_label = new QLabel(widget_3);
        name_label->setObjectName("name_label");
        name_label->setMinimumSize(QSize(0, 40));
        name_label->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_4->addWidget(name_label);


        verticalLayout->addWidget(widget_3);

        widget_4 = new QWidget(widget_6);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        backupTip_label = new QLabel(widget_4);
        backupTip_label->setObjectName("backupTip_label");
        backupTip_label->setMinimumSize(QSize(60, 40));
        backupTip_label->setMaximumSize(QSize(60, 40));

        horizontalLayout_3->addWidget(backupTip_label);

        backupName_label = new QLabel(widget_4);
        backupName_label->setObjectName("backupName_label");
        backupName_label->setMinimumSize(QSize(0, 40));
        backupName_label->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_3->addWidget(backupName_label);


        verticalLayout->addWidget(widget_4);

        widget_5 = new QWidget(widget_6);
        widget_5->setObjectName("widget_5");
        horizontalLayout_2 = new QHBoxLayout(widget_5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        genderTip_label = new QLabel(widget_5);
        genderTip_label->setObjectName("genderTip_label");
        genderTip_label->setMinimumSize(QSize(60, 40));
        genderTip_label->setMaximumSize(QSize(60, 40));

        horizontalLayout_2->addWidget(genderTip_label);

        gender_label = new QLabel(widget_5);
        gender_label->setObjectName("gender_label");
        gender_label->setMinimumSize(QSize(0, 40));
        gender_label->setMaximumSize(QSize(16777215, 40));

        horizontalLayout_2->addWidget(gender_label);


        verticalLayout->addWidget(widget_5);


        horizontalLayout_5->addWidget(widget_6);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);


        verticalLayout_2->addWidget(widget);

        widget_2 = new QWidget(ContactInfoPage);
        widget_2->setObjectName("widget_2");
        horizontalLayout = new QHBoxLayout(widget_2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(81, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        chat_btn = new Customized_button(widget_2);
        chat_btn->setObjectName("chat_btn");
        chat_btn->setMinimumSize(QSize(50, 50));
        chat_btn->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(chat_btn);

        horizontalSpacer_2 = new QSpacerItem(81, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        audio_btn = new Customized_button(widget_2);
        audio_btn->setObjectName("audio_btn");
        audio_btn->setMinimumSize(QSize(50, 50));
        audio_btn->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(audio_btn);

        horizontalSpacer_3 = new QSpacerItem(80, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        video_btn = new Customized_button(widget_2);
        video_btn->setObjectName("video_btn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(50);
        sizePolicy.setVerticalStretch(50);
        sizePolicy.setHeightForWidth(video_btn->sizePolicy().hasHeightForWidth());
        video_btn->setSizePolicy(sizePolicy);
        video_btn->setMinimumSize(QSize(50, 50));
        video_btn->setMaximumSize(QSize(50, 50));

        horizontalLayout->addWidget(video_btn);

        horizontalSpacer_4 = new QSpacerItem(81, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout_2->addWidget(widget_2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(ContactInfoPage);

        QMetaObject::connectSlotsByName(ContactInfoPage);
    } // setupUi

    void retranslateUi(QWidget *ContactInfoPage)
    {
        ContactInfoPage->setWindowTitle(QCoreApplication::translate("ContactInfoPage", "Form", nullptr));
        icon_label->setText(QString());
        nameTip_label->setText(QCoreApplication::translate("ContactInfoPage", "\346\230\265   \347\247\260\357\274\232", nullptr));
        name_label->setText(QCoreApplication::translate("ContactInfoPage", "\345\215\240\344\275\215\347\254\246", nullptr));
        backupTip_label->setText(QCoreApplication::translate("ContactInfoPage", "\345\244\207\346\263\250\345\220\215\357\274\232", nullptr));
        backupName_label->setText(QCoreApplication::translate("ContactInfoPage", "\345\215\240\344\275\215\347\254\246", nullptr));
        genderTip_label->setText(QCoreApplication::translate("ContactInfoPage", "\346\200\247   \345\210\253\357\274\232", nullptr));
        gender_label->setText(QCoreApplication::translate("ContactInfoPage", "\345\215\240\344\275\215\347\254\246", nullptr));
        chat_btn->setText(QString());
        audio_btn->setText(QString());
        video_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ContactInfoPage: public Ui_ContactInfoPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTINFOPAGE_H
