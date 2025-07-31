/********************************************************************************
** Form generated from reading UI file 'searchcontactfaileddialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHCONTACTFAILEDDIALOG_H
#define UI_SEARCHCONTACTFAILEDDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_searchcontactfaileddialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *searchFailed_TipLabel;
    QPushButton *searchFailed_ReturnBtn;

    void setupUi(QDialog *searchcontactfaileddialog)
    {
        if (searchcontactfaileddialog->objectName().isEmpty())
            searchcontactfaileddialog->setObjectName("searchcontactfaileddialog");
        searchcontactfaileddialog->resize(280, 190);
        searchcontactfaileddialog->setMinimumSize(QSize(280, 190));
        searchcontactfaileddialog->setMaximumSize(QSize(280, 190));
        verticalLayout = new QVBoxLayout(searchcontactfaileddialog);
        verticalLayout->setObjectName("verticalLayout");
        searchFailed_TipLabel = new QLabel(searchcontactfaileddialog);
        searchFailed_TipLabel->setObjectName("searchFailed_TipLabel");

        verticalLayout->addWidget(searchFailed_TipLabel, 0, Qt::AlignmentFlag::AlignHCenter);

        searchFailed_ReturnBtn = new QPushButton(searchcontactfaileddialog);
        searchFailed_ReturnBtn->setObjectName("searchFailed_ReturnBtn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(35);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(searchFailed_ReturnBtn->sizePolicy().hasHeightForWidth());
        searchFailed_ReturnBtn->setSizePolicy(sizePolicy);
        searchFailed_ReturnBtn->setMinimumSize(QSize(100, 35));
        searchFailed_ReturnBtn->setMaximumSize(QSize(100, 35));

        verticalLayout->addWidget(searchFailed_ReturnBtn, 0, Qt::AlignmentFlag::AlignHCenter);


        retranslateUi(searchcontactfaileddialog);

        QMetaObject::connectSlotsByName(searchcontactfaileddialog);
    } // setupUi

    void retranslateUi(QDialog *searchcontactfaileddialog)
    {
        searchcontactfaileddialog->setWindowTitle(QCoreApplication::translate("searchcontactfaileddialog", "Dialog", nullptr));
        searchFailed_TipLabel->setText(QCoreApplication::translate("searchcontactfaileddialog", "\346\237\245\346\211\276\347\224\250\346\210\267\345\244\261\350\264\245\357\274\201", nullptr));
        searchFailed_ReturnBtn->setText(QCoreApplication::translate("searchcontactfaileddialog", "\350\277\224\345\233\236", nullptr));
    } // retranslateUi

};

namespace Ui {
    class searchcontactfaileddialog: public Ui_searchcontactfaileddialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHCONTACTFAILEDDIALOG_H
