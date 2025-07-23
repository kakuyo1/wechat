#include "friendrequestpage.h"
#include "ui_friendrequestpage.h"

FriendRequestPage::FriendRequestPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendRequestPage)
{
    ui->setupUi(this);

    // 连接tcpmanager信号
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_get_authResponse, this, &FriendRequestPage::slot_get_authResponse);

    // 测试
    Test_LoadFakeRequestData();
}

void FriendRequestPage::addNewContactRequest(std::shared_ptr<AddContactRequest> requestInfo)
{
    //
}

void FriendRequestPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void FriendRequestPage::Test_LoadFakeRequestData()
{
    for (int i = 0; i < 13; i++) {
        int index = QRandomGenerator::global()->bounded(100);
        int str_i = index % strs.size();
        int head_i = index % heads.size();
        int name_i = index % names.size();

        FriendRequestListItem* item = new FriendRequestListItem(this);
        std::shared_ptr<RequestInfo> requestInfo = std::make_shared<RequestInfo>(
            index,
            names[name_i],
            strs[str_i],
            heads[head_i],
            "这是一个测试的描述",
            1, // 性别
            0 // 状态
        );
        item->setInfo(requestInfo);
        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint());
        listItem->setFlags(listItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable); // 禁用选择和启用
        ui->FR_list->addItem(listItem);
        ui->FR_list->setItemWidget(listItem, item);
        // 监听"添加好友"按钮点击事件
        connect(item, &FriendRequestListItem::signal_addFriendClicked, this, [this](){
            qDebug() << "添加好友按钮被点击";
        });
    }
}

void FriendRequestPage::slot_get_authResponse(std::shared_ptr<AuthResponse> response)
{
    qDebug() << "收到服务器好友认证响应 ";
    int uid = response->_peeruid;
    auto it = _requestItems.find(uid);
    if (it == _requestItems.end()) {
        qDebug() << "未找到对应的好友请求项";
        return;
    }
    FriendRequestListItem* item = it->second;
    item->showAddButton(false); // 隐藏添加按钮
}
