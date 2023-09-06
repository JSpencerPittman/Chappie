#include "imagemanip.h"

using namespace chap;

ImageManip::ImageManip( cv::Mat image )
{
    m_image = image.clone();
}

cv::Mat ImageManip::Image() const { return m_image; }

cv::Mat ImageManip::RotateImage(int angle, bool inplace )
{
    // Calculate the rotation matrix to rotate about the calculated m_center
    cv::Point2i center(m_image.cols / 2, m_image.rows / 2);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1);

    // rotate the m_image
    cv::Mat rotatedImage;
    cv::Size2i outputSize(m_image.cols, m_image.rows);
    cv::warpAffine(m_image, rotatedImage, rotationMatrix, outputSize);

    // Save changes to our m_image
    if ( inplace ) m_image = rotatedImage;

    return rotatedImage;
}