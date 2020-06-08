import QtQuick          2.3
import QtLocation       5.3
import QtPositioning    5.3

import QtQuick.Controls         2.4
import QtQuick.Dialogs          1.3
import QtQuick.Layouts          1.11

import QtQuick.Window           2.2
import QtQml.Models             2.1

import QGroundControl               1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.FlightMap     1.0

Item {
    id: _root
    //The following properties must be set by the consumer.
    property var        map
    property var        longitude
    property var        latitude
    property var        imPath:     "file:///C:/Users/Robin/Desktop/DUCK.jpg"


    property var        _RadarVisual
    property var        _pinColor:  "red"
    property var        _imOpacity: 1.00
    property bool       _isVisible: false

//    Component.onCompleted: {    //TODO: Instead of creating one component on render, create as many as needed based on file watcher system.
//        _RadarVisual = radarVisualComponent.createObject(map)
//        map.addMapItem(_RadarVisual)
//    }

    function newFile(path, longitude, latitude){
        _RadarVisualComponent.createObject(map)
        map.addMapItem(_RadarVisual) //Not sure if you can reuse _radarVisual
    }

    Component {
        id: radarVisualComponent

        MapQuickItem {
            anchorPoint.x:          width / 2   // center the pin
            anchorPoint.y:          height      // make anchor at the bottom of the pin
            //coordinate:             QtPositioning.coordinate(35.282089, -120.660800)//Temporary
            coordinate:             QtPositioning.coordinate(longitude, latitude)
            z:                      QGroundControl.zOrderMapItems + 2
            sourceItem:

            QGCColoredImage{
                 id:             geoTag
                 source:         "qrc:/qmlimages/GeoTagIcon"
                 mipmap:         true
                 antialiasing:   true
                 fillMode:       Image.PreserveAspectFit
                 width:          ScreenTools.defaultFontPixelHeight * 2
                 height:         ScreenTools.defaultFontPixelHeight * 2
                 visible:        true
                 color:          "red"

                 Rectangle{
                     id:        imBorder
                     color:     "black"
                     width:     im.width + 10
                     height:    im.height + 45
                     visible:   _isVisible
                     anchors.left: geoTag.right
                     anchors.bottom: geoTag.top
                     opacity: 0.75
                     Label {    id: lat
                                text: " Latitude: " + latitude.toString()
                                color: "white"
                                visible: _isVisible
                                anchors.leftMargin: 20
                                anchors.bottomMargin: 20
                                anchors.bottom: imBorder.bottom
                     }
                     Label {
                                 text: " Longitude: " + longitude.toString()
                                 color: "white"
                                 visible: _isVisible
                                 anchors.topMargin: 15
                                 anchors.top: lat.top
                     }
                 }

                 Image{
                     id:                    im
                     source:                imPath
                     fillMode:              Image.PreserveAspectFit
                     width:                 ScreenTools.defaultFontPixelHeight * 20
                     height:                Image.height
                     anchors.bottomMargin:  40
                     anchors.leftMargin:    5
                     anchors.left:          geoTag.right
                     anchors.bottom:        geoTag.top

                     opacity:               _imOpacity
                     visible:               _isVisible
                 }

                 Button{ //Implemented so that image is clickable.
                     anchors.bottom:        geoTag.BottomLeft
                     text:                  qsTr("Open Image")
                     width:                 ScreenTools.defualtFontPixelWidth * 22
                     anchors.fill:          im
                     onClicked:             Qt.openUrlExternally(imPath)
                     visible:               _isVisible
                     opacity:               hovered - 0.85
                     hoverEnabled:          true
                 }
            }

            MouseArea {
                anchors.fill:       parent
                anchors.margins:    ScreenTools.isMobile ? -ScreenTools.defaultFontPixelHeight : 0
                width:              50

                onClicked: {
                    if(_pinColor == "red"){
                        _pinColor = "blue"
                        _isVisible = true
                    } else {
                        _pinColor = "red"
                        _isVisible = false;
                    }
                }
            }
        }
    }
}
