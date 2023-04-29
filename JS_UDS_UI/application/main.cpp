#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <QFile>
#include <QTime>

QString logFileName = "../JS_UDSWB_LOGGING.log";

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Format data for logging String
    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QTime time = QTime::currentTime();
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    QString logLevelName = msgLevelHash[type];

    // Create logging string
    QString logging_line = QString("%1 %2: %3 (File: %4, Line: %5, Function: %6)").arg(formattedTime, logLevelName, msg,  context.file, QString::number(context.line), context.function);

    // append log string to file
    QFile loggingFile(logFileName);
    loggingFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&loggingFile);
    ts << logging_line << Qt::endl;

    if (type == QtFatalMsg) {abort();}

}

void initMessageHandling(){
    QFile f(logFileName);
    f.resize(0);
    qInstallMessageHandler(myMessageOutput);
}


extern "C"{
int maincode_main(int argc, char *argv[])
{
    initMessageHandling();

    QApplication a(argc, argv);

    // STYLESHEET
    QFile styleSheetFile(":\\stylesheets\\stc_styles.qss");
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    a.setStyleSheet(styleSheet);

    // MAIN WINDOW
    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}
}


