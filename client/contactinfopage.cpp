#include "contactinfopage.h"
#include "ui_contactinfopage.h"
#include <QDir>

ContactInfoPage::ContactInfoPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactInfoPage)
    , _contactInfo(nullptr) // 初始化联系人信息为空
{
    ui->setupUi(this);
    ui->chat_btn->setState("normal", "hover", "pressed");
    ui->video_btn->setState("normal", "hover", "pressed");
    ui->audio_btn->setState("normal", "hover", "pressed");
    this->setObjectName("ContactInfoPage");
}

ContactInfoPage::~ContactInfoPage()
{
    delete ui;
}

void ContactInfoPage::updateContactInfoUIAndInfo(std::shared_ptr<AuthResponse> contactInfo)
{
    _contactInfo = contactInfo;
    if (!_contactInfo) {
        qDebug() << "Contact info is not set.";
        return; // 如果联系人信息为空，直接返回
    }

    // 更新UI显示联系人信息
    ui->name_label->setText(_contactInfo->getName());
    ui->gender_label->setText(_contactInfo->getGender() == 1 ? "男" : "女");
    ui->backupName_label->setText(_contactInfo->getNickname());
    QString appPath = QCoreApplication::applicationDirPath();
    QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + _contactInfo->getIcon());
    QPixmap avatarPixmap(iconPath);
    if (!avatarPixmap.isNull()) {
        avatarPixmap = avatarPixmap.scaled(ui->icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->icon_label->setScaledContents(true); // 确保图标适应标签大小
        ui->icon_label->setPixmap(avatarPixmap);
    } else {
        ui->icon_label->setText("头像加载失败");
    }
}

void ContactInfoPage::on_chat_btn_clicked()
{
    emit signal_jumpTo_sessionItem(_contactInfo); // 跳转到对应的会话项
}

