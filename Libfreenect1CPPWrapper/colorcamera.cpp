/* 
 *    Copyright [2012] Olaf - blinky0815 - christ ]
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#include <colorcamera.h>

ColorCamera::ColorCamera(freenect_device* ptr) : videoResolution(FREENECT_RESOLUTION_MEDIUM), videoFormat(FREENECT_VIDEO_RGB) {
    devicePtr = ptr;
    freenect_set_user(devicePtr, this);
    freenect_set_video_mode(devicePtr, freenect_find_video_mode(getVideoResolution(), getVideoFormat()));
    freenect_set_video_callback(devicePtr, freenect_video_callback);
}

ColorCamera::~ColorCamera() noexcept(false){
    stopVideo();
    if (freenect_close_device(devicePtr) < 0) {
        throw std::runtime_error("could not close the video camera device");
    }
}

void ColorCamera::startVideo() {
    if (freenect_start_video(devicePtr) < 0) throw std::runtime_error("Cannot start video callback");
}

void ColorCamera::stopVideo() {
    if (freenect_stop_video(devicePtr) < 0) throw std::runtime_error("Cannot stop video callback");
}

void ColorCamera::setVideoFormat(freenect_video_format requested_format, freenect_resolution requested_resolution) {
    if (requested_format != getVideoFormat() || requested_resolution != getVideoResolution()) {
        freenect_stop_video(devicePtr);
        freenect_frame_mode mode = freenect_find_video_mode(requested_resolution, requested_format);
        if (!mode.is_valid) throw std::runtime_error("Cannot set video format: invalid mode");
        if (freenect_set_video_mode(devicePtr, mode) < 0) throw std::runtime_error("Cannot set video format");
        freenect_start_video(devicePtr);
        videoFormat = requested_format;
        videoResolution = requested_resolution;
    }
}

freenect_video_format ColorCamera::getVideoFormat() {
    return videoFormat;
}

freenect_resolution ColorCamera::getVideoResolution() {
    return videoResolution;
}

int ColorCamera::getVideoBufferSize() {
    switch (getVideoFormat()) {
        case FREENECT_VIDEO_RGB:
        case FREENECT_VIDEO_BAYER:
        case FREENECT_VIDEO_IR_8BIT:
        case FREENECT_VIDEO_IR_10BIT:
        case FREENECT_VIDEO_IR_10BIT_PACKED:
        case FREENECT_VIDEO_YUV_RGB:
        case FREENECT_VIDEO_YUV_RAW:
            return freenect_find_video_mode(getVideoResolution(), getVideoFormat()).bytes;
        default:
            return 0;
    }
}

void ColorCamera::videoCallback(void *video, uint32_t timestamp) {
    cout << "if you see this, you're either running the selftest or you forgot to use your own class" << endl;
}