/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick          2.3
import QtQuick.Dialogs  1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0

FactTextField {
    unitsLabel:         fact ? fact.units : ""
    extraUnitsLabel:    fact ? _altitudeModeExtraUnits : ""
    showUnits:          true
    showHelp:           true

    property int altitudeMode: QGroundControl.AltitudeModeNone

    readonly property string _altModeNoneExtraUnits:            ""
    readonly property string _altModeRelativeExtraUnits:        qsTr("(Rel)")
    readonly property string _altModeAbsoluteExtraUnits:        qsTr("(AMSL)")
    readonly property string _altModeAboveTerrainExtraUnits:    qsTr("(Abv Terr)")
    readonly property string _altModeTerrainFrameExtraUnits:    qsTr("(TerrF)")

    property string _altitudeModeExtraUnits:    _altModeNoneExtraUnits
    property Fact   _terrainFrameWarning:       QGroundControl.settingsManager.planViewSettings.terrainFrameWarning

    onAltitudeModeChanged: updateAltitudeModeExtraUnits()

    function updateAltitudeModeExtraUnits() {
        if (altitudeMode === QGroundControl.AltitudeModeNone) {
            _altitudeModeExtraUnits = _altModeNoneExtraUnits
        } else if (altitudeMode === QGroundControl.AltitudeModeRelative) {
            //_altitudeModeExtraUnits = _altModeRelativeExtraUnits
            _altitudeModeExtraUnits = "" // Showing (rel) all the time is too noisy
        } else if (altitudeMode === QGroundControl.AltitudeModeAbsolute) {
            _altitudeModeExtraUnits = _altModeAbsoluteExtraUnits
        } else if (altitudeMode === QGroundControl.AltitudeModeAboveTerrain) {
            _altitudeModeExtraUnits = _altModeAboveTerrainExtraUnits
        } else if (altitudeMode === QGroundControl.AltitudeModeTerrainFrame) {
            if (!_terrainFrameWarning.rawValue) {
                mainWindow.showComponentDialog(terrainFrameWarning, qsTr("Warning"), mainWindow.showDialogDefaultWidth, StandardButton.Ok)
            }
            _altitudeModeExtraUnits = _altModeTerrainFrameExtraUnits
        } else {
            console.log("AltitudeFactTextField Internal error: Unknown altitudeMode", altitudeMode, QGroundControl.AltitudeModeTerrainFrame)
            _altitudeModeExtraUnits = ""
        }
    }

    Component {
        id: terrainFrameWarning
        QGCViewDialog {
            ColumnLayout {
                anchors.left:   parent.left
                anchors.right:  parent.right
                spacing:        ScreenTools.defaultFontPixelHeight

                QGCLabel {
                    Layout.fillWidth:   true
                    wrapMode:           Text.WordWrap
                    text:               qsTr("'Terrain Frame' is only supported by Ardupilot with a downward facing rangefinder. Use with caution.")
                }

                FactCheckBox {
                    text: qsTr("Don't show again")
                    fact: _terrainFrameWarning
                }
            }
        }
    }
}
