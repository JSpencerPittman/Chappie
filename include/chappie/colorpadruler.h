#ifndef MATE2023_COLORPADRULER_H
#define MATE2023_COLORPADRULER_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "square.h"
#include "bowl.h"
#include "boundingbox.h"

typedef std::vector<cv::Point> Contour;

namespace chap {
    class ColorPadRuler {
    public:
        ColorPadRuler(cv::Mat image, bool exclusion = true);

        cv::Mat getImage() const;

        const Square &getUpperPad() const;

        const Square &getLowerPad() const;

        bool padsFound() const;

        cv::Mat highlight() const;

    private:
        void identifyPads(bool exclusion);

        void identifyPadsWithBowl();

        cv::Mat horizontalExclusion(int left, int right);

        static Square horizontalSquareShift(const Square &sq, int shift);

        cv::Mat image;
        Square *lowerPad;
        Square *upperPad;
    };
}

#endif //MATE2023_COLORPADRULER_H
