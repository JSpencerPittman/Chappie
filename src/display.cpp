#include "display.h"

using namespace chap;

Display::Display(const cv::Mat& image)
{
    this->image = image.clone();
}

cv::Mat Display::getImage() const { return this->image; }

void Display::drawOutline(const chap::Outline& outline)
{
    cv::Scalar red(0, 0, 255);

    auto ptItr = outline.begin();
    for( ; ptItr != outline.end(); ptItr++ )
        this->drawPoint(*ptItr);
}

void Display::drawBoundingBox(const BoundingBox& bbox, int linewidth, char color)
{
    const cv::Point2i topLeft(bbox.getLeft(), bbox.getTop());
    const cv::Point2i topRight(bbox.getRight(), bbox.getTop());
    const cv::Point2i botRight(bbox.getRight(), bbox.getBottom());
    const cv::Point2i botLeft(bbox.getLeft(), bbox.getBottom());

    // leftCP side
    this->drawLine(botLeft, topLeft, linewidth, color);
    // topCP side
    this->drawLine(topLeft, topRight, linewidth, color);
    // rightCP side
    this->drawLine(topRight, botRight, linewidth, color);
    // bottomCP side
    this->drawLine(botRight, botLeft, linewidth, color);
}

void Display::drawSquare(const Square &sq, char color, int linewidth, float alpha)
{
    this->drawSquareBorder(sq, linewidth, color);
    this->drawSquareFill(sq, alpha, color);
    this->drawSquareCross(sq, linewidth, color);
}

void Display::drawSquareBorder(const Square &sq, int linewidth, char color)
{
    this->drawLine(sq.getBotLeft(), sq.getTopLeft(), linewidth, color);
    this->drawLine(sq.getTopLeft(), sq.getTopRight(), linewidth, color);
    this->drawLine(sq.getTopRight(), sq.getBotRight(), linewidth, color);
    this->drawLine(sq.getBotRight(), sq.getBotLeft(), linewidth, color);
}

void Display::drawSquareFill(const Square &sq, float alpha, char color)
{
    cv::Scalar colorExpanded = Display::decryptColor(color);

    std::vector<cv::Point2i> vertices;
    vertices.push_back(sq.getTopLeft());
    vertices.push_back(sq.getTopRight());
    vertices.push_back(sq.getBotRight());
    vertices.push_back(sq.getBotLeft());

    cv::Mat imageCopy = image.clone();

    cv::fillConvexPoly(imageCopy, vertices, colorExpanded);

    cv::addWeighted(this->image, 1 - alpha, imageCopy, alpha, 0, this->image);
}

void Display::drawSquareCross(const Square &sq, int linewidth, char color)
{
    this->drawLine(sq.getLeftCP(), sq.getRightCP(), linewidth, color);
    this->drawLine(sq.getTopCP(), sq.getBottomCP(), linewidth, color);
}

void Display::drawPoint(const cv::Point& pt, int radius, char color)
{
    cv::Scalar expandedColor = Display::decryptColor(color);
    cv::circle(this->image, pt, radius, expandedColor, cv::FILLED);
}

void Display::drawLine(const cv::Point& pt1, const cv::Point& pt2, int linewidth, char color)
{
    cv::Scalar expandedColor = Display::decryptColor(color);
    cv::line(this->image, pt1, pt2, expandedColor, linewidth);
}

cv::Scalar Display::decryptColor(char c) {
    switch (c) {
        case 'b': // blue
            return {255, 0, 0};
        case 'g': // green
            return {0, 255, 0};
        case 'r': // red
            return {0, 0, 255};
        case 'o': // orange
            return {0, 165, 255};
        case 'p': // purple
            return {255, 51, 153};
        default:
            return {0, 0, 0};
    }
}