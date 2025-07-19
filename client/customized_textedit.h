#ifndef CUSTOMIZED_TEXTEDIT_H
#define CUSTOMIZED_TEXTEDIT_H
#include <QWidget>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeType>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData> // 用于拖拽事件(用来在不同程序或部件之间传递数据, 如拖曳)
#include <QVector>
#include <QPainter>
#include <QStyleOption>
#include <QKeyEvent>
#include <QList>
#include <QUrl>
#include <QMessageBox>
#include <QImageReader>
#include <QStringList>
#include "global.h"
/*
✅ 支持输入文字
✅ 支持拖拽图片到输入框
    dragEnterEvent:当用户拖动一个对象（如文件、文本等）首次进入目标控件（或窗口）的边界时触发。
        1.通过 event->acceptProposedAction() 或 event->accept() 表示允许拖放。
        2.通过 event->ignore() 表示拒绝拖放（后续的 dropEvent 不会被触发）。
    dropEvent:当用户释放鼠标按钮，将拖动的对象放置到目标控件（或窗口）时触发。
        1.通过 event->mimeData() 获取拖拽的数据（如文件路径、文本内容等）。
        2.通过 event->setDropAction() 可以修改拖放的行为（如复制、移动等）。
✅ 支持拖拽文件到输入框，并显示一个文件图标和文件信息
✅ 按下 Enter 键直接发送消息，Shift+Enter 换行
✅ 能把输入框里的内容（文字、图片、文件）整理成一个消息列表 QVector<MsgInfo>

    消息类型有 "text"  "image"
*/

class Customized_TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit Customized_TextEdit(QWidget* parent = nullptr);
    ~Customized_TextEdit() = default;
    QVector<messageInfo> createAndGetAllMessages();
protected:
    virtual void dragEnterEvent(QDragEnterEvent* event) override; // 拖拽进入事件
    virtual void dropEvent(QDropEvent* event) override; // 拖拽放下事件
    virtual void keyPressEvent(QKeyEvent* event) override; // 键盘按下事件
private:
    bool isImageFile(const QString& filePath); // 判断文件是否为图片
    void insertImageToTextEditByUrl(const QString& imageUrl); // 插入图片到文本编辑器
    bool addMessageToVector(QVector<messageInfo>& messageVector,
                            const QString& type, const QString& content, const QPixmap& image);
private:
    QVector<messageInfo> _allMessages; // 存储所有类型的消息信息
    QVector<messageInfo> _richTextMessages; // 存储富文本消息(图片，文件)
signals:
    void signal_send_messages();
};

#endif // CUSTOMIZED_TEXTEDIT_H
