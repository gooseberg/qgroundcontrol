#ifndef RADARVISUALCONTROLLER_H
#define RADARVISUALCONTROLLER_H

#include "DirWatcher.h"
#include <QDebug>
#include <QObject>
#include <QMap>
#include "SettingsManager.h"

// Includes for loading pins from folder
#include <string>
#include <filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

class RadarVisualController : public QObject
{
    Q_OBJECT

public:
    ~RadarVisualController();
    Q_INVOKABLE void start(QString);
    Q_INVOKABLE void loadPinsFromFolder(QString);

private slots:
    void fileAdded(const QString& path, const QString& file);
    void fileRemoved(const QString& path, const QString& file);

signals:
    void addPin(const QString& path, const QString& lat, const QString& lon, const QString& pinId);
    void destroyPin(const QString& path, const QString& pinId);

private:
    static QMap<QString, QString> parseFileName(const QString& file);

};

#endif // RADARVISUALCONTROLLER_H
