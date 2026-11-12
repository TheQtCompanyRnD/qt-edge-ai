// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtCore/qcommandlineparser.h>
#include <QtEdgeAi/qinferencebackendfactory.h>
#include <QtGui/qguiapplication.h>
#include <QtMultimedia/qcameradevice.h>
#include <QtMultimedia/qmediadevices.h>
#include <QtQml/qqmlapplicationengine.h>

// The largest YUYV format at 30 fps, or a null format to let Qt choose. Qt prefers MJPEG,
// which decodes to I420, and the imsdk converter does not accept I420.
static QCameraFormat preferredCameraFormat()
{
    const auto area = [](const QCameraFormat &format) {
        return format.resolution().width() * format.resolution().height();
    };
    QCameraFormat best;
    const auto formats = QMediaDevices::defaultVideoInput().videoFormats();
    for (const QCameraFormat &format : formats) {
        if (format.pixelFormat() != QVideoFrameFormat::Format_YUYV || format.maxFrameRate() < 29.f)
            continue;
        if (best.isNull() || area(format) > area(best))
            best = format;
    }
    return best;
}

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

    // The step provider that runs the model, and the configuration file the vision step reads:
    // by default configs/<provider>/inference.json, relative to the directory of the
    // executable.
    QCommandLineParser parser;
    parser.addHelpOption();
    const QCommandLineOption providerOption(
            QStringLiteral("provider"),
            QStringLiteral("The name of the inference step provider. The default is the "
                           "first available vision step provider."),
            QStringLiteral("name"));
    parser.addOption(providerOption);
    // Not every provider reads a JSON file: some have a configuration format of their own.
    const QCommandLineOption configOption(
            QStringLiteral("config"),
            QStringLiteral("The provider's configuration file. The default is "
                           "configs/<provider>/inference.json relative to the directory "
                           "of the executable."),
            QStringLiteral("file"));
    parser.addOption(configOption);
    parser.process(app);
    QString provider = parser.value(providerOption);
    if (!parser.isSet(providerOption)) {
        // The providers that the target has depend on the SDKs that it was built with.
        provider = QInferenceBackendFactory::instance()->defaultStepProvider();
        if (provider.isEmpty()) {
            qWarning("No vision step provider is available. Select one with --provider.");
            return 1;
        }
    } else if (provider.isEmpty()) {
        qWarning("The provider name must not be empty");
        return 1;
    }
    QString configFile = parser.value(configOption);
    if (configFile.isEmpty())
        configFile = QLatin1StringView("configs/") + provider + QLatin1StringView("/inference.json");

    QQmlApplicationEngine engine;
    engine.setInitialProperties({ { QStringLiteral("provider"), provider },
                                  { QStringLiteral("configFile"), configFile },
                                  { QStringLiteral("cameraFormat"),
                                    QVariant::fromValue(preferredCameraFormat()) } });
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("SimpleFaceDetection", "Main");
    return app.exec();
}
