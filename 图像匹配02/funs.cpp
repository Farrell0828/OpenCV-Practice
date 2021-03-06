#include "funs.h"
// #include <iostream>
// using namespace std;

int Match(const vector<KeyPoint> &key_points1, const vector<KeyPoint> &key_points2,
	const Mat &descriptors1, const Mat &descriptors2)
{
	Ptr<DescriptorMatcher> descriptor_matcher =
		DescriptorMatcher::create("BruteForce");//创建特征匹配器

												//特征匹配    
	vector<DMatch> matches;//匹配结果    
	descriptor_matcher->match(descriptors1, descriptors2, matches);//匹配两个图像的特征矩阵    
	// cout << "Match个数：" << matches.size() << endl;

	//计算匹配结果中距离的最大和最小值    
	//距离是指两个特征向量间的欧式距离，表明两个特征的差异，值越小表明两个特征点越接近    
	double max_dist = 0;
	double min_dist = 100;
	for (int i = 0; i < matches.size(); i++)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		if (dist > max_dist) max_dist = dist;
	}
	// cout << "最大距离：" << max_dist << endl;
	// cout << "最小距离：" << min_dist << endl;

	//筛选出较好的匹配点    
	vector<DMatch> goodMatches;
	for (int i = 0; i < matches.size(); i++)
	{
		if (matches[i].distance < 0.2 * max_dist)
		{
			goodMatches.push_back(matches[i]);
		}
	}
	if (goodMatches.size() == 0) return 0;
	// cout << "goodMatch个数：" << goodMatches.size() << endl;

	//画出匹配结果    
	//Mat img_matches;
	//红色连接的是匹配的特征点对，绿色是未匹配的特征点    
	//drawMatches(img1, m_LeftKey, img2, m_RightKey, goodMatches, img_matches,
		//Scalar::all(-1)/*CV_RGB(255,0,0)*/, CV_RGB(0, 255, 0), Mat(), 2);

	//imshow("MatchSUFT", img_matches);
	//IplImage result = img_matches;

	//RANSAC匹配过程  
	vector<DMatch> m_Matches = goodMatches;
	// 分配空间  
	int ptCount = (int)m_Matches.size();
	Mat p1(ptCount, 2, CV_32F);
	Mat p2(ptCount, 2, CV_32F);

	// 把Keypoint转换为Mat  
	Point2f pt;
	for (int i = 0; i<ptCount; i++)
	{
		pt = key_points1[m_Matches[i].queryIdx].pt;
		p1.at<float>(i, 0) = pt.x;
		p1.at<float>(i, 1) = pt.y;

		pt = key_points2[m_Matches[i].trainIdx].pt;
		p2.at<float>(i, 0) = pt.x;
		p2.at<float>(i, 1) = pt.y;
	}

	// 用RANSAC方法计算F  
	Mat m_Fundamental;
	vector<uchar> m_RANSACStatus;       // 这个变量用于存储RANSAC后每个点的状态  
	findFundamentalMat(p1, p2, m_RANSACStatus, FM_RANSAC);

	// 计算野点个数  

	int OutlinerCount = 0;
	for (int i = 0; i<m_RANSACStatus.size(); i++)
	{
		if (m_RANSACStatus[i] == 0)    // 状态为0表示野点  
		{
			OutlinerCount++;
		}
	}
	int InlinerCount = ptCount - OutlinerCount;   // 计算内点  
	// cout << "内点数为：" << InlinerCount << endl;

	return InlinerCount;
}

void LoadXML(const String & xml_file_name, vector<vector<KeyPoint>>& key_points, vector<Mat>& descriptors)
{
	FileStorage fs(xml_file_name, FileStorage::READ);
	int num_of_pictures;			// 模板图片的总数
	vector<KeyPoint> temp_k;		// 存储一张图片的特征点vector的临时变量
	Mat temp_d;						// 存储一张图片的特征向量的临时变量
	String img_no;					// 图片编号
	String img_key_points_name;		// 图片特征点vector名称
	String img_descriptor_name;		// 图片特征向量名称

	fs["NumOfPictures"] >> num_of_pictures;
	for (int i = 0; i < num_of_pictures; i++)
	{
		stringstream ss;
		ss << i;
		img_no = ss.str();

		img_key_points_name = "KeyPoints" + img_no;
		fs[img_key_points_name] >> temp_k;
		key_points.push_back(temp_k);

		img_descriptor_name = "Descriptor" + img_no;
		fs[img_descriptor_name] >> temp_d;
		descriptors.push_back(temp_d);
	}
}

void ShowResult(int no)
{
	switch (no)
	{
	case 0: cout << "图3-1 S50MC-C型柴油机"; break;
	case 1: cout << "图3-8 十字头式活塞"; break;
	case 2: cout << "图3-9 插管喷射冷却活塞"; break;
	case 3: cout << "图3-10 筒形活塞"; break;
	case 4: cout << "图3-18 活塞杆填料函"; break;
	case 5: cout << "图3-22 筒形柴油机的气缸"; break;
	case 6: cout << "图3-25 压力拉伸工具"; break;
	case 7: cout << "图3-26 WartsilaL32型柴油机气缸盖"; break;
	case 8: cout << "图3-32 十字头的构造"; break;
	case 9: cout << "图3-36 Wartsila38型柴油机连杆"; break;
	case 10: cout << "图3-38 为Wartsila46型柴油机曲轴"; break;
	case 11: cout << "图3-39 S-MC-C柴油机曲轴"; break;
	case 12: cout << "图3-46 S-MC-C型柴油机机座"; break;
	case 13: cout << "图3-50 Wartsila32型中速柴油机机体"; break;
	case 14: cout << "图3-53 Wartsila柴油机的倒挂式主轴承"; break;
	case 15: cout << "图3-57 360°推力轴承"; break;
	case 16: cout << "图3-58 弹性推力环"; break;
	case 17: cout << "图4-6 回油孔式喷油泵基本结构图"; break;
	case 18: cout << "图4-10 出油阀偶件"; break;
	case 19: cout << "图4-11 Wartsila32型柴油机喷油泵"; break;
	case 20: cout << "图4-12 MAN B&W S-MC-C型柴油机喷油泵"; break;
	case 21: cout << "图4-18 Wartsila38型柴油机喷油器"; break;
	case 22: cout << "图6-3 MC型柴油机排气阀座"; break;
	case 23: cout << "图6-5 不带阀壳气阀机构"; break;
	case 24: cout << "图6-17 TCA增压器的外形"; break;
	case 25: cout << "图6-21 增压器转子"; break;
	case 26: cout << "图6-24 VTR系列增压器的效率及压比范围"; break;
	case 27: cout << "图6-25 TPL系列增压器"; break;
	case 28: cout << "图6-26 NAS系列增压器的结构图"; break;
	case 29: cout << "图7-9 带比重环全部排渣型分油机结构"; break;
	case 30: cout << "图7-10 ALFA-LAVAL S系列无比重环型分油机结构"; break;
	case 31: cout << "图8-46 盖斯林格高阻尼簧片联轴器"; break;
	case 32: cout << "图10-9 UG8表盘式调速器结构原理图"; break;
	case 33: cout << "图10-10 PGA调速器结构原理图"; break;
	case 34: cout << "图11-4 喷油电液控制阀"; break;
	case 35: cout << "图11-29 EG3P液压执行器"; break;
	case 36: cout << "图12-3 单气路控制式气缸起动阀(S50MC-C)"; break;
	case 37: cout << "图12-4 双气路控制式气缸起动阀"; break;
	case 38: cout << "图12-6 MAN B&W S50MC-C柴油机回转式空气分配器"; break;
	case 39: cout << "图12-7 RT-flex柴油机非均衡式主起动阀"; break;
	case 40: cout << "图13-11 ME-GI柴油机的燃烧室结构"; break;
	default: cout << "匹配失败！"; break;
	}
	cout << endl;
}