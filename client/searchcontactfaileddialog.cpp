#include "searchcontactfaileddialog.h"
#include "ui_searchcontactfaileddialog.h"

searchcontactfaileddialog::searchcontactfaileddialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchcontactfaileddialog)
{
    ui->setupUi(this);
}

searchcontactfaileddialog::~searchcontactfaileddialog()
{
    delete ui;
}

void searchcontactfaileddialog::on_searchFailed_ReturnBtn_clicked()
{
    this->close(); // 关闭对话框
}

