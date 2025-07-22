#include "friendrequestpage.h"
#include "ui_friendrequestpage.h"

FriendRequestPage::FriendRequestPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendRequestPage)
{
    ui->setupUi(this);
}

FriendRequestPage::~FriendRequestPage()
{
    delete ui;
}
