#pragma once

#include <vector>
#include <functional>
#include <fstream>
#include <filesystem>
using namespace std;

void sis_cor_turn(double x_o, double y_o, double alf, double* x_n, double* y_n)
{
	*x_n = x_o * cos(alf) - y_o * sin(alf);
	*y_n = x_o * sin(alf) + y_o * cos(alf);
}

class ImageReader;

void drawPixel(int x, int y, COLORREF color)
{
	//txSetPixel(2 * x    , 2 * y    , color);
	//txSetPixel(2 * x + 1, 2 * y    , color);
	//txSetPixel(2 * x    , 2 * y + 1, color);
	//txSetPixel(2 * x + 1, 2 * y + 1, color);
    txSetPixel(x, y, color);
}

template<auto WIDTH, auto HEIGHT = WIDTH>
class Image
{
public:
	static constexpr auto ADD_R = 50;
	static constexpr auto EXWIDTH  = ADD_R + WIDTH + ADD_R;
	static constexpr auto EXHEIGHT = ADD_R + HEIGHT + ADD_R;

	friend class ImageReader;

public:
	Image() : image{} {}

	void load(const std::string &file, int xCoord, int yCoord)
	{
		std::ifstream ifstr(file);
		if (!ifstr.is_open())
		{
			std::cerr << "Cannot open file(" << file << ")\n";

			std::exit(-1);
		}

		for (int i{}; i < WIDTH; ++i)
			for (int j{}; j < HEIGHT; ++j)
			{
				char c;
				ifstr >> c;

				image[i + ADD_R][j + ADD_R] = int(c - '0');
			}
		
		/*for (int i{}; i < WIDTH; ++i)
		{
			for (int j{}; j < HEIGHT; ++j)
				cout << img[i][j];

			cout << endl;
		}*/

		for (int i{}; i < EXWIDTH; ++i)
			for (int j{}; j < EXHEIGHT; ++j)
				if(image[i][j]) 
					drawPixel(i+xCoord, j+yCoord, TX_RED);

		ifstr.close();
	}

	static vector<pair<int, int>> getCircle(double alf, double k)
	{
		vector<pair<int, int>> data;

		for (int i = -WIDTH / 2; i < WIDTH / 2; i++)
		{
			for (int j = -HEIGHT / 2; j < HEIGHT / 2; j++)
			{
				double x, y;
				sis_cor_turn(i, j, alf, &x, &y);

				x *= k; 
				y /= k;

				if (x * x + y * y <= 100) 
					data.push_back({ i, j });
			}
		}
		return data;
	}

	int pattern(double alf, double k)
	{
		int ans = 0;

		auto epsOcr = getCircle(alf, k);

		bool img_copy[EXWIDTH][EXHEIGHT]{};

		for (int i{}; i < EXWIDTH; ++i)
			for (int j{}; j < EXHEIGHT; ++j)
				img_copy[i][j] = image[i][j];

		//vector<pair<int, int>> boarder;

		int counter = 0;
		for (int i{1}; i < EXWIDTH - 1; ++i)
			for (int j{1}; j < EXHEIGHT - 1; ++j)
			{
				if (image[i][j])
				{
					if (image[i - 1][j] && image[i + 1][j] && image[i][j - 1] && image[i][j + 1]) 
						continue;

					
					//drawPixel(i, j, TX_GREEN);

					counter++;
					if (counter % 201 == 0)
						for (auto[x, y] : epsOcr)
							img_copy[i + x][j + y] = true;
					//boarder.push_back({ i, j });
				}
			}
		for (int i{}; i < EXWIDTH; ++i)
			for (int j{}; j < EXHEIGHT; ++j)
				if (img_copy[i][j] && !image[i][j])
				{
					//drawPixel(i, j, TX_BLUE);
					ans++;
				}
		cout.precision(8);
		cout << fixed << ans << " ";
		return ans;
	}

private:
	bool image[EXWIDTH][EXHEIGHT];
};

class ImageReader
{
public:
	const std::streampos BMPHEADER = 54;

public:
	ImageReader(std::filesystem::path path) : m_path{ path } {};

	template<auto W, auto H>
    void convertToBinary(Image<W, H> *dest)
	{
		FILE *file = nullptr;
		fopen_s(&file, m_path.generic_string().c_str(), "rb");
		if (!file)
		{
			std::cerr << "Cannot open file(" << m_path << ")\n";

			std::exit(-1);
		}

		fseek(file, BMPHEADER, 0);

		unsigned int *buff = new unsigned int[W * H]{};
		fread(buff, W * H * 4, 1, file);

		for (int i{}; i < W; ++i)
			for (int j{}; j < H; ++j)
				dest->image[i + dest->ADD_R][j + dest->ADD_R] = buff[(H - 1 - j) * W + i] > 125;

		delete[] buff;
		fclose(file);
	}

private:
	std::filesystem::path m_path;
};