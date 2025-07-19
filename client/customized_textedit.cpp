#include "customized_textedit.h"

Customized_TextEdit::Customized_TextEdit(QWidget *parent) :
    QTextEdit(parent)
{
    setAcceptDrops(true); // 允许拖放事件
    this->setMaximumHeight(60);
}

QVector<messageInfo> Customized_TextEdit::createAndGetAllMessages()
{
    _allMessages.clear();
    // 首先将TextEdit的文本内容(包括图片,文件)等全部转换为plain text,
    // 遍历每一个QChar，如果是QChar::ObjectReplacementCharacter，则认为是图片(文件)，否则是文本
    QString plainText = this->document()->toPlainText();
    QString normalText; // 用来存储普通文本内容
    int userInsertedRichMessageCount = _richTextMessages.size(); // 记录用户插入的富文本消息数量
    int RichMessageIndex= 0; // 富文本消息索引，作用是在遍历过程中找到对应的富文本消息
    for (int i = 0; i < plainText.size(); ++i) {
        if (plainText.at(i) != QChar::ObjectReplacementCharacter) {
            normalText.append(plainText.at(i));
        } else { // 富文本资源
            // 首先将富文本之前的普通文本内容添加到_allMessages中
            if (!normalText.isEmpty()) {
                addMessageToVector(_allMessages, "text", normalText, QPixmap());
                normalText.clear();
            }
            // 然后添加富文本消息
            if (RichMessageIndex < userInsertedRichMessageCount) { // 说明还有需要添加的富文本
                _allMessages.append(_richTextMessages[RichMessageIndex]);
                RichMessageIndex++;
            }
        }
    }
    // 如果还有普通文本内容，则添加到_allMessages中
    if (!normalText.isEmpty()) {
        addMessageToVector(_allMessages, "text", normalText, QPixmap());
    }
    // 最后返回_allMessages
    _richTextMessages.clear();
    this->clear(); // 清空文本编辑器
    return _allMessages;
}

void Customized_TextEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) { // 检查是否有URL(图片，文件)
        event->acceptProposedAction(); // 接受拖放事件
    } else {
        event->ignore();
    }
}

void Customized_TextEdit::dropEvent(QDropEvent *event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    for (const auto& url : urls) {
        QString filePath = url.toLocalFile(); // 获取文件路径
        if (isImageFile(filePath)) {
            // 如果是图片文件，插入到文本编辑器中
            insertImageToTextEditByUrl(filePath);
        } else {
            QMessageBox::warning(this, tr("Unsupported File Type"), tr("Only richText files can be dropped here."));
        }
    }
    event->acceptProposedAction(); // 接受拖放事件
}

void Customized_TextEdit::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
        !(event->modifiers() & Qt::ShiftModifier)) {
        // 如果按下回车键或Enter键且没有Shift键，则发送消息
        emit signal_send_messages();
    } else {
        // 否则，调用基类的处理方法
        QTextEdit::keyPressEvent(event);
    }
}

bool Customized_TextEdit::isImageFile(const QString &filePath)
{
    QImageReader reader(filePath);
    if (reader.canRead()) {
        QList<QByteArray> supportedFormats = reader.supportedImageFormats();
        QByteArray currentFormat = reader.format().toLower(); // 统一转为小写
        for (const auto &format : std::as_const(supportedFormats)) {
            if (currentFormat == format.toLower()) {
                return true; // 如果当前格式在支持的格式列表中，则是图片文件
            }
        }
    }
    return false;
}

void Customized_TextEdit::insertImageToTextEditByUrl(const QString &imageUrl)
{
    QImage image(imageUrl);
    if (image.isNull()) {
        return;
    }
    // 按比例缩放
    const int maxW = 120;
    const int maxH = 80;
    if (image.width() > maxW || image.height() > maxH) {
        image = image.scaled(maxW, maxH, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    // 将图片插入到文本编辑器中
    QTextCursor cursor = this->textCursor();
    cursor.insertImage(image, imageUrl);
    this->setTextCursor(cursor); // 更新光标位置
    addMessageToVector(_richTextMessages, "image", imageUrl, QPixmap::fromImage(image));
}

bool Customized_TextEdit::addMessageToVector(QVector<messageInfo> &messageVector, const QString &type, const QString &content, const QPixmap &image)
{
    messageInfo msg;
    msg.messageType = type;
    msg.messageContent = content;
    msg.image = image;
    messageVector.append(msg);
    return true;
}
