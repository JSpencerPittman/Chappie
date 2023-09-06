#ifndef MATE2023_WHITEPATCH_H
#define MATE2023_WHITEPATCH_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "square.h"
#include "boundingbox.h"
#include "bowl.h"

namespace chap {

    /*
     * @ClassName WhitePatch
     * @brief A class build for detecting White Patches(Squares)
     */
    class WhitePatch {
    public:
        /*
         * @brief constructor
         *
         * @param m_image to find the patches in
         */
        WhitePatch(cv::Mat image);

        /*
         * @brief get m_image
         */
        cv::Mat Image() const;

        /*
         * @brief Locate patches in the m_image
         */
        std::vector<Square> Locate() const;

        /*
         * @brief Highlight white patches in the m_image
         *
         * in this case we're really just highlighting the color white
         */
        cv::Mat Highlight() const;

    private:
        /*
         * @brief Check if a square is in a given bounding box
         *
         * @param bbox encapsulating bounding box
         * @param sq square that were checking is in the bounding box
         */
        static bool squareInBoundingBox(const BoundingBox &bbox, const Square &sq);

        /*
         * @brief Is a point in the bounding box
         *
         * @param bbox encapsulating bounding box
         * @param pt point that were checking is in the bounding box
         */
        static bool pointInBoundingBox(const BoundingBox &bbox, const cv::Point &pt);

        cv::Mat m_image;
    };
}

#endif //MATE2023_WHITEPATCH_H
