#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , networkManager(new NetworkManager(this))
{
    ui->setupUi(this);

    minecraftDir = QDir::homePath() + "/.minecraft";
    setupDirectories();

    connect(networkManager, &NetworkManager::versionsLoaded,
            this, &MainWindow::onVersionsLoaded);
    connect(networkManager, &NetworkManager::downloadProgress,
            this, &MainWindow::onDownloadProgress);
    connect(networkManager, &NetworkManager::downloadFinished,
            this, &MainWindow::onDownloadFinished);
    connect(networkManager, &NetworkManager::downloadError,
            this, &MainWindow::onDownloadError);

    networkManager->loadVersions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupDirectories()
{
    QDir dir(minecraftDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (!dir.exists("versions")) {
        dir.mkpath("versions");
    }

    if (!dir.exists("libraries")) {
        dir.mkpath("libraries");
    }
}

void MainWindow::on_refreshButton_clicked()
{
    ui->statusLabel->setText("Обновление списка версий...");
    networkManager->loadVersions();
}

void MainWindow::on_downloadButton_clicked()
{
    if (selectedVersion.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите версию для загрузки");
        return;
    }

    ui->statusLabel->setText("Загрузка " + selectedVersion + "...");
    ui->downloadProgress->setValue(0);
    networkManager->downloadVersion(selectedVersion, minecraftDir);
}

void MainWindow::on_launchButton_clicked()
{
    if (selectedVersion.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите версию для запуска");
        return;
    }

    QString versionDir = minecraftDir + "/versions/" + selectedVersion;
    QString jarPath = versionDir + "/" + selectedVersion + ".jar";

    if (!QFile::exists(jarPath)) {
        QMessageBox::warning(this, "Ошибка", "Версия не загружена. Сначала скачайте её.");
        return;
    }

    ui->statusLabel->setText("Запуск " + selectedVersion + "...");

    QProcess *process = new QProcess(this);
    QStringList arguments;
    arguments << "-jar" << jarPath;

    process->start("java", arguments);

    if (!process->waitForStarted()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось запустить Minecraft");
    }
}

void MainWindow::on_versionsList_itemClicked(QListWidgetItem *item)
{
    selectedVersion = item->text();
    ui->statusLabel->setText("Выбрана версия: " + selectedVersion);
}

void MainWindow::onVersionsLoaded(const QJsonArray &versions)
{
    ui->versionsList->clear();

    for (const QJsonValue &value : versions) {
        QJsonObject versionObj = value.toObject();
        QString versionId = versionObj["id"].toString();
        ui->versionsList->addItem(versionId);
    }

    ui->statusLabel->setText("Список версий загружен");
}

void MainWindow::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {
        int progress = (bytesReceived * 100) / bytesTotal;
        ui->downloadProgress->setValue(progress);
    }
}

void MainWindow::onDownloadFinished(const QString &filePath)
{
    ui->downloadProgress->setValue(100);
    ui->statusLabel->setText("Загрузка завершена: " + filePath);
}

void MainWindow::onDownloadError(const QString &error)
{
    ui->statusLabel->setText("Ошибка: " + error);
    QMessageBox::critical(this, "Ошибка загрузки", error);
}
