#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include "logging.hpp"

int frameDifference (std::string inputFile, std::string outputFile) {
    cv::VideoCapture input(inputFile);

    if (!input.isOpened()) {
        Log::error("Failed initialise read of input file");
        return 1;
    }

    double totalFrames = input.get(cv::CAP_PROP_FRAME_COUNT);
    std::cout << "There are " << std::to_string(totalFrames) << " frames" << std::endl;

    double fps = input.get(cv::CAP_PROP_FPS);
    cv::VideoWriter output(
        outputFile, cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps, cv::Size(input.get(cv::CAP_PROP_FRAME_WIDTH), input.get(cv::CAP_PROP_FRAME_HEIGHT))
    );
    if (!output.isOpened()) {
        Log::error("Failed to open output file");
        return 1;
    }

    bool rSuccess = true;
    int fNo = 1;
    std::time_t startTime = std::time(nullptr);
    cv::Mat nFrame;
    cv::Mat pFrame;
    while (rSuccess) {
        if (1 < fNo) {
            cv::Mat frame;
            rSuccess = input.read(frame);
            if (rSuccess) {
                nFrame = pFrame - frame;
                pFrame = frame;
            }
        } else {
            cv::Mat frame;
            rSuccess = input.read(frame);
            pFrame = frame.clone();
        }
        if (rSuccess && fNo < totalFrames - 1) {
            output.write(nFrame);
            std::cout << fNo << "/" << totalFrames << std::endl;
        }
        fNo++; 
    }
    std::time_t endTime = std::time(nullptr);

    // auto timeTaken = std::asctime(std::localtime(endTime - startTime));
    // std::cout << "Processed " << totalFrames << " frames in " << timeTaken
    //           << " seconds, averaging " << timeTaken/totalFrames << std::endl;

    return 0;
}