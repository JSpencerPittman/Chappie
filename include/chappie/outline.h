#ifndef MATE2023_OUTLINE_H
#define MATE2023_OUTLINE_H

#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

#define NUM_CANDIDATE_CONTOURS 3
#define KERNEL_SIZE 5

typedef std::vector<cv::Point> Contour;

namespace chap {
    class Outline {
    public:
        explicit Outline(const cv::Mat &imgBinary, int dil_iter = 1, int erode_iter = 1);

        cv::Point operator[] (int index) const;
        Contour::size_type size() const;
        Contour::const_iterator begin() const;
        Contour::const_iterator end() const;

        bool foundOutline() const;

    private:
        void prepareImage();

        void findBestOutline();

        static double area2LengthRatio(const Contour &cnt);

        cv::Mat image;
        int dil_iter, erode_iter;
        Contour bestOutline;
    };
}

#endif //MATE2023_OUTLINE_H
