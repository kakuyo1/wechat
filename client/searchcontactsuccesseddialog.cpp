#include "searchcontactsuccesseddialog.h"
#include "ui_searchcontactsuccesseddialog.h"
#include "friendrequestdialog.h"

searchContactSuccessedDialog::searchContactSuccessedDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::searchContactSuccessedDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    // 设置按钮样式
    ui->add_contact_btn->setState("normal", "hover", "pressed");
    ui->cancel_btn->setState("normal", "hover", "pressed");
    // 设置模态对话框
    this->setModal(true);
}

searchContactSuccessedDialog::~searchContactSuccessedDialog()
{
    delete ui;
}

void searchContactSuccessedDialog::setContactInfo(std::shared_ptr<SearchInfo> contactInfo)
{
    ui->name_label->setText(contactInfo->getName());
    QString searchUser_AvatarPath = contactInfo->getAvatarPath();
    qDebug() << "Search User Avatar Path: " << searchUser_AvatarPath;
    // 从static文件夹中获取要添加的头像(Icon path: "B:\\qt learing\\llfc_client\\build\\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\\debug\\static\\head_1.jpg")
    QString appPath = QCoreApplication::applicationDirPath();
    QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + searchUser_AvatarPath);
    QPixmap pixmap(iconPath);
    qDebug() << "Icon Path: " << iconPath;
    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->icon_label->setPixmap(pixmap);
    } else {
        ui->icon_label->setText("头像加载失败");
    }
    _contactInfo = contactInfo;
}

void searchContactSuccessedDialog::on_add_contact_btn_clicked()
{
    this->close();
    auto friendRequestDialog = new FriendRequestDialog(parentWidget());
    friendRequestDialog->setContactInfo(_contactInfo); // 设置联系人信息
    friendRequestDialog->show(); // 显示添加好友对话框
    friendRequestDialog->setModal(true);
}


void searchContactSuccessedDialog::on_cancel_btn_clicked()
{
    this->close(); // 关闭对话框
}

