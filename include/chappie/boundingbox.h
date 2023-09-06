#ifndef MATE2023_BOUNDINGBOX_H
#define MATE2023_BOUNDINGBOX_H

#include <algorithm>
#include <cmath>

#include <opencv2/opencv.hpp>

#include "outline.h"

namespace chap {

    /*
     * @class FindBoundingBox
     * @brief A box that encapsulates an object in an m_image
    */
    class BoundingBox {
    public:
        /*
         * @brief constructor
         *
         * this will set all the numerical attributes to -1.
         * Sets the exists property to false in order to
         *  indicate this box hasn't been created yet.
         */
        explicit BoundingBox();

        /*
         * @brief constructor
         *
         * this will create a bounding box with the position and size to encapsulate
         * the given outline.
         *
         * @param outline of the object in question
         */
        explicit BoundingBox(chap::Outline outline);

        /*
         * @brief x-coordinate of left side
         */
        int Left() const;

        /*
         * @brief y-coordinate of top side
         */
        int Top() const;

        /*
         * @brief x-coordinate of right side
         */
        int Right() const;

        /*
         * @brief y-coordinate of bottom side
         */
        int Bottom() const;

        /*
         * @brief m_width of box(pixels)
         */
        int Width() const;

        /*
         * @brief m_height of box(pixels)
         */
        int Height() const;

        /*
         * @brief m_area of box(pixels)
         */
        int Area() const;

        /*
         * @brief hypotenuse of box(pixels)
         */
        double Hypotenuse() const;

        /*
         * @brief m_center of box
         */
        cv::Point Center() const;

        /*
         * @brief does this box actually encapsulate an outline
         */
        bool doesExist() const;

        /*
         * @brief returns an m_image centered on the bounding box
         *
         * The returned m_image is centered on the given bounding box.
         * The new m_height and m_width are the hypotenuse of the bounding box which
         *  also helps to add a margin on both sides of the m_image.
         *
         *  @param m_image with the object in question
         *  @param bounding box surrounding the object in question
         */
        static cv::Mat zoomInOnBoundingBox(cv::Mat image, const BoundingBox &bbox);

    private:
        int m_left, m_top, m_right, m_bottom;
        int m_width, m_height, m_area;
        double m_hypotenuse;
        cv::Point2i m_center;
        bool m_exists;
    };
}

#endif //MATE2023_BOUNDINGBOX_H
