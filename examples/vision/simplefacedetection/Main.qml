// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtMultimedia
import QtEdgeAi
import SimpleFaceDetection

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Camera")

    // Set from the --backend command line option; "edgeimpulse" by default.
    required property string backend
    // Set from the --config command line option; "<backend>_inference.json" by default.
    required property string configFile

    Component.onCompleted: {
        session.camera.start()
        pipeline.start(window)
    }

    CaptureSession {
        id: session
        camera: Camera {}
        videoOutput: videoOutput
    }

    VideoOutput {
        id: videoOutput
        anchors.fill: parent
    }

    InferenceFlow {
        id: pipeline

        InferenceVideoSource {
            id: videoSource
            captureSession: session
        }

        VisionInference {
            id: detectionStep
            name: window.backend
            configFile: window.configFile
        }

        InferenceResultParser {
            id: jsonParser
            onDataAvailable: data => console.log(data)
        }
    }
}
