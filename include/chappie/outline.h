#ifndef MATE2023_OUTLINE_H
#define MATE2023_OUTLINE_H

#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

#define NUM_CANDIDATE_CONTOURS 3
#define KERNEL_SIZE 5

// A list of points outlining an object
typedef std::vector<cv::Point> Contour;

namespace chap {

    /*
     * @class Outline
     * @brief An outline of an object structured as a list of distinct points
     *
     * This class finds an outline of the most prominent figure in an outline.
     * This class acts as a wrapper around a vector of points outlining an object
     *  providing access through begin(), end() and [] operator.
     */
    class Outline {
    public:
        /*
         * @brief Find the outline in the binary m_image
         *
         * @param imgBinary binary m_image that has some highlighted central figure
         * @param dilIter How many times are the lines dilated(thickened)
         * @param erodeIter How manny times are the lines eroded(thinned)
         */
        explicit Outline(const cv::Mat &imgBinary, int dilIter = 1, int erodeIter = 1);

        cv::Point operator[] (int index) const;
        Contour::size_type size() const;
        Contour::const_iterator begin() const;
        Contour::const_iterator end() const;

        /*
         * @brief Was an outline found?
         */
        bool FoundOutline() const;

    private:
        /*
         * @brief emphasizes any objects in an m_image for easier detection
         *
         * First we apply a gaussian blur to reduce noise in the m_image.
         * Then we apply a threshold on the pixels making converting it from grayscale
         *  to black and white.
         * Finally, we apply dilation and erosion to get rid of discontinuities in the m_image.
         */
        void PreprocessImage();

        /*
         * @brief Identify the outline in the m_image
         *
         * The selection process is as follows:
         * 1. Find all possible contours in the m_image
         * 2. Select the top NUM_CANDIDATE_COUNTOURS to move on as candidates
         * 3. Of these candidates select the one with the highest m_area/len ratio.
         * 4. Set the selected candidate as the outline of the desired object.
         */
        void FindOutline();

        /*
         * @brief calculate the m_area to length ratio.
         *
         * Take the m_area of the contour divided by the length
         *  which in this case is the number of actual points.
         *
         *  @param cnt contour were calculating the m_area:length ratio
         */
        static double Area2LengthRatio(const Contour &cnt);

        cv::Mat m_image;             // Image of an object
        int m_dilIter, m_erodeIter;  // Iterations for the erosion and dilation processes in PreprocessImage
        Contour m_outline;           // The identified outline of the object
    };
}

#endif //MATE2023_OUTLINE_H
