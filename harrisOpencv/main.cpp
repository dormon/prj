#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;

std::string const source_window = "Source image";
std::string const corners_window = "Corners detected";

/// Function header
void cornerHarris_demo( int, void* );

void getCorners(std::vector<CvPoint>&points,Mat const&img){
  Mat gray;
  cvtColor(img,gray,CV_BGR2GRAY);
  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32F );

  /// Detector parameters
  int    const blockSize    = 2   ;
  int    const apertureSize = 3   ;
  double const k            = 0.04;

  /// Detecting corners
  cornerHarris( gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32F, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
    for( int i = 0; i < dst_norm.cols; i++ )
      if( (int) dst_norm.at<float>(j,i) > thresh )
        points.emplace_back(i,j);
}

/** @function main */
int main( int argc, char** argv )
{
  if(argc<2)return 0;
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );
  std::vector<CvPoint>corners;
  getCorners(corners,src);
  std::cout << corners.size() << std::endl;
  for(auto const&x:corners)
    circle(src,x,2,CV_RGB(255,0,0),2,8,0);
  imshow("output",src);
  waitKey(0);
  return 0;
  cvtColor( src, src_gray, CV_BGR2GRAY );


  /// Create a window and a trackbar
  namedWindow( source_window.c_str(), CV_WINDOW_AUTOSIZE );
  createTrackbar( "Threshold: ", source_window.c_str(), &thresh, max_thresh, cornerHarris_demo );
  imshow( source_window.c_str(), src );

  cornerHarris_demo( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function cornerHarris_demo */
void cornerHarris_demo( int, void* ){
  Mat dst, dst_norm, dst_norm_scaled;
  dst = Mat::zeros( src.size(), CV_32F );

  /// Detector parameters
  int    const blockSize    = 2   ;
  int    const apertureSize = 3   ;
  double const k            = 0.04;

  /// Detecting corners
  cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32F, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
    for( int i = 0; i < dst_norm.cols; i++ )
      if( (int) dst_norm.at<float>(j,i) > thresh )
        circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
  /// Showing the result
  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
  imshow( corners_window, dst_norm_scaled );
}
