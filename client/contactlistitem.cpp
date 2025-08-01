#include "contactlistitem.h"
#include "ui_contactlistitem.h"
#include <QDir>

ContactListItem::ContactListItem(QWidget *parent)
    : BaseListItem(parent)
    , ui(new Ui::ContactListItem)
    , _avatarUrl("")
    , _name("")
    , _uid(0)
{
    ui->setupUi(this);
    setListItem(ListItemType::ContactItem);

    // 设置红点初始状态
    QPixmap redpointPixmap(":/images/red_point.png");
    redpointPixmap = redpointPixmap.scaled(ui->contact_redpoint_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->contact_redpoint_label->setPixmap(redpointPixmap);
    ui->contact_redpoint_label->setScaledContents(true); // 确保红点适应标签大小
    ui->contact_redpoint_label->setVisible(false); // 默认不显示红点
}


QSize ContactListItem::sizeHint() const
{
    return QSize(250, 70);
}

void ContactListItem::setInfo(int uid, QString name, QString avatarUrl)
{
    _uid = uid;
    _name = name;
    _avatarUrl = avatarUrl;
    QString appPath = QCoreApplication::applicationDirPath();
    QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + _avatarUrl);
    QPixmap avatarPixmap(iconPath);
    if (!avatarPixmap.isNull()) {
        avatarPixmap = avatarPixmap.scaled(ui->contact_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->contact_icon_label->setScaledContents(true); // 确保图标适应标签大小
        ui->contact_icon_label->setPixmap(avatarPixmap);
    } else {
        ui->contact_icon_label->setText("头像加载失败");
    }
    ui->contact_name_label->setText(name);
}

void ContactListItem::setAddContactItemInfoByLocal(QString name, QString avatarUrl)
{
    QPixmap avatarPixmap(avatarUrl);
    if (!avatarPixmap.isNull()) {
        avatarPixmap = avatarPixmap.scaled(ui->contact_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->contact_icon_label->setScaledContents(true); // 确保图标适应标签大小
        ui->contact_icon_label->setPixmap(avatarPixmap);
    } else {
        ui->contact_icon_label->setText("头像加载失败");
    }
    ui->contact_name_label->setText(name);
}

void ContactListItem::showRedPoint(bool show)
{
    if (listItemType != ListItemType::AddContactItem) {
        return; // 仅在添加联系人项上显示红点
    }
    if (show) {
        ui->contact_redpoint_label->setVisible(true); // 显示红点
    } else {
        ui->contact_redpoint_label->setVisible(false); // 隐藏红点
    }
}
