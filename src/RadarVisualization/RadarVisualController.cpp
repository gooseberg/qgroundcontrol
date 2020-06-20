#include "RadarVisualController.h"

//Debugging
#include <string>
#include <iostream>
using namespace std;

// Start the file watching
void RadarVisualController::start(QString path){
    qDebug()<<"Starting Radar Visualization Controller";
    qDebug()<<"Adding path to watcher: "<<path;
    DirWatcher::stopWatching();     // Ensure that no other watcher is runner
    DirWatcher::addPath(path);      // Get the directory path and start watching

    // Connect the watcher class to RadarVisualController methods
    connect(DirWatcher::getDirWatcher(), SIGNAL(fileAdded  (QString, QString)), this, SLOT(fileAdded(QString, QString)));
    connect(DirWatcher::getDirWatcher(), SIGNAL(fileRemoved(QString, QString)), this, SLOT(fileRemoved(QString, QString)));

}

// When the user presses the load pins button, read all the file names in the folder and add pins to the map that are associated with valid file names
void RadarVisualController::loadPinsFromFolder(QString path){
    qDebug()<<"Loading pins from folder...";
    QString qFile;
    std::string tempPath = path.toStdString();
    // Loop through the files in the folder and create a pin for every file
    int i = 0;
    int inValidCount = 0;
    for (const auto & entry : fs::directory_iterator(tempPath)){
        bool isValid = false;

        std::string file = entry.path().filename().string();    // Convert the path to a string
        qFile = QString::fromStdString(file);                   // Convert the path to a QString
        isValid = fileAdded(path,qFile);                        // Try to add the pin and return boolean stating if it did or not

        // Add up the files that were invalid
        if(!isValid){
            inValidCount+=1;
        }
        i += 1;
    }

    // Let the user know the outcome of loading pins, if any were loaded or not
    if(i == 0){
        qDebug()<<"The folder is empty. No pins to add.\n";
    }else{
        qDebug()<< "Loaded: " + QString::number(i-inValidCount) + " pins.";
        qDebug()<<"Loading pins from folder complete.\n";
    }
}

// When a file is added to the folder, add a pin to the map
bool RadarVisualController::fileAdded(const QString& path, const QString& file){
    QString networkLocation = path;
    bool isValid = false;

    QMap<QString, QString> fileInfo = parseFileName(file, isValid); // Parse the file name and check for valid integers for Time Stamp, Lat, and Lon (ID is ignored for adaptability)

    // If the file name had valid numbers, then add the pin to the map
    if(isValid){
        networkLocation = networkLocation.prepend("file:///").append("/").append(file); // Put the file name in proper format for QML
        emit addPin(networkLocation, fileInfo["lat"], fileInfo["lon"], fileInfo["id"]); // Send signal to QML to add the pin to the map
    }else{
        qDebug() << "Pin not created.\n";
    }

    return isValid;

}

// When the a file is removed from the folder, remove the pin from the map that is associated with it
void RadarVisualController::fileRemoved(const QString& path, const QString& file){
    QString networkLocation = path;
    bool isValid = false;

    QMap<QString, QString> fileInfo = parseFileName(file, isValid); // Parse the file name and check for valid integers for Time Stamp, Lat, and Lon (ID is ignored for adaptability)

    // If the file name had valid numbers then add the pin to the map
    if(isValid){
        networkLocation = networkLocation.prepend("file:///").append("/").append(file); // Put the file name in proper format for QML
        emit destroyPin(networkLocation, fileInfo["id"]);                               // Send signal to QML to remove the pin from the map
    }else{
        qDebug() << "Pin not removed.\n";
    }
}

// Strip the the time stamp, lat, lon, and ID from the filename, check if the first three are valid numbers, and return the parse file name parameters
QMap<QString, QString> RadarVisualController::parseFileName(const QString& file, bool &isValid){
    QMap<QString, QString> fileInfo;
    int tempSize;
    QString first, second, third;

    // Try to parse the filename, and if there is an error let the user know
    try {
        QStringList delimitedFileName = file.split(',');
        tempSize = delimitedFileName.size();
        if(tempSize<4){
            qDebug()<< "RadarVisualController.cpp ERROR: Invalid filename. Must be in the format of: \"TimeStamp,Lat,Lon,ID\"";
            throw 5;
        } else {
        // Strip the file name parameters (time stamp, lat, lon)
        first = delimitedFileName.first();
        fileInfo["time"] = first;
        delimitedFileName.pop_front();
        second = delimitedFileName.first();
        fileInfo["lat"] = second;
        delimitedFileName.pop_front();
        third = delimitedFileName.first();
        fileInfo["lon"] = third;

        // Check if the first three parmeters of the file name are valid numbers (time stamp, lat, lon)
        if(validNumber(first) && validNumber(second) && validNumber(third)){
            // Strip the ID parameter from the file name
            isValid = true;
            delimitedFileName.pop_front();
            fileInfo["id"] = delimitedFileName.first();
            fileInfo["id"].chop(4);
            qDebug()<<fileInfo;
        }else{
            isValid = false;
            qDebug()<< "RadarVisualController.cpp ERROR: Invalid filename. Time Stamp, Lat, and Lon must be valid numbers.";
        }// end if else validNumber(first) && ...
        } // if else if(tempSize<4)
    } catch (int throwInt) {
        // Do nothing
        throwInt = throwInt + 1;
    } // end try catch

    return fileInfo;
}// end Radar


// This function checks if the QString contains a valid number
bool validNumber(QString& str){
    bool isValid;
    double tempDouble;

    tempDouble = str.toDouble(&isValid);    // Return the number and if the value is valid

    return isValid;
}

RadarVisualController::~RadarVisualController(){
}
