#include "crosspoints.h"

using namespace chap;

CrossPoints::CrossPoints(const chap::Outline &outline, const BoundingBox &bbox)
{
    enum Sides { left, top, right, bottom };

    std::vector<cv::Point2i> crossPoints(4, cv::Point2i(0,0));
    // Sometimes the outline intersects the bounding box multiple times
    //  so we need to keep count of how many intersections on each side
    std::vector<int> crossPointCnt(4, 0);

    // For every point
    //  if it intersects one of the bounding boxes sides
    //      add its coordinates to the corresponding crossPoint in the
    //      crossPoints vector
    auto ptItr = outline.begin();
    for ( ; ptItr != outline.end() ; ptItr++ ) {
        Sides side;
        if ( ptItr->x == bbox.Left() ) side = left;
        else if ( ptItr->y == bbox.Top() ) side = top;
        else if ( ptItr->x == bbox.Right() ) side = right;
        else if ( ptItr->y == bbox.Bottom() ) side = bottom;
        else continue;

        crossPoints[side].x += ptItr->x;
        crossPoints[side].y += ptItr->y;
        crossPointCnt[side]++;
    }

    // For the sides with multiple intersections average out
    //  the coordinates
    for( int i = 0; i < 4; i++) {
        crossPoints[i].x /= crossPointCnt[i];
        crossPoints[i].y /= crossPointCnt[i];
    }

    m_leftCP = crossPoints[left];
    m_topCP = crossPoints[top];
    m_rightCP = crossPoints[right];
    m_bottomCP = crossPoints[bottom];
}

cv::Point2i CrossPoints::Left() const { return m_leftCP; }
cv::Point2i CrossPoints::Top() const { return m_topCP; }
cv::Point2i CrossPoints::Right() const { return m_rightCP; }
cv::Point2i CrossPoints::Bottom() const { return m_bottomCP; }
