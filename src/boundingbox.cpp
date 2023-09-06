#include "boundingbox.h"

using namespace chap;

BoundingBox::BoundingBox() {
    m_left = m_top = m_right = m_bottom = -1;
    m_width = m_height = m_area = m_hypotenuse = -1;
    m_center = cv::Point2i(-1, -1);
    m_exists = false;
};

BoundingBox::BoundingBox(chap::Outline outline)
{
    // Find the edges of the outline
    int minX, maxX, minY, maxY;
    minX = outline[0].x, minY = outline[0].y;
    maxX = outline[0].x, maxY = outline[0].y;

    auto ptItr = outline.begin();
    for( ; ptItr != outline.end() ; ptItr++ ) {
        minX = std::min(ptItr->x, minX);
        maxX = std::max(ptItr->x, maxX);

        minY = std::min(ptItr->y, minY);
        maxY = std::max(ptItr->y, maxY);
    }

    m_left = minX;
    m_top = minY;
    m_right = maxX;
    m_bottom = maxY;

    // Calculate the dimensions of the box
    m_width = m_right - m_left;
    m_height = m_bottom - m_top;
    m_area = m_width * m_height;

    m_hypotenuse = 0;
    m_hypotenuse += std::pow(m_width, 2);
    m_hypotenuse += std::pow(m_height, 2);
    m_hypotenuse = std::sqrt(m_hypotenuse);

    // Find the m_center of the box
    int cX = m_left + (m_width / 2);
    int cY = m_top + (m_height / 2);
    m_center = cv::Point2i(cX, cY);

    // A common problem is an outline surrounds the whole m_image which is invalid
    //  including 0,0
    //  @todo implement this in the outline code not the bounding box
    if(m_left == 0 || m_top == 0) m_exists = false;
    else m_exists = true;
}

int BoundingBox::Left() const { return m_left; }
int BoundingBox::Top() const { return m_top; }
int BoundingBox::Right() const { return m_right; }
int BoundingBox::Bottom() const { return m_bottom; }
int BoundingBox::Width() const { return m_width; }
int BoundingBox::Height() const { return m_height; }
int BoundingBox::Area() const { return m_area; }
double BoundingBox::Hypotenuse() const { return m_hypotenuse; }
cv::Point BoundingBox::Center() const { return m_center; }

bool BoundingBox::doesExist() const { return m_exists; }

cv::Mat BoundingBox::zoomInOnBoundingBox(cv::Mat image, const BoundingBox& bbox)
{
    cv::Size2d patchSize(bbox.Hypotenuse(), bbox.Hypotenuse());

    cv::Mat imageZoomed;
    cv::getRectSubPix(image, patchSize, bbox.Center(), imageZoomed);

    return imageZoomed;
}
