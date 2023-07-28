#include "colorpadruler.h"

using namespace chap;

ColorPadRuler::ColorPadRuler(cv::Mat image, bool exclusion)
{
    this->image = std::move(image);
    this->lowerPad = nullptr;
    this->upperPad = nullptr;

    this->identifyPads(exclusion);
}

cv::Mat ColorPadRuler::getImage() const { return this->image; }

const Square &ColorPadRuler::getUpperPad() const { return *(this->upperPad); }
const Square &ColorPadRuler::getLowerPad() const { return *(this->lowerPad); }


cv::Mat ColorPadRuler::highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(this->image, imageHighlighted, cv::COLOR_BGR2Lab);
    cv::extractChannel(imageHighlighted, imageHighlighted, 1);
    cv::threshold(imageHighlighted, imageHighlighted, 127, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
    return imageHighlighted;
}

bool ColorPadRuler::padsFound() const
{
    return this->lowerPad != nullptr && this->upperPad != nullptr;
}


void ColorPadRuler::identifyPads( bool exclusion )
{
    std::vector<Square> pads = Square::findSquares(this->highlight());

    if ( pads.size() == 2 ) {
        if ( pads[0].getCenter().y > pads[1].getCenter().y ) {
            this->upperPad = new Square(pads[1]);
            this->lowerPad = new Square(pads[0]);
        } else {
            this->upperPad = new Square(pads[0]);
            this->lowerPad = new Square(pads[1]);
        }
    } else if ( exclusion ) this->identifyPadsWithBowl();
}

void ColorPadRuler::identifyPadsWithBowl() {
    Bowl bowl(this->image);
    BoundingBox bbox = bowl.boundingBox();

    if ( !bbox.doesExist() ) return;

    cv::Mat imageWithoutBowl = this->horizontalExclusion(bbox.getLeft(), bbox.getRight());

    ColorPadRuler cpr(imageWithoutBowl, false);

    if ( cpr.lowerPad == nullptr || cpr.upperPad == nullptr ) return;

    Square lowPadExcl = *(cpr.lowerPad);
    Square uppPadExcl = *(cpr.upperPad);

    int leftSideWidth = bbox.getLeft();
    int exclWidth = bbox.getRight() - bbox.getLeft() + 1;

    if ( lowPadExcl.getCenter().x >= leftSideWidth ) {
        lowPadExcl = ColorPadRuler::horizontalSquareShift(lowPadExcl, exclWidth);
        uppPadExcl = ColorPadRuler::horizontalSquareShift(uppPadExcl, exclWidth);
    }

    this->lowerPad = new Square(lowPadExcl);
    this->upperPad = new Square(uppPadExcl);
}

cv::Mat ColorPadRuler::horizontalExclusion(int left, int right)
{
    cv::Range allRows(0, this->image.rows);

    cv::Range leftColRange(0, left);
    cv::Range rightColRange((right+1), this->image.cols);

    cv::Mat leftSide = this->image(allRows, leftColRange);
    cv::Mat rightSide = this->image(allRows, rightColRange);

    if ( !leftSide.cols && !rightSide.cols ) return this->image;
    else if ( !leftSide.cols ) return rightSide;
    else if ( !rightSide.cols ) return leftSide;

    cv::Mat merged;
    cv::hconcat(leftSide, rightSide, merged);

    return merged;
}

Square ColorPadRuler::horizontalSquareShift(const Square& sq, int shift)
{
    Contour corners = sq.getCorners();
    auto corItr = corners.begin();
    for( ; corItr != corners.end(); corItr++ )
        corItr->x += shift;
    return { corners };
}