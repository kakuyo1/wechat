#include "contactlist.h"
#include "tcpmanager.h"
#include <QTimer>

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
    newFriendGroupItem->setGroupTipText("添加联系人");
    QListWidgetItem* groupListItem = new QListWidgetItem(this);
    groupListItem->setSizeHint(newFriendGroupItem->sizeHint());
    groupListItem->setFlags(groupListItem->flags() & ~Qt::ItemIsSelectable); // 设置不可选中
    this->insertItem(0, groupListItem);
    this->setItemWidget(groupListItem, newFriendGroupItem);

    // 设置添加联系人项
    _addContactItem->setObjectName("add_contact_item");
    _addContactItem->setAddContactItemInfoByLocal("新的朋友", ":/images/add_friend_pressed.png");
    _addContactItem->setListItem(ListItemType::AddContactItem);
    QListWidgetItem* addContactListItem = new QListWidgetItem(this);
    addContactListItem->setSizeHint(_addContactItem->sizeHint());
    this->insertItem(1, addContactListItem);
    this->setItemWidget(addContactListItem, _addContactItem);

    // 设置组"联系人"项
    auto contactGroupItem = new ContactGroupTipItem(this);
    contactGroupItem->setObjectName("contact_group_item");
    contactGroupItem->setGroupTipText("联系人");
    contactGroupListItem = new QListWidgetItem(this);
    contactGroupListItem->setSizeHint(contactGroupItem->sizeHint());
    contactGroupListItem->setFlags(contactGroupListItem->flags() & ~Qt::ItemIsSelectable); // 设置不可选中
    this->insertItem(2, contactGroupListItem);
    this->setItemWidget(contactGroupListItem, contactGroupItem);

    // 连接信号槽
    connect(this, &QListWidget::itemClicked, this, &ContactList::slot_contactItem_clicked);

    // 处理添加新联系人项的ACK响应
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_getACK_auth_friend_request_success_addNewItem, this, &ContactList::slot_AfterACK_addNewContactItem);
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_getPush_auth_friend_request_success, this, &ContactList::slot_AfterACK_addNewContactItem);

    // 添加联系人项红点亮起
    connect(TcpManager::GetInstance().get(), &TcpManager::signal_addcontactlistitem_showRedPoint, this, [this]() {
        _addContactItem->showRedPoint(true); // 显示红点
    });

    // QTimer::singleShot(0, this, [this]() {
    //     if (this->count() > 3) {
    //         QListWidgetItem* item = this->item(3);
    //         this->setCurrentItem(item);
    //         this->slot_contactItem_clicked(item);
    //     }
    // });

    // 初始化测试添加联系人
    // Test_AddContacts();
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

// void ContactList::Test_AddContacts()
// {
//     for (int i = 0; i < 13; ++i) {
//         int randomValue = QRandomGenerator::global()->bounded(100); // 生成0到99之间的随机整数
//         int head_i = randomValue%heads.size();
//         int name_i = randomValue%names.size();
//         auto *item = new ContactListItem(this);
//         item->setInfo(0, names[name_i], heads[head_i]);
//         QListWidgetItem *listItem = new QListWidgetItem(this);
//         listItem->setSizeHint(item->sizeHint()); // 设置列表项的大小提示
//         this->addItem(listItem); // 添加列表项
//         this->setItemWidget(listItem, item); // 设置列表项的widget
//     }
// }

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
        // widget 转换为 ContactListItem 类型
        ContactListItem* contactItem = qobject_cast<ContactListItem*>(widget);
        // 取消红点
        contactItem->showRedPoint(false);
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

void ContactList::slot_AfterACK_addNewContactItem(std::shared_ptr<AuthResponse> response)
{
    // 添加新的联系人项，位置为 "新的朋友" contactGroupItem分组下方，
    // 也就是临时放在所有联系人的第一位，下次登陆重新加载则会到最下方
    qDebug() << "处理添加新联系人项的ACK响应";
    if (!response) {
        qDebug() << "响应为空，无法添加新联系人项";
        return;
    }
    // 创建新的联系人项
    ContactListItem* newContactItem = new ContactListItem(this);
    newContactItem->setInfo(response); // 设置联系人信息
    newContactItem->setListItem(ListItemType::ContactItem); // 设置为联系人项类型
    // 创建新的列表项
    QListWidgetItem* newListItem = new QListWidgetItem(); // ❌不要传 this！因为立刻就把 newListItem 添加到了QListWidget 的末尾，相当于执行了 addItem()。
    //你后续再调用 insertItem(groupIndex + 1, newListItem) 是无效的 —— Qt 不会把已存在的 item 从末尾“搬到中间”。
    newListItem->setSizeHint(newContactItem->sizeHint()); // 设置列表项的大小提示
    // 获取contactGroupItem的索引
    int groupIndex = this->row(contactGroupListItem);

    // 在contactGroupItem后插入新联系人
    this->insertItem(groupIndex + 1, newListItem);
    this->setItemWidget(newListItem, newContactItem);
    qDebug() << "添加新的联系人项成功";
}
