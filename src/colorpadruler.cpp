#include "colorpadruler.h"

using namespace chap;

ColorPadRuler::ColorPadRuler(cv::Mat image, bool exclusion)
{
    m_image = std::move(image);
    m_lowerPad = nullptr;
    m_upperPad = nullptr;

    IdentifyPads(exclusion);
}

cv::Mat ColorPadRuler::Image() const { return m_image; }

const Square &ColorPadRuler::UpperPad() const { return *(m_upperPad); }
const Square &ColorPadRuler::LowerPad() const { return *(m_lowerPad); }


cv::Mat ColorPadRuler::Highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(m_image, imageHighlighted, cv::COLOR_BGR2Lab);
    cv::extractChannel(imageHighlighted, imageHighlighted, 1);
    cv::threshold(imageHighlighted, imageHighlighted, 127, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    return imageHighlighted;
}

bool ColorPadRuler::PadsFound() const
{
    return m_lowerPad != nullptr && m_upperPad != nullptr;
}


void ColorPadRuler::IdentifyPads(bool exclusion)
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
    } else if ( exclusion ) IdentifyPadsWithBowl();
}

void ColorPadRuler::IdentifyPadsWithBowl() {
    // Locate the bowl
    Bowl bowl(m_image);
    BoundingBox bbox = bowl.FindBoundingBox();

    if ( !bbox.doesExist() ) return;

    // Remove bowl from the image
    cv::Mat imageWithoutBowl = HorizontalExclusion(bbox.Left(), bbox.Right());

    // Find the device in the image without the bowl
    ColorPadRuler cpr(imageWithoutBowl, false);

    // Did it find it?
    if (cpr.m_lowerPad == nullptr || cpr.m_upperPad == nullptr ) return;

    // Extract cpr's detected pads
    Square lowPadExcl = *(cpr.m_lowerPad);
    Square uppPadExcl = *(cpr.m_upperPad);

    // Translate the location from the image without the bowl
    //  to the location in the image with a bowl still in it
    int leftSideWidth = bbox.Left();
    int exclWidth = bbox.Right() - bbox.Left() + 1;

    if (lowPadExcl.Center().x >= leftSideWidth ) {
        lowPadExcl = ColorPadRuler::HorizontalSquareShift(lowPadExcl, exclWidth);
        uppPadExcl = ColorPadRuler::HorizontalSquareShift(uppPadExcl, exclWidth);
    }

    // Update pads
    m_lowerPad = new Square(lowPadExcl);
    m_upperPad = new Square(uppPadExcl);
}

cv::Mat ColorPadRuler::HorizontalExclusion(int left, int right)
{
    cv::Range allRows(0, m_image.rows);

    cv::Range leftColRange(0, left);
    cv::Range rightColRange((right+1), m_image.cols);

    cv::Mat leftSide = m_image(allRows, leftColRange);
    cv::Mat rightSide = m_image(allRows, rightColRange);

    if ( !leftSide.cols && !rightSide.cols ) return m_image;
    else if ( !leftSide.cols ) return rightSide;
    else if ( !rightSide.cols ) return leftSide;

    cv::Mat merged;
    cv::hconcat(leftSide, rightSide, merged);

    return merged;
}

Square ColorPadRuler::HorizontalSquareShift(const Square& sq, int shift)
{
    Contour corners = sq.Corners();
    auto corItr = corners.begin();
    for( ; corItr != corners.end(); corItr++ )
        corItr->x += shift;
    return { corners };
}