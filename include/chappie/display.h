#ifndef MATE2023_DISPLAY_H
#define MATE2023_DISPLAY_H

#include <opencv2/opencv.hpp>

#include "boundingbox.h"
#include "outline.h"
#include "square.h"

namespace chap {

    /*
     * @ClassName Display
     * @brief a collection of functions to automate drawing on an m_image
     */
    class Display {
    public:
        /*
         * @brief constructor
         *
         * @param m_image to be drawn on
         */
        Display(const cv::Mat &image);

        /*
         * @brief get m_image
         */
        cv::Mat Image() const;

        /*
         * @brief draw an outline on the m_image
         *
         * @param outline to be drawn
         */
        void DrawOutline(const chap::Outline &outline);

        /*
         * @brief draw a bounding box on the m_image
         *
         * @param bbox bounding box to be drawn
         * @param linewidth width of line
         * @param color of lines
         */
        void DrawBoundingBox(const BoundingBox &bbox,
                             int linewidth = 1,
                             char color = 'r');
        /*
         * @brief draw a square on the m_image
         *
         * @param sq square to be drawn
         * @param linewidth width of line
         * @param color of lines and filling
         * @param alpha transparency of the squares filling
         */
        void DrawSquare(const Square &sq,
                        char color = 'r',
                        int linewidth = 1,
                        float alpha = 0.5);

        /*
         * @brief draw a square's border on the m_image
         *
         * @param sq square to be drawn
         * @param linewidth width of line
         * @param color of lines
         */
        void DrawSquareBorder(const Square &sq,
                              int linewidth = 1,
                              char color = 'r');

        /*
         * @brief draw a square's filling on the m_image
         *
         * @param sq square to be drawn
         * @param color of lines
         * @param alpha transparency of the squares filling
         */
        void DrawSquareFill(const Square &sq,
                            float alpha = 0.5,
                            char color = 'r');

        /*
         * @brief draw a square's cross-section on the m_image
         *
         * @param sq square to be drawn
         * @param linewidth width of line
         * @param color of lines
         */
        void DrawSquareCross(const Square &sq,
                             int linewidth = 1,
                             char color = 'r');

    private:
        /*
         * @brief draw a point on the m_image
         *
         * @param pt point to be drawn
         * @param radius of the point
         * @param color of the point
         */
        void DrawPoint(const cv::Point &pt, int radius = 3, char color = 'r');

        /*
         * @brief draw a line on the m_image
         *
         * @param pt1 first point on the lines segment
         * @param pt2 second point on the line segment
         * @param linewidth width of line
         * @param color of line
         */
        void DrawLine(const cv::Point &pt1,
                      const cv::Point &pt2,
                      int linewidth = 3,
                      char color = 'r');

        /*
         * @brief map the character to its corresponding 3-value color
         *
         * makes colors much easier to work with and simpler
         */
        static cv::Scalar TranscribeColor(char c);

        cv::Mat m_image;
    };
}

#endif //MATE2023_DISPLAY_H
