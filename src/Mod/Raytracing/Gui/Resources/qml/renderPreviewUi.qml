import QtQuick 1.1
Item {

    id: previewWidget
    function updatePreview() {
        // Bit of a hack to reload the image
        if(previewImage.source = "image://previewImage/reload/picture"){
             previewImage.source = "image://previewImage/picture"
        } else {
              previewImage.source = "image://previewImage/reload/picture"
        }
        if(!previewLoaded)
        {
            loadingAnim.start()
        }

        previewLoaded = true;
    }

    signal stopRender()
    signal saveOutput()

    width: 1600
    height: 1000
    property bool previewLoaded: false

    Rectangle {
        id: rectangle1
        anchors.fill: parent
       gradient: Gradient {
           GradientStop { position: 0.0; color: "#333"}
                   GradientStop { position: 1.0; color: "#555" }
               }

      Loading {
          id: imageLoading
           anchors.centerIn:  rectangle1
        }

        PreviewImage {
            id: previewImage
            anchors.centerIn: parent;
            opacity: 0
        }

       Row {
           anchors.right: parent.right
           anchors.rightMargin: 30
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 25
           spacing: 10

            Button {
                id: saveButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                text: qsTr("Save")
                enabled: false
                onClicked: previewWidget.saveOutput()

            }
            Button {
                id: stopButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                text: qsTr("Stop Render")
                onClicked: previewWidget.stopRender()
            }
       }
    }
    states: [
        State {name:"enabled"; when: previewLoaded;
            PropertyChanges { target: saveButton; enabled: true}
    }]

    SequentialAnimation {
        id: loadingAnim
             running: false
             NumberAnimation  { target: imageLoading; property: "opacity"; to: 0; duration: 500 }
             NumberAnimation { target: previewImage; property: "opacity"; to: 1; duration: 500 }
         }

}
