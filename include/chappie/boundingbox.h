#ifndef MATE2023_BOUNDINGBOX_H
#define MATE2023_BOUNDINGBOX_H

#include <algorithm>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "outline.h"

namespace chap {
    class BoundingBox {
    public:
        explicit BoundingBox();

        explicit BoundingBox(chap::Outline outline);

        int getLeft() const;

        int getTop() const;

        int getRight() const;

        int getBottom() const;

        int getWidth() const;

        int getHeight() const;

        int getArea() const;

        double getHypotenuse() const;

        cv::Point getCenter() const;

        bool doesExist() const;

        static cv::Mat zoomInOnBoundingBox(cv::Mat image, const BoundingBox &bbox);

    private:
        int left, top, right, bottom;
        int width, height, area;
        double hypotenuse;
        cv::Point2i center;
        bool exists;
    };
}

#endif //MATE2023_BOUNDINGBOX_H
