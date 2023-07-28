#ifndef MATE2023_IMAGEMANIP_H
#define MATE2023_IMAGEMANIP_H

#include <utility>

#include <opencv2/opencv.hpp>

namespace chap {
    class ImageManip {
    public:
        ImageManip(cv::Mat image);

        cv::Mat getImage() const;

        cv::Mat rotateImage(int angle, bool inplace = false);

    private:
        cv::Mat image;
    };
}


#endif //MATE2023_IMAGEMANIP_H
