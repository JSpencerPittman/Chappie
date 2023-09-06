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

    /*
     * @className Bowl
     * @brief provides a host of functions for dealing with bowls
     */
    class Bowl {
    public:
        /*
        * @brief constructor
        *
        * @param m_image m_image with bowl in it
        */
        explicit Bowl(cv::Mat image);

        /*
         * @brief get m_image
         */
        cv::Mat Image() const;

        /*
         * @brief get bounding box surrounding the bowl
         *
         * @param dilIter @see outline.h:Outline
         * @param erodeIter @see outline.h:Outline
         */
        BoundingBox FindBoundingBox(int dilIter = 1, int erodeIter = 1) const;

        /*
         * @brief magnify the bowl in the m_image
         *
         * @param inplace do we save the magnified m_image to this class's local m_image
         */
        Bowl Magnify(bool inplace = false);

        /*
         * @brief rotate the m_image until the bowl is straight up and down
         *
         * @param inplace do we save the leveled m_image to this class's local m_image
         */
        Bowl Level(bool inplace = false);

        /*
         * @brief Highlight the bowl in the m_image
         *
         * Uses Otsu's thresholding to find reddish objects in the m_image
         */
        cv::Mat Highlight() const;

    private:
        /*
         * @brief What angle is the bowl tilted in the m_image
         *
         * finds out how many degrees the bowl would need to be rotated and in what
         *  direction to make it level with the ground.
         */
        int FindAngleOfRotation() const;

        cv::Mat m_image;
    };
}

#endif //MATE2023_BOWL_H
