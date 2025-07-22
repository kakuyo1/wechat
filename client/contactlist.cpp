#include "contactlist.h"

ContactList::ContactList(QWidget *parent) :
    QListWidget(parent),
    _addContactItem(new ContactListItem(this))
{
    // 设置滚动条
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 安装事件过滤器
    this->viewport()->installEventFilter(this);

    // 设置组"新的朋友"项
    auto * newFriendGroupItem = new ContactGroupTipItem(this);
    newFriendGroupItem->setObjectName("new_friend_group_item");
    QListWidgetItem* groupListItem = new QListWidgetItem(this);
    groupListItem->setSizeHint(newFriendGroupItem->sizeHint());
    groupListItem->setFlags(groupListItem->flags() & ~Qt::ItemIsSelectable); // 设置不可选中
    this->addItem(groupListItem);
    this->setItemWidget(groupListItem, newFriendGroupItem);

    // 设置添加联系人项
    _addContactItem->setObjectName("add_contact_item");
    _addContactItem->setInfo(0, "新的朋友", ":/images/add_friend_pressed.png");
    _addContactItem->setListItem(ListItemType::AddContactItem);
    QListWidgetItem* addContactListItem = new QListWidgetItem(this);
    addContactListItem->setSizeHint(_addContactItem->sizeHint());
    this->addItem(addContactListItem);
    this->setItemWidget(addContactListItem, _addContactItem);

    // 设置组"联系人"项
    auto * contactGroupItem = new ContactGroupTipItem(this);
    contactGroupItem->setObjectName("contact_group_item");
    QListWidgetItem* contactGroupListItem = new QListWidgetItem(this);
    contactGroupListItem->setSizeHint(contactGroupItem->sizeHint());
    contactGroupListItem->setFlags(contactGroupListItem->flags() & ~Qt::ItemIsSelectable); // 设置不可选中
    this->addItem(contactGroupListItem);
    this->setItemWidget(contactGroupListItem, contactGroupItem);

    // 连接信号槽
    connect(this, &QListWidget::itemClicked, this, &ContactList::slot_contactItem_clicked);

    // 初始化测试添加联系人
    Test_AddContacts();
}

void ContactList::showAddContactRedPoint(bool show)
{
    _addContactItem->showRedPoint(show);
}

bool ContactList::eventFilter(QObject *watched, QEvent *event)
{
    // 处理鼠标进入离开滚动条的显示与隐藏
    if (watched == this->viewport() && event->type() == QEvent::Enter) {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (watched == this->viewport() && event->type() == QEvent::Leave) {
        this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    // 处理鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
        if (wheelEvent) {
            // 处理滚轮事件
            int steps = wheelEvent->angleDelta().y() / 120; // 每120个单位为一个步长
            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - steps * this->verticalScrollBar()->singleStep());

            // 到达底层加载更多联系人
            if (this->verticalScrollBar()->value() >= this->verticalScrollBar()->maximum()) {
                emit signal_load_contacts(); // 发出加载联系人信号
                return true;
            }
        }
    }
    return QListWidget::eventFilter(watched, event); // 让基类处理其他事件
}

void ContactList::Test_AddContacts()
{
    for (int i = 0; i < 13; ++i) {
        int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
        int head_i = randomValue%heads.size();
        int name_i = randomValue%names.size();
        auto *item = new ContactListItem(this);
        item->setInfo(0, names[name_i], heads[head_i]);
        QListWidgetItem *listItem = new QListWidgetItem(this);
        listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
        this->addItem(listItem); // 添加列表项
        this->setItemWidget(listItem, item); // 设置列表项的widget
    }
}

void ContactList::slot_contactItem_clicked(QListWidgetItem* item)
{
    QWidget* widget = this->itemWidget(item);
    if (!widget) {
        qDebug() << "点击的项没有关联的widget";
        return;
    }

    BaseListItem* baseItem = static_cast<BaseListItem*>(widget);
    ListItemType itemType = baseItem->getListItemType();
    if (!baseItem) {
        qDebug() << "点击的项不是BaseListItem类型";
        return;
    }

    // 根据不同的ListItemType处理点击事件
    if (itemType == ListItemType::AddContactItem) {
        qDebug() << "点击了添加联系人项";
        emit signal_switchTo_friendRequestPage(); // 切换到好友申请页面
        return;
    } else if (itemType == ListItemType::ContactItem) {
        qDebug() << "点击了联系人项";
        emit signal_switchTo_contactInfoPage(); // 切换到联系人信息页面
        return;
    } else if (itemType == ListItemType::ContactGroupTipItem || itemType == ListItemType::NonClickableItem) {
        qDebug() << "点击了分组提示项";
        return;
    }
}
