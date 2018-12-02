#include "..\include\TXLib.h"
#include "Image.hpp"

signed _cdecl main(int argc, char **argv)
{
	std::ios_base::sync_with_stdio(false);

	txCreateWindow(1600, 1000);


	Image<351> image;
	Image<351> image2;
	Image<351> image3;
	//ImageReader("..//res//Вилка2.bmp").convertToBinary(&image);
	image.load("..//res//Вилка1.txt", 0, 100);
    image2.load("..//res//Вилка2.txt", 400, 100);
	image3.load("..//res//Вилка3.txt", 800, 100);

 
	long double v1[20], v2[20], v3[20];

	long double s1 = 0, s2 = 0, s3 = 0;


	double step = 3.1415926 / 10;
	int ind = 0;
	for (double alf = 0; ind < 20; alf += step)
	{
		v1[ind] = image.pattern(alf, 2);
		s1 += v1[ind] * v1[ind];

		v2[ind] = image2.pattern(alf, 2);
		s2 += v2[ind] * v2[ind];
		
		v3[ind] = image3.pattern(alf, 2);
		s3 += v3[ind] * v3[ind];
		
		cout << endl;
		++ind;
	}


	s1 /= 10000;
	s2 /= 10000;
	s3 /= 10000;

    ind = 0;
	for ( ; ind < 20;  ++ind)
	{
		v1[ind] /= s1;
		v2[ind] /= s2; 
		v3[ind] /= s3;
	}



	long long distTo1Min = LLONG_MAX;
	for (int offset = 0; offset < 20; offset++)
	{
		long long dist = 0;
		for (int i = 0; i < 20; i++)
		{
			dist += (v3[(i/* + offset*/) % 20] - v1[i])*(v3[(i/* + offset*/) % 20] - v1[i]);
		}
		distTo1Min = min(distTo1Min, dist);
	}

	long double distTo2Min = LLONG_MAX;
	for (int offset = 0; offset < 20; offset++)
	{
		long double dist = 0;
		for (int i = 0; i < 20; i++)
		{
			dist += (v3[(i/* + offset*/) % 20] - v2[i])*(v3[(i/* + offset*/) % 20] - v2[i]);
		}
		distTo2Min = min(distTo2Min, dist);
	}

	long double distTo3Min = LLONG_MAX;
	for (int offset = 0; offset < 20; offset++)
	{
		long double dist = 0;
		for (int i = 0; i < 20; i++)
		{
			dist += (v2[(i/* + offset*/) % 20] - v1[i])*(v2[(i /*+ offset*/) % 20] - v1[i]);
		}
		distTo3Min = min(distTo3Min, dist);
	}

	cout << "distance between 1 and 3: " << distTo1Min << endl;
	cout << "distance between 2 and 3: " << distTo2Min << endl;
	cout << "distance between 2 and 1: " << distTo3Min << endl;

	long double dist = 0;
	for (int i = 0; i < 20; i++)
	{
		dist += (v1[i]-v3[i])*(v1[i] - v3[i]);
	}
	cout << dist;

	//{image.pattern(0, 2), image.pattern(0.5, 2), image.pattern(1, 2), image.pattern(1.5, 2) << endl;
	//cout << image.pattern(2, 2) << endl;
	//cout << image.pattern(2.5, 2) << endl;
	//cout << image.pattern(3, 2) << endl;
	//auto data = Image<200>::getCircle(1, 2);
	//cout << data.size() << endl;
	
	txBegin();
	//for (auto &&[x, y] : data)
	//{
		//txCircle(x + 100, y + 100, 1);
		//cout << x << " " << y << endl;
	//}

	txEnd();

	system("pause");
	return 0;
}