#include "DirWatcher.h"

DirWatcher* DirWatcher::_instance = 0;

void DirWatcher::addPath(QString path){
    // Allocate memory to singleton if not already created.
    if(_instance == 0){
        _instance = new DirWatcher;
        _instance->_sysWatcher = new QFileSystemWatcher();

        //Connect signals from QFileSystemWatcher to slots in our DirWatcher class.
        connect(_instance->_sysWatcher,  SIGNAL(directoryChanged(QString)), _instance, SLOT(slotDirectoryChanged(QString)));
        connect(_instance->_sysWatcher,  SIGNAL(fileChanged(QString)), _instance, SLOT(slotFileChanged(QString)));
    }

    //Add path to watch.
    _instance->_sysWatcher->addPath(path);

    QFileInfo f(path);

    if(f.isDir()){
        const QDir dir(path);
        _instance->_fileList[path] = dir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Time);
    }
}

void DirWatcher::stopWatching(){
    if(!(_instance == 0)){
        _instance->_sysWatcher->~QFileSystemWatcher();
    }
    _instance = 0;
}

//This slot is invoked when a watched directory is updated.
void DirWatcher::slotDirectoryChanged(const QString& path){
    //qDebug()<<"Directory updated: "<<path; //FOR DEBUGGING (Creates too many messages)

    //Compare latest contents to the last saved contents to find change.
    QStringList oldFileList = _instance->_fileList[path];
    const QDir dir(path);

    QStringList newFileList = dir.entryList(QDir::NoDotAndDotDot | QDir::Files, QDir::Time);

    QSet<QString> newFileSet = QSet<QString>::fromList(newFileList);
    QSet<QString> oldFileSet = QSet<QString>::fromList(oldFileList);

    // Added files
    QSet<QString> addedFiles = newFileSet - oldFileSet;
    QStringList addedFileList = addedFiles.toList();

    //Removed files
    QSet<QString> removedFiles = oldFileSet - newFileSet;
    QStringList removedFileList = removedFiles.toList();

    //Update file list.
    _instance->_fileList[path] = newFileList;

    //Choose an action depending on if file was added or removed.
    if(!addedFileList.isEmpty() && !removedFileList.isEmpty()){
        if(addedFileList.count() == 1 && removedFileList.count() == 1){   //File was renamed
            qDebug()<<"File was renamed from "<<removedFileList.first()<<" to "<<addedFileList.first();
        }
    } else {
        if(!addedFileList.isEmpty()){ //File has been added.
            foreach(QString file, addedFileList){
                //Do something for each file!
                qDebug()<<"New file added: "<<file;
            }
        }
        if(!removedFileList.isEmpty()){ //File has been removed.

            foreach(QString file, removedFileList){
                //Do something for each file!
                qDebug()<<"File removed: "<<file;
            }
        }
    }
}

void DirWatcher::slotFileChanged(const QString& path){
    qDebug()<<"File updated: "<<path;
}
