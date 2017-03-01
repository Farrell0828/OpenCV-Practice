#include <iostream>
using namespace std;

#include "funs.h"

int main()
{
	
	String xml_file_name = "TempletInformation-600px.xml";
	vector< vector<KeyPoint> > key_points_arr;
	vector<Mat> descriptor_arr;
	double t = (double)getTickCount();
	LoadXML(xml_file_name, key_points_arr, descriptor_arr);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "加载XML文档用时: " << t << "s" << endl;

	double t_begin = (double)getTickCount();
	String img_file_name = "3-25.jpg";
	Mat img = imread(img_file_name);
	//resize(img, img, img.size() / 3);
	if (img.empty())
	{
		cout << "打开文件" << img_file_name << "失败！\n";
		return -1;
	}
	Ptr<SURF> detector = SURF::create(800);
	vector<KeyPoint> key_points;
	detector->detect(img, key_points);
	Mat descriptor;
	detector->compute(img, key_points, descriptor);

	int max_index = -1;
	int max_num = 0;
	int num_of_good_matchs;
	for (int i = 0; i < descriptor_arr.size(); i++)
	{
		double t = (double)getTickCount();
		num_of_good_matchs = Match(key_points_arr[i], key_points, descriptor_arr[i], descriptor);
		t = ((double)getTickCount() - t) / getTickFrequency();
		cout << i << ": " << num_of_good_matchs << "	" << t << "s" << endl;
		if (num_of_good_matchs > max_num)
		{
			max_num = num_of_good_matchs;
			max_index = i;
		}
	}
	ShowResult(max_index);
	t_begin = ((double)getTickCount() - t_begin) / getTickFrequency();
	cout << "匹配用时: " << t_begin << "s" << endl;

	

	waitKey();
	return 0;
}