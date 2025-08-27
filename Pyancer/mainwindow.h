#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "networkmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_refreshButton_clicked();
    void on_downloadButton_clicked();
    void on_launchButton_clicked();
    void on_versionsList_itemClicked(QListWidgetItem *item);

    void onVersionsLoaded(const QJsonArray &versions);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished(const QString &filePath);
    void onDownloadError(const QString &error);

private:
    Ui::MainWindow *ui;
    NetworkManager *networkManager;
    QString selectedVersion;
    QString minecraftDir;

    void setupDirectories();
};
#endif
