#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>

#include "logging.hpp"

class Video {
    public:
        Video (std::string path)
            : iPath(path),
              iOpenForRead(false),
              iOpenForWrite(false) {}

        ~Video () {
            if (iReadCap != nullptr)
                delete iReadCap;
            if (iWriteCap != nullptr)
                delete iWriteCap;
        }

        int readOpen() {
            iReadCap = new cv::VideoCapture(iPath);

            if (!iReadCap->isOpened()) {
                Log::error("Failed to open video for read: " + iPath);
                return 1;
            }

            iTotalFrames = iReadCap->get(cv::CAP_PROP_FRAME_COUNT);
            iFPS = iReadCap->get(cv::CAP_PROP_FPS);

            iOpenForRead = true;
        }

        /*
        int writeOpen() {
            iWriteCap = new cv::VideoWriter(iPath);

            if (!iWriteCap->isOpened()) {
                Log::error("Failed to open video for write: " + iPath);
                return 1;
            }

            iTotalFrames = iReadCap->get(cv::CAP_PROP_FRAME_COUNT);
            iFPS = iReadCap->get(cv::CAP_PROP_FPS);

            iOpenForWrite = true;
        }
        */

        cv::Mat readFrame() {
            if (!iOpenForRead) {
                Log::error("Tried to read video that is not open for reading!");
                throw;
            }

            cv::Mat frame;
            bool success = iReadCap->read(frame);
            if (!success) {
                Log::error("Failed to read video frame");
                throw;
            }

            return frame;
        }

        int writeFrame(cv::Mat frame) {
            if (!iOpenForWrite) {
                Log::error("Tried to write video that is not open for writing!");
                throw;
            }

            iWriteCap->write(frame);
            return 0;
        }
    
    private:
        std::string iPath;
        cv::VideoCapture* iReadCap;
        cv::VideoWriter* iWriteCap;
        double iTotalFrames;
        double iFPS;
        bool iOpenForRead;
        bool iOpenForWrite;
};