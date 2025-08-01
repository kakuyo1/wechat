#include "friendrequestlistitem.h"
#include "ui_friendrequestlistitem.h"
#include <QDir>

FriendRequestListItem::FriendRequestListItem(QWidget *parent)
    : BaseListItem(parent)
    , ui(new Ui::FriendRequestListItem)
    , _requestInfo(nullptr)
    , _AlreadyAdded(false)
{
    ui->setupUi(this);
    setListItem(ListItemType::FriendRequestItem);
    ui->FR_add_btn->setState("normal", "hover", "pressed");
    ui->FR_add_btn->hide();
    // 连接信号槽
    connect(ui->FR_add_btn, &Customized_button::clicked, this, [this](){
        emit signal_addFriendClicked(_requestInfo);
    });
}

QSize FriendRequestListItem::sizeHint() const
{
    return QSize(250, 80);
}

void FriendRequestListItem::showAddButton(bool show)
{
    if (show) {
        ui->FR_add_btn->setVisible(true);
        ui->already_add_label->setVisible(false);
        _AlreadyAdded = false;
    } else {
        ui->already_add_label->setVisible(true);
        ui->FR_add_btn->setVisible(false);
        _AlreadyAdded = true;
    }
}

void FriendRequestListItem::setInfo(std::shared_ptr<RequestInfo> requestInfo) // Test
{
    // 传入的 requestInfo 是由网络Response或其他地方转换的好友请求信息
    _requestInfo = requestInfo;
    if (_requestInfo) {
        QPixmap avatarPixmap(_requestInfo->_avatarPath);
        if (!avatarPixmap.isNull()) {
            avatarPixmap = avatarPixmap.scaled(ui->FR_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->FR_icon_label->setScaledContents(true); // 确保图标适应标签大小
            ui->FR_icon_label->setPixmap(avatarPixmap);
        } else {
            ui->FR_icon_label->setText("头像加载失败");
        }
        ui->friend_name_label->setText(_requestInfo->_name);
        ui->friend_message_label->setText(_requestInfo->_description);
    }
}

void FriendRequestListItem::setInfoByServerIntialData(const QString &from_avatarPath, const QString &from_name, const QString &from_description)
{
    // 从static文件夹中获取要添加的头像(Icon path: "B:\\qt learing\\llfc_client\\build\\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\\debug\\static\\head_1.jpg")
    QString appPath = QCoreApplication::applicationDirPath();
    QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + from_avatarPath);
    QPixmap avatarPixmap(iconPath);
    if (!avatarPixmap.isNull()) {
        avatarPixmap = avatarPixmap.scaled(ui->FR_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->FR_icon_label->setScaledContents(true); // 确保图标适应标签大小
        ui->FR_icon_label->setPixmap(avatarPixmap);
    } else {
        ui->FR_icon_label->setText("头像加载失败");
    }
    ui->friend_name_label->setText(from_name);
    ui->friend_message_label->setText(from_description);
}

void FriendRequestListItem::setInfoByAddContactResponse(std::shared_ptr<AddContactResponse> addContactResponse)
{
    _addContactItemInfo = addContactResponse;
    if (_addContactItemInfo) {
        _requestUid = _addContactItemInfo->_uid; // 保存请求的UID
        // 从static文件夹中获取要添加的头像(Icon path: "B:\\qt learing\\llfc_client\\build\\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\\debug\\static\\head_1.jpg")
        QString appPath = QCoreApplication::applicationDirPath();
        QString avatarPath = _addContactItemInfo->_avatarPath;
        QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + avatarPath);
        QPixmap avatarPixmap(iconPath);
        if (!avatarPixmap.isNull()) {
            avatarPixmap = avatarPixmap.scaled(ui->FR_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->FR_icon_label->setScaledContents(true); // 确保图标适应标签大小
            ui->FR_icon_label->setPixmap(avatarPixmap);
        } else {
            ui->FR_icon_label->setText("头像加载失败");
        }
        ui->friend_name_label->setText(_addContactItemInfo->_name);
        ui->friend_message_label->setText(_addContactItemInfo->_description);
    }
}
