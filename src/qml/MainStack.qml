import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0

Rectangle {

    Control {
        id: backcontrol
        hoverEnabled: true // 开启 Hover 属性
        property Palette backgroundColor: Palette {
            normal: "#F8F8F8"
            normalDark:"#000000"
        }
    }
    //标题栏
    ViewTopTitle{
         id:titleRect
         z:parent.z+1
    }

    id: stackView

    property int currentWidgetIndex: 0

//    property alias source: imageViewer.source
//    initialItem: rect
    anchors.fill: parent
    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_Control:
            global.ctrlPressed = true
            console.info("control btn clicked..")
            break
        }
        event.accepted = true
    }
    Keys.onReleased: {
        switch (event.key) {
        case Qt.Key_Control:
            global.ctrlPressed = false
            console.info("control btn released..")
            break
        }
    }

    OpenImageWidget{
        anchors.fill: parent
        visible: currentWidgetIndex===0?true:false

    }

    FullThumbnail{
        anchors.fill: parent
        visible: currentWidgetIndex===1?true:false
        id :mainView
    }

    SliderShow{
        id:sliderMainShow
        visible: currentWidgetIndex===2?true:false
        anchors.fill: parent
    }

    DropArea {
        id: dropArea;
        anchors.fill: parent;
        onEntered: {
            background.color = "gray";
            drag.accept (Qt.CopyAction);
            console.log("onEntered");
        }
        onDropped: {
            console.log ("onDropped");
            var DropImagePath =""
            if(drop.hasUrls){
                for(var i = 0; i < drop.urls.length; i++){
                    console.log(drop.urls[i]);
                    if(fileControl.isImage(drop.urls[i])){
                        mainView.sourcePaths = fileControl.getDirImagePath(drop.urls[i]);
                        mainView.source = drop.urls[i]
                        mainView.currentIndex=mainView.sourcePaths.indexOf(mainView.source)
                        if(mainView.sourcePaths.length >0){

                            mainView.setThumbnailCurrentIndex(mainView.sourcePaths.indexOf(mainView.source))
                            console.log( "test",mainView.source)
                            stackView.currentWidgetIndex= 1
                        }
                        i =drop.urls.length
                        return
                    }
                }
            }


        }
        onExited: {
            bckground.color = "white";
            console.log ("onExited");
        }
    }
//    interactive: false
//    currentIndex: currentWidgetIndex

}
