#ifndef FACCDETECTION_H
#define FACCDETECTION_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

#define DLIB_PNG_SUPPORT
#define DLIB_JPEG_SUPPORT

using namespace dlib;
using namespace std;

class FaccDetection
{
public:
    FaccDetection();
};

#endif // FACCDETECTION_H
