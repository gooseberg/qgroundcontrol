#ifndef DIRWATCHER_H
#define DIRWATCHER_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QMap>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDir>

class DirWatcher : public QWidget{
Q_OBJECT

private:
    static DirWatcher*  _instance;
    QMap<QString, QStringList>         _fileList;
    QFileSystemWatcher* _sysWatcher;

public:
    static void addPath(QString path);
    static void stopWatching();

private slots:
    void slotDirectoryChanged   (const QString&);
    void slotFileChanged        (const QString&);


};

#endif // DIRWATCHER_H
