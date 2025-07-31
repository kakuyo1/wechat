#ifndef FRIENDREQUESTDIALOG_H
#define FRIENDREQUESTDIALOG_H

#include <QDialog>
#include <vector>
#include <QString>
#include <contactinfodata.h>
#include <memory>
#include <QMap>
#include "taglabel.h"
#include "customized_label.h"
#include "chosentag.h"
#include <QEvent>
#include <QScrollBar>
#include <QScrollArea>
#include <QJsonObject>
#include <QJsonDocument>

namespace Ui {
class FriendRequestDialog;
}

constexpr int tags_spacing = 15; // 标签间距
constexpr int all_x_start = 2;
constexpr int all_y_start = 6; // 所有标签初始位置
constexpr int tags_line_spacing = 15;
constexpr int MINIMUM_TAG_LINEEDIT_LENGTH = 40; // 标签输入框最小长度
const QString default_addTag_tip = "添加标签: "; // 默认添加标签提示文本

class FriendRequestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FriendRequestDialog(QWidget *parent = nullptr);
    ~FriendRequestDialog();
    void setContactInfo(std::shared_ptr<SearchInfo> contactInfo); // 设置联系人信息
    virtual bool eventFilter(QObject *watched, QEvent *event) override; // 事件过滤器，用于处理标签点击事件
    void initTagLabels(); // 初始化标签
private:
    Ui::FriendRequestDialog *ui;
    QMap<QString, Customized_Label*> _allTagLabels; // 存储所有的标签
    std::vector<QString> _allTagLabels_keys; // 存储所有标签的键值
    QPoint _current_Tag_point; // 当前标签(属于_allTagLabels)的位置
    QMap<QString, ChosenTag*> _chosenTags; // 存储已选择的标签
    std::vector<QString> _chosenTags_keys; // 存储已选择标签的键值
    QPoint _current_ChosenTag_point; // 当前已选择标签的位置
    std::shared_ptr<SearchInfo> _contactInfo; // 存储联系人信息()
    std::vector<QString> _default_tag_texts; // 默认的一些标签文本,用于测试
    bool _isShowingAllTags = false; // 默认只显示两行
private:
    void addTagToWidget(Customized_Label* tagLabel, QPoint currentPoint, QPoint & nextPoint, int textWidth, int textHeight);
    void addToChosenTags(const QString &tagText); // 添加到已选择标签
    void removeFromChosenTags(const QString &tagText); // 从已选择标签中移除
    void rearrangeChosenTags(); // 重新布局
    void addTagToallTagLabelsAndUpdateUI(const QString &text); // 添加标签到_allTagLabels并更新UI
    void relayoutAllTags(); // 重新布局所有标签
public slots:
    void slot_showMoreTags(); // 显示更多标签
    void slot_checkIfTagShallAdd(); // 检查标签是否可以添加
    void slot_enterKeyPressed(); // 处理标签行回车键按下事件
    void slot_tagLineEdit_textChanged(const QString &text); // 标签输入框文本改变事件
    void slot_tagLineEdit_editFinished(); // 标签输入框编辑完成事件
    void slot_tipLabel_clicked(); // 标签提示点击事件
    void slot_confirmBtn_clicked();
    void slot_cancelBtn_clicked();
};

#endif // FRIENDREQUESTDIALOG_H
