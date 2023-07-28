#ifndef MATE2023_SQUARE_H
#define MATE2023_SQUARE_H

#include <vector>
#include <algorithm>
#include <cmath>

#include <opencv2/opencv.hpp>

#define AREA_LENGTH_RATIO_THRESHOLD 2
#define SIDES_ON_SQUARE 4

typedef std::vector<cv::Point> Contour;

namespace chap {
    class Square {
    public:
        Square(Contour contour);

        Square(const Square &sq);

        const cv::Point2i &getTopLeft() const;

        const cv::Point2i &getTopRight() const;

        const cv::Point2i &getBotRight() const;

        const cv::Point2i &getBotLeft() const;

        const cv::Point2i &getLeftCP() const;

        const cv::Point2i &getTopCP() const;

        const cv::Point2i &getRightCP() const;

        const cv::Point2i &getBottomCP() const;

        double getWidth() const;

        double getHeight() const;

        double getArea() const;

        cv::Point2i getCenter() const;

        Contour getCorners() const;

        static std::vector<Square> findSquares(const cv::Mat &imageBinary);

    private:
        static cv::Point2i calcMidpoint(const cv::Point2i &a, const cv::Point2i &b);

        static double calcDistance(const cv::Point2i &a, const cv::Point2i &b);

        cv::Point2i topLeft, topRight, botRight, botLeft;

    private:
        cv::Point2i leftCP, topCP, rightCP, bottomCP;
        double width, height, area;
        cv::Point2i center;

        Contour corners;
    };
}

#endif //MATE2023_SQUARE_H
