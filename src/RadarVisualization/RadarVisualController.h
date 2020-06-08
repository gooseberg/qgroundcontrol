#ifndef RADARVISUALCONTROLLER_H
#define RADARVISUALCONTROLLER_H

#include "DirWatcher.h"
#include <QDebug>
#include <QObject>
#include "SettingsManager.h"

class RadarVisualController : public QObject
{
    Q_OBJECT

public:
    ~RadarVisualController();

    Q_INVOKABLE void start(QString);
};

#endif // RADARVISUALCONTROLLER_H
