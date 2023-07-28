#include "outline.h"

using namespace chap;

Outline::Outline(const cv::Mat& imgBinary, int dil_iter, int erode_iter )
{
    this->image = imgBinary;
    this->dil_iter = dil_iter;
    this->erode_iter = erode_iter;

    this->prepareImage();
    this->findBestOutline();
}

cv::Point Outline::operator[] (int index) const
{
    return this->bestOutline[index];
}

Contour::size_type Outline::size() const
{
    return this->bestOutline.size();
}

Contour::const_iterator Outline::begin() const
{
    return this->bestOutline.cbegin();
}

Contour::const_iterator Outline::end() const
{
    return this->bestOutline.cend();
}

bool Outline::foundOutline() const
{
    return !this->bestOutline.empty();
}

void Outline::prepareImage()
{
    cv::Size2i blurKernelSize(KERNEL_SIZE,KERNEL_SIZE);
    cv::Mat kernel = cv::Mat::ones(KERNEL_SIZE, KERNEL_SIZE, CV_8UC1);
    cv::Point2i anchorCenter(-1, -1);

    // reduce noise in the image and refine edges
    blur(this->image, this->image, blurKernelSize);
    // polarize the image
    threshold(this->image, this->image, 0, 255, cv::THRESH_BINARY);

    // remove small discontinuities between the lines
    dilate(this->image, this->image, kernel, anchorCenter, dil_iter);
    erode(this->image, this->image, kernel, anchorCenter, erode_iter);
}

void Outline::findBestOutline()
{
    // identify contours
    std::vector<Contour> contours;
    findContours(this->image, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    if ( contours.empty() ) return;

    // select the three contours with the most points
    sort(contours.begin(), contours.end(), [](const Contour& a, const Contour& b){
        return a.size() > b.size();
    });
    int totalCandidates = std::min(NUM_CANDIDATE_CONTOURS, (int)contours.size());
    std::vector<Contour> candidateContours(contours.begin(), contours.begin() + totalCandidates);
    this->bestOutline = candidateContours[0];
    double bestRatio = area2LengthRatio(this->bestOutline );

    auto cntItr = candidateContours.begin() + 1;
    for ( ; cntItr != candidateContours.end(); cntItr++ ) {
        double ratio = area2LengthRatio(*cntItr);
        if ( ratio > bestRatio ) {
            this->bestOutline  = *cntItr;
            bestRatio = ratio;
        }
    }
}

double Outline::area2LengthRatio(const Contour& cnt )
{
    double area = contourArea(cnt);
    auto length = (double)cnt.size();
    return area / length;
}
