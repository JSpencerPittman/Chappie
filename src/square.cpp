#include "square.h"

using namespace chap;

Square::Square(Contour contour)
{
    std::sort(contour.begin(), contour.end(),
              [] ( const cv::Point& a, const cv::Point& b ) {
            return a.y > b.y;
    });

    if ( contour[0].x < contour[1].x ) {
        this->topLeft = contour[0];
        this->topRight = contour[1];
    } else {
        this->topLeft = contour[1];
        this->topRight = contour[0];
    }

    if ( contour[2].x < contour[3].x ) {
        this->botLeft = contour[2];
        this->botRight = contour[3];
    } else {
        this->botLeft = contour[3];
        this->botRight = contour[2];
    }

    this->leftCP = Square::calcMidpoint(this->botLeft, this->topLeft);
    this->topCP = Square::calcMidpoint(this->topLeft, this->topRight);
    this->rightCP = Square::calcMidpoint(this->topRight, this->botRight);
    this->bottomCP = Square::calcMidpoint(this->botRight, this->botLeft);

    this->width = Square::calcDistance(this->leftCP, this->rightCP);
    this->height = Square::calcDistance(this->topCP, this->bottomCP);
    this->area = this->width * this->height;

    cv::Point2i c1 = Square::calcMidpoint(this->topLeft, this->botRight);
    cv::Point2i c2 = Square::calcMidpoint(this->topRight, this->botLeft);
    int cx = ( c1.x + c2.x ) / 2;
    int cy = ( c1.y + c2.y ) / 2;
    this->center = cv::Point2i(cx, cy);

    this->corners = contour;
}

Square::Square(const Square &sq)
{
    this->topLeft = sq.topLeft;
    this->topRight = sq.topRight;
    this->botRight = sq.botRight;
    this->botLeft = sq.botLeft;

    this->leftCP = sq.leftCP;
    this->topCP = sq.topCP;
    this->rightCP = sq.rightCP;
    this->bottomCP = sq.bottomCP;

    this->width = sq.width;
    this->height = sq.height;
    this->area = sq.area;

    this->center = sq.center;

    this->corners = sq.corners;
}

double Square::getWidth() const { return this->width; }
double Square::getHeight() const { return this->height; }
double Square::getArea() const { return this->area; }
cv::Point2i Square::getCenter() const { return this->center; }
Contour Square::getCorners() const { return this->corners; }

std::vector<Square> Square::findSquares(const cv::Mat& imageBinary) {
    std::vector<Contour> contours;
    cv::findContours(imageBinary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<Square> squares;
    auto cntItr = contours.begin();
    for( ; cntItr != contours.end() ; cntItr++ ) {
        double area = cv::contourArea(*cntItr);

        if ( ( area / (double)cntItr->size() ) < AREA_LENGTH_RATIO_THRESHOLD )
            continue;

        double perimeter = cv::arcLength(*cntItr, true);

        Contour approxCurve;
        cv::approxPolyDP(*cntItr, approxCurve, 0.02 * perimeter, true);

        if ( approxCurve.size() == SIDES_ON_SQUARE )
            squares.emplace_back(approxCurve);
    }

    return squares;
}

cv::Point2i Square::calcMidpoint(const cv::Point2i& a, const cv::Point2i& b)
{
    int cx = ( a.x + b.x ) / 2;
    int cy = ( a.y + b.y ) / 2;
    return {cx, cy};
}

double Square::calcDistance(const cv::Point2i& a, const cv::Point2i& b)
{
    int horiDist = a.x - b.x;
    int vertDist = a.y - b.y;
    return std::sqrt(std::pow(horiDist, 2) + std::pow(vertDist, 2));
}

const cv::Point2i &Square::getTopLeft() const { return this->topLeft; }
const cv::Point2i &Square::getTopRight() const { return this->topRight; }
const cv::Point2i &Square::getBotRight() const { return botRight; }
const cv::Point2i &Square::getBotLeft() const { return botLeft; }
const cv::Point2i &Square::getLeftCP() const { return leftCP; }
const cv::Point2i &Square::getTopCP() const { return topCP; }
const cv::Point2i &Square::getRightCP() const { return rightCP; }
const cv::Point2i &Square::getBottomCP() const { return bottomCP; }
