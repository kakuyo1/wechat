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
    });

    connect(ui->side_contact_label, &StateWidget::clicked, this, [this]() {
        ui->chat_list->hide();
        ui->search_list->hide();
        ui->contact_list->show();
        ui->side_contact_label->clearState();
        ui->stackedWidget->setCurrentWidget(ui->contact_info_page); // 切换到联系人页面
        chatUIMode = ChatUIMode::ContactMode; // 切换到联系人模式
    });

    // 为searchList 设置lineEdit
    ui->search_list->setLineEdit(ui->search_lineEdit);

    // 设置会话列表的加载更多事件
    connect(ui->chat_list, &SessionList::signal_loading_sessionItems, this, &ChatDialog::slot_load_more_sessionitems);

    // 加上联系人列表的加载更多事件
    connect(ui->contact_list, &ContactList::signal_load_contacts, this, &ChatDialog::slot_load_more_contactitems);

    // 切换到好友申请页面
    connect(ui->contact_list, &ContactList::signal_switchTo_friendRequestPage, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->friend_request_page); // 切换到好友申请页面
    });

    // side_contact_label的红点状态
    connect(ui->friend_request_page, &FriendRequestPage::signal_sideContact_showRedPoint, this, [this]() {
        ui->side_contact_label->showRedPoint(true); // 显示红点
    });

    // 安装事件过滤器
    this->installEventFilter(this);

    // Test 添加会话列表项
    Test_addSessionItem();

    initializeContactList(); // 初始化联系人列表
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
        item->setSessionInfo(names[name_i], heads[head_i], strs[str_i]);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        ui->chat_list->addItem(listItem); // 添加列表项
        ui->chat_list->setItemWidget(listItem, item); // 设置列表项的widget
    }
}

void ChatDialog::Test_AddMoreContacts()
{
    for (int i = 0; i < 13; ++i) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();
        auto *item = new ContactListItem(this);
        item->setInfo(0, names[name_i], heads[head_i]);
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        ui->contact_list->addItem(listItem); // 添加列表项
        ui->contact_list->setItemWidget(listItem, item); // 设置列表项的widget
    }
}

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
        item->setInfo(contactInfo->getUid(), contactInfo->getName(), contactInfo->getIcon());
        QListWidgetItem *listItem = new QListWidgetItem();
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        ui->contact_list->addItem(listItem); // 添加列表项
        ui->contact_list->setItemWidget(listItem, item); // 设置列表项的widget
    }
    currentContactIndex = endIndex; // 更新当前联系人索引
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
        Test_addSessionItem(); // 测试添加更多会话项
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
