#include "chatdialog.h"
#include "ui_chatdialog.h"
#include "sessionlistitem.h"

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
    ui->search_lineEdit->setFocusPolicy(Qt::ClickFocus); // 设置搜索输入框的焦点策略为点击获取焦点
    // 初始显示chat_list,隐藏search_list和contact_list
    ui->chat_list->show();
    ui->search_list->hide();
    ui->contact_list->hide();

    // TODO加载头像
    QPixmap avatarPixmap(":/images/head_1.jpg");
    avatarPixmap = avatarPixmap.scaled(ui->side_icon_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->side_icon_label->setPixmap(avatarPixmap);
    ui->side_icon_label->setScaledContents(true); // 确保图标适应标签大小

    // 设置搜索输入框焦点变化时搜索页面的显示和隐藏
    connect(ui->search_lineEdit, &Customized_EditLine::signal_focusIn, this, [this](){
        showSearchList(true); // 当获得焦点时显示搜索列表
    });
    connect(ui->search_lineEdit, &Customized_EditLine::signal_focusOut, this, [this]() {
        showSearchList(false); // 当失去焦点时隐藏搜索列表
    });

    // 设置侧边栏按钮的点击事件，进行contents_panal页面的切换
    connect(ui->side_chat_label, &StateWidget::clicked, this, [this]() {
        ui->contact_list->hide();
        ui->search_list->hide();
        ui->chat_list->show();
        chatUIMode = ChatUIMode::ChatMode; // 切换到聊天模式
    });

    connect(ui->side_contact_label, &StateWidget::clicked, this, [this]() {
        ui->chat_list->hide();
        ui->search_list->hide();
        ui->contact_list->show();
        chatUIMode = ChatUIMode::ContactMode; // 切换到联系人模式
    });

    // 设置会话列表的加载更多事件
    connect(ui->chat_list, &SessionList::signal_loading_sessionItems, this, &ChatDialog::slot_load_more_sessionitems);

    // Test 添加会话列表项
    Test_addSessionItem();
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

QSize ChatDialog::sizeHint() const
{
    return QSize(1225, 810);
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
