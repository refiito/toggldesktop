// Copyright 2014 Toggl Desktop developers.

#include "./aboutdialog.h"
#include "./ui_aboutdialog.h"

#include <QDebug>  // NOLINT
#include <QDesktopServices>  // NOLINT
#include <QApplication>  // NOLINT

#include "./toggl.h"

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
ui(new Ui::AboutDialog) {
    ui->setupUi(this);

    ui->releaseChannel->addItem("stable");
    ui->releaseChannel->addItem("beta");
    ui->releaseChannel->addItem("dev");

    ui->version->setText(QApplication::applicationVersion());

    connect(TogglApi::instance, SIGNAL(displayUpdate(bool,UpdateView*)),  // NOLINT
            this, SLOT(displayUpdate(bool,UpdateView*)));  // NOLINT
}

AboutDialog::~AboutDialog() {
    delete ui;
}

void AboutDialog::displayUpdate(const bool open, UpdateView *view) {
    if (open) {
        show();
    }

    url = view->URL;

    ui->releaseChannel->setCurrentText(view->UpdateChannel);
    ui->releaseChannel->setEnabled(!view->IsChecking);
    ui->updateButton->setEnabled(!view->IsChecking && view->IsUpdateAvailable);

    if (view->IsChecking) {
        ui->updateButton->setText("Checking for update..");
    } else if (view->IsUpdateAvailable) {
        ui->updateButton->setText(
            "Click here to download update! (" + view->Version + ")");
    } else {
        ui->updateButton->setText("Toggl Desktop is up to date");
    }
}

void AboutDialog::on_updateButton_clicked() {
    QDesktopServices::openUrl(QUrl(url));
    TogglApi::instance->shutdown = true;
    qApp->exit(0);
}

void AboutDialog::on_releaseChannel_activated(const QString &arg1) {
    TogglApi::instance->setUpdateChannel(arg1);
}
