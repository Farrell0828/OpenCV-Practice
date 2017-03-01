#include <iostream>
using namespace std;

#include "funs.h"

int main()
{
	String xml_file_name = "TempletInformation.xml";
	vector< vector<KeyPoint> > key_points_arr;
	vector<Mat> descriptor_arr;
	double t = (double)getTickCount();
	LoadXML(xml_file_name, key_points_arr, descriptor_arr);
	t = ((double)getTickCount() - t) / getTickFrequency();
	cout << "����XML�ĵ���ʱ: " << t << "s" << endl;

	VideoCapture camera;
	Mat frame, img; 
	camera.open(1);
	if (!camera.isOpened())
	{
		cout << "Open the camera is faile!\n";
		return -1;
	}
	cout << "Camera open successful!\n";
	namedWindow("Camera");

	while (true)
	{
		camera >> frame;
		imshow("Camera", frame);
		if (waitKey(10) == ' ')
		{
			img = frame;
			destroyWindow("Camera");
			break;
		}
	}
	double t_begin = (double)getTickCount();
	if (img.empty())
	{
		cout << "��ȡ��Ƶ֡ʧ�ܣ�\n";
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

	cout << "   ���   ƥ�����������     ƥ����ʱ" << endl;
	for (int i = 0; i < descriptor_arr.size(); i++)
	{
		double t = (double)getTickCount();
		num_of_good_matchs = Match(key_points_arr[i], key_points, descriptor_arr[i], descriptor);
		t = ((double)getTickCount() - t) / getTickFrequency();
		cout << "   " << setw(2) << i << "            " << num_of_good_matchs << "          " << t << "s" << endl;

		if (num_of_good_matchs > max_num)
		{
			max_num = num_of_good_matchs;
			max_index = i;
		}
	}
	cout << "\nƥ��ͼ���: " << max_index << endl;
	ShowResult(max_index);
	t_begin = ((double)getTickCount() - t_begin) / getTickFrequency();
	cout << "ƥ����ʱ: " << t_begin << "s" << endl;

	waitKey();
	return 0;
}