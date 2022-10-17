#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "lsd.hpp"

int main(int argc, char** argv){
    //std::string path("../test_data/out2.png");
    //cv::Mat img_c = cv::imread(path, cv::IMREAD_COLOR);
    //cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    cv::Mat img_c;
    cv::Mat img_gray;
    cv::Mat img_d;
    cv::VideoCapture cap;
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    for (;;)
    {
        cap.read(img_c);
        cv::resize(img_c, img_c, cv::Size(), 0.5, 0.5);
        cv::cvtColor(img_c, img_gray, cv::COLOR_RGB2GRAY);
        img_gray.convertTo(img_d, CV_64F);
    
        //img /= 255.;
        //to raw array
        int h = img_gray.rows;
        int w = img_gray.cols;
        lines_t* out = lsd_with_line_merge(img_d.ptr<double>(0), w, h);
        std::cout << "Lines-v " << out->len_v << std::endl;
        std::cout << "Lines-h " << out->len_h << std::endl;

        for (int i=0; i< out->len_v; ++i){
           cv::line(img_c, 
                    cv::Point((int)out->lines_v[i][0], (int)out->lines_v[i][1]), 
                    cv::Point((int)out->lines_v[i][2], (int)out->lines_v[i][3]), 
                    cv::Scalar(0,255,0), 2, cv::LINE_AA, 0);
        }
        for (int i=0; i< out->len_h; ++i){
           cv::line(img_c, 
                    cv::Point((int)out->lines_h[i][0], (int)out->lines_h[i][1]), 
                    cv::Point((int)out->lines_h[i][2], (int)out->lines_h[i][3]), 
                    cv::Scalar(255,0,0), 2, cv::LINE_AA, 0);
        }

        cv::namedWindow( "image", 1 );
        cv::imshow( "image", img_c );
        if( cv::waitKey(5) >=0){
            break;
        }
    }
};