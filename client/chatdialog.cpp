#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "sessionlistitem.h"
#include "usermanager.h"

ChatDialog::ChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChatDialog)
    , chatUIMode(ChatUIMode::ChatMode) // 默认聊天模式
    , isLoading(false)
{
    ui->setupUi(this);
    ui->add_btn->setState("normal","hover","pressed"); // content_panal的添加好友按钮
    ui->side_chat_label->setState("normal","hover","pressed"); // 侧边栏的聊天按钮
    ui->side_contact_label->setState("normal","hover","pressed"); // 侧边栏的联系人按钮
    addSideBarButtons(ui->side_chat_label);
    addSideBarButtons(ui->side_contact_label);

    ui->search_lineEdit->setFocusPolicy(Qt::ClickFocus); // 设置搜索输入框的焦点策略为点击获取焦点
    // 初始显示chat_list,隐藏search_list和contact_list
    ui->chat_list->show();
    ui->search_list->hide();
    ui->contact_list->hide();

    // 从static文件夹中获取要添加的头像(Icon path: "B:\\qt learing\\llfc_client\\build\\Desktop_Qt_6_9_0_MinGW_64_bit-Debug\\debug\\static\\head_1.jpg")
    QString appPath = QCoreApplication::applicationDirPath();
    QString avatarPath = UserManager::GetInstance()->getIconPath(); // 获取用户头像路径
    QString iconPath = QDir::toNativeSeparators(appPath + QDir::separator() + "static" + QDir::separator() + avatarPath);
    QPixmap avatarPixmap(iconPath);
    avatarPixmap = avatarPixmap.scaled(ui->side_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->side_icon_label->setPixmap(avatarPixmap);
    ui->side_icon_label->setScaledContents(true); // 确保图标适应标签大小

    // 设置搜索输入框焦点变化时搜索页面的显示和隐藏
    connect(ui->search_lineEdit, &Customized_EditLine::signal_focusIn, this, [this](){
        QTimer::singleShot(0, this, [this]() { // 延迟到事件循环再执行
            showSearchList(true);
        });
    });
    connect(ui->search_lineEdit, &Customized_EditLine::signal_focusOut, this, [this]() {
        // 延迟一点点时间判断，避免和 focusIn 同时发生冲突
        QTimer::singleShot(0, this, [this]() {
            if (!ui->search_lineEdit->hasFocus()) {
                QPoint globalPos = QCursor::pos();
                QRect searchRect = ui->search_list->rect();
                QRect globalSearchRect(ui->search_list->mapToGlobal(searchRect.topLeft()),
                                       ui->search_list->mapToGlobal(searchRect.bottomRight()));
                if (!globalSearchRect.contains(globalPos)) {
                    showSearchList(false);
                    ui->search_lineEdit->clear();
                }
            }
        });
    });

    // 设置侧边栏按钮的点击事件，进行contents_panal页面的切换
    connect(ui->side_chat_label, &StateWidget::clicked, this, [this]() {
        ui->contact_list->hide();
        ui->search_list->hide();
        ui->chat_list->show();
        ui->side_chat_label->clearState(); // 清除聊天按钮的红点状态
        ui->stackedWidget->setCurrentWidget(ui->chat_page); // 切换到聊天页面
        chatUIMode = ChatUIMode::ChatMode; // 切换到聊天模式
        ui->side_chat_label->setSelected(true); // 设置聊天按钮的选中状态
        ui->side_contact_label->setSelected(false); // 取消联系人按钮的选中状态
    });

    connect(ui->side_contact_label, &StateWidget::clicked, this, [this]() {
        ui->chat_list->hide();
        ui->search_list->hide();
        ui->contact_list->show();
        ui->side_contact_label->clearState();
        ui->stackedWidget->setCurrentWidget(ui->contact_info_page); // 切换到联系人页面
        chatUIMode = ChatUIMode::ContactMode; // 切换到联系人模式
        ui->side_chat_label->setSelected(false); // 取消聊天按钮的选中状态
        ui->side_contact_label->setSelected(true); // 设置联系人按钮的选中状态
        // 自动选择第一个联系人
        if (ui->contact_list->count() > 3) {
            QListWidgetItem* item = ui->contact_list->item(3);
            ui->contact_list->setCurrentItem(item);
            ui->contact_list->slot_contactItem_clicked(item);
        }
    });

    // 为searchList 设置lineEdit
    ui->search_list->setLineEdit(ui->search_lineEdit);

    // 设置会话列表的加载更多事件
    connect(ui->chat_list, &SessionList::signal_loading_sessionItems, this, &ChatDialog::slot_load_more_sessionitems);

    // 加上联系人列表的加载更多事件
    connect(ui->contact_list, &ContactList::signal_load_contacts, this, &ChatDialog::slot_load_more_contactitems);

    // 添加聊天消息到聊天区域
    connect(ui->chat_page, &ChatPage::signal_append_chat_message, this, &ChatDialog::slot_append_chat_message);

    // 切换到好友申请页面
    connect(ui->contact_list, &ContactList::signal_switchTo_friendRequestPage, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->friend_request_page); // 切换到好友申请页面
    });

    // 切换到联系人信息页面
    connect(ui->contact_list, &ContactList::signal_switchTo_contactInfoPage, this, &ChatDialog::slot_switchTo_contactInfoPage);

    // side_contact_label的红点状态
    connect(ui->friend_request_page, &FriendRequestPage::signal_sideContact_showRedPoint, this, [this]() {
        ui->side_contact_label->showRedPoint(true); // 显示红点
    });

    // 接收到新的聊天消息批次
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_receive_chat_text_message, this, &ChatDialog::slot_get_new_text_chat_batch);

    // 处理会话点击
    connect(ui->chat_list, &QListWidget::itemClicked, this, &ChatDialog::slot_sessionItem_clicked);

    // 从联系人信息页面跳转到对应的会话项
    connect(ui->contact_info_page, &ContactInfoPage::signal_jumpTo_sessionItem, this, &ChatDialog::slot_switchTo_sessionItem_from_contactInfo);

    // 安装事件过滤器
    this->installEventFilter(this);

    // // Test 添加会话列表项
    // Test_addSessionItem();

    initializeContactList(); // 初始化联系人列表

    // 初始化会话列表并选中第一个会话项
    intialzeSessionListAndSelectFirstItem();
}

QString ChatDialog::getSearchLineEditText() const
{
    return ui->search_lineEdit->text(); // 获取搜索输入框的文本内容
}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::showSearchList(bool show)
{
    if (show) {
        ui->search_list->show();
        ui->contact_list->hide();
        ui->chat_list->hide();
        chatUIMode = ChatUIMode::SearchMode;
    } else {
        ui->search_list->hide();
        ui->contact_list->hide();
        ui->chat_list->show();
        chatUIMode = ChatUIMode::ChatMode; // 默认回到聊天模式
    }
}

void ChatDialog::Test_addSessionItem()
{
    for (int i = 0; i < 13; ++i) {
        int randomIndex = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机数
        int str_i = randomIndex % strs.size();
        int head_i = randomIndex % heads.size();
        int name_i = randomIndex % names.size();

        auto *item = new SessionListItem(this);
        item->setSessionUI(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        ui->chat_list->addItem(listItem); // 添加列表项
        ui->chat_list->setItemWidget(listItem, item); // 设置列表项的widget
    }
}

// void ChatDialog::Test_AddMoreContacts()
// {
//     for (int i = 0; i < 13; ++i) {
//         int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
//         int head_i = randomValue%heads.size();
//         int name_i = randomValue%names.size();
//         auto *item = new ContactListItem(this);
//         item->setInfo(0, names[name_i], heads[head_i]);
//         QListWidgetItem *listItem = new QListWidgetItem();
//         listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
//         ui->contact_list->addItem(listItem); // 添加列表项
//         ui->contact_list->setItemWidget(listItem, item); // 设置列表项的widget
//     }
// }

QSize ChatDialog::sizeHint() const
{
    return QSize(1225, 810);
}

void ChatDialog::addSideBarButtons(StateWidget *button)
{
    if (!button) return;
    sideBarButtons.append(button);
}

void ChatDialog::initializeContactList()
{
    fullContactList = UserManager::GetInstance()->getContactList();
    if (fullContactList.empty()) {
        qDebug() << "No contacts available to initialize.";
        return; // 如果没有联系人，直接返回
    }
    currentContactIndex = 0; // 重置当前联系人索引
    loadMoreContacts(); // 加载第一页
}

void ChatDialog::loadMoreContacts()
{
    if (static_cast<size_t>(currentContactIndex) >= fullContactList.size()) {
        qDebug() << "No more contacts to load.";
        return; // 如果已经加载完所有联系人，直接返回
    }

    // 计算当前页的联系人范围, 如果只有少于 MAX_CONTACTS_PER_PAGE 个联系人，则加载全部，否则每次10个
    int endIndex = std::min(currentContactIndex + MAX_CONTACTS_PER_PAGE, static_cast<int>(fullContactList.size()));

    // 加载当前页的联系人
    for (int i = currentContactIndex; i < endIndex; ++i) {
        auto contactInfo = fullContactList[i];
        if (!contactInfo) continue; // 如果联系人信息为空，跳过
        auto *item = new ContactListItem(this);
        item->setInfo(contactInfo);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        ui->contact_list->addItem(listItem); // 添加列表项
        ui->contact_list->setItemWidget(listItem, item); // 设置列表项的widget
    }
    currentContactIndex = endIndex; // 更新当前联系人索引
}

void ChatDialog::updateSessionChatViewMsg(std::vector<std::shared_ptr<TextChatData>> messageBatch)
{
    for (const auto& msg: messageBatch) {
        if (msg->_from_uid != _current_sessionItem) {
            // 如果不是当前会话的消息，跳过
            continue;
        }
        ui->chat_page->AppendChatMessage(msg); // 添加到聊天页面
    }
}
// 作用：设置选中的会话项
void ChatDialog::setSelectedSessionItem(int index)
{
    if (ui->chat_list->count() < 0) {
        qDebug() << "Invalid session item index:" << index;
        return; // 如果索引无效，直接返回
    }

    if (index == 0) { // 如果是第一项，直接选中(用于初始化)
        ui->chat_list->setCurrentRow(0); // 设置选中第一项
        QListWidgetItem* item = ui->chat_list->item(0);
        if (!item) {
            qDebug() << "No session item at index 0.";
            return;
        }
        // 转为widget
        QWidget* widget = ui->chat_list->itemWidget(item);
        if (!widget) {
            qDebug() << "Session item at index 0 has no associated widget.";
            return;
        }
        // 转为 SessionListItem
        SessionListItem* sessionItem = qobject_cast<SessionListItem*>(widget);
        if (!sessionItem) {
            qDebug() << "Widget at index 0 is not a SessionListItem.";
            return;
        }
        qDebug() << "A";
        _current_sessionItem = sessionItem->getSessionInfo()->_peeruid; // 设置当前会话项的uid
        qDebug() << "B";
        return;
    }
    // 不是第一项
    auto iter = _sessionItemMap.find(index);
    if (iter == _sessionItemMap.end()) {
        qDebug() << "Session item index not found in map:" << index;
        ui->chat_list->setCurrentRow(0); // 设置选中第一项
        return; // 如果索引不在映射中，直接返回
    }
    ui->chat_list->setCurrentItem(iter.value()); // 设置选中对应的项
    _current_sessionItem = index; // 设置当前会话项的索引
}
// 作用：设置选中的聊天页面，根据会话项索引设置聊天页面信息
void ChatDialog::setSelectedChatPage(int index)
{
    if (ui->chat_list->count() <= 0) {
        qDebug() << "No session items available to select.";
        return; // 如果没有会话项，直接返回
    }
    if (index == 0) {
        auto item = ui->chat_list->item(0);
        if (!item) {
            qDebug() << "No session item at index 0.";
            return;
        }
        // 转为widget
        QWidget* widget = ui->chat_list->itemWidget(item);
        if (!widget) {
            qDebug() << "Session item at index 0 has no associated widget.";
            return;
        }
        // 转为 SessionListItem
        SessionListItem* sessionItem = qobject_cast<SessionListItem*>(widget);
        if (!sessionItem) {
            qDebug() << "Widget at index 0 is not a SessionListItem.";
            return;
        }
        // 设置信息
        auto iter = sessionItem->getSessionInfo();
        if (!iter) {
            qDebug() << "Session info at index 0 is null.";
            return;
        }
        ui->chat_page->setPeerInfo(iter); // 设置聊天页面信息
        return;
    }
    // 不是第一项
    qDebug() << "a";
    auto iter = _sessionItemMap.find(index);
    if (iter == _sessionItemMap.end()) {
        qDebug() << "Session item index not found in map:" << index;
        return; // 如果索引不在映射中，直接返回
    }
    qDebug() << "b";
    // 转为widget
    QWidget* widget = ui->chat_list->itemWidget(iter.value());
    if (!widget) {
        qDebug() << "Session item at index" << index << "has no associated widget.";
        return;
    }
    qDebug() << "c";
    // 转为 ListItemBase
    BaseListItem* baseItem = qobject_cast<BaseListItem*>(widget);
    if (!baseItem) {
        qDebug() << "Widget at index" << index << "is not a BaseListItem.";
        return;
    }
    qDebug() << "d";
    auto itemType = baseItem->getListItemType();
    if (itemType == ListItemType::ChatItem) {
        auto sessionItem = qobject_cast<SessionListItem*>(baseItem);
        qDebug() << "e";
        if (!sessionItem) {
            qDebug() << "Widget at index" << index << "is not a SessionListItem.";
            return;
        }
        // 设置信息
        auto sessionInfo = sessionItem->getSessionInfo();
        qDebug() << "f";
        ui->chat_page->setPeerInfo(sessionInfo); // 设置聊天页面信息
        qDebug() << "g";
        return;
    }
}

void ChatDialog::intialzeSessionListAndSelectFirstItem()
{
    // std::vector<std::shared_ptr<AuthResponse>> fullContactList; // 全部联系人数据缓存
    // 选fullContactList的前10名联系人直接作为会话项(方便测试)
    int count = std::min(10, static_cast<int>(fullContactList.size()));
    for (int i = 0; i < count; ++i) {
        auto contactInfo = fullContactList[i];
        if (!contactInfo) continue; // 如果联系人信息为空，跳过
        auto sessionItem = new SessionListItem();
        sessionItem->setSessionUI(contactInfo->getName(), contactInfo->getIcon(), "这是最后一条消息的预览");
        qDebug() << "1";
        // 构造AuthResponse类型的会话信息
        AuthResponse authResponse(contactInfo->_peeruid, contactInfo->_peergender, contactInfo->_peername,
                                  contactInfo->_peernickname, contactInfo->_peericon, contactInfo->_peeremail, contactInfo->_peerdescription);
        std::shared_ptr<SessionInfo> sessionInfo = std::make_shared<SessionInfo>(std::make_shared<AuthResponse>(authResponse), std::vector<std::shared_ptr<TextChatData>>{});
        if (!sessionInfo) {
            qDebug() << "No session info found for uid:" << contactInfo->getUid();
            return; // 如果没有找到对应的会话信息，跳过
        }
        sessionItem->setFullSessionInfo(sessionInfo); // 设置完整的会话信息
        qDebug() << "2";
        QListWidgetItem *listItem = new QListWidgetItem;
        listItem->setSizeHint(sessionItem->sizeHint()); // 设置列表项的大小提示
        ui->chat_list->addItem(listItem); // 添加列表项
        ui->chat_list->setItemWidget(listItem, sessionItem); // 设置列表项的widget
        _sessionItemMap.insert(contactInfo->getUid(), listItem); // 建立索引和列表项的映射
        qDebug() << "3";
    }
    setSelectedSessionItem(0); // 选中第一项
    setSelectedChatPage(0);    // 切换到对应的聊天页面
    ui->stackedWidget->setCurrentWidget(ui->chat_page); // 切换到聊天页面
    chatUIMode = ChatUIMode::ChatMode;
    ui->side_chat_label->setSelected(true); // 设置聊天按钮为选中状态
    ui->side_contact_label->setSelected(false); // 设置联系人按钮为未选中状态
    qDebug() << "4";
}

bool ChatDialog::eventFilter(QObject *watched, QEvent *event)
{
     // 点击TextEdit不会触发该情况，因为点击TextEdit只是导致失去焦点，而不是点击事件
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        QPoint globalPos = mouseEvent->globalPosition().toPoint(); // 获取全局坐标;

        // 计算 search_list 在全局的矩形
        QRect searchRect = ui->search_list->rect();
        QPoint topLeft = ui->search_list->mapToGlobal(searchRect.topLeft());
        QPoint bottomRight = ui->search_list->mapToGlobal(searchRect.bottomRight());
        QRect globalSearchRect(topLeft, bottomRight);

        if (chatUIMode != ChatUIMode::SearchMode) {
            return false;  // 不拦截，让事件继续
        } else {
            if (globalSearchRect.contains(globalPos)) {
                return false; // 点击在 search_list 内，不处理
            } else {
                showSearchList(false);
                ui->search_lineEdit->clear();
                return true;
            }
        }
    }
    return QDialog::eventFilter(watched, event);
}

void ChatDialog::slot_load_more_sessionitems()
{
    /*进入槽函数后，首先往sessionlist增加加载gif动画效果
     * ，设置模态窗口，然后加载sessionitems*/
    if (isLoading) return;
    isLoading = true;
    // 动画效果
    QLabel *loadingLabel = new QLabel(this);
    QMovie *loadingMovie = new QMovie(":/images/loading.gif");
    loadingLabel->setMovie(loadingMovie);
    loadingLabel->setFixedSize(250, 70);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingMovie->setScaledSize(QSize(50, 50));
    QListWidgetItem *loadingItem = new QListWidgetItem();
    loadingItem->setSizeHint(loadingLabel->sizeHint());
    ui->chat_list->addItem(loadingItem);
    ui->chat_list->setItemWidget(loadingItem, loadingLabel);
    loadingMovie->start();
    // 模拟加载数据
    QTimer::singleShot(100, this, [this, loadingItem, loadingLabel]() {
        // Test_addSessionItem(); // 测试添加更多会话项
        // 调用真正的加载会话项函数
        // TODO
        // loadMoreSessionItems();
        // 模拟加载完成后移除加载动画
        ui->chat_list->removeItemWidget(loadingItem);
        delete loadingLabel; // 删除加载标签
        delete loadingItem; // 删除加载项
        isLoading = false; // 重置加载状态
    });
}

void ChatDialog::slot_load_more_contactitems()
{
    /*进入槽函数后，首先往sessionlist增加加载gif动画效果
     * ，设置模态窗口，然后加载sessionitems*/
    if (isLoading || static_cast<size_t>(currentContactIndex) >= fullContactList.size()) return; // 如果正在加载或没有更多联系人，直接返回
    isLoading = true;
    // 动画效果
    QLabel *loadingLabel = new QLabel(this);
    QMovie *loadingMovie = new QMovie(":/images/loading.gif");
    loadingLabel->setMovie(loadingMovie);
    loadingLabel->setFixedSize(250, 70);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingMovie->setScaledSize(QSize(50, 50));
    QListWidgetItem *loadingItem = new QListWidgetItem();
    loadingItem->setSizeHint(loadingLabel->sizeHint());
    ui->contact_list->addItem(loadingItem);
    ui->contact_list->setItemWidget(loadingItem, loadingLabel);
    loadingMovie->start();
    // 模拟加载数据
    QTimer::singleShot(100, this, [this, loadingItem, loadingLabel]() {
        // 真正加载联系人
        loadMoreContacts();
        // 加载完成后移除加载动画
        ui->chat_list->removeItemWidget(loadingItem);
        delete loadingLabel; // 删除加载标签
        delete loadingItem; // 删除加载项
        isLoading = false; // 重置加载状态
    });
}

void ChatDialog::slot_switchTo_contactInfoPage()
{
    // 获取点击的联系人项，拿到联系人信息
    auto item = ui->contact_list->currentItem();
    if (!item) {
        qDebug() << "No contact item selected.";
        return; // 如果没有选中项，直接返回
    }
    QWidget *widget = ui->contact_list->itemWidget(item);
    if (!widget) {
        qDebug() << "No widget associated with the selected contact item.";
        return; // 如果没有关联的widget，直接返回
    }
    ContactListItem *contactItem = qobject_cast<ContactListItem *>(widget);
    if (!contactItem) {
        qDebug() << "Selected item is not a ContactListItem.";
        return; // 如果选中的项不是ContactListItem，直接返回
    }
    // 获取联系人信息
    auto contactInfo = contactItem->getContactInfo();
    if (!contactInfo) {
        qDebug() << "Contact information is null.";
        return; // 如果联系人信息为空，直接返回
    }
    // 设置联系人信息到联系人信息页面
    ui->contact_info_page->updateContactInfoUIAndInfo(contactInfo); // 更新联系人信息页面的UI并且保存联系人信息
    ui->stackedWidget->setCurrentWidget(ui->contact_info_page); // 切换到联系人信息页面
}

void ChatDialog::slot_append_chat_message(std::shared_ptr<TextChatData> message)
{
    // 将自己发的消息 加入聊天记录容器
    // 并同步更新到全局好友列表 _friendsessionInfo_map
    if (!message) {
        qDebug() << "Received null message.";
        return; // 如果消息为空，直接返回
    }

    auto iter = _sessionItemMap.find(_current_sessionItem);
    if (iter == _sessionItemMap.end()) {
        qDebug() << "Current session item not found.";
        return; // 如果当前会话项未找到，直接返回
    }

    // 转为Widget
    QWidget *widget = ui->chat_list->itemWidget(iter.value());
    if (!widget) {
        qDebug() << "No widget associated with the current session item.";
        return; // 如果没有关联的widget，直接返回
    }

    BaseListItem *item = qobject_cast<BaseListItem *>(widget);
    if (!item) {
        qDebug() << "Current session item is not a BaseListItem.";
        return; // 如果当前会话项不是BaseListItem，直接返回
    }
    auto itemType = item->getListItemType();

    if (itemType != ListItemType::ChatItem) {
        qDebug() << "Current session item is not a SessionListItem.";
        return; // 如果当前会话项不是ChatItem，直接返回
    }

    SessionListItem *sessionItem = qobject_cast<SessionListItem *>(item);
    if (!sessionItem) {
        qDebug() << "Current session item is not a SessionListItem.";
        return; // 如果当前会话项不是SessionListItem，直接返回
    }
    // 设置消息内容到会话项
    auto session_info = sessionItem->getSessionInfo();
    if (!session_info) {
        qDebug() << "Session info is null.";
        return; // 如果会话信息为空，直接返回
    }
    session_info->_chatHistory.push_back(message); // 添加消息到自己的会话聊天历史
    UserManager::GetInstance()->appendNewChatMsgToFriendSession(_current_sessionItem, message); // 更新好友会话信息
}
// 接收到新的聊天消息批次，对方发来的
void ChatDialog::slot_get_new_text_chat_batch(std::shared_ptr<TextChatBatch> batch)
{
    // 找到发送者的会话项
    auto it = _sessionItemMap.find(batch->_from_uid);
    if (it != _sessionItemMap.end()) {
        // 找到对应的会话项
        qDebug() << "New Batch Received! Found session item for user ID:" << batch->_from_uid;
        QWidget* widget = ui->chat_list->itemWidget(it.value());
        auto sessionItem = qobject_cast<SessionListItem *>(widget);
        if (!sessionItem) {
            qDebug() << "Session item is not a SessionListItem.";
            return; // 如果会话项不是SessionListItem，直接返回
        }
        // 设置会话项的最新消息内容(在会话项昵称的下方)
        sessionItem->updateSessionItemLastMsg(batch->_messages);
        // 更新当前聊天页面的对话信息
        updateSessionChatViewMsg(batch->_messages);
        return;
    }
    // 没有找到会话项，新建(第一次聊天)
    qDebug() << "New Batch Received! Creating new session item for user ID:" << batch->_from_uid;
    auto sessionItem = new SessionListItem(this);
    // 查询好友信息
    auto peerInfo = UserManager::GetInstance()->getFriendSessionInfoByUid(batch->_from_uid);
    if (!peerInfo) {
        qDebug() << "Peer info not found for user ID:" << batch->_from_uid;
        return; // 如果没有找到好友信息，直接返回
    }
    // 构造AuthResponse类型的会话信息
    AuthResponse authResponse(batch->_from_uid, peerInfo->_peergender, peerInfo->_peername,
                                      peerInfo->_peernickname, peerInfo->_peericon, peerInfo->_peeremail, peerInfo->_peerdescription);
    std::shared_ptr<SessionInfo> sessionInfo = std::make_shared<SessionInfo>(std::make_shared<AuthResponse>(authResponse), batch->_messages);
    sessionItem->setFullSessionInfo(sessionInfo); // 设置完整的会话信息
    sessionItem->setSessionUI(peerInfo->_peername, peerInfo->_peericon, peerInfo->_last_message_to_show);
    // 添加到聊天列表
    QListWidgetItem *listItem = new QListWidgetItem();
    listItem->setSizeHint(sessionItem->sizeHint()); // 设置列表项的大小提示
    sessionItem->updateSessionItemLastMsg(batch->_messages);
    UserManager::GetInstance()->appendNewChatMsgToFriendSession(batch->_from_uid, batch->_messages.back()); // 更新好友会话信息
    ui->chat_list->insertItem(0, listItem); // 插入到列表的顶部
    ui->chat_list->setItemWidget(listItem, sessionItem); // 设置列表项的widget
    _sessionItemMap[batch->_from_uid] = listItem; // 更新会话项映射
}

void ChatDialog::slot_sessionItem_clicked(QListWidgetItem *item)
{
    QWidget *widget = ui->chat_list->itemWidget(item);
    if (!widget) {
        qDebug() << "No widget associated with the clicked session item.";
        return; // 如果没有关联的widget，直接返回
    }
    BaseListItem *baseItem = qobject_cast<BaseListItem *>(widget);
    if (!baseItem) {
        qDebug() << "Clicked item is not a BaseListItem.";
        return; // 如果点击的项不是BaseListItem，直接返回
    }
    auto itemType = baseItem->getListItemType();
    if (itemType == ListItemType::NonClickableItem) {
        qDebug() << "Clicked item is not a SessionListItem.";
        return; // 如果点击的项不是SessionListItem，直接返回
    }
    if (itemType == ListItemType::ChatItem) {
        qDebug() << "Clicked item is a SessionListItem.";
        SessionListItem *sessionItem = qobject_cast<SessionListItem *>(baseItem);
        if (!sessionItem) {
            qDebug() << "Clicked item is not a SessionListItem.";
            return; // 如果点击的项不是SessionListItem，直接返回
        }
        // 获取会话信息
        auto sessionInfo = sessionItem->getSessionInfo();
        if (!sessionInfo) {
            qDebug() << "Session info is null.";
            return; // 如果会话信息为空，直接返回
        }
        _current_sessionItem = sessionInfo->_peeruid; // 更新当前会话项的用户ID
        ui->chat_page->setPeerInfo(sessionInfo); // 设置聊天页面的对方用户信息
        // 跳转到聊天页面
        ui->stackedWidget->setCurrentWidget(ui->chat_page);
        chatUIMode = ChatUIMode::ChatMode; // 切换到聊天模式
        return;
    }
}

void ChatDialog::slot_switchTo_sessionItem_from_contactInfo(std::shared_ptr<AuthResponse> contactInfo)
{
    qDebug() << "Switching to session item for user ID:" << contactInfo->_peeruid;
    auto iter = _sessionItemMap.find(contactInfo->_peeruid);
    // 找到了
    if (iter != _sessionItemMap.end()) {
        ui->chat_list->scrollToItem(iter.value()); // 滚动到对应的会话项
        ui->side_chat_label->setSelected(true); // 选中侧边聊天按钮
        ui->side_contact_label->setSelected(false); // 取消联系人按钮的选中状态
        setSelectedSessionItem(contactInfo->_peeruid); // 设置选中的会话项
        setSelectedChatPage(contactInfo->_peeruid); // 更新选中的聊天页面
        ui->stackedWidget->setCurrentWidget(ui->chat_page); // 切换到聊天页面
        chatUIMode = ChatUIMode::ChatMode; // 切换到聊天模式
        showSearchList(false); // 默认回到聊天列表栏
        return;
    }
    // 没找到(新建一个sessionItem)
    auto sessionItem = new SessionListItem();
    sessionItem->setSessionUI(contactInfo->_peername, contactInfo->_peericon, ""); // 设置会话UI
    // 构造AuthResponse类型的会话信息
    AuthResponse authResponse(contactInfo->_peeruid, contactInfo->_peergender, contactInfo->_peername,
                              contactInfo->_peernickname, contactInfo->_peericon, contactInfo->_peeremail, contactInfo->_peerdescription);
    std::shared_ptr<SessionInfo> sessionInfo = std::make_shared<SessionInfo>(std::make_shared<AuthResponse>(authResponse), std::vector<std::shared_ptr<TextChatData>>{});
    sessionItem->setFullSessionInfo(sessionInfo); // 设置完整的会话信息
    // 添加到聊天列表
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(sessionItem->sizeHint()); // 设置列表项的大小提示
    ui->chat_list->insertItem(0, listItem); // 插入到列表的顶部
    qDebug() << "1";
    ui->chat_list->setItemWidget(listItem, sessionItem); // 设置列表项的widget
    qDebug() << "2";
    _sessionItemMap[contactInfo->_peeruid] = listItem; // 更新会话项映射
    qDebug() << "3";
    ui->side_chat_label->setSelected(true); // 选中侧边聊天按钮
    ui->side_contact_label->setSelected(false); // 取消联系人按钮的选中状态
    qDebug() << "4";
    setSelectedSessionItem(contactInfo->_peeruid); // 设置选中的会话项
    qDebug() << "5";
    setSelectedChatPage(contactInfo->_peeruid); // 更新选中的聊天页面
    qDebug() << "6";
    ui->stackedWidget->setCurrentWidget(ui->chat_page); // 切换到聊天页面
    chatUIMode = ChatUIMode::ChatMode; // 切换到聊天模式
    showSearchList(false); // 默认回到聊天列表栏
    qDebug() << "7";
}
