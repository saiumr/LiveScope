#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHash>
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QNetworkAccessManager>
#include "widget/roomwidget.h"
#include "network/bilibiliroominitapi.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addRoom();
    void onRoomVerifyOk(QString realRoomId, QString displayName);
    void onRoomVerifyFail(QString errMsg);

private:
    Ui::MainWindow*         ui;
    QHash<RoomWidget*, int> m_roomTabs;
    QToolBar*               m_toolBar;
    QAction*                m_addRoomAction;
    BilibiliRoomInitApi*       m_roomInitApi;
};
#endif // MAINWINDOW_H
