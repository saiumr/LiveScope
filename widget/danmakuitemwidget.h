#ifndef DANMAKUITEMWIDGET_H
#define DANMAKUITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QPalette>
#include "model/Danmaku.h"

class DanmakuItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DanmakuItemWidget(const Danmaku& danmaku, QWidget *parent = nullptr);

protected:
    void changeEvent(QEvent* event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QLabel* m_levelLabel;
    QLabel* m_adminLabel;
    QLabel* m_scLabel;
    QLabel* m_nameLabel;
    QLabel* m_timeLabel;
    QLabel* m_textLabel;
    QString m_uid;

    void updateLabelStyle();
};

#endif // DANMAKUITEMWIDGET_H
