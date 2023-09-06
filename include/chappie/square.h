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

    /*
     * @ClassName Square
     * @brief info about a square found in the m_image
     */
    class Square {
    public:
        /*
         * @brief constructor
         *
         * @param contour the contour outlining the square
         */
        Square(Contour contour);

        /*
         * @brief copy constructor
         *
         * @param sq square to be copied
         */
        Square(const Square &sq);

        /*
         * @brief top left corner
         */
        const cv::Point2i &TopLeft() const;

        /*
         * @brief top right corner
         */
        const cv::Point2i &TopRight() const;

        /*
         * @brief bottom right corner
         */
        const cv::Point2i &BotRight() const;

        /*
         * @brief bottom left corner
         */
        const cv::Point2i &BotLeft() const;

        /*
         * @brief middle of left side
         */
        const cv::Point2i &LeftMidpoint() const;

        /*
         * @brief middle of top side
         */
        const cv::Point2i &TopMidpoint() const;

        /*
         * @brief middle of right side
         */
        const cv::Point2i &RightMidpoint() const;

        /*
         * @brief middle of bottom side
         */
        const cv::Point2i &BottomMidpoint() const;

        /*
         * @brief width of the square
         */
        double Width() const;

        /*
         * @brief height of the square
         */
        double Height() const;

        /*
         * @brief area of the square
         */
        double Area() const;

        /*
         * @brief center of the square
         */
        cv::Point2i Center() const;

        /*
         * @brief corners of the square
         */
        Contour Corners() const;

        /*
         * @brief finds all squares in a binary m_image
         *
         * @param imageBinary binary m_image highlighting the squares
         */
        static std::vector<Square> FindSquares(const cv::Mat &imageBinary);

    private:
        /*
         * @brief calculate midpoint of two points
         *
         * @param a first point
         * @param b second point
         */
        static cv::Point2i CalcMidpoint(const cv::Point2i &a, const cv::Point2i &b);

        /*
         * @brief calculate distance between two points
         *
         * @param a first point
         * @param b second point
         */
        static double CalcDistance(const cv::Point2i &a, const cv::Point2i &b);

        cv::Point2i topLeft, topRight, botRight, botLeft;
        cv::Point2i m_leftMidpoint, m_topMidpoint, m_rightMidpoint, m_bottomMidpoint;
        double m_width, m_height, m_area;
        cv::Point2i m_center;
        Contour m_corners;
    };
}

#endif //MATE2023_SQUARE_H
