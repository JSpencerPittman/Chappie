#include "square.h"

using namespace chap;

Square::Square(Contour contour)
{
    // sort points based on y coordinate in descending order
    std::sort(contour.begin(), contour.end(),
              [] ( const cv::Point& a, const cv::Point& b ) {
            return a.y > b.y;
    });

    // Determine each corner
    if ( contour[0].x < contour[1].x ) {
        topLeft = contour[0];
        topRight = contour[1];
    } else {
        topLeft = contour[1];
        topRight = contour[0];
    }

    if ( contour[2].x < contour[3].x ) {
        botLeft = contour[2];
        botRight = contour[3];
    } else {
        botLeft = contour[3];
        botRight = contour[2];
    }

    // determine midpoints
    m_leftMidpoint = Square::CalcMidpoint(botLeft, topLeft);
    m_topMidpoint = Square::CalcMidpoint(topLeft, topRight);
    m_rightMidpoint = Square::CalcMidpoint(topRight, botRight);
    m_bottomMidpoint = Square::CalcMidpoint(botRight, botLeft);

    // determine dimensions
    m_width = Square::CalcDistance(m_leftMidpoint, m_rightMidpoint);
    m_height = Square::CalcDistance(m_topMidpoint, m_bottomMidpoint);
    m_area = m_width * m_height;

    // find the center
    cv::Point2i c1 = Square::CalcMidpoint(topLeft, botRight);
    cv::Point2i c2 = Square::CalcMidpoint(topRight, botLeft);
    int cx = ( c1.x + c2.x ) / 2;
    int cy = ( c1.y + c2.y ) / 2;
    m_center = cv::Point2i(cx, cy);

    m_corners = contour;
}

Square::Square(const Square &sq)
{
    topLeft = sq.topLeft;
    topRight = sq.topRight;
    botRight = sq.botRight;
    botLeft = sq.botLeft;

    m_leftMidpoint = sq.m_leftMidpoint;
    m_topMidpoint = sq.m_topMidpoint;
    m_rightMidpoint = sq.m_rightMidpoint;
    m_bottomMidpoint = sq.m_bottomMidpoint;

    m_width = sq.m_width;
    m_height = sq.m_height;
    m_area = sq.m_area;

    m_center = sq.m_center;

    m_corners = sq.m_corners;
}

const cv::Point2i &Square::TopLeft() const { return topLeft; }
const cv::Point2i &Square::TopRight() const { return topRight; }
const cv::Point2i &Square::BotRight() const { return botRight; }
const cv::Point2i &Square::BotLeft() const { return botLeft; }
const cv::Point2i &Square::LeftMidpoint() const { return m_leftMidpoint; }
const cv::Point2i &Square::TopMidpoint() const { return m_topMidpoint; }
const cv::Point2i &Square::RightMidpoint() const { return m_rightMidpoint; }
const cv::Point2i &Square::BottomMidpoint() const { return m_bottomMidpoint; }
double Square::Width() const { return m_width; }
double Square::Height() const { return m_height; }
double Square::Area() const { return m_area; }
cv::Point2i Square::Center() const { return m_center; }
Contour Square::Corners() const { return m_corners; }

std::vector<Square> Square::FindSquares(const cv::Mat& imageBinary) {
    // Find all countours in the m_image
    std::vector<Contour> contours;
    cv::findContours(imageBinary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    std::vector<Square> squares;
    auto cntItr = contours.begin();
    for( ; cntItr != contours.end() ; cntItr++ ) {
        // A square should have a high area:length ratio
        double area = cv::contourArea(*cntItr);
        if ( ( area / (double)cntItr->size() ) < AREA_LENGTH_RATIO_THRESHOLD )
            continue;

        double perimeter = cv::arcLength(*cntItr, true);

        // Approximate the contour as a polygon
        Contour approxCurve;
        cv::approxPolyDP(*cntItr, approxCurve, 0.02 * perimeter, true);

        // if our polygon is 4 sided then our contour resembles a square
        if ( approxCurve.size() == SIDES_ON_SQUARE )
            squares.emplace_back(approxCurve);
    }

    return squares;
}

cv::Point2i Square::CalcMidpoint(const cv::Point2i& a, const cv::Point2i& b)
{
    int cx = ( a.x + b.x ) / 2;
    int cy = ( a.y + b.y ) / 2;
    return {cx, cy};
}

double Square::CalcDistance(const cv::Point2i& a, const cv::Point2i& b)
{
    int horiDist = a.x - b.x;
    int vertDist = a.y - b.y;
    return std::sqrt(std::pow(horiDist, 2) + std::pow(vertDist, 2));
}
