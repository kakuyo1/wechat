#include "searchlist.h"
#include "chatdialog.h"
#include "tcpmanager.h"
#include "customized_editline.h"

SearchList::SearchList(QWidget *parent) :
    QListWidget(parent),
    _searchContactSuccessedDialog(std::make_shared<searchContactSuccessedDialog>(this)),
    _loadingDialog(new LoadingDialog(this)),
    _isLoading(false),
    _search_lineEdit(nullptr) // 初始化搜索框指针为nullptr
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    intialAddContactTipItem(); // 初始化添加联系人提示项

    // connect the itemclicked signal to a slot
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_itemClicked);

    // 连接搜索用户成功/失败的信号到槽函数
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_search_user_success,
            this, &SearchList::slot_search_user_success);
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_search_user_failed,
            this, &SearchList::slot_search_user_failed);
}

void SearchList::setLineEdit(QWidget *searchLineEdit)
{
    _search_lineEdit = searchLineEdit; // 设置搜索框
}

bool SearchList::eventFilter(QObject *watched, QEvent *event)
{
    // 处理鼠标进入离开时滚动条的显示和隐藏
    if (watched == this->viewport() && event->type() == QEvent::Enter) {
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (watched == this->viewport() && event->type() == QEvent::Leave) {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    // 处理鼠标滚动事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
        int steps = wheelEvent->angleDelta().y() / 120; // steps 表示滚了几格。
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - steps * this->verticalScrollBar()->singleStep());

        // // 如果滚动条已经到达底部，则加载更多内容
        // if (this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum()) {
        //     emit signal_loading_searchItems(); // 发出加载更多会话项的信号
        // }
        return true;
    }
    // 其他事件交给基类处理
    return QListWidget::eventFilter(watched, event);
}

void SearchList::intialAddContactTipItem()
{
    // 视觉分隔、留出间距的高度 10 像素的不可选中空白项
    QWidget * untouchableWidget = new QWidget(this);
    untouchableWidget->setObjectName("untouchableWidget");
    QListWidgetItem *listItem = new QListWidgetItem();
    listItem->setSizeHint(QSize(250, 10));
    this->addItem(listItem);
    this->setItemWidget(listItem, untouchableWidget);
    listItem->setFlags(listItem->flags() & ~Qt::ItemIsSelectable); // 设置不可选中

    // 添加联系人提示项
    AddContactItem *addContactItem = new AddContactItem(this);
    QListWidgetItem *addContactListItem = new QListWidgetItem();
    addContactListItem->setSizeHint(addContactItem->sizeHint());
    this->addItem(addContactListItem);
    this->setItemWidget(addContactListItem, addContactItem);
}

bool SearchList::isPureDigit(const QString &str) const
{
    // 检查字符串是否全为数字
    for (const QChar &c : str) {
        if (!c.isDigit()) {
            return false; // 只要有一个字符不是数字，就返回false
        }
    }
    return !str.isEmpty(); // 如果字符串不为空且全为数字，返回true
}

void SearchList::slot_itemClicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item);
    if (widget == nullptr) {
        qDebug() << "Clicked item widget is null";
        return;
    }
    BaseListItem *baseItem = qobject_cast<BaseListItem *>(widget);
    if (baseItem == nullptr) {
        qDebug() << "Clicked item is not a BaseListItem";
        return;
    }
    ListItemType itemType = baseItem->getListItemType();
    if (itemType == ListItemType::NonClickableItem) {
        qDebug() << "Clicked item is a non-clickable item";
        return;
    }
    if (itemType == ListItemType::AddContactTipItem) {
        auto search_lineedit = dynamic_cast<Customized_EditLine*>(_search_lineEdit);
        if (search_lineedit == nullptr) {
            qDebug() << "Search line edit is null";
            return;
        }
        QString search_content = search_lineedit->text().trimmed(); // 获取搜索内容并去除首尾空格
        if (search_content.isEmpty()) {
            qDebug() << "Search content is empty";
            return;
        }
        // 显示加载对话框
        if (!_isLoading && _loadingDialog) {
            _isLoading = true;
            _loadingDialog->show();
        }
        // 如果是全数字，发送uid，否则发送name
        QTimer::singleShot(0, [=]() { // 延迟执行，确保加载对话框显示, 使用 QTimer 避免阻塞 UI
            if (isPureDigit(search_content)) {
            QJsonObject jsonObj;
            jsonObj["uid"] = search_content.toInt(); // 发送uid
            QJsonDocument doc(jsonObj);
            QString jsonData = doc.toJson(QJsonDocument::Compact);
            emit TcpManager::GetInstance()->signal_send_data(RequestType::MESSAGE_CLIENT_SEARCH_USER_REQUEST, jsonData);
        } else {
            QJsonObject jsonObj;
            jsonObj["name"] = search_content; // 发送name
            QJsonDocument doc(jsonObj);
            QString jsonData = doc.toJson(QJsonDocument::Compact);
            emit TcpManager::GetInstance()->signal_send_data(RequestType::MESSAGE_CLIENT_SEARCH_USER_REQUEST, jsonData);
        }
        });
    }
}

void SearchList::slot_search_user_success(std::shared_ptr<SearchInfo> searchInfoPtr)
{
    // 情况1，未添加好友
    if (_loadingDialog) {
        _loadingDialog->close(); // 关闭加载对话框
        _isLoading = false;
    }
    if (searchInfoPtr) {
        _searchContactSuccessedDialog->setContactInfo(searchInfoPtr);
        _searchContactSuccessedDialog->show();
    } else {
        qDebug() << "Search user success but searchInfoPtr is null";
    }
    // TODO 情况2，已经添加好友，直接跳转到聊天界面
}

void SearchList::slot_search_user_failed(QString errorMessage)
{
    if (_loadingDialog) {
        _loadingDialog->close(); // 关闭加载对话框
        _isLoading = false;
    }
    if (_searchContactFailedDialog == nullptr) {
        _searchContactFailedDialog = std::make_shared<searchcontactfaileddialog>(this);
    }
    _searchContactFailedDialog->show();
    qDebug() << "Search user failed with error message:" << errorMessage;
}
