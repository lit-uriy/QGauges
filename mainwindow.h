#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSessionManager>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void createActions();
    void createTrayIcon();
    void retranslateUi();
    void renameToggleVisibleAction(bool isMainWindowVisible);

    Ui::MainWindow *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *toggleVisibleAction;
    QAction *quitAction;

private slots:
    void toggleVisible();
    void commitData(QSessionManager &manager);

    void refreshPorts();
};

#endif // MAINWINDOW_H
