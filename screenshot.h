#include <vector>
#include <Windows.h>
#include <iostream>


class ScreenShot {
public:
	ScreenShot();
	ScreenShot(int, int, int, int, int, int);
//	cv::Mat Screen_mat();
	std::vector<BYTE> Screen_byte();
//	cv::Mat byteStreamToMat(const std::vector<BYTE>& );
	void resize(int, int);
	void size_add();
	void size_sub();
	int max_x_size;
	int max_y_size;
	int min_x_size;
	int min_y_size;
	int x_size;
	int y_size;
private:
	std::vector<BYTE> BitmapToByteStream(HBITMAP , HDC );
};