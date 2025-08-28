#include "networkmanager.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void NetworkManager::loadVersions()
{
    QUrl versionsUrl("https://launchermeta.mojang.com/mc/game/version_manifest.json");
    QNetworkRequest request(versionsUrl);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished,
            this, [this, reply]() { onVersionsReplyFinished(reply); });
}

void NetworkManager::downloadVersion(const QString &version, const QString &basePath)
{
    for (const QJsonValue &value : versionsList) {
        QJsonObject versionObj = value.toObject();
        if (versionObj["id"].toString() == version) {
            QString versionUrl = versionObj["url"].toString();

            QUrl url(versionUrl);
            QNetworkRequest request(url);

            QNetworkReply *reply = manager->get(request);

            connect(reply, &QNetworkReply::finished,
                    this, [this, reply, basePath]() { onVersionDataReplyFinished(reply); });
            break;
        }
    }
}

void NetworkManager::onVersionsReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit downloadError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();
    versionsList = root["versions"].toArray();

    emit versionsLoaded(versionsList);
    reply->deleteLater();
}

void NetworkManager::onVersionDataReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit downloadError(reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject versionData = doc.object();

    parseVersionData(versionData, QDir::homePath() + "/.minecraft");

    reply->deleteLater();
}

void NetworkManager::parseVersionData(const QJsonObject &versionData, const QString &basePath)
{
    QString versionId = versionData["id"].toString();

    QString versionDir = basePath + "/versions/" + versionId;
    QDir().mkpath(versionDir);

    QJsonObject downloads = versionData["downloads"].toObject();
    QJsonObject client = downloads["client"].toObject();
    QString clientUrl = client["url"].toString();
    QString clientPath = versionDir + "/" + versionId + ".jar";

    downloadFile(QUrl(clientUrl), clientPath);

    QJsonArray libraries = versionData["libraries"].toArray();
    downloadLibraries(libraries, basePath);
}

void NetworkManager::downloadLibraries(const QJsonArray &libraries, const QString &basePath)
{
    for (const QJsonValue &value : libraries) {
        QJsonObject library = value.toObject();
        QJsonObject downloads = library["downloads"].toObject();

        if (downloads.contains("artifact")) {
            QJsonObject artifact = downloads["artifact"].toObject();
            QString url = artifact["url"].toString();
            QString path = artifact["path"].toString();

            QString fullPath = basePath + "/libraries/" + path;
            QFileInfo fileInfo(fullPath);
            QDir().mkpath(fileInfo.path());

            downloadFile(QUrl(url), fullPath);
        }
    }
}

void NetworkManager::downloadFile(const QUrl &url, const QString &filePath)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::downloadProgress,
            this, &NetworkManager::onDownloadProgress);

    connect(reply, &QNetworkReply::finished, this, [this, reply, filePath]() {
        if (reply->error() != QNetworkReply::NoError) {
            emit downloadError(reply->errorString());
            reply->deleteLater();
            return;
        }

        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(reply->readAll());
            file.close();
            emit downloadFinished(filePath);
        } else {
            emit downloadError("Don't save file: " + filePath);
        }

        reply->deleteLater();
    });
}

void NetworkManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    emit downloadProgress(bytesReceived, bytesTotal);
}
