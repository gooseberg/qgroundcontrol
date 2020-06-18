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
import QGroundControl.FlightMap     1.0

MapQuickItem {
    id: radarVisualComponent
    // Constructed in FlightDisplayView.qml
    property var        longitude       //  The longitude of where the image was taken, set when constructed from signal lot
    property var        latitude        //  The latitude of where the image was taken, set when constructed from signal lat
    property var        imPath          //  The file path of the image associated with the pin, set when constructed from signal path
    property var        componentPinId  //  The id associated with the pin, set when constructed from signal pinId
    property variant    pinObjectList   //  The list of pins set by the parent
    property var        parentId        //  The grandest parent to get access to its properties, specifically clearPinsEnabled.  Set by parent

    property var        _RadarVisual        //  ************* Should this be removed
    property var        _pinColor:  "red"
    property var        _imOpacity: 1.00
    property bool       _isVisible: false

    anchorPoint.x:      width / 2   // center the pin
    anchorPoint.y:      height      // make anchor at the bottom of the pin
    coordinate:         QtPositioning.coordinate(longitude, latitude)
    z:                  QGroundControl.zOrderMapItems + 2
    sourceItem:         // Everything below this line is the "sourceItem"

    QGCColoredImage{
         id:             geoTag
         source:         "qrc:/qmlimages/GeoTagIcon"
         mipmap:         true
         antialiasing:   true
         fillMode:       Image.PreserveAspectFit
         width:          ScreenTools.defaultFontPixelHeight * 2
         height:         ScreenTools.defaultFontPixelHeight * 2
         visible:        true
         color:          _pinColor

         Rectangle{ // To display information about the image and house the delete button
             id:        imBorder
             color:     "black"
             width:     im.width + 10
             height:    im.height + 55//45
             visible:   _isVisible
             anchors.left: geoTag.right
             anchors.bottom: geoTag.top
             opacity: 0.75
             Label { // Put id underneath image
                     id: labelPinId
                     text: "   Pin ID:       " + componentPinId.toString()
                     color: "white"
                     visible: _isVisible
                     anchors.leftMargin: 20
                     anchors.bottomMargin: 35//20
                     anchors.bottom: imBorder.bottom
             }
             Label {// Put latitude underneath image
                    id: lat
                    text: "   Latitude:   " + latitude.toString()
                    color: "white"
                    visible: _isVisible
                    anchors.topMargin: 15
                    anchors.top: labelPinId.top
             }
             Label { // Put longitude underneath image
                     id: lon
                     text: "   Longitude: " + longitude.toString()
                     color: "white"
                     visible: _isVisible
                     anchors.topMargin: 15
                     anchors.top: lat.top
             }

             Button{// To destroy the pin
                 id: destroyPinButton
                 anchors.right: imBorder.right
                 anchors.rightMargin: 7
                 anchors.bottom: imBorder.bottom
                 anchors.bottomMargin: 10
                 text:                  qsTr("Delete pin")
                 contentItem: Text {
                     text: destroyPinButton.text
                     color: (destroyPinButton.hovered) ? "black" : "white"
                     horizontalAlignment:   Text.horizontalCenter
//                     anchors.verticalCenter: destroyPinButton.verticalCenter
                     anchors.topMargin: 15
//                     horizontalAlignment:   Text.AlignHCenter
//                     verticalAlignment:     Text.AlignVCenter
                 }
                 // You have control of the mouse Jon!
                 visible:               _isVisible
                 hoverEnabled:          true
                 onClicked: {
                     // Remove the pin from the list, return if the clear pins button should be enabled or not, and destroy the pin object
                     parentId.clearPinsEnabled = removePin(pinObjectList,componentPinId)
                     radarVisualComponent.destroy()
                 }
                 background: Rectangle{
                     width: ScreenTools.implicitComboBoxHeight + 45
                     height: ScreenTools.implicitComboBoxHeight
                     color : (destroyPinButton.hovered) ? qgcPal.buttonHighlight : qgcPal.hoverColor
                 } // end background: Rectangle
             }// end Button{// To destroy the pin
         } // end Rectangle{ // To display information

         Image{ // The image to be displayed
             id:                    im
             source:                imPath
             fillMode:              Image.PreserveAspectFit
             width:                 ScreenTools.defaultFontPixelHeight * 20
             height:                Image.height
             anchors.bottomMargin:  50//40
             anchors.leftMargin:    5
             anchors.left:          geoTag.right
             anchors.bottom:        geoTag.top
             opacity:               _imOpacity
             visible:               _isVisible
         }// end Image{ // The image to be displayed

         Button{ //Implemented so that image is clickable.
             anchors.bottom:        geoTag.BottomLeft
             text:                  qsTr("Open Image")
             width:                 ScreenTools.defualtFontPixelWidth * 22
             anchors.fill:          im
             onClicked:             Qt.openUrlExternally(imPath)
             visible:               _isVisible
             opacity:               hovered - 0.85
             hoverEnabled:          true
         } // end Button{ //Implemented so that image is clickable.
    }// end QGCColoredImage

    MouseArea {
        anchors.fill:       parent
        anchors.margins:    ScreenTools.isMobile ? -ScreenTools.defaultFontPixelHeight : 0
        width:              50

        // If the pin is click make it blue, otherwise keep it red
        onClicked: {
            if(_pinColor == "red"){
                _pinColor = "blue"
                _isVisible = true
            } else {
                _pinColor = "red"
                _isVisible = false;
            }
        } // end onClicked
    } // end MouseArea

    // This function removes a pin from the map, removes it from the pin list, and returns if the clear all pins button should be disabled
    function removePin(pinList,pinId){
        var tempLen = pinList.length    // The number of pins
        var tempObj // The object that will be removed
        var i
        // Loop through the list looking for the object that has the same pin id
        for (i = 0 ; i < tempLen ; ++i){
            tempObj = pinList[i]     // Get a new pin

            // Check if the pin id matches with the pin id from the list and break out of the loop if so
            if (tempObj.componentPinId === pinId){
               break
            }
        } // end for (i = 0 ; i < tempLen ; ++i)

        // Remove the pin and let the user know
        console.log("Removing pinId: " + tempObj.componentPinId)
        tempObj.destroy()            // Remove the pin from the map
        pinList.splice(i,1)             // Remove the pin from the list

        // Check if there are no more pins
        tempLen = pinList.length
        console.log("Pins remaining: " + tempLen + "\n")

        // Return if the clear all pins button should be enabled or disabled
        if (tempLen === 0){
            return false
        }else{
            return true
        }
    } // end function removePin(pinId)
}
