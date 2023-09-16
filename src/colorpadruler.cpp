#include "colorpadruler.h"

using namespace chap;

ColorPadRuler::ColorPadRuler(cv::Mat image)
{
    m_image = std::move(image);
    m_lowerPad = nullptr;
    m_upperPad = nullptr;

    IdentifyPads();
}

cv::Mat ColorPadRuler::Image() const { return m_image; }

const Square &ColorPadRuler::UpperPad() const { return *(m_upperPad); }
const Square &ColorPadRuler::LowerPad() const { return *(m_lowerPad); }


cv::Mat ColorPadRuler::Highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(m_image, imageHighlighted, cv::COLOR_BGR2Lab);
    cv::extractChannel(imageHighlighted, imageHighlighted, 1);

    for(int r = 0; r < imageHighlighted.rows; r++) {
        for(int c = 0; c < imageHighlighted.cols; c++) {
            uchar currVal = imageHighlighted.at<uchar>(r, c);
            uchar newVal = currVal <= LAB_THRESHOLD ? 255 : 0;
            imageHighlighted.at<uchar>(r, c) = newVal;
        }
    }

    return imageHighlighted;
}

bool ColorPadRuler::PadsFound() const
{
    return m_lowerPad != nullptr && m_upperPad != nullptr;
}


void ColorPadRuler::IdentifyPads()
{
    std::vector<Square> pads = Square::FindSquares(Highlight());

    // did we find two pads in the image if not the bowl may be interfering
    //      in the highlighting process
    if ( pads.size() == 2 ) {
        if (pads[0].Center().y > pads[1].Center().y ) {
            m_upperPad = new Square(pads[1]);
            m_lowerPad = new Square(pads[0]);
        } else {
            m_upperPad = new Square(pads[0]);
            m_lowerPad = new Square(pads[1]);
        }
    }
}
