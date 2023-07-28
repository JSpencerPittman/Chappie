#include "bowl.h"

#include <utility>

using namespace chap;

Bowl::Bowl(cv::Mat image)
{
    this->image = std::move(image);
}

cv::Mat Bowl::getImage() const
{
    return this->image;
}

BoundingBox Bowl::boundingBox( int dil_iter, int erode_iter ) const
{
    cv::Mat imageHighlighted = this->highlight();
    chap::Outline ol(imageHighlighted, dil_iter, erode_iter);
    if ( ol.foundOutline() )
        return BoundingBox(ol);
    else
        return BoundingBox();
}

Bowl Bowl::magnify( bool inplace )
{
    BoundingBox bbox = this->boundingBox(2, 2);
    cv::Mat imageZoomed = BoundingBox::zoomInOnBoundingBox(this->image, bbox);

    if ( inplace )
        this->image = imageZoomed;

    return Bowl(imageZoomed);
}

Bowl Bowl::level( bool inplace )
{
    int angleOfRotation = this->findAngleOfRotation();
    ImageManip levelImage(this->image);
    levelImage.rotateImage(angleOfRotation, true);

    if ( inplace ) this->image = levelImage.getImage();

    return Bowl(levelImage.getImage());
}

cv::Mat Bowl::highlight() const
{
    cv::Mat imageHighlighted;
    cv::cvtColor(this->image, imageHighlighted, cv::COLOR_BGR2Lab);
    cv::extractChannel(imageHighlighted, imageHighlighted, 1);
    cv::threshold(imageHighlighted, imageHighlighted, 127, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    return imageHighlighted;
}

int Bowl::findAngleOfRotation() const
{
    chap::Outline ol(this->highlight());
    BoundingBox bbox(ol);
    CrossPoints cp(ol, bbox);

    int offset = cp.getBottom().x - bbox.getCenter().x;

    if ( !offset ) return 0;

    int direction = offset / std::abs(offset);
    int jump = INIT_JUMP;
    bool complete = false;

    int finalAngle = 0;

    ImageManip rotImage(this->image);

    std::vector<int> prev3Angles(3, 180);

    while ( std::abs(offset) > 0 ) {
        while ( offset / direction > 0 ) {
            int angle = direction * jump;
            rotImage.rotateImage(angle, true);

            cv::Mat imageHighlight = Bowl(rotImage.getImage()).highlight();
            ol = chap::Outline(imageHighlight);
            bbox = BoundingBox(ol);
            cp = CrossPoints(ol, bbox);

            offset = cp.getBottom().x - bbox.getCenter().x;

            if ( std::abs(offset) < TOLERANCE ) {
                complete = true;
                break;
            }

            finalAngle += angle;

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