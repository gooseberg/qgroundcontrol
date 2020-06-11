#include "RadarVisualController.h"

void RadarVisualController::start(QString path){
    qDebug()<<"Starting Radar Visualization Controller";
    qDebug()<<"Adding path to watcher: "<<path;
    DirWatcher::stopWatching();
    DirWatcher::addPath(path);

    connect(DirWatcher::getDirWatcher(), SIGNAL(fileAdded(QString, QString)), this, SLOT(fileAdded(QString, QString)));
    connect(DirWatcher::getDirWatcher(), SIGNAL(fileRemoved(QString, QString)), this, SLOT(fileRemoved(QString, QString)));
}

void RadarVisualController::fileAdded(const QString& path, const QString& file){
    QString networkLocation = path;

    QMap<QString, QString> fileInfo = parseFileName(file);      // Put this in try cach block **********************

    networkLocation = networkLocation.prepend("file:///").append("/").append(file);

    emit addPin(networkLocation, fileInfo["lat"], fileInfo["lon"], fileInfo["id"]);
}

void RadarVisualController::fileRemoved(const QString& path, const QString& file){
    QString networkLocation = path;

    QMap<QString, QString> fileInfo = parseFileName(file);

    networkLocation = networkLocation.prepend("file:///").append("/").append(file);

    emit destroyPin(networkLocation, fileInfo["id"]);
}

QMap<QString, QString> RadarVisualController::parseFileName(const QString& file){
    QMap<QString, QString> fileInfo;
    QStringList delimitedFileName = file.split(',');
    if(delimitedFileName.size()<4){
        qDebug()<<"Non conforming filename detected...";
        throw "Non conforming filename detected...";        //Catch me if you caaaan :3 file: Catch this *********************
    } else {
        fileInfo["time"] = delimitedFileName.first();
        delimitedFileName.pop_front();
        fileInfo["lat"] = delimitedFileName.first();
        delimitedFileName.pop_front();
        fileInfo["lon"] = delimitedFileName.first();
        delimitedFileName.pop_front();
        fileInfo["id"] = delimitedFileName.first();
        fileInfo["id"].chop(4);
        qDebug()<<fileInfo;
    }
    return fileInfo;
}

RadarVisualController::~RadarVisualController(){
}
