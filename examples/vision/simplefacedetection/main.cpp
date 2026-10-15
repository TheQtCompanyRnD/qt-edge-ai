// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtGui/qguiapplication.h>
#include <QtQml/qqmlapplicationengine.h>

int main(int argc, char *argv[])
{
    // Optional, but keeps every video frame out of an extra memcpy: when Qt Multimedia
    // also uses GStreamer, QtEdgeAi's GStreamer backend can pass its GstBuffers straight
    // into the inference pipeline. Without it the example still runs, it just copies
    // each frame. Set before QGuiApplication, because Qt Multimedia reads this when it
    // creates its media integration, which happens on the first camera or capture session.
    // Note this is Qt Multimedia's variable; QtEdgeAi selects its own backend with
    // QT_INFERENCE_FLOW_BACKEND.
    qputenv("QT_MEDIA_BACKEND", "gstreamer");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("SimpleFaceDetection", "Main");
    return app.exec();
}
