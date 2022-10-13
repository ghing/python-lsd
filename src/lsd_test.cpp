#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

#include "lsd.hpp"

int main(int argc, char** argv){
    std::string path("../test_data/out2.png");
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat img_d;

    cv::Mat img_c = cv::imread(path, cv::IMREAD_COLOR);

    img.convertTo(img_d, CV_64F);
    
    img /= 255.;
    //to raw array
    int h = img.rows;
    int w = img.cols;
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
    cv::waitKey();
    
};