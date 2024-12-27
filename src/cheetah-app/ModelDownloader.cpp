#include <filesystem>

#include <ModelDownloader.h>


ModelDownloader::ModelDownloader (std::string modelName) : modelName(modelName) {
    // String, configuration: URLSessionConfiguration = .default
    //session = URLSession(configuration: configuration)
    filename = "/" + modelName + ".bin";
    modelURL = cacheDirectoryPath + filename;

    resume();
}

ModelDownloader::~ModelDownloader(){}

void ModelDownloader::onFileDownloaded(QNetworkReply* pReply){
    m_DownloadedData = pReply->readAll();
    // emit file downloaded signal.
    pReply->deleteLater();
    emit onModelDownloaded();
}

void ModelDownloader::resume() {
    QFileInfo checkFile((std::filesystem::path(modelURL)));

    if (checkFile.exists() && checkFile.isFile()){
        this->state = State::Completed;
        return;
    }

    QDir cacheDirectory = checkFile.absoluteDir();
    if (!cacheDirectory.exists()){
        cacheDirectory.mkdir(".");
    }

    auto requestURL = QUrl(QString::fromStdString(baseURL + filename));

    connect(
        &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
        this, SLOT (onFileDownloaded(QNetworkReply*))
    );
    QNetworkRequest request(requestURL);
    m_WebCtrl.get(request);
}
