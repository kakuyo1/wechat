#include "friendrequestlistitem.h"
#include "ui_friendrequestlistitem.h"

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

void FriendRequestListItem::setInfoByAddContactResponse(std::shared_ptr<AddContactResponse> addContactResponse)
{
    _addContactItemInfo = addContactResponse;
    if (_addContactItemInfo) {
        QPixmap avatarPixmap(_addContactItemInfo->_avatarPath);
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
