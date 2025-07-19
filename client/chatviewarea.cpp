#include "chatviewarea.h"
#include "ui_chatviewarea.h"

ChatViewArea::ChatViewArea(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatViewArea)
{
    ui->setupUi(this);
    ui->scrollArea->viewport()->setStyleSheet("background-color: #E8E8E8; border:none;");
    // 取出scrollArea内部的scrollAreaWidgetContents
    QWidget* contents = ui->scrollArea->widget();
    // 创建垂直布局并设置给contents
    _messageLayout = new QVBoxLayout(contents);
    _messageLayout->setContentsMargins(0, 0, 0, 0);
    _messageLayout->setSpacing(25);
    _messageLayout->setAlignment(Qt::AlignTop); // 设置对齐方式为顶部对齐
    _messageLayout->addStretch(1); // 添加一个伸缩项，确保内容从顶部开始显示(布局从上往下排列消息，最后的伸缩项会占据剩余空间把消息顶上去)
    contents->setLayout(_messageLayout);

    // 设置有新消息时自动滚动到最底部
    connect(ui->scrollArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, [this](){
        ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
    });
    // 安装事件过滤器
    ui->scrollArea->viewport()->installEventFilter(this);

    // testAddMessages();  // 直接调用测试
}

ChatViewArea::~ChatViewArea()
{
    delete ui;
}

void ChatViewArea::appendMessageWidget(QWidget *item)
{
    if (_messageLayout) { // 新消息插入到伸缩项前面，确保消息顺序从上到下。
        _messageLayout->insertWidget(_messageLayout->count() - 1, item); // 消息从顶部开始添加
    }
}

void ChatViewArea::testAddMessages()
{
    for (int i = 0; i < 50; ++i) {
        QLabel *msg = new QLabel(QString("Message %1").arg(i + 1));
        msg->setWordWrap(true); // 自动换行
        msg->setStyleSheet(i % 2 == 0
                               ? "background-color: #E0E0E0; border-radius: 5px; padding: 5px;"
                               : "background-color: #ADD8E6; border-radius: 5px; padding: 5px; color: black;");

        // 偶数条左对齐，奇数条右对齐模拟左右聊天气泡
        if (i % 2 == 0) {
            msg->setAlignment(Qt::AlignLeft);
        } else {
            msg->setAlignment(Qt::AlignRight);
        }

        appendMessageWidget(msg);
    }
}

void ChatViewArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

bool ChatViewArea::eventFilter(QObject *watched, QEvent *event)
{
    // 检测鼠标的进入和离开scrollArea事件
    if (watched == ui->scrollArea->viewport()) {
        if (event->type() == QEvent::Enter) {
            ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    return QWidget::eventFilter(watched, event); // 继续处理其他事件
}
