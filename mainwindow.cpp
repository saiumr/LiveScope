#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_toolBar(new QToolBar(this))
    , m_addRoomAction(new QAction("+ Add Room", this))
    , m_roomInitApi(new BilibiliRoomInitApi(this))
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/app.png"));

    // ==== UI design ====
    // + Button
    addToolBar(Qt::TopToolBarArea, m_toolBar);
    m_toolBar->setMovable(false);
    m_addRoomAction->setToolTip("Add a new living room monitor");
    m_toolBar->addAction(m_addRoomAction);

    connect(m_addRoomAction, &QAction::triggered, this, &MainWindow::addRoom);
    // x Button on tab
    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index) {
        QWidget* widget {ui->tabWidget->widget(index) };
        ui->tabWidget->removeTab(index);
        widget->deleteLater();
    });

    // verify whether the room is real
    connect(m_roomInitApi, &BilibiliRoomInitApi::roomVerifyOk, this, &MainWindow::onRoomVerifyOk);
    connect(m_roomInitApi, &BilibiliRoomInitApi::roomVerifyFail, this, &MainWindow::onRoomVerifyFail);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRoom()
{
    // input room id
    QString inputRoomId {
        QInputDialog::getText(
            this,
            "Add Room",
            "Room ID:"
        )
    };

    if (inputRoomId.isEmpty()) return;
    m_roomInitApi->verifyRoomId(inputRoomId);
}

void MainWindow::onRoomVerifyOk(QString realRoomId, QString displayName)
{
    // here must be real room id
    auto roomWidget { new RoomWidget(realRoomId) };

    // new tab window
    int index { ui->tabWidget->addTab(roomWidget, realRoomId) };
    m_roomTabs.insert(roomWidget, index);

    connect(roomWidget, &RoomWidget::roomTitleChanged, this, [this, roomWidget](const QString& title) {
        int index { ui->tabWidget->indexOf(roomWidget) };
        if (index >= 0) {
            ui->tabWidget->setTabText(index, title);
        }
    });
}

void MainWindow::onRoomVerifyFail(QString errMsg)
{
    QMessageBox::warning(this, "添加房间失败", errMsg);
}


