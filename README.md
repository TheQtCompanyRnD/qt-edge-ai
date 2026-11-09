# Qt Edge AI examples

Example applications for the [Qt Edge AI](https://doc-snapshots.qt.io/qtedgeai-1.0/qtedgeai-index.html)
module.

Qt Edge AI runs AI inference on live camera video on the device, without
backend-specific pipeline code. An application describes an inference flow in
QML or C++, and a backend plugin on the target runs it. The module is a
Technical Preview, and its API may change.

## Examples

| Example | Description |
| --- | --- |
| [Simple Face Detection](examples/vision/simplefacedetection) | Shows live camera video and runs face detection on the same frames, logging each result to the console. The inference flow is declared in QML. |

## Requirements

- Qt 6.12.0 or later, with the Qt Edge AI module installed. Qt Edge AI uses Qt
  Core, Qt Qml, Qt Quick, and Qt Multimedia.
- GStreamer 1.20 or later on the target.
- A USB camera.
- One of the supported platforms and its AI runtime:

  | Platform | AI runtime |
  | --- | --- |
  | Qualcomm Dragonwing IQ9 | Edge Impulse, Qualcomm Intelligent Multimedia SDK |
  | NVIDIA Jetson AGX Orin, NVIDIA Jetson Thor | NVIDIA DeepStream |

For each AI runtime there is its own backend provider plugin that the Edge AI module loads.

Qt Edge AI builds on a development PC, but it runs inference only on the
supported platforms. For installation and the full list of prerequisites, see the
[Qt Edge AI documentation](https://doc-snapshots.qt.io/qtedgeai-1.0/qtedgeai-index.html).

## Building

Build an example with the Qt 6.12 kit that has Qt Edge AI installed, for the
target device. For example, for Simple Face Detection:

```sh
qt-cmake -S examples/vision/simplefacedetection -B build
cmake --build build
```

The build copies the backend configuration files for each backend provider in `configs/` next to the
executable, because the backends read them relative to it.

## AI models

The AI models are not part of this repository. Get or train a model for the
backend provider that you use, and put it where its configuration file says, relative to
the executable:

- **Edge Impulse:** train a model in Edge Impulse Studio and deploy it as a
  GStreamer plugin. The configuration expects `libgstedgeimpulse.so`.
- **Qualcomm IM SDK:** use a TensorFlow Lite detection model, for example one
  from the IM SDK's model zoo. The configuration expects
  `models/Lightweight-Face-Detection_w8a8.tflite` and `models/face_detection.labels`.
- **NVIDIA DeepStream:** use the model of the DeepStream sample applications that
  `configs/deepstream/dsdetect1_pgie_config.yml` names.

## Running

Simple Face Detection takes two options:

| Option | Description |
| --- | --- |
| `--provider <name>` | The inference backend provider: `edgeimpulse`, `imsdk` or `deepstream`. |
| `--config <file>` | The provider's configuration file. The default is `configs/<name>/inference.json` next to the executable. |

```sh
# Edge Impulse
./simplefacedetection --provider edgeimpulse

# Qualcomm IM SDK
./simplefacedetection --provider imsdk

# NVIDIA DeepStream
./simplefacedetection --provider deepstream --config configs/deepstream/dsdetect1_config.yml
```

The example shows the camera video in a window and logs the detection results to
the console. It sets `QT_MEDIA_BACKEND` to `gstreamer`, so Qt Multimedia uses the
same media framework as the Qt Edge AI backend. Qt Edge AI can then pass the video
frames to the inference pipeline without copying them, if Qt Multimedia is built
with the Qt GStreamer API (`-DFEATURE_gstreamer_qt_api=ON`). Without it, the
example still runs, but copies every frame.

## Reporting issues

Report problems with Qt Edge AI in the Qt bug tracker, as described in
[Reporting Bugs](https://wiki.qt.io/Reporting_Bugs), and set the Components field
to "Edge AI".

## License

The examples are available under either the BSD 3-Clause license or a Qt
commercial license; see the [LICENSES](LICENSES) directory. The Qt Edge AI
module itself is available under a commercial license only.
