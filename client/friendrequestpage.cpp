#include "friendrequestpage.h"
#include "ui_friendrequestpage.h"
#include "usermanager.h"

FriendRequestPage::FriendRequestPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendRequestPage)
    , authDialog(std::make_shared<FriendAuthDialog>(parentWidget()))
{
    ui->setupUi(this);

    // 往申请列表内添加新的好友申请信号
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_add_contact_request_success, this, &FriendRequestPage::slot_add_new_contact_request);

    // 测试
    // Test_LoadFakeRequestData();

    // 认证好友成功后，处理listlitem，如隐藏"添加好友"按钮等
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_getACK_auth_friend_request_success_handlerequestItem, this, &FriendRequestPage::slot_afterACK_handleTheRequestItem);

    // 初始化好友请求列表
    intializeRequestList();
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
        connect(item, &FriendRequestListItem::signal_addFriendClicked, this, &FriendRequestPage::slot_addBtn_clicked);
    }
}

void FriendRequestPage::intializeRequestList()
{
    auto friendRequestList = UserManager::GetInstance()->getFriendRequestList();
    for (auto it = friendRequestList.rbegin(); it != friendRequestList.rend(); ++it) { // 反向迭代，好友申请从最新到最旧，由上到下排列
        const auto& requestInfo = *it;
        if (_requestItems.find(requestInfo->_requestUid) != _requestItems.end()) {
            qDebug() << "好友请求已存在，忽略重复请求";
            continue; // 忽略重复的好友请求
        }
        int status = requestInfo->_status;
        QString from_name = requestInfo->_name;
        QString from_description = requestInfo->_desc;
        QString from_avatarPath = requestInfo->_avatarPath;
        FriendRequestListItem* item = new FriendRequestListItem(this);
        item->setInfoByServerIntialData(from_avatarPath, from_name, from_description);

        if (0 == status) {
            item->showAddButton(true); // 显示添加按钮
            // 监听"添加好友"按钮点击事件
            connect(item, &FriendRequestListItem::signal_addFriendClicked, this, &FriendRequestPage::slot_addBtn_clicked);
        } else if (1 == status) {
            item->showAddButton(false); // 隐藏添加按钮
        }

        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint());
        listItem->setFlags(listItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable); // 禁用选择和启用
        ui->FR_list->addItem(listItem);
        ui->FR_list->setItemWidget(listItem, item);
        _requestItems[requestInfo->_requestUid] = item; // 存储好友请求项
    }
}

void FriendRequestPage::slot_add_new_contact_request(std::shared_ptr<AddContactResponse> response)
{
    qDebug() << "收到新的好友请求: " << response->_name;
    int uid = response->_uid;
    if (_requestItems.find(uid) != _requestItems.end()) {
        qDebug() << "好友请求已存在，忽略重复请求";
        return;
    }
    FriendRequestListItem* item = new FriendRequestListItem(this);
    item->setInfoByAddContactResponse(response);
    item->showAddButton(true); // 显示添加按钮
    QListWidgetItem* listItem = new QListWidgetItem();
    listItem->setSizeHint(item->sizeHint());
    listItem->setFlags(listItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable); // 禁用选择和启用
    ui->FR_list->insertItem(0, listItem); // 将新请求添加到列表顶部
    ui->FR_list->setItemWidget(listItem, item);

    // side_contact_label红点亮起
    emit signal_sideContact_showRedPoint();

    // 添加到请求项映射中
    _requestItems[uid] = item;
    // 监听"添加好友"按钮点击事件
    connect(item, &FriendRequestListItem::signal_addFriendClicked, this, &FriendRequestPage::slot_addBtn_clicked);
}

void FriendRequestPage::slot_addBtn_clicked()
{
    qDebug() << "'添加好友'按钮被点击";
    FriendRequestListItem* item = qobject_cast<FriendRequestListItem*>(sender());
    authDialog->setToUid(item->getRequestUid()); // 设置验证对象 UID
    authDialog->setModal(true); // 设置为模态对话框
    authDialog->setWindowTitle("添加好友验证");
    authDialog->show(); // 显示验证对话框
    qDebug() << "弹出添加好友验证对话框";
}

void FriendRequestPage::slot_afterACK_handleTheRequestItem(std::shared_ptr<AuthResponse> response)
{
    // 找到对应的请求项并处理(隐藏添加按钮等)
    int requestUid = response->getUid();
    auto it = _requestItems.find(requestUid);
    if (it != _requestItems.end()) {
        FriendRequestListItem* item = it->second;
        item->showAddButton(false); // 隐藏添加按钮
        qDebug() << "处理好友认证请求的ACK响应，隐藏添加按钮";
    } else {
        qDebug() << "未找到对应的好友请求项，无法处理ACK响应";
    }
}
