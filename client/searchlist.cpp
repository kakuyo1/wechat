#include "searchlist.h"

SearchList::SearchList(QWidget *parent) :
    QListWidget(parent),
    _searchContactSuccessedDialog(std::make_shared<searchContactSuccessedDialog>(this))
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->viewport()->installEventFilter(this);
    intialAddContactTipItem(); // 初始化添加联系人提示项

    // connect the itemclicked signal to a slot
    connect(this, &QListWidget::itemClicked, this, &SearchList::slot_itemClicked);
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
    if (itemType == ListItemType::AddContactTipItem) {
        if (!_searchContactSuccessedDialog) {
            qDebug() << "Dialog pointer is null";
            return;
        }

        std::shared_ptr<SearchInfo> contactInfoPtr = std::make_shared<SearchInfo>(0, "张三", "小张", "这是一个测试用户", 1);
        _searchContactSuccessedDialog->setContactInfo(contactInfoPtr);
        _searchContactSuccessedDialog->show();
    }
}
