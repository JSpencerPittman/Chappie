#ifndef MATE2023_BOWL_H
#define MATE2023_BOWL_H

#include <opencv2/opencv.hpp>

#include "boundingbox.h"
#include "outline.h"
#include "crosspoints.h"
#include "imagemanip.h"

#define TOLERANCE 1
#define INIT_JUMP 15

namespace chap {
    class Bowl {
    public:
        // constructor
        explicit Bowl(cv::Mat image);

        // getter
        cv::Mat getImage() const;

        BoundingBox boundingBox(int dil_iter = 1, int erode_iter = 1) const;

        Bowl magnify(bool inplace = false);

        Bowl level(bool inplace = false);

        cv::Mat highlight() const;

    private:
        int findAngleOfRotation() const;

        cv::Mat image;
    };
}

#endif //MATE2023_BOWL_H
