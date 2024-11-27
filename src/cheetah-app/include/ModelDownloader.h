#pragma once

#include <string>

#include <QObject>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>

// modification of https://wiki.qt.io/Download_Data_from_URL sample from Qt wiki.

class ModelDownloader : public QObject {
    Q_OBJECT
public:
    enum State {
        Pending,
        Completed,
        Failed,
    };
    
    State state = State::Pending;
    
    std::string baseURL = "https://huggingface.co/datasets/ggerganov/whisper.cpp/resolve/main";
    std::string modelName;
    std::string cacheDirectoryPath = ""; // TODO: save path to model on disk via QSettings.
    std::string modelURL = "";
    std::string filename = "";

    /*
    let session: URLSession
    let filename: String
    let modelURL: URL
    */

    explicit ModelDownloader (std::string modelName);

    virtual ~ModelDownloader();
    QByteArray downloadedData() const;

signals:
    void onModelDownloaded();

private slots:

    void onFileDownloaded(QNetworkReply* pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

    void resume();
};
