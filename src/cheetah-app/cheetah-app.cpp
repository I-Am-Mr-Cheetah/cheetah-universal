#include <QDir>
#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlProperty>

#include <ModelDownloader.h>
#include <LibWhisper.h>
#include <WhisperStream.h>
#include <CaptureDevice.h>
#include <ConversationAnalyzer.h>
#include <OpenAIExecutor.h>

enum AnswerRequest {
    None,
    AnswerQuestion,
    RefineAnswer,
    AnalyzeCode
};

const std::string defaultWhisperModel = "ggml-medium.en";

struct AppViewModel{
    QString authToken;
    bool useGPT4;
    
    std::vector<CaptureDevice> devices;
    CaptureDevice& selectedDevice;

    std::string whisperModel = defaultWhisperModel;
    ModelDownloader::State modelDownloadState = ModelDownloader::State::Idle; 

    ConversationAnalyzer conversationAnalyzer;
    AnswerRequest answerRequest = AnswerRequest::None;
    std::string errorDescription;

    std::string transcript;
    std::string answer;
    std::string codeAnswer;

    bool buttonsAlwaysEnabled = false;
};

struct CheetahApp
{
    AppViewModel viewModel;

    ModelDownloader download;
    WhisperStream stream;
    // IPCServer ipcServer; // TODO: add browser extension and plugin.
    // BrowserExtensionState extensionState;


}

int main(int argc, char *argv[])
{
    qint64 startupTime = QDateTime::currentMSecsSinceEpoch();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QDirIterator it(":", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        qDebug() << it.next();
    }

    //Q_INIT_RESOURCE(cheetah_app_resources);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Specify directory with leading ":"
    QDir myDir("qrc:/");

    // Specify selection criteria (empty list if you want all files)
    const QStringList sel = QStringList({"*"});

    // read files (and ignore directories) that match selection criteria
    QStringList files = myDir.entryList(sel,QDir::AllEntries);

    qDebug() << "QML files in resource directory:";
    // iterate through file list
    foreach(QString filename, files)
    {
        qDebug() << filename.toStdString();
    }

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/cheetah-app/qml/cheetah-app-window.qml")));

    if (engine.rootObjects().size() > 0)
    {
        QVariant qmlStartupTime = QQmlProperty::read(engine.rootObjects().first(), "startupTime");
        qDebug() << "Load time:" << qmlStartupTime.toDouble() - startupTime;
    }

    return app.exec();
}
