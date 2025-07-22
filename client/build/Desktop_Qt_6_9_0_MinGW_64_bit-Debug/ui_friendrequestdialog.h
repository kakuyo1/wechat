/********************************************************************************
** Form generated from reading UI file 'friendrequestdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDREQUESTDIALOG_H
#define UI_FRIENDREQUESTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <customized_button.h>
#include <customized_editline.h>
#include <taglabel.h>

QT_BEGIN_NAMESPACE

class Ui_FriendRequestDialog
{
public:
    QVBoxLayout *verticalLayout_5;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_6;
    QLabel *title_label;
    QWidget *request_widget;
    QVBoxLayout *verticalLayout_4;
    QFrame *line;
    QLabel *request_label;
    Customized_EditLine *request_lineEdit;
    QLabel *backup_label;
    Customized_EditLine *backup_lineEdit;
    QLabel *tag_label;
    QWidget *tags_chosen_group;
    QVBoxLayout *verticalLayout_2;
    QWidget *tags_grid_widget;
    Customized_EditLine *tags_lineEdit;
    QWidget *input_tip_widget;
    QVBoxLayout *verticalLayout_3;
    TagLabel *tip_label;
    QWidget *tags_display_widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *all_tags_displayed;
    QWidget *more_widget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    TagLabel *moretags_icon;
    QSpacerItem *verticalSpacer_2;
    QWidget *buttons_widget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    Customized_button *confirm_btn;
    QSpacerItem *horizontalSpacer;
    Customized_button *cancel_btn;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *FriendRequestDialog)
    {
        if (FriendRequestDialog->objectName().isEmpty())
            FriendRequestDialog->setObjectName("FriendRequestDialog");
        FriendRequestDialog->resize(360, 620);
        FriendRequestDialog->setMinimumSize(QSize(360, 620));
        FriendRequestDialog->setMaximumSize(QSize(360, 620));
        verticalLayout_5 = new QVBoxLayout(FriendRequestDialog);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(FriendRequestDialog);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setMinimumSize(QSize(0, 500));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 358, 498));
        verticalLayout_6 = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout_6->setSpacing(0);
        verticalLayout_6->setObjectName("verticalLayout_6");
        verticalLayout_6->setContentsMargins(0, 11, 0, 11);
        title_label = new QLabel(scrollAreaWidgetContents);
        title_label->setObjectName("title_label");
        title_label->setMinimumSize(QSize(0, 25));
        title_label->setMaximumSize(QSize(16777215, 25));
        title_label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_6->addWidget(title_label, 0, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        request_widget = new QWidget(scrollAreaWidgetContents);
        request_widget->setObjectName("request_widget");
        verticalLayout_4 = new QVBoxLayout(request_widget);
        verticalLayout_4->setSpacing(5);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(11, -1, -1, -1);
        line = new QFrame(request_widget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout_4->addWidget(line);

        request_label = new QLabel(request_widget);
        request_label->setObjectName("request_label");
        request_label->setMinimumSize(QSize(0, 25));
        request_label->setMaximumSize(QSize(16777215, 25));

        verticalLayout_4->addWidget(request_label);

        request_lineEdit = new Customized_EditLine(request_widget);
        request_lineEdit->setObjectName("request_lineEdit");
        request_lineEdit->setMinimumSize(QSize(0, 35));
        request_lineEdit->setMaximumSize(QSize(16777215, 35));

        verticalLayout_4->addWidget(request_lineEdit);

        backup_label = new QLabel(request_widget);
        backup_label->setObjectName("backup_label");
        backup_label->setMinimumSize(QSize(0, 25));
        backup_label->setMaximumSize(QSize(16777215, 25));

        verticalLayout_4->addWidget(backup_label);

        backup_lineEdit = new Customized_EditLine(request_widget);
        backup_lineEdit->setObjectName("backup_lineEdit");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(35);
        sizePolicy.setHeightForWidth(backup_lineEdit->sizePolicy().hasHeightForWidth());
        backup_lineEdit->setSizePolicy(sizePolicy);
        backup_lineEdit->setMinimumSize(QSize(0, 35));
        backup_lineEdit->setMaximumSize(QSize(16777215, 35));

        verticalLayout_4->addWidget(backup_lineEdit);

        tag_label = new QLabel(request_widget);
        tag_label->setObjectName("tag_label");
        tag_label->setMinimumSize(QSize(0, 25));
        tag_label->setMaximumSize(QSize(16777215, 25));

        verticalLayout_4->addWidget(tag_label);

        tags_chosen_group = new QWidget(request_widget);
        tags_chosen_group->setObjectName("tags_chosen_group");
        tags_chosen_group->setMinimumSize(QSize(0, 50));
        verticalLayout_2 = new QVBoxLayout(tags_chosen_group);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        tags_grid_widget = new QWidget(tags_chosen_group);
        tags_grid_widget->setObjectName("tags_grid_widget");
        tags_grid_widget->setMinimumSize(QSize(0, 40));
        tags_grid_widget->setMaximumSize(QSize(16777215, 40));
        tags_lineEdit = new Customized_EditLine(tags_grid_widget);
        tags_lineEdit->setObjectName("tags_lineEdit");
        tags_lineEdit->setGeometry(QRect(0, 0, 113, 35));
        tags_lineEdit->setMaximumSize(QSize(16777215, 35));

        verticalLayout_2->addWidget(tags_grid_widget);

        input_tip_widget = new QWidget(tags_chosen_group);
        input_tip_widget->setObjectName("input_tip_widget");
        input_tip_widget->setMinimumSize(QSize(0, 35));
        verticalLayout_3 = new QVBoxLayout(input_tip_widget);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        tip_label = new TagLabel(input_tip_widget);
        tip_label->setObjectName("tip_label");

        verticalLayout_3->addWidget(tip_label);


        verticalLayout_2->addWidget(input_tip_widget);


        verticalLayout_4->addWidget(tags_chosen_group);

        tags_display_widget = new QWidget(request_widget);
        tags_display_widget->setObjectName("tags_display_widget");
        tags_display_widget->setMinimumSize(QSize(0, 60));
        horizontalLayout_2 = new QHBoxLayout(tags_display_widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        all_tags_displayed = new QWidget(tags_display_widget);
        all_tags_displayed->setObjectName("all_tags_displayed");
        all_tags_displayed->setMinimumSize(QSize(287, 50));
        all_tags_displayed->setMaximumSize(QSize(287, 16777215));

        horizontalLayout_2->addWidget(all_tags_displayed, 0, Qt::AlignmentFlag::AlignTop);

        more_widget = new QWidget(tags_display_widget);
        more_widget->setObjectName("more_widget");
        more_widget->setMinimumSize(QSize(30, 0));
        verticalLayout = new QVBoxLayout(more_widget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(2, 2, 2, 2);
        verticalSpacer = new QSpacerItem(20, 54, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        moretags_icon = new TagLabel(more_widget);
        moretags_icon->setObjectName("moretags_icon");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(25);
        sizePolicy1.setVerticalStretch(25);
        sizePolicy1.setHeightForWidth(moretags_icon->sizePolicy().hasHeightForWidth());
        moretags_icon->setSizePolicy(sizePolicy1);
        moretags_icon->setMinimumSize(QSize(25, 25));
        moretags_icon->setMaximumSize(QSize(25, 25));

        verticalLayout->addWidget(moretags_icon);

        verticalSpacer_2 = new QSpacerItem(20, 54, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addWidget(more_widget);


        verticalLayout_4->addWidget(tags_display_widget);


        verticalLayout_6->addWidget(request_widget);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);

        buttons_widget = new QWidget(FriendRequestDialog);
        buttons_widget->setObjectName("buttons_widget");
        buttons_widget->setMinimumSize(QSize(0, 50));
        horizontalLayout = new QHBoxLayout(buttons_widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(-1, -1, -1, 11);
        horizontalSpacer_2 = new QSpacerItem(34, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        confirm_btn = new Customized_button(buttons_widget);
        confirm_btn->setObjectName("confirm_btn");
        confirm_btn->setMinimumSize(QSize(100, 30));
        confirm_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout->addWidget(confirm_btn);

        horizontalSpacer = new QSpacerItem(33, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancel_btn = new Customized_button(buttons_widget);
        cancel_btn->setObjectName("cancel_btn");
        cancel_btn->setMinimumSize(QSize(100, 30));
        cancel_btn->setMaximumSize(QSize(100, 30));

        horizontalLayout->addWidget(cancel_btn);

        horizontalSpacer_3 = new QSpacerItem(34, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout_5->addWidget(buttons_widget);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_3);


        retranslateUi(FriendRequestDialog);

        QMetaObject::connectSlotsByName(FriendRequestDialog);
    } // setupUi

    void retranslateUi(QDialog *FriendRequestDialog)
    {
        FriendRequestDialog->setWindowTitle(QCoreApplication::translate("FriendRequestDialog", "Dialog", nullptr));
        title_label->setText(QCoreApplication::translate("FriendRequestDialog", "\347\224\263\350\257\267\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        request_label->setText(QCoreApplication::translate("FriendRequestDialog", "\345\217\221\351\200\201\350\257\267\346\261\202", nullptr));
        backup_label->setText(QCoreApplication::translate("FriendRequestDialog", "\345\244\207\346\263\250", nullptr));
        tag_label->setText(QCoreApplication::translate("FriendRequestDialog", "\346\240\207\347\255\276", nullptr));
        tip_label->setText(QCoreApplication::translate("FriendRequestDialog", "TextLabel", nullptr));
        moretags_icon->setText(QString());
        confirm_btn->setText(QCoreApplication::translate("FriendRequestDialog", "\347\241\256\350\256\244", nullptr));
        cancel_btn->setText(QCoreApplication::translate("FriendRequestDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendRequestDialog: public Ui_FriendRequestDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDREQUESTDIALOG_H
