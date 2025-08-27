#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    void loadVersions();
    void downloadVersion(const QString &version, const QString &basePath);

signals:
    void versionsLoaded(const QJsonArray &versions);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(const QString &filePath);
    void downloadError(const QString &error);

private slots:
    void onVersionsReplyFinished(QNetworkReply *reply);
    void onVersionDataReplyFinished(QNetworkReply *reply);
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    QNetworkAccessManager *manager;
    QJsonArray versionsList;

    void parseVersionData(const QJsonObject &versionData, const QString &basePath);
    void downloadLibraries(const QJsonArray &libraries, const QString &basePath);
    void downloadFile(const QUrl &url, const QString &filePath);
};

#endif // NETWORKMANAGER_H
