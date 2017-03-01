#ifndef FUNS_H_

#include <vector>
using namespace std;

#include <opencv2\opencv.hpp>
#include <opencv2\xfeatures2d.hpp>
using namespace cv;
using namespace xfeatures2d;

int Match(const vector<KeyPoint> &key_points1, const vector<KeyPoint> &key_points2,
	const Mat &descriptors1, const Mat &descriptors2);

void LoadXML(const String &xml_file_name, vector< vector<KeyPoint> > &key_points, vector<Mat> &descriptors);

void ShowResult(int no);

#endif // !FUNS_H_
