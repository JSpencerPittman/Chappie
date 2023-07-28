#include "whitepatch.h"

using namespace chap;

WhitePatch::WhitePatch(cv::Mat image) {
    this->image = std::move(image);
}

cv::Mat WhitePatch::getImage() const { return this->image; }

std::vector<Square> WhitePatch::locate() const
{
    Bowl bowl(this->image);
    BoundingBox bbox = bowl.boundingBox();

    if ( !bbox.doesExist() ) return {};

    std::vector<Square> squares =  Square::findSquares(this->highlight());
    std::vector<Square> approved;
    for( const Square& square : squares ) {
        if ( WhitePatch::squareInBoundingBox(bbox, square) )
            approved.push_back(square);
    }
    return approved;
}

cv::Mat WhitePatch::highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(this->image, imageHighlighted, cv::COLOR_BGR2GRAY);
    cv::threshold(imageHighlighted, imageHighlighted, 150, 255, cv::THRESH_BINARY);
    return imageHighlighted;
};

bool WhitePatch::squareInBoundingBox(const BoundingBox& bbox, const Square& sq)
{
    return WhitePatch::pointInBoundingBox(bbox, sq.getCenter());
}

bool WhitePatch::pointInBoundingBox(const BoundingBox& bbox, const cv::Point& pt)
{
    if (bbox.getLeft() > pt.x || bbox.getRight() < pt.x) return false;
    if (bbox.getTop() > pt.y || bbox.getBottom() < pt.y) return false;
    return true;
}
