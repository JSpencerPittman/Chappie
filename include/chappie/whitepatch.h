#ifndef MATE2023_WHITEPATCH_H
#define MATE2023_WHITEPATCH_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "square.h"
#include "boundingbox.h"
#include "bowl.h"

namespace chap {
    class WhitePatch {
    public:
        WhitePatch(cv::Mat image);

        cv::Mat getImage() const;

        std::vector<Square> locate() const;

        cv::Mat highlight() const;

    private:
        static bool squareInBoundingBox(const BoundingBox &bbox, const Square &sq);

        static bool pointInBoundingBox(const BoundingBox &sq, const cv::Point &pt);

        cv::Mat image;
    };
}

#endif //MATE2023_WHITEPATCH_H
