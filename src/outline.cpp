#include "outline.h"

using namespace chap;

Outline::Outline(const cv::Mat& imgBinary, int dilIter, int erodeIter )
{
    m_image = imgBinary;
    m_dilIter = dilIter;
    m_erodeIter = erodeIter;

    PreprocessImage();
    FindOutline();
}

cv::Point Outline::operator[] (int index) const
{
    return m_outline[index];
}

Contour::size_type Outline::size() const
{
    return m_outline.size();
}

Contour::const_iterator Outline::begin() const
{
    return m_outline.cbegin();
}

Contour::const_iterator Outline::end() const
{
    return m_outline.cend();
}

bool Outline::FoundOutline() const
{
    return !m_outline.empty();
}

void Outline::PreprocessImage()
{
    // reduce noise in the m_image and refine edges
    blur(m_image, m_image, {KERNEL_SIZE, KERNEL_SIZE});

    // polarize the m_image
    threshold(m_image, m_image, 0, 255, cv::THRESH_BINARY);

    // remove discontinuities in any lines
    cv::Mat kernel = cv::Mat::ones(KERNEL_SIZE, KERNEL_SIZE, CV_8UC1);
    cv::Point2i anchorCenter(-1, -1); // m_center of m_image by default
    dilate(m_image, m_image, kernel, anchorCenter, m_dilIter);
    erode(m_image, m_image, kernel, anchorCenter, m_erodeIter);
}

void Outline::FindOutline()
{
    // identify contours
    std::vector<Contour> contours;
    findContours(m_image, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    if ( contours.empty() ) return;

    // select the three(NUM_CANDIDATE_CONTOURS) contours with the most points
    //  if there aren't three contours then select all of them as candidates
    sort(contours.begin(), contours.end(), [](const Contour& a, const Contour& b){
        return a.size() > b.size();
    });
    int totalCandidates = std::min(NUM_CANDIDATE_CONTOURS, (int)contours.size());
    std::vector<Contour> candidateContours(contours.begin(), contours.begin() + totalCandidates);

    // Choose the best contour as the one with the highest
    //  m_area:length ratio.
    m_outline= candidateContours[0];
    double bestRatio = Area2LengthRatio(m_outline);

    auto cntItr = candidateContours.begin() + 1;
    for ( ; cntItr != candidateContours.end(); cntItr++ ) {
        double ratio = Area2LengthRatio(*cntItr);
        if ( ratio > bestRatio ) {
            m_outline  = *cntItr;
            bestRatio = ratio;
        }
    }
}

double Outline::Area2LengthRatio(const Contour& cnt )
{
    double area = contourArea(cnt);
    auto length = (double)cnt.size();
    return area / length;
}
