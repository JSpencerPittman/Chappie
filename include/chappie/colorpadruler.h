#ifndef MATE2023_COLORPADRULER_H
#define MATE2023_COLORPADRULER_H

#include <vector>

#include <opencv2/opencv.hpp>

#include "square.h"
#include "bowl.h"
#include "boundingbox.h"

typedef std::vector<cv::Point> Contour;

namespace chap {
    /*
     * @ClassName ColorPadRuler
     * @brief A tool for finding and using the measuring device
     *
     * The color pad ruler is two panels attached at a 90 degree angle
     * with each panel bearing an 8cm x 8cm green patch.
     *
     * This tool's purpose is to act as a relative measuring device for
     *  translating from pixels to real world units.
     */
    class ColorPadRuler {
    public:
        /*
         * @brief constructor
         *
         * @param m_image containing the measuring device
         * @param exclusion if there's a bowl do we remove it out of the m_image
         */
        ColorPadRuler(cv::Mat image, bool exclusion = true);

        /*
         * @brief get m_image
         */
        cv::Mat Image() const;

        /*
         * @brief get upper pad on the measuring device
         */
        const Square &UpperPad() const;

        /*
         * @brief get lower pad on the measuring device
         */
        const Square &LowerPad() const;

        /*
         * @brief were any pads found
         */
        bool PadsFound() const;

        /*
         * @brief highlight the green pads in the m_image
         *
         * note I use Otsu's thresholding which is great for objects with
         *  a single entity in the m_image but with the bowl this function
         *     doesn't work well at all, hence exclusion
         */
        cv::Mat Highlight() const;

    private:
        /*
         * @brief find pads in the m_image
         *
         * @param exclusion if there's a bowl do we remove it out of the m_image
         */
        void IdentifyPads(bool exclusion);

        /*
         * @brief find the green pads with a bowl in the m_image
         */
        void IdentifyPadsWithBowl();

        /*
         * @brief remove the cols of the m_image from [left, right]
         *
         * @param left leftmost column to remove
         * @param right rightmost column to remove
         */
        cv::Mat HorizontalExclusion(int left, int right);

        /*
         * @brief shift a square horizontally
         *
         * @param sq square to be shifted
         * @param shift how many pixels to shift said square
         */
        static Square HorizontalSquareShift(const Square &sq, int shift);

        cv::Mat m_image;
        Square *m_lowerPad;
        Square *m_upperPad;
    };
}

#endif //MATE2023_COLORPADRULER_H
