#include <iostream>
#include <vector>
using namespace std;

#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>
using namespace cv;
using namespace xfeatures2d;

int main()
{
	Ptr<SurfFeatureDetector> detector
		= SurfFeatureDetector::create(800);		// Surf特征检测器
	Ptr<SurfDescriptorExtractor> descriptor_extractor
		= SurfDescriptorExtractor::create(800);	// Surf特征向量生成器
	Mat img = imread("piston1.jpg");
	vector<KeyPoint> key_points;		// 特征点存储vector
	detector->detect(img, key_points);
	Mat descriptor;
	descriptor_extractor->compute(img, key_points, descriptor);

	vector<int> int_vect = { 1, 2, 3, 4, 5 };
	Mat mat = Mat::eye(2, 3, CV_32F);

	FileStorage fs_write("test.xml", FileStorage::WRITE);
	fs_write << "IntVector" << int_vect;
	fs_write << "Mat" << mat;
	fs_write << "KeyPoints" << key_points;
	fs_write << "Descriptor" << descriptor;
	fs_write.release();

	vector<KeyPoint> key_points_copy;
	Mat descripter_copy;

	vector<int> int_vect_copy;
	Mat mat_copy;

	FileStorage fs_read("test.xml", FileStorage::READ);
	fs_read["IntVector"] >> int_vect_copy;
	fs_read["Mat"] >> mat_copy;
	fs_read["KeyPoints"] >> key_points_copy;
	fs_read["Descriptor"] >> descripter_copy;
	fs_read.release();

	Mat img_key;
	drawKeypoints(img, key_points, img_key, CV_RGB(255, 0, 0));
	imshow("KeyPoints", img_key);
	Mat img_key_copy;
	drawKeypoints(img, key_points_copy, img_key_copy, CV_RGB(255, 0, 0));
	imshow("KeyPointsCopy", img_key_copy);
	waitKey();
	return 0;
}