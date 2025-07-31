#include "friendauthdialog.h"
#include "ui_friendauthdialog.h"

FriendAuthDialog::FriendAuthDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FriendAuthDialog)
    , _current_Tag_point(2, 6)
    , _current_ChosenTag_point(5, 5)
{
    ui->setupUi(this);
    setWindowTitle("同意添加好友");
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint); // 设置无边框窗口
    this->setObjectName("FriendAuthDialog");
    this->setModal(true);

    ui->backup_lineEdit->setPlaceholderText("添加好友备注");
    ui->tags_lineEdit->setPlaceholderText("搜索、添加标签");

    // 隐藏搜索图标
    ui->backup_lineEdit->removeAction(ui->backup_lineEdit->getSearchAction());
    ui->tags_lineEdit->removeAction(ui->tags_lineEdit->getSearchAction());

    ui->tags_lineEdit->MsetMaxTextLength(21); // 限制输入长度为20个字符
    ui->tags_lineEdit->move(2, 2);
    ui->tags_lineEdit->setFixedHeight(20);
    ui->input_tip_widget->hide();

    _default_tag_texts = {
        "同学", "家人", "菜鸟教程", "Python", "Java", "C++", "C#", "JavaScript",
        "前端开发", "后端开发", "全栈开发", "数据科学", "人工智能", "机器学习", "叶萌", "叶欢", "叶贤伟"
    };

    // 显示更多标签
    connect(ui->moretags_icon, &TagLabel::clicked, this, &FriendAuthDialog::slot_showMoreTags);

    // 初始化标签
    initTagLabels();

    // 设置滚动条策略
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->scrollArea->installEventFilter(this);
    ui->confirm_btn->setState("normal", "hover", "pressed");
    ui->cancel_btn->setState("normal", "hover", "pressed");

    // 连接信号槽
    connect(ui->confirm_btn, &Customized_button::clicked, this, &FriendAuthDialog::slot_confirmBtn_clicked);
    connect(ui->cancel_btn, &Customized_button::clicked, this, &FriendAuthDialog::slot_cancelBtn_clicked);
    connect(ui->tags_lineEdit, &Customized_EditLine::returnPressed, this, &FriendAuthDialog::slot_enterKeyPressed);
    connect(ui->tags_lineEdit, &Customized_EditLine::textChanged, this, &FriendAuthDialog::slot_tagLineEdit_textChanged);
    connect(ui->tags_lineEdit, &Customized_EditLine::editingFinished, this, &FriendAuthDialog::slot_tagLineEdit_editFinished);
    connect(ui->tip_label, &TagLabel::clicked, this, &FriendAuthDialog::slot_tipLabel_clicked);
}

FriendAuthDialog::~FriendAuthDialog()
{
    delete ui;
}

bool FriendAuthDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->scrollArea && event->type() == QEvent::Enter) {
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (watched == ui->scrollArea && event->type() == QEvent::Leave) {
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    return QDialog::eventFilter(watched, event); // 让基类处理其他事件
}

void FriendAuthDialog::initTagLabels()
{
    // 遍历 _default_tag_texts 创建标签
    int line = 1; // 标签行数
    int fmHeight = 0; // 字体高度
    for (const auto& tagText : _default_tag_texts) {
        Customized_Label* label = new Customized_Label(ui->all_tags_displayed);
        label->setObjectName("tagLabel");
        label->setState("normal", "hover", "pressed");
        label->setCursor(Qt::PointingHandCursor);
        label->setText(tagText);

        // 监听用户是否点击标签，从而决定是否添加到已选标签
        connect(label, &Customized_Label::clicked, this, &FriendAuthDialog::slot_checkIfTagShallAdd);

        // 计算标签文字的宽度和高度
        QFontMetrics fm(label->font());
        int textWidth = fm.horizontalAdvance(tagText);
        int textHeight = fm.height();
        fmHeight = textHeight;
        // 设置标签的位置
        if (_current_Tag_point.x() + textWidth + tags_spacing > ui->all_tags_displayed->width()) {
            // 标签够一行，换行
            line++;
            if (line > 2) { // 默认只显示两行标签, 要等待用户点击更多标签后才显示更多
                label->hide(); // TODE
                _allTagLabels[tagText] = label; // 存储标签
                _allTagLabels_keys.push_back(tagText); // 存储标签键值
                continue;
            }
            _current_Tag_point.setX(all_x_start); // 重置X坐标
            _current_Tag_point.setY(_current_Tag_point.y() + textHeight + tags_line_spacing); // 换行后Y坐标增加
        }
        // 不换行的情况下，标签位置在当前点
        QPoint next_point = _current_Tag_point;
        addTagToWidget(label, _current_Tag_point, next_point, textWidth, textHeight);
        _current_Tag_point = next_point; // 更新当前标签位置
    }
    // 设置标签显示区域的高度为两行标签的高度
    int twoLineHeight = all_y_start + fmHeight * 2 + tags_line_spacing * 2;
    ui->all_tags_displayed->setFixedHeight(twoLineHeight);
}

void FriendAuthDialog::addTagToWidget(Customized_Label *tagLabel, QPoint currentPoint, QPoint &nextPoint, int textWidth, int textHeight)
{
    Q_UNUSED(textHeight);
    tagLabel->move(currentPoint);
    tagLabel->show();
    _allTagLabels[tagLabel->text()] = tagLabel; // 存储标签
    _allTagLabels_keys.push_back(tagLabel->text()); // 存储标签键值
    nextPoint.setX(currentPoint.x() + textWidth + 15); // 更新下一个标签位置, // TODO
    nextPoint.setY(currentPoint.y()); // 保持Y坐标不变
}

void FriendAuthDialog::addToChosenTags(const QString &tagText)
{
    // 检查标签是否已经存在于已选择标签中
    if (_chosenTags.contains(tagText)) {
        qDebug() << "标签已存在于已选择标签中: " << tagText;
        return;
    }

    // 创建新的已选择标签
    ChosenTag* chosenTag = new ChosenTag(ui->tags_grid_widget);
    chosenTag->setText(tagText);
    chosenTag->setObjectName("chosenTag");
    chosenTag->setCursor(Qt::PointingHandCursor);

    // 监听移除标签的信号
    connect(chosenTag, &ChosenTag::signal_removeTag, this, &FriendAuthDialog::removeFromChosenTags);

    int maxWidth = ui->tags_grid_widget->width();
    // 判断是否需要换行
    if (chosenTag->width() + _current_ChosenTag_point.x() > maxWidth) {
        _current_ChosenTag_point.setX(2); // 换行，重置X坐标
        _current_ChosenTag_point.setY(_current_ChosenTag_point.y() + chosenTag->height() + 6); // 换行后Y坐标增加
    }
    // 不需要换行的情况下，标签位置在当前点
    chosenTag->move(_current_ChosenTag_point);
    chosenTag->show();
    _chosenTags[tagText] = chosenTag; // 存储已选择标签
    _chosenTags_keys.push_back(tagText); // 存储已选择标签键值
    // 更新下一个已选择标签位置
    _current_ChosenTag_point.setX(_current_ChosenTag_point.x() + chosenTag->width() + 2); // 更新X坐标
    _current_ChosenTag_point.setY(_current_ChosenTag_point.y()); // 保持Y坐标不变
    // 判断lineedit是否需要换行
    if (_current_ChosenTag_point.x() + MINIMUM_TAG_LINEEDIT_LENGTH > ui->tags_grid_widget->width()) {
        ui->tags_lineEdit->move(2, chosenTag->height() + _current_ChosenTag_point.y() + 2); // 换行后移动输入框
    } else {
        ui->tags_lineEdit->move(_current_ChosenTag_point); // 保持输入框位置下一个选择标签位置
    }

    // 动态调整 tags_grid_widget 的高度，防止标签过多导致显示不全
    int needHeight = _current_ChosenTag_point.y() + chosenTag->height() * 2 + 2;
    if (ui->tags_grid_widget->height() < needHeight) {
        ui->tags_grid_widget->setFixedHeight(needHeight);
    }
    // 清空输入框，准备添加下一个标签
    ui->tags_lineEdit->clear();
}

void FriendAuthDialog::removeFromChosenTags(const QString &tagText)
{
    qDebug() << "尝试删除标签: " << tagText;
    auto it = _chosenTags.find(tagText);
    if (it == _chosenTags.end()) {
        qDebug() << "标签不存在, 无法删除: " << tagText;
        return;
    }
    ChosenTag* chosenTag = it.value();
    if (!chosenTag) {
        qDebug() << "ChosenTag pointer is null for key: " << tagText;
        return;
    }
    // 从已选择标签中移除
    _chosenTags.erase(it);
    // 从标签keys中移除
    auto keyIt = std::find(_chosenTags_keys.begin(), _chosenTags_keys.end(), tagText);
    if (keyIt != _chosenTags_keys.end()) {
        _chosenTags_keys.erase(keyIt);
    } else {
        qDebug() << "Key not found in _chosenTags_keys: " << tagText;
    }
    // 删除ChosenTag对象
    chosenTag->deleteLater(); // 使用deleteLater确保在事件循环中安全删除
    // 重新布局已选择标签
    rearrangeChosenTags();

    // 取消选中标签状态
    auto labelIt = _allTagLabels.find(tagText);
    if (labelIt != _allTagLabels.end()) {
        Customized_Label* label = labelIt.value();
        if (label) {
            label->setSelected(false); // 设置标签状态为未选中
        } else {
            qDebug() << "Label pointer is null for key: " << tagText;
        }
    } else {
        qDebug() << "标签不存在于_allTagLabels中: " << tagText;
    }
}

void FriendAuthDialog::rearrangeChosenTags()
{
    // 从初始位置开始重新布局已选择标签
    _current_ChosenTag_point = QPoint(2, 6);

    for (const auto& key : _chosenTags_keys) {
        ChosenTag* chosenTag = _chosenTags.value(key);
        if (!chosenTag) {
            qDebug() << "ChosenTag pointer is null for key: " << key;
            continue;
        }

        // 检查是否需要换行
        int maxWidth = ui->tags_grid_widget->width();

        if (chosenTag->width() + _current_ChosenTag_point.x() > maxWidth) {
            _current_ChosenTag_point.setX(all_x_start); // 换行，重置X坐标
            _current_ChosenTag_point.setY(_current_ChosenTag_point.y() + chosenTag->height() + 6); // 换行后Y坐标增加
        }
        // 不需要换行的情况下，标签位置在当前点
        chosenTag->move(_current_ChosenTag_point);
        chosenTag->show();
        // 更新下一个已选择标签位置
        _current_ChosenTag_point.setX(_current_ChosenTag_point.x() + chosenTag->width() + 2); // 更新X坐标
        _current_ChosenTag_point.setY(_current_ChosenTag_point.y()); // 保持Y坐标不变
        // 判断lineedit是否需要换行
        if (_current_ChosenTag_point.x() + MINIMUM_TAG_LINEEDIT_LENGTH > ui->tags_grid_widget->width()) {
            ui->tags_lineEdit->move(2, chosenTag->height() + _current_ChosenTag_point.y() + 2); // 换行后移动输入框
        } else {
            ui->tags_lineEdit->move(_current_ChosenTag_point); // 保持输入框位置下一个选择标签位置
        }
        // 动态调整 tags_grid_widget 的高度，防止标签过多导致显示不全
        int needHeight = _current_ChosenTag_point.y() + chosenTag->height() * 2 + 2;
        if (ui->tags_grid_widget->height() < needHeight) {
            ui->tags_grid_widget->setFixedHeight(needHeight);
        } else {
            ui->tags_grid_widget->setFixedHeight(ui->tags_grid_widget->height()); // 保持高度不变
        }
    }
    // 如果已经没有任何 chosenTag，恢复初始位置
    if (_chosenTags_keys.empty()) {
        _current_ChosenTag_point = QPoint(2, 6);
        ui->tags_lineEdit->move(_current_ChosenTag_point);
        ui->tags_grid_widget->setFixedHeight(40); // 根据初始高度调整
    }
}

void FriendAuthDialog::addTagToallTagLabelsAndUpdateUI(const QString &text)
{
    // 1. 检查是否已存在该标签
    auto it = _allTagLabels.find(text);
    if (it != _allTagLabels.end()) {
        it.value()->setSelected(true); // 已存在，直接选中
        return;
    }
    // 2. 创建新标签
    Customized_Label* newTagLabel = new Customized_Label(ui->all_tags_displayed);
    newTagLabel->setObjectName("tagLabel");
    newTagLabel->setState("normal", "hover", "pressed");
    newTagLabel->setCursor(Qt::PointingHandCursor);
    newTagLabel->setText(text);
    newTagLabel->setSelected(true); // 设置标签状态为选中
    // 监听用户是否点击标签，从而决定是否添加到已选标签
    connect(newTagLabel, &Customized_Label::clicked, this, &FriendAuthDialog::slot_checkIfTagShallAdd);
    // 3. 计算标签尺寸
    QFontMetrics fm(newTagLabel->font());
    int textWidth = fm.horizontalAdvance(text);
    int textHeight = fm.height();
    // 4. 检查是否需要换行
    if (_current_Tag_point.x() + textWidth + tags_spacing > ui->all_tags_displayed->width()) {
        // 换行
        _current_Tag_point.setX(all_x_start); // 重置X坐标
        _current_Tag_point.setY(_current_Tag_point.y() + textHeight + 15); // 换行后Y坐标增加
    }
    // 5.不需要换行的情况下，标签位置在当前点
    QPoint next_point = _current_Tag_point;
    addTagToWidget(newTagLabel, _current_Tag_point, next_point, textWidth, textHeight);
    _current_Tag_point = next_point; // 更新当前标签位置

    // 动态调整 all_tags_displayed 的高度，防止标签过多导致显示不全
    int needHeight = _current_Tag_point.y() + textHeight + tags_spacing;
    if (ui->all_tags_displayed->height() < needHeight) {
        ui->all_tags_displayed->setFixedHeight(needHeight);
    } else {
        ui->all_tags_displayed->setFixedHeight(ui->all_tags_displayed->height()); // 保持高度不变
    }
}

void FriendAuthDialog::relayoutAllTags()
{
    QPoint point(all_x_start, all_y_start);
    int textHeight = 0;
    int currentLine = 1; // 当前行数

    for (const auto &key : _allTagLabels_keys) {
        Customized_Label* label = _allTagLabels.value(key);
        if (!label) continue;

        QFontMetrics fm(label->font());
        int textWidth = fm.horizontalAdvance(label->text());
        textHeight = fm.height();

        // 是否需要换行
        if (point.x() + textWidth + tags_spacing > ui->all_tags_displayed->width()) {
            point.setX(all_x_start);
            point.setY(point.y() + textHeight + tags_line_spacing);\
                currentLine++;
        }

        // 如果不是显示全部，并且行数大于 2，就隐藏
        if (!_isShowingAllTags && currentLine > 2) {
            label->hide();
            continue;
        }
        // 否则正常布局显示
        label->move(point);
        label->show();
        qDebug() << "标签位置: " << point << ", 文本: " << label->text();

        point.setX(point.x() + textWidth + tags_spacing);
    }

    // 根据状态调整高度
    int needHeight = point.y() + textHeight + tags_spacing;
    if (!_isShowingAllTags) {
        // 只显示两行的高度
        QFontMetrics fm(font());
        int twoLineHeight = all_y_start + fm.height() * 2 + tags_line_spacing * 2;
        ui->all_tags_displayed->setFixedHeight(twoLineHeight);
    } else {
        // 显示全部的高度
        if (needHeight > ui->all_tags_displayed->height()) {
            ui->all_tags_displayed->setFixedHeight(needHeight);
        }
    }
}

void FriendAuthDialog::slot_showMoreTags()
{
    // 显示更多按钮隐藏
    // ui->moretags_icon->hide();
    // ui->all_tags_displayed->setFixedWidth(325); // 设置更多标签容器宽度
    _isShowingAllTags = !_isShowingAllTags; // 切换状态
    relayoutAllTags(); // 重新布局所有标签
}

void FriendAuthDialog::slot_checkIfTagShallAdd()
{
    // 将sender()转换为Customized_Label指针
    Customized_Label* label = qobject_cast<Customized_Label*>(sender());
    if (!label) {
        qDebug() << "标签指针转换失败，无法处理点击事件";
        return;
    }
    qDebug() << "检查选择标签是否添加/删除: " << label->text();
    // 查询这个标签是否已经在已选区
    if (_chosenTags.contains(label->text())) {
        // 在已选区 -> 删除
        removeFromChosenTags(label->text());
        label->setSelected(false); // 取消选中
    } else {
        // 不在已选区 -> 添加
        addToChosenTags(label->text());
        label->setSelected(true);  // 选中
    }
}

void FriendAuthDialog::slot_enterKeyPressed()
{
    auto Text = ui->tags_lineEdit->text().trimmed();
    if (Text.isEmpty()) {
        return; // 如果输入框为空，直接返回
    }
    addToChosenTags(Text); // 添加到已选择标签
    ui->input_tip_widget->hide(); // 隐藏输入提示

    // 根据情况是否添加标签到_allTagLabels并更新UI
    addTagToallTagLabelsAndUpdateUI(Text);
    relayoutAllTags();
}

void FriendAuthDialog::slot_tagLineEdit_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        ui->tip_label->setText("");
        ui->input_tip_widget->hide();
        return; // 如果输入框为空，直接返回
    }
    auto it = _allTagLabels.find(text);
    if (it == _allTagLabels.end()) {
        // 标签不存在，显示加上默认提示
        ui->tip_label->setText(default_addTag_tip + text);
        ui->input_tip_widget->show();
        return;
    } else {
        // 标签已存在, 普通提示
        ui->tip_label->setText(text);
        ui->input_tip_widget->show();
    }
}

void FriendAuthDialog::slot_tagLineEdit_editFinished()
{
    ui->input_tip_widget->hide(); // 编辑完成后隐藏输入提示
}

void FriendAuthDialog::slot_tipLabel_clicked()
{
    // 从tipLabel中找到default_addTag_tip = "添加标签: "后的文本
    QString tagText = ui->tip_label->text().mid(default_addTag_tip.length()).trimmed();
    if (tagText.isEmpty()) {
        return; // 如果标签文本为空，直接返回
    }
    addToChosenTags(tagText); // 添加到已选择标签
    ui->input_tip_widget->hide(); // 隐藏输入提示

    // 根据情况是否添加标签到_allTagLabels并更新UI
    addTagToallTagLabelsAndUpdateUI(tagText);
}

void FriendAuthDialog::slot_confirmBtn_clicked()
{
    qDebug() <<"confirmBtn clicked";
}

void FriendAuthDialog::slot_cancelBtn_clicked()
{
    this->close(); // 关闭对话框
    for (auto* tag : _chosenTags) tag->deleteLater();
    for (auto* tag : _allTagLabels) tag->deleteLater();
    _chosenTags.clear();
    _chosenTags_keys.clear();
    _allTagLabels.clear();
    _allTagLabels_keys.clear();
    _current_Tag_point = QPoint(2, 6);
    _current_ChosenTag_point = QPoint(5, 5);
}
