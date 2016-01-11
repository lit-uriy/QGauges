#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "usbserialdevice.h"

#include <QIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    connect(qApp, SIGNAL(commitDataRequest(QSessionManager&)), this, SLOT(commitData(QSessionManager&)));

    retranslateUi();

    trayIcon->show();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createActions() {
    toggleVisibleAction = new QAction(this);
    toggleVisibleAction->setIcon(QIcon(":/icons/preferences-system.png"));
    connect(toggleVisibleAction, SIGNAL(triggered(bool)), this, SLOT(toggleVisible()));

    quitAction = new QAction(this);
    quitAction->setIcon(QIcon(":/icons/application-exit.png"));
    connect(quitAction, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(toggleVisibleAction);
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(QIcon(":/icons/qgauges.png"), this);
    trayIcon->setContextMenu(trayIconMenu);
#ifndef Q_OS_OSX
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(toggleVisible()));
#endif
}

void MainWindow::retranslateUi() {
    ui->retranslateUi(this); // Double call
    renameToggleVisibleAction(isVisible());
    quitAction->setText(tr("Quit"));
}

void MainWindow::renameToggleVisibleAction(bool isMainWindowVisible) {
    toggleVisibleAction->setText(isMainWindowVisible ? tr("Hide settings") : tr("Show settings"));
}

// ----- Slots -----

void MainWindow::toggleVisible() {
    if (isVisible()) {
        hide();
        renameToggleVisibleAction(false);
    } else {
        show();
        renameToggleVisibleAction(true);
    }
}

void MainWindow::commitData(QSessionManager &manager) {
    if (manager.allowsInteraction()) {
        manager.setRestartHint(QSessionManager::RestartIfRunning);
        manager.release();
    }
}

void MainWindow::refreshPorts() {
    ui->portsListWidget->clear();
    QList<QPair<int, int> > usbIdentifiers;
    usbIdentifiers << qMakePair(0x2341, 0x8037);
    foreach(const UsbSerialDevice &info, UsbSerialDevice::findDevices(usbIdentifiers)) {
        ui->portsListWidget->addItem(info.portName());
    }
}

// ----- Handlers -----

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);
    renameToggleVisibleAction(false);
}
