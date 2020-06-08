#include "RadarVisualController.h"

void RadarVisualController::start(QString path){
    qDebug()<<"Starting Radar Visualization Controller";
    qDebug()<<"Adding path to watcher: "<<path;
        DirWatcher::stopWatching();
        DirWatcher::addPath(path);
}

RadarVisualController::~RadarVisualController(){

}
