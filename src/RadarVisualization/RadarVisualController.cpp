#include "RadarVisualController.h"

void RadarVisualController::start(QString path){
    qDebug()<<"Starting Radar Visualization Controller";
    qDebug()<<"Adding path to watcher: "<<path;
    DirWatcher::stopWatching();
    DirWatcher::addPath(path);

    connect(DirWatcher::getDirWatcher(), SIGNAL(fileAdded(QString, QString)), this, SLOT(fileAdded(QString, QString)));
}

void RadarVisualController::fileAdded(const QString& path, const QString& file){
    QString networkLocation = path;

    QMap<QString, QString> fileInfo = parseFileName(file);

    networkLocation = networkLocation.prepend("file:///").append("/").append(file);

    emit addPin(networkLocation, fileInfo["lat"], fileInfo["lon"]);
}

void RadarVisualController::fileRemoved(const QString& path, const QString& file){
    QString networkLocation = path;

    QMap<QString, QString> fileInfo = parseFileName(file);

    networkLocation = networkLocation.prepend("file:///").append("/").append(file);

    emit destroyPin(networkLocation);
}

QMap<QString, QString> RadarVisualController::parseFileName(const QString& file){
    QMap<QString, QString> fileInfo;
    QStringList delimitedFileName = file.split(',');
    if(delimitedFileName.size()<3){
        qDebug()<<"Non conforming filename detected...";
        throw "Non conforming filename detected...";        //Catch me if you caaaan :3
    } else {
        fileInfo["time"] = delimitedFileName.first();
        delimitedFileName.pop_front();
        fileInfo["lat"] = delimitedFileName.first();
        delimitedFileName.pop_front();
        fileInfo["lon"] = delimitedFileName.first();
        fileInfo["lon"].chop(4);
        qDebug()<<fileInfo;
    }
    return fileInfo;
}

RadarVisualController::~RadarVisualController(){
}
