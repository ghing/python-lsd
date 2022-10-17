/*----------------------------------------------------------------------------

  LSD - Line Segment Detector on digital images

  This code is part of the following publication and was subject
  to peer review:

    "LSD: a Line Segment Detector" by Rafael Grompone von Gioi,
    Jeremie Jakubowicz, Jean-Michel Morel, and Gregory Randall,
    Image Processing On Line, 2012. DOI:10.5201/ipol.2012.gjmr-lsd
    http://dx.doi.org/10.5201/ipol.2012.gjmr-lsd

  Copyright (c) 2007-2011 rafael grompone von gioi <grompone@gmail.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.

  ----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** @file lsd.h
    LSD module header
    @author rafael grompone von gioi <grompone@gmail.com>
 */
/*----------------------------------------------------------------------------*/
#ifndef LSD_HEADER
#define LSD_HEADER
#include <vector>


#ifndef NDEBUG
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <random>

class LineDrawer{
  private:
    cv::Mat img;

  public:
    void setImg(double* img, int x, int y){
      cv::Mat cvImg(x, y, CV_64F, img);      
      cvImg.convertTo(this->img, CV_8UC1);
      cv::cvtColor(this->img, this->img, cv::COLOR_GRAY2RGB);
    };
    void setImg(cv::Mat img){
      img.convertTo(this->img, CV_8UC1);
      cv::cvtColor(this->img, this->img, cv::COLOR_GRAY2RGB);

    };

    void draw(const double line[4], std::string str="Window", cv::Scalar color=(0,0,255)){
      cv::Mat img_;
      this->img.copyTo(img_);
      cv::line(img_, 
          cv::Point((int)line[0], (int)line[1]), 
          cv::Point((int)line[2], (int)line[3]), 
          color,
          2, cv::LINE_AA, 0);
      cv::namedWindow(str, 1 );
      cv::imshow(str, img_);
      cv::waitKey(10);
    };

    void draw(const double(*lines)[4], const int n_out, std::string str="window"){

      std::uniform_int_distribution<> rnd(0,255);
      std::random_device rd;
      std::mt19937 gen(rd());

      cv::Mat img_;
      this->img.copyTo(img_);
      for (int i=0; i<n_out; ++i){
          cv::line(img_, 
              cv::Point((int)lines[i][0], (int)lines[i][1]), 
              cv::Point((int)lines[i][2], (int)lines[i][3]), 
              cv::Scalar(rnd(gen), 
                         rnd(gen), 
                         rnd(gen)), 1, cv::LINE_AA, 0);
      }
      cv::namedWindow(str, 1 );
      cv::imshow(str, img_);
      cv::waitKey(1);
    };
};
#endif

/*----------------------------------------------------------------------------*/
/** LSD Full Interface

    @param n_out       Pointer to an int where LSD will store the number of
                       line segments detected.

    @param img         Pointer to input image data. It must be an array of
                       doubles of size X x Y, and the pixel at coordinates
                       (x,y) is obtained by img[x+y*X].

    @param X           X size of the image: the number of columns.

    @param Y           Y size of the image: the number of rows.

    @param scale       When different from 1.0, LSD will scale the input image
                       by 'scale' factor by Gaussian filtering, before detecting
                       line segments.
                       Example: if scale=0.8, the input image will be subsampled
                       to 80% of its size, before the line segment detector
                       is applied.
                       Suggested value: 0.8

    @param sigma_scale When scale!=1.0, the sigma of the Gaussian filter is:
                       sigma = sigma_scale / scale,   if scale <  1.0
                       sigma = sigma_scale,           if scale >= 1.0
                       Suggested value: 0.6

    @param quant       Bound to the quantization error on the gradient norm.
                       Example: if gray levels are quantized to integer steps,
                       the gradient (computed by finite differences) error
                       due to quantization will be bounded by 2.0, as the
                       worst case is when the error are 1 and -1, that
                       gives an error of 2.0.
                       Suggested value: 2.0

    @param ang_th      Gradient angle tolerance in the region growing
                       algorithm, in degrees.
                       Suggested value: 22.5

    @param log_eps     Detection threshold, accept if -log10(NFA) > log_eps.
                       The larger the value, the more strict the detector is,
                       and will result in less detections.
                       (Note that the 'minus sign' makes that this
                       behavior is opposite to the one of NFA.)
                       The value -log10(NFA) is equivalent but more
                       intuitive than NFA:
                       - -1.0 gives an average of 10 false detections on noise
                       -  0.0 gives an average of 1 false detections on noise
                       -  1.0 gives an average of 0.1 false detections on nose
                       -  2.0 gives an average of 0.01 false detections on noise
                       .
                       Suggested value: 0.0

    @param density_th  Minimal proportion of 'supporting' points in a rectangle.
                       Suggested value: 0.7

    @param n_bins      Number of bins used in the pseudo-ordering of gradient
                       modulus.
                       Suggested value: 1024

    @param reg_img     Optional output: if desired, LSD will return an
                       int image where each pixel indicates the line segment
                       to which it belongs. Unused pixels have the value '0',
                       while the used ones have the number of the line segment,
                       numbered 1,2,3,..., in the same order as in the
                       output list. If desired, a non NULL int** pointer must
                       be assigned, and LSD will make that the pointer point
                       to an int array of size reg_x x reg_y, where the pixel
                       value at (x,y) is obtained with (*reg_img)[x+y*reg_x].
                       Note that the resulting image has the size of the image
                       used for the processing, that is, the size of the input
                       image scaled by the given factor 'scale'. If scale!=1
                       this size differs from XxY and that is the reason why
                       its value is given by reg_x and reg_y.
                       Suggested value: NULL

    @param reg_x       Pointer to an int where LSD will put the X size
                       'reg_img' image, when asked for.
                       Suggested value: NULL

    @param reg_y       Pointer to an int where LSD will put the Y size
                       'reg_img' image, when asked for.
                       Suggested value: NULL

    @return            A double array of size 7 x n_out, containing the list
                       of line segments detected. The array contains first
                       7 values of line segment number 1, then the 7 values
                       of line segment number 2, and so on, and it finish
                       by the 7 values of line segment number n_out.
                       The seven values are:
                       - x1,y1,x2,y2,width,p,-log10(NFA)
                       .
                       for a line segment from coordinates (x1,y1) to (x2,y2),
                       a width 'width', an angle precision of p in (0,1) given
                       by angle_tolerance/180 degree, and NFA value 'NFA'.
                       If 'out' is the returned pointer, the 7 values of
                       line segment number 'n+1' are obtained with
                       'out[7*n+0]' to 'out[7*n+6]'.
 */
double * LineSegmentDetection( int * n_out,
                               double * img, int X, int Y,
                               double scale, double sigma_scale, double quant,
                               double ang_th, double log_eps, double density_th,
                               int n_bins,
                               int ** reg_img, int * reg_x, int * reg_y );

/*----------------------------------------------------------------------------*/
/** LSD Simple Interface with Scale and Region output.

    @param n_out       Pointer to an int where LSD will store the number of
                       line segments detected.

    @param img         Pointer to input image data. It must be an array of
                       doubles of size X x Y, and the pixel at coordinates
                       (x,y) is obtained by img[x+y*X].

    @param X           X size of the image: the number of columns.

    @param Y           Y size of the image: the number of rows.

    @param scale       When different from 1.0, LSD will scale the input image
                       by 'scale' factor by Gaussian filtering, before detecting
                       line segments.
                       Example: if scale=0.8, the input image will be subsampled
                       to 80% of its size, before the line segment detector
                       is applied.
                       Suggested value: 0.8

    @param reg_img     Optional output: if desired, LSD will return an
                       int image where each pixel indicates the line segment
                       to which it belongs. Unused pixels have the value '0',
                       while the used ones have the number of the line segment,
                       numbered 1,2,3,..., in the same order as in the
                       output list. If desired, a non NULL int** pointer must
                       be assigned, and LSD will make that the pointer point
                       to an int array of size reg_x x reg_y, where the pixel
                       value at (x,y) is obtained with (*reg_img)[x+y*reg_x].
                       Note that the resulting image has the size of the image
                       used for the processing, that is, the size of the input
                       image scaled by the given factor 'scale'. If scale!=1
                       this size differs from XxY and that is the reason why
                       its value is given by reg_x and reg_y.
                       Suggested value: NULL

    @param reg_x       Pointer to an int where LSD will put the X size
                       'reg_img' image, when asked for.
                       Suggested value: NULL

    @param reg_y       Pointer to an int where LSD will put the Y size
                       'reg_img' image, when asked for.
                       Suggested value: NULL

    @return            A double array of size 7 x n_out, containing the list
                       of line segments detected. The array contains first
                       7 values of line segment number 1, then the 7 values
                       of line segment number 2, and so on, and it finish
                       by the 7 values of line segment number n_out.
                       The seven values are:
                       - x1,y1,x2,y2,width,p,-log10(NFA)
                       .
                       for a line segment from coordinates (x1,y1) to (x2,y2),
                       a width 'width', an angle precision of p in (0,1) given
                       by angle_tolerance/180 degree, and NFA value 'NFA'.
                       If 'out' is the returned pointer, the 7 values of
                       line segment number 'n+1' are obtained with
                       'out[7*n+0]' to 'out[7*n+6]'.
 */
double * lsd_scale_region( int * n_out,
                           double * img, int X, int Y, double scale,
                           int ** reg_img, int * reg_x, int * reg_y );

/*----------------------------------------------------------------------------*/
/** LSD Simple Interface with Scale

    @param n_out       Pointer to an int where LSD will store the number of
                       line segments detected.

    @param img         Pointer to input image data. It must be an array of
                       doubles of size X x Y, and the pixel at coordinates
                       (x,y) is obtained by img[x+y*X].

    @param X           X size of the image: the number of columns.

    @param Y           Y size of the image: the number of rows.

    @param scale       When different from 1.0, LSD will scale the input image
                       by 'scale' factor by Gaussian filtering, before detecting
                       line segments.
                       Example: if scale=0.8, the input image will be subsampled
                       to 80% of its size, before the line segment detector
                       is applied.
                       Suggested value: 0.8

    @return            A double array of size 7 x n_out, containing the list
                       of line segments detected. The array contains first
                       7 values of line segment number 1, then the 7 values
                       of line segment number 2, and so on, and it finish
                       by the 7 values of line segment number n_out.
                       The seven values are:
                       - x1,y1,x2,y2,width,p,-log10(NFA)
                       .
                       for a line segment from coordinates (x1,y1) to (x2,y2),
                       a width 'width', an angle precision of p in (0,1) given
                       by angle_tolerance/180 degree, and NFA value 'NFA'.
                       If 'out' is the returned pointer, the 7 values of
                       line segment number 'n+1' are obtained with
                       'out[7*n+0]' to 'out[7*n+6]'.
 */
double * lsd_scale(int * n_out, double * img, int X, int Y, double scale);

/*----------------------------------------------------------------------------*/
/** LSD Simple Interface

    @param n_out       Pointer to an int where LSD will store the number of
                       line segments detected.

    @param img         Pointer to input image data. It must be an array of
                       doubles of size X x Y, and the pixel at coordinates
                       (x,y) is obtained by img[x+y*X].

    @param X           X size of the image: the number of columns.

    @param Y           Y size of the image: the number of rows.

    @return            A double array of size 7 x n_out, containing the list
                       of line segments detected. The array contains first
                       7 values of line segment number 1, then the 7 values
                       of line segment number 2, and so on, and it finish
                       by the 7 values of line segment number n_out.
                       The seven values are:
                       - x1,y1,x2,y2,width,p,-log10(NFA)
                       .
                       for a line segment from coordinates (x1,y1) to (x2,y2),
                       a width 'width', an angle precision of p in (0,1) given
                       by angle_tolerance/180 degree, and NFA value 'NFA'.
                       If 'out' is the returned pointer, the 7 values of
                       line segment number 'n+1' are obtained with
                       'out[7*n+0]' to 'out[7*n+6]'.
 */

struct lines_t{
  lines_t(double(*v)[4], double(*h)[4], int lv, int lh)
        :lines_v(v), lines_h(h), len_v(lv), len_h(lh)
  {};

  double(*lines_v)[4];
  double(*lines_h)[4];
  int len_v;
  int len_h;
};

#ifdef __cplusplus
extern "C"{
#endif

double * lsd(int * n_out, double * img, int X, int Y);
lines_t* lsd_with_line_merge(double * img, int X, int Y);
void free_memory(void* ptr);

#ifdef __cplusplus
}
#endif




int reduce_lines(double (*lines)[4], 
                  const int len, 
                  double * dist, 
                  const int size,
                  double (*lines_reduced)[4],
                  double thresh);


lines_t* reduce_graph(double (*lines)[4], int& size);
void reduce_parallel(double (*lines)[4], int& size);

int argmin(const double* dist, const int row, const int size);


int argmax(const double* dist, const int row, const int size);
bool is_in(std::vector<int> & visited, const int val);

int __next_node(const int cur_idx, 
                double * dist, 
                const int size,
                std::vector<int> & visited,
                double thresh);

void merge_lines_max_dist(const double (&l1)[4], const double (&l2)[4], double (&out)[4]);
int* cluster_angles(double* angles, const int size);
double *merge_lines_parallel(const double (*lines)[4], const int idx_a, const int idx_b);

double calculate_distance_thresh(const double* dist, const int size);

#endif /* !LSD_HEADER */
/*----------------------------------------------------------------------------*/
