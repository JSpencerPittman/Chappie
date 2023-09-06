#include "whitepatch.h"

using namespace chap;

WhitePatch::WhitePatch(cv::Mat image) {
    this->m_image = std::move(image);
}

cv::Mat WhitePatch::Image() const { return this->m_image; }

std::vector<Square> WhitePatch::Locate() const
{
    // White patches will be on the bowl therefore we need to detect it
    Bowl bowl(this->m_image);
    BoundingBox bbox = bowl.FindBoundingBox();

    // If there's no bowl there are no patches
    if ( !bbox.doesExist() ) return {};

    // Find all squares in the entire m_image
    std::vector<Square> squares = Square::FindSquares(this->Highlight());

    // Now only keep the squares in the bounding box of the detected bowl
    std::vector<Square> approved;
    for( const Square& square : squares ) {
        if ( WhitePatch::squareInBoundingBox(bbox, square) )
            approved.push_back(square);
    }
    return approved;
}

cv::Mat WhitePatch::Highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(this->m_image, imageHighlighted, cv::COLOR_BGR2GRAY);
    cv::threshold(imageHighlighted, imageHighlighted, 150, 255, cv::THRESH_BINARY);
    return imageHighlighted;
};

bool WhitePatch::squareInBoundingBox(const BoundingBox& bbox, const Square& sq)
{
    return WhitePatch::pointInBoundingBox(bbox, sq.Center());
}

bool WhitePatch::pointInBoundingBox(const BoundingBox& bbox, const cv::Point& pt)
{
    if (bbox.Left() > pt.x || bbox.Right() < pt.x) return false;
    if (bbox.Top() > pt.y || bbox.Bottom() < pt.y) return false;
    return true;
}
