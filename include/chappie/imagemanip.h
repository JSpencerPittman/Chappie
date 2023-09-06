#ifndef MATE2023_IMAGEMANIP_H
#define MATE2023_IMAGEMANIP_H

#include <utility>

#include <opencv2/opencv.hpp>

namespace chap {
    /*
     * @className ImageManip
     * @brief provides a class that provides inplace operations
     *
     * PS: this class is expected to house more functions
     */
    class ImageManip {
    public:
        /*
         * @brief constructor
         *
         * @param m_image m_image to be manipulated
         */
        ImageManip(cv::Mat image);

        /*
         * @brief get m_image
         */
        cv::Mat Image() const;

        /*
         * @brief rotate the m_image
         *
         * @param angle how much to rotate m_image in degrees
         * @param inplace do we save changes to the class's m_image
         */
        cv::Mat RotateImage(int angle, bool inplace = false);

    private:
        cv::Mat m_image;
    };
}


#endif //MATE2023_IMAGEMANIP_H
