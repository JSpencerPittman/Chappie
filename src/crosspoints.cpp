#include "crosspoints.h"

using namespace chap;

CrossPoints::CrossPoints(const chap::Outline &outline, const BoundingBox &bbox)
{
    enum Sides { left, top, right, bottom };

    std::vector<cv::Point2i> crossPoints(4, cv::Point2i(0,0));
    std::vector<int> crossPointCnt(4, 0);

    auto ptItr = outline.begin();
    for ( ; ptItr != outline.end() ; ptItr++ ) {
        Sides side;
        if ( ptItr->x == bbox.getLeft() ) side = left;
        else if ( ptItr->y == bbox.getTop() ) side = top;
        else if ( ptItr->x == bbox.getRight() ) side = right;
        else if ( ptItr->y == bbox.getBottom() ) side = bottom;
        else continue;

        crossPoints[side].x += ptItr->x;
        crossPoints[side].y += ptItr->y;
        crossPointCnt[side]++;
    }

    for( int i = 0; i < 4; i++) {
        crossPoints[i].x /= crossPointCnt[i];
        crossPoints[i].y /= crossPointCnt[i];
    }

    this->leftCP = crossPoints[left];
    this->topCP = crossPoints[top];
    this->rightCP = crossPoints[right];
    this->bottomCP = crossPoints[bottom];
}

cv::Point2i CrossPoints::getLeft() const { return this->leftCP; }
cv::Point2i CrossPoints::getTop() const { return this->topCP; }
cv::Point2i CrossPoints::getRight() const { return this->rightCP; }
cv::Point2i CrossPoints::getBottom() const { return this->bottomCP; }
