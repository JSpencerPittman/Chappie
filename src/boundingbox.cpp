#include "boundingbox.h"

using namespace chap;

BoundingBox::BoundingBox() {
    this->left = this->top = this->right = this->bottom = -1;
    this->width = this->height = this->area = this->hypotenuse = -1;
    this->center = cv::Point2i (-1, -1);
    this->exists = false;
};

BoundingBox::BoundingBox(chap::Outline outline)
{
    int minX, maxX, minY, maxY;
    minX = outline[0].x, minY = outline[0].y;
    maxX = outline[0].x, maxY = outline[0].y;

    auto ptItr = outline.begin();
    for( ; ptItr != outline.end() ; ptItr++ ) {
        minX = std::min(ptItr->x, minX);
        maxX = std::max(ptItr->x, maxX);

        minY = std::min(ptItr->y, minY);
        maxY = std::max(ptItr->y, maxY);
    }

    this->left = minX;
    this->top = minY;
    this->right = maxX;
    this->bottom = maxY;

    this->width = this->right - this->left;
    this->height = this->bottom - this->top;
    this->area = this->width * this->height;

    this->hypotenuse = 0;
    this->hypotenuse += std::pow(this->width, 2);
    this->hypotenuse += std::pow(this->height, 2);
    this->hypotenuse = std::sqrt(this->hypotenuse);

    int cX = this->left + (this->width / 2);
    int cY = this->top + (this->height / 2);
    this->center = cv::Point2i(cX, cY);

    if(this->left == 0 || this->top == 0) this->exists = false;
    else this->exists = true;
}

int BoundingBox::getLeft() const { return this->left; }
int BoundingBox::getTop() const { return this->top; }
int BoundingBox::getRight() const { return this->right; }
int BoundingBox::getBottom() const { return this->bottom; }
int BoundingBox::getWidth() const { return this->width; }
int BoundingBox::getHeight() const { return this->height; }
int BoundingBox::getArea() const { return this->area; }
double BoundingBox::getHypotenuse() const { return this->hypotenuse; }
cv::Point BoundingBox::getCenter() const { return this->center; }

bool BoundingBox::doesExist() const { return this->exists; }

cv::Mat BoundingBox::zoomInOnBoundingBox(cv::Mat image, const BoundingBox& bbox)
{
    cv::Size2d patchSize(bbox.getHypotenuse(), bbox.getHypotenuse());

    cv::Mat imageZoomed;
    cv::getRectSubPix(image, patchSize, bbox.getCenter(), imageZoomed);

    return imageZoomed;
}
