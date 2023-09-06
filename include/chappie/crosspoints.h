#ifndef MATE2023_CROSSPOINTS_H
#define MATE2023_CROSSPOINTS_H

#include <opencv2/opencv.hpp>

#include "outline.h"
#include "boundingbox.h"

namespace chap {

    /*
     * @ClassName CrossPoints
     *
     * Keeps track of the points of intersection between the outlined object and
     *  its encapsulating bounding box.
     * The way a bowl's outline intersects a bounding box is typically in the shape of a cross
     *  hence the name CrossPoints.
     */
    class CrossPoints {
    public:
        /*
         * @brief constructor
         *
         * @param outline outline of the object in question
         * @param bbox bounding box of the object in question
         */
        CrossPoints(const chap::Outline &outline, const BoundingBox &bbox);

        /*
         * @brief left cross point
         *
         * where the object in question intersects the left side
         */
        cv::Point2i Left() const;

        /*
         * @brief top cross point
         *
         * where the object in question intersects the top side
         */
        cv::Point2i Top() const;

        /*
         * @brief right cross point
         *
         * where the object in question intersects the right side
         */
        cv::Point2i Right() const;

        /*
         * @brief bottom cross point
         *
         * where the object in question intersects the bottom side
         */
        cv::Point2i Bottom() const;

    private:
        cv::Point2i m_leftCP, m_topCP, m_rightCP, m_bottomCP;
    };
}


#endif //MATE2023_CROSSPOINTS_H
