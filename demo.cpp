#include <vector>
#include <algorithm>
#include <string>

#include <opencv2/opencv.hpp>

#include "bowl.h"
#include "display.h"
#include "whitepatch.h"
#include "colorpadruler.h"
#include "square.h"

using namespace std;

void printUsage()
{
    cout << "usage for images     : demo.cpp IMAGE <PROGRAM> <PATH>" << endl;
    cout << "usage for live videos: demo.cpp VIDEO PROGRAM" << endl;
    cout << "PROGRAM = DETECTION | <COLOR> | LEVEL | NONE" << endl;
    cout << "COLOR   = RED | WHITE | GREEN" << endl;
}

cv::Mat* detection(const cv::Mat& image) {
    chap::Display display(image);

    // Bowl
    chap::Bowl bowl(image);
    chap::BoundingBox bowlBBox = bowl.FindBoundingBox();
    if ( bowlBBox.doesExist() )
        display.DrawBoundingBox(bowlBBox, 2, 'b');

    // WhitePatch
    chap::WhitePatch wp(image);
    vector<chap::Square> squares = wp.Locate();
    for (const chap::Square& square : squares)
        display.DrawSquare(square, 'o', 2, 0.3);

    // ColorPadRuler
    chap::ColorPadRuler cpr(image);
    if (cpr.PadsFound() ) {
        display.DrawSquare(cpr.UpperPad(), 'r', 2, 0.3);
        display.DrawSquare(cpr.LowerPad(), 'r', 2, 0.3);
    }

    return new cv::Mat(display.Image());
}

cv::Mat* red(const cv::Mat& image) {
    chap::Bowl bowl(image);
    return new cv::Mat(bowl.Highlight());
}

cv::Mat* white(const cv::Mat& image) {
    chap::WhitePatch wp(image);
    return new cv::Mat(wp.Highlight());
}

cv::Mat* green(const cv::Mat& image) {
    chap::ColorPadRuler cpr(image);
    return new cv::Mat(cpr.Highlight());
}

cv::Mat* level(const cv::Mat& image) {
    chap::Bowl bowl(image);
    chap::BoundingBox bbox = bowl.FindBoundingBox();
    if (bbox.doesExist()) {
        return new cv::Mat(bowl.Level().Image());
    }
    else
        return new cv::Mat(image);
}

cv::Mat* processImage(const cv::Mat& image, const char* program) {
    if ( strcmp(program, "DETECTION") == 0 ) {
        return detection(image);
    }
    else if ( strcmp(program, "RED") == 0 ) {
        return red(image);
    }
    else if ( strcmp(program, "WHITE") == 0 ) {
        return white(image);
    }
    else if ( strcmp(program, "GREEN") == 0 ) {
        return green(image);
    }
    else if ( strcmp(program, "LEVEL") == 0 ) {
        return level(image);
    }
    else if ( strcmp(program, "NONE") == 0 ) {
        return new cv::Mat(image);
    }
    else {
        printUsage();
        exit(-1);
    }
}

void runImage(const char* program, const char* path)
{
    cv::Mat initialImage;
    initialImage = imread(path, cv::IMREAD_COLOR);

    if( !initialImage.data ){
        printf("No m_image data \n");
        return;
    }

    cv::Mat* finalImage = processImage(initialImage, program);

    namedWindow("Image", cv::WINDOW_AUTOSIZE);
    imshow("Image", *finalImage);

    cv::waitKey(0);
    cv::destroyAllWindows();

    delete finalImage;
}

void runVideo(const char* program)
{
    cv::VideoCapture capture(0);

    if ( !capture.isOpened() ) {
        cerr << "error: Could not find camera!" << endl;
        return;
    }

    while ( true ) {
        cv::Mat initialFrame;
        capture >> initialFrame;

        if ( initialFrame.empty() ) break;

        cv::Mat* finalFrame = processImage(initialFrame, program);

        imshow("PROCESSED",  *finalFrame);
        imshow("RAW", initialFrame);

        int key = cv::waitKey(1);

        if ( key == 27 ) break;

        delete finalFrame;
    }

    capture.release();
    cv::destroyAllWindows();
}

int main(int argc, char* argv[]) {
    if ( argc < 3 ) {
        printUsage();
        return -1;
    }

    if ( strcmp(argv[1], "IMAGE") == 0 ) {
        if ( argc != 4 ) {
            printUsage();
            return -1;
        }

        runImage(argv[2], argv[3]);
    }

    else if ( strcmp(argv[1], "VIDEO") == 0 ) {
        if ( argc != 3 ) {
            printUsage();
            return -1;
        }

        runVideo(argv[2]);
    }

    else {
        printUsage();
        return -1;
    }

    return 0;
}