#ifndef MATE2023_DISPLAY_H
#define MATE2023_DISPLAY_H

#include <opencv2/opencv.hpp>

#include "boundingbox.h"
#include "outline.h"
#include "square.h"

#define PointRadius 3

namespace chap {
    class Display {
    public:
        Display(const cv::Mat &image);

        cv::Mat getImage() const;

        void drawOutline(const chap::Outline &outline);

        void drawBoundingBox(const BoundingBox &bbox,
                             int linewidth = 1,
                             char color = 'r');

        void drawSquare(const Square &sq,
                        char color = 'r',
                        int linewidth = 1,
                        float alpha = 0.5);

        void drawSquareBorder(const Square &sq,
                              int linewidth = 1,
                              char color = 'r');

        void drawSquareFill(const Square &sq,
                            float alpha = 0.5,
                            char color = 'r');

        void drawSquareCross(const Square &sq,
                             int linewidth = 1,
                             char color = 'r');

    private:
        void drawPoint(const cv::Point &pt, int radius = 3, char color = 'r');

        void drawLine(const cv::Point &pt1,
                      const cv::Point &pt2,
                      int linewidth = 3,
                      char color = 'r');

        static cv::Scalar decryptColor(char c);

        cv::Mat image;
    };
}

#endif //MATE2023_DISPLAY_H
