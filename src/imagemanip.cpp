#include "imagemanip.h"

using namespace chap;

ImageManip::ImageManip( cv::Mat image )
{
    this->image = image.clone();
}

cv::Mat ImageManip::getImage() const { return this->image; }

cv::Mat ImageManip::rotateImage( int angle, bool inplace )
{
    cv::Point2i center(this->image.cols / 2, this->image.rows / 2);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1);

    cv::Mat rotatedImage;
    cv::Size2i outputSize(this->image.cols, this->image.rows);
    cv::warpAffine(this->image, rotatedImage, rotationMatrix, outputSize);

    if ( inplace ) this->image = rotatedImage;

    return rotatedImage;
}