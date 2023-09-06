#include "display.h"

using namespace chap;

Display::Display(const cv::Mat& image)
{
    this->m_image = image.clone();
}

cv::Mat Display::Image() const { return this->m_image; }

void Display::DrawOutline(const chap::Outline& outline)
{
    cv::Scalar red(0, 0, 255);

    auto ptItr = outline.begin();
    for( ; ptItr != outline.end(); ptItr++ )
        this->DrawPoint(*ptItr);
}

void Display::DrawBoundingBox(const BoundingBox& bbox, int linewidth, char color)
{
    const cv::Point2i topLeft(bbox.Left(), bbox.Top());
    const cv::Point2i topRight(bbox.Right(), bbox.Top());
    const cv::Point2i botRight(bbox.Right(), bbox.Bottom());
    const cv::Point2i botLeft(bbox.Left(), bbox.Bottom());

    // left side
    this->DrawLine(botLeft, topLeft, linewidth, color);
    // top side
    this->DrawLine(topLeft, topRight, linewidth, color);
    // right side
    this->DrawLine(topRight, botRight, linewidth, color);
    // bottom side
    this->DrawLine(botRight, botLeft, linewidth, color);
}

void Display::DrawSquare(const Square &sq, char color, int linewidth, float alpha)
{
    this->DrawSquareBorder(sq, linewidth, color);
    this->DrawSquareFill(sq, alpha, color);
    this->DrawSquareCross(sq, linewidth, color);
}

void Display::DrawSquareBorder(const Square &sq, int linewidth, char color)
{
    this->DrawLine(sq.BotLeft(), sq.TopLeft(), linewidth, color);
    this->DrawLine(sq.TopLeft(), sq.TopRight(), linewidth, color);
    this->DrawLine(sq.TopRight(), sq.BotRight(), linewidth, color);
    this->DrawLine(sq.BotRight(), sq.BotLeft(), linewidth, color);
}

void Display::DrawSquareFill(const Square &sq, float alpha, char color)
{
    cv::Scalar colorExpanded = Display::TranscribeColor(color);

    std::vector<cv::Point2i> vertices;
    vertices.push_back(sq.TopLeft());
    vertices.push_back(sq.TopRight());
    vertices.push_back(sq.BotRight());
    vertices.push_back(sq.BotLeft());

    cv::Mat imageCopy = m_image.clone();

    cv::fillConvexPoly(imageCopy, vertices, colorExpanded);

    cv::addWeighted(this->m_image, 1 - alpha, imageCopy, alpha, 0, this->m_image);
}

void Display::DrawSquareCross(const Square &sq, int linewidth, char color)
{
    this->DrawLine(sq.LeftMidpoint(), sq.RightMidpoint(), linewidth, color);
    this->DrawLine(sq.TopMidpoint(), sq.BottomMidpoint(), linewidth, color);
}

void Display::DrawPoint(const cv::Point& pt, int radius, char color)
{
    cv::Scalar expandedColor = Display::TranscribeColor(color);
    cv::circle(this->m_image, pt, radius, expandedColor, cv::FILLED);
}

void Display::DrawLine(const cv::Point& pt1, const cv::Point& pt2, int linewidth, char color)
{
    cv::Scalar expandedColor = Display::TranscribeColor(color);
    cv::line(this->m_image, pt1, pt2, expandedColor, linewidth);
}

cv::Scalar Display::TranscribeColor(char c) {
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