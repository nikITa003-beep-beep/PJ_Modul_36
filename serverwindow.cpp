#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <QTimer>
#include <QListWidgetItem>
#include <QMessageBox>

ServerWindow::ServerWindow(std::shared_ptr<Database> dbPtr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow),
    m_dbPtr(dbPtr)
{
    ui->setupUi(this);
    setWindowTitle("Chat Server Administration");

    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &ServerWindow::updateAllData);
    m_updateTimer->start(2000);

    updateAllData();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::updateAllData()
{
    ui->allMessagesBrowser->clear();
    auto chatMsgs = m_dbPtr->getChatMessages();
    for (const auto &msg : chatMsgs) {
        ui->allMessagesBrowser->append(QString::fromStdString("[PUBLIC] " + msg));
    }
    
    auto privateMsgs = m_dbPtr->getPrivateMessage(-1);
    for (const auto &msg : privateMsgs) {
        QString formattedMsg = QString("[PRIVATE] <%1> to <%2>: %3")
                                   .arg(QString::fromStdString(msg.getSender()))
                                   .arg(QString::fromStdString(m_dbPtr->getUserName(msg.getDest())))
                                   .arg(QString::fromStdString(msg.getText()));
        ui->allMessagesBrowser->append(formattedMsg);
    }

    ui->userListWidget->clear();
    auto userList = m_dbPtr->getUserList();
    for (const auto &user : userList) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(user));
        if(m_dbPtr->isUserBanned(user)) {
            item->setBackground(Qt::red);
            item->setText(item->text() + " [BANNED]");
        }
        ui->userListWidget->addItem(item);
    }
}

void ServerWindow::on_banUserButton_clicked()
{
    auto currentItem = ui->userListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Select a user to ban.");
        return;
    }
    QString username = currentItem->text().split(" [BANNED]")[0];
    if (m_dbPtr->banUser(username.toStdString())) {
        updateAllData();
        QMessageBox::information(this, "Success", QString("User '%1' banned successfully").arg(username));
    } else {
        QMessageBox::critical(this, "Error", "Failed to ban user.");
    }
}

void ServerWindow::on_unbanUserButton_clicked()
{
    auto currentItem = ui->userListWidget->currentItem();
    if (!currentItem) {
        QMessageBox::warning(this, "Warning", "Select a user to unban.");
        return;
    }
    QString username = currentItem->text().split(" [BANNED]")[0];
    if (m_dbPtr->unbanUser(username.toStdString())) {
        updateAllData();
        QMessageBox::information(this, "Success", QString("User '%1' unbanned successfully").arg(username));
    } else {
        QMessageBox::critical(this, "Error", "Failed to unban user.");
    }
}

void ServerWindow::on_refreshButton_clicked()
{
    updateAllData();
}
