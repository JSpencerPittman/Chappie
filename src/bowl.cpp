#include "bowl.h"

#include <utility>

using namespace chap;

Bowl::Bowl(cv::Mat image)
{
    m_image = std::move(image);
}

cv::Mat Bowl::Image() const
{
    return m_image;
}

BoundingBox Bowl::FindBoundingBox(int dilIter, int erodeIter) const
{
    cv::Mat imageHighlighted = Highlight();
    // find the outline
    chap::Outline ol(imageHighlighted, dilIter, erodeIter);
    if (ol.FoundOutline() )
        return BoundingBox(ol);
    else
        return BoundingBox();
}

Bowl Bowl::Magnify(bool inplace)
{
    // Find the bounding box surrounding the bowl
    BoundingBox bbox = FindBoundingBox(2, 2);
    // Get the zoomed in m_image
    cv::Mat imageZoomed = BoundingBox::zoomInOnBoundingBox(m_image, bbox);

    if ( inplace )
        m_image = imageZoomed;

    return Bowl(imageZoomed);
}

Bowl Bowl::Level(bool inplace)
{
    int angleOfRotation = FindAngleOfRotation();
    ImageManip levelImage(m_image);
    levelImage.RotateImage(angleOfRotation, true);

    if ( inplace ) m_image = levelImage.Image();

    return Bowl(levelImage.Image());
}

cv::Mat Bowl::Highlight() const
{
    cv::Mat imageHighlighted;
    // Convert to LAB colorspace
    //  composed of
    //  1. L(Lightness)
    //  2. A(Green -> Red)
    //  3. B(Blue -> Yellow)
    cv::cvtColor(m_image, imageHighlighted, cv::COLOR_BGR2Lab);
    // Get the Green -> Red channel
    cv::extractChannel(imageHighlighted, imageHighlighted, 1);
    // Use Otsu's thresholding which chooses an optimal threshold value for a bimodal distribution
    //  based on the m_image's color values
    cv::threshold(imageHighlighted, imageHighlighted, 127, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    return imageHighlighted;
}

int Bowl::FindAngleOfRotation() const
{
    chap::Outline ol(Highlight());
    BoundingBox bbox(ol);
    CrossPoints cp(ol, bbox);

    // The offset measures how off the bowl is from being level
    //  the more tilted the bowl the further its outlines intersection
    //  with the bottom of the bounding box from the m_center of the
    //  bounding box's bottom
    int offset = cp.Bottom().x - bbox.Center().x;

    if ( !offset ) return 0;

    // Do we tilt counter-clockwise or clockwise
    int direction = offset / std::abs(offset);
    // How much we change the angle between iterations
    int jump = INIT_JUMP;
    // Is the bowl's offset less than the TOLERANCE value
    bool complete = false;

    int finalAngle = 0;

    ImageManip rotImage(m_image);

    // Need to make sure it's not repeating the same pattern
    // a    nd consequently not making any progress
    std::vector<int> prev3Angles(3, 180);

    // We use an approach of simulated annealing where
    //  we narrow in on a target angle starting off with
    //  dramatic shifts in the angle and working our way to
    //  smaller shifts until we've reached the desired goal

    // If the offset is 0 the bowl should be level
    while ( std::abs(offset) > 0 ) {
        // Keep rotating bowl until it has
        //  been rotated pass the m_center vertical axis
        while ( offset / direction > 0 ) {
            int angle = direction * jump;
            rotImage.RotateImage(angle, true);

            cv::Mat imageHighlight = Bowl(rotImage.Image()).Highlight();
            ol = chap::Outline(imageHighlight);
            bbox = BoundingBox(ol);
            cp = CrossPoints(ol, bbox);

            offset = cp.Bottom().x - bbox.Center().x;

            // Have we gotten the bowl close enough to being level?
            if ( std::abs(offset) < TOLERANCE ) {
                complete = true;
                break;
            }

            finalAngle += angle;

            // Have we entered an infinite pattern?
            if ( prev3Angles[1] == angle && prev3Angles[0] == prev3Angles[2] )
                complete = true;
            else {
                prev3Angles[0] = prev3Angles[1];
                prev3Angles[1] = prev3Angles[2];
                prev3Angles[2] = angle;
            }
        }

        if ( complete ) break;

        direction = offset / std::abs(offset);
        jump = std::max(jump / 2, 1);
    }

    return finalAngle;
}