#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Database.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(std::shared_ptr<Database> dbPtr, QWidget *parent = nullptr);
    ~ServerWindow();

public slots:
    void updateAllData();

private slots:
    void on_banUserButton_clicked();
    void on_unbanUserButton_clicked();
    void on_refreshButton_clicked();

private:
    Ui::ServerWindow *ui;
    std::shared_ptr<Database> m_dbPtr;
    QTimer* m_updateTimer;
};

#endif // SERVERWINDOW_H
