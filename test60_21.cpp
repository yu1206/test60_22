#include "MSRCR.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <io.h>
#include <Windows.h>
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <direct.h>

using namespace cv;
using namespace std;
extern int readDir(char *dirName, vector<string> &filesName);
int test60_21(int argc, char *argv[])
{
	//string  imgpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete2-2class";
	//string  imgpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete2-1";
	string  imgpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak";
	
	vector<string> filesName1;
	readDir((char*)imgpath.c_str(), filesName1);
	int classnum[7] = { 0 };
	for (int i = 0; i<filesName1.size(); i++)
	{
		string imgfile = filesName1[i];
		Mat img = imread(imgfile);
		if (img.data==NULL)
		{
			cout << "img.data 数据打开错误！" << endl;
			continue;
		}
		int pos = imgfile.find_last_of('\\');
		int pos2 = imgfile.find_last_of('.');
		string name1 = imgfile.substr(pos + 1, pos2 - pos - 1);
		string labelfile = imgpath + "\\" + name1 + ".txt";
		fstream finRead;
		finRead.open(labelfile, ios::in);
		if (!finRead.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		
		string line;
		while (getline(finRead, line))
		{
			if (line.empty())
			{
				break;
			}

			int label; float x, y, w, h;

			stringstream str_s(line);
			str_s >> label >> x >> y >> w >> h;


			classnum[label]++;
		}
		finRead.close();
		
	}

	for (int i = 1; i < 7; i++)
	{
		printf("%d,classnum:%d\n", i, classnum[i]);
	}
	return 0;
}
struct imfo60_22
{
	string name;
	int objw;
	int objh;
	int width;
	int height;
	float ratiow;
	float ratioh;
};


//升序
bool LessSortw(imfo60_22 a, imfo60_22 b)
{

	return (a.ratiow<b.ratiow);
}
//升序
bool LessSorth(imfo60_22 a, imfo60_22 b)
{
	return (a.ratioh<b.ratioh);
}
int test60_22(int argc, char *argv[])
{



	string imgpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete2-2class";
	string imgpathdraw = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete2-2class-draw";
	mkdir(imgpathdraw.c_str());
	string wtxt = imgpath+ "\\" + "0_wratio.txt";
	fstream finWrite;
	finWrite.open(wtxt, ios::out);
	if (!finWrite.is_open())
	{
		cout << "finRead1 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}

	string htxt = imgpath + "\\" + "0_hratio.txt";
	fstream finWrite2;
	finWrite2.open(htxt, ios::out);
	if (!finWrite2.is_open())
	{
		cout << "finRead2 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}



	map<int, vector<imfo60_22>>mvfo;

	vector<string> v_img_; int num = 0;
	readDir((char*)imgpath.c_str(), v_img_);
	int short_size = 600; int max_long_size = 1000;

	float minw = 10000; float minh = 10000;
	float maxw = 0; float maxh = 0;
	vector<imfo60_22>vfo;
	for (int i = 0; i<v_img_.size(); i++)
	{
		string imgfile = v_img_[i];
		Mat img = imread(imgfile);
		if (img.data == NULL)
		{
			cout << "img.data 数据打开错误！" << endl;
			continue;
		}
		int pos = imgfile.find_last_of('\\');
		int pos2 = imgfile.find_last_of('.');
		string name1 = imgfile.substr(pos + 1, pos2 - pos - 1);
		string labelfile = imgpath + "\\" + name1 + ".txt";
		fstream finRead;
		finRead.open(labelfile, ios::in);
		if (!finRead.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		string line; int num1 = 0;

		while (getline(finRead, line))
		{

			if (line.empty())
			{
				break;
			}

			num1++;

			int label; int x1, y1, x2, y2;
			stringstream str_s(line);
			str_s >> label >> x1 >> y1 >> x2 >> y2;

			int objw = x2 - x1 + 1; int objh = y2 - y1 + 1;

			Rect rt; rt.x = x1; rt.y = y1; rt.width = objw; rt.height = objh;
			float objperw = objw*1.0 / img.cols;
			float objperh = objh*1.0 / img.rows;

			imfo60_22 vi;
			vi.name = name1;

			vi.ratiow = objperw;
			vi.ratioh = objperh;

			vi.objw = objw;
			vi.objh = objh;

			vi.width = img.cols;
			vi.height = img.rows;
			vfo.push_back(vi);


			if (objperw<minw)
			{
				minw = objperw;
			}
			if (objperh < minh)
			{
				minh = objperh;
			}

			if (objperw>maxw)
			{
				maxw = objperw;
			}
			if (objperh > maxh)
			{
				maxh = objperh;
			}

			rectangle(img,rt,Scalar(0,0,255),1);


		}

		finRead.close();

		string str = imgpathdraw + "\\" + name1 + ".jpg";
		imwrite(str,img);
	}
	printf("perminw=%f,perminh=%f,permaxw=%f,permaxh=%f\n", minw, minh, maxw, maxh);

	sort(vfo.begin(), vfo.end(), LessSortw);//升序排列

	for (int j = 0; j < vfo.size(); j++)
	{
		imfo60_22 vi = vfo[j];
		finWrite << vi.name << " " << vi.ratiow << " " << vi.ratioh << " " << vi.objw << " " << vi.objh
			<< " " << vi.width << " " << vi.height << endl;
	}

	finWrite.close();


	sort(vfo.begin(), vfo.end(), LessSorth);//升序排列

	for (int j = 0; j < vfo.size(); j++)
	{
		imfo60_22 vi = vfo[j];
		finWrite2 << vi.name << " " << vi.ratiow << " " << vi.ratioh << " " << vi.objw << " " << vi.objh
			<< " " << vi.width << " " << vi.height << endl;
	}

	finWrite2.close();




	return 0;
}

int test60_23(int argc, char *argv[])
{



	string imgpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete-10\\leak-frames-1";
	string labelpath = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete-10\\leak-labels-1";
	string wtxt = "E:\\LISA-Traffic-Light\\dayTrainingall\\weight-train-leak-delete-10\\leak-train-1.txt";

	fstream finWrite;
	finWrite.open(wtxt, ios::out);
	if (!finWrite.is_open())
	{
		cout << "finRead1 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}

	vector<string> v_img_;
	readDir((char*)imgpath.c_str(), v_img_);
	
	for (int i = 0; i<v_img_.size(); i++)
	{
		string imgfile = v_img_[i];
		Mat img = imread(imgfile);
		if (img.data == NULL)
		{
			cout << "img.data 数据打开错误！" << endl;
			continue;
		}
		int pos = imgfile.find_last_of('\\');
		int pos2 = imgfile.find_last_of('.');
		string name1 = imgfile.substr(pos + 1, pos2 - pos - 1);
		
		string imgstr = "leak-frames-1"+string("/")+name1+".png";
		string txtstr = "leak-labels-1" + string("/") + name1 + ".txt";
		finWrite << imgstr<<" "<<txtstr<<endl;
		
	}
	




	return 0;
}