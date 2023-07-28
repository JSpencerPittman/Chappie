#ifndef MATE2023_CROSSPOINTS_H
#define MATE2023_CROSSPOINTS_H

#include <opencv2/opencv.hpp>

#include "outline.h"
#include "boundingbox.h"

namespace chap {
    class CrossPoints {
    public:
        CrossPoints(const chap::Outline &outline, const BoundingBox &bbox);

        cv::Point2i getLeft() const;

        cv::Point2i getTop() const;

        cv::Point2i getRight() const;

        cv::Point2i getBottom() const;

    private:
        cv::Point2i leftCP, topCP, rightCP, bottomCP;
    };
}


#endif //MATE2023_CROSSPOINTS_H
