#include "screenshot.h"


std::vector<BYTE> ScreenShot::BitmapToByteStream(HBITMAP hBitmap, HDC hdc) {
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;
    HANDLE hf;                 // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbmih;    // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE* hp;                   // byte pointer 

    // 获取位图信息
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp);
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else
        cClrBits = 32;

    // 为位图信息结构体分配内存空间
    if (cClrBits != 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

    pbmih = (PBITMAPINFOHEADER)pbmi;
    pbmih->biSize = sizeof(BITMAPINFOHEADER);
    pbmih->biWidth = bmp.bmWidth;
    pbmih->biHeight = bmp.bmHeight;
    pbmih->biPlanes = bmp.bmPlanes;
    pbmih->biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmih->biClrUsed = (1 << cClrBits);

    pbmih->biCompression = BI_RGB;
    pbmih->biSizeImage = ((pbmih->biWidth * cClrBits + 31) & ~31) / 8
        * pbmih->biHeight;
    pbmih->biClrImportant = 0;

    // 分配位图字节数组
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbmih->biSizeImage);

    // 获取位图数据
    GetDIBits(hdc, hBitmap, 0, (WORD)pbmih->biHeight, lpBits, pbmi, DIB_RGB_COLORS);

    // 创建字节流
    std::vector<BYTE> byteStream;
    byteStream.resize(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmih->biSizeImage);

    // 填充BITMAPFILEHEADER
    hdr.bfType = 0x4D42;  // 'BM'
    hdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmih->biSizeImage;
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // 复制BITMAPFILEHEADER到字节流
    memcpy(&byteStream[0], &hdr, sizeof(BITMAPFILEHEADER));

    // 复制BITMAPINFOHEADER到字节流
    memcpy(&byteStream[sizeof(BITMAPFILEHEADER)], pbmih, sizeof(BITMAPINFOHEADER));

    // 复制像素数据到字节流
    memcpy(&byteStream[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)], lpBits, pbmih->biSizeImage);

    // 清理内存
    GlobalFree((HGLOBAL)lpBits);
    LocalFree(pbmi);

    return byteStream;
}

//cv::Mat ScreenShot::Screen_mat(){
//    HWND hwndDesktop = GetDesktopWindow();
//    HDC hwindowDC = GetDC(hwndDesktop);
//    int screen_width = GetSystemMetrics(SM_CXSCREEN); // 获取屏幕宽度
//    int screen_height = GetSystemMetrics(SM_CYSCREEN); // 获取屏幕高度
//
//    int x = (screen_width - x_size) / 2; // 计算左上角的 x 坐标
//    int y = (screen_height - y_size) / 2; // 计算左上角的 y 坐标
//
//    cv::Mat src(x_size, y_size, CV_8UC4); // 创建空的 Mat 对象用于存储图像
//
//    BITMAPINFOHEADER bi;
//    bi.biSize = sizeof(BITMAPINFOHEADER);
//    bi.biWidth = x_size;
//    bi.biHeight = -y_size;  // 此行使其绘制上下颠倒
//    bi.biPlanes = 1;
//    bi.biBitCount = 32;
//    bi.biCompression = BI_RGB;
//    bi.biSizeImage = 0;
//    bi.biXPelsPerMeter = 0;
//    bi.biYPelsPerMeter = 0;
//    bi.biClrUsed = 0;
//    bi.biClrImportant = 0;
//
//
//    // 创建兼容的设备上下文
//    HDC hMemDC = CreateCompatibleDC(hwindowDC);
//    HBITMAP hBitmap = CreateCompatibleBitmap(hwindowDC, x_size, y_size);
//    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
//
//    // 从屏幕截取图像到内存设备上下文
//    BitBlt(hMemDC, 0, 0, x_size, y_size, hwindowDC, x, y, SRCCOPY);
//
//    // 从内存设备上下文复制图像到Mat对象
//    GetDIBits(hMemDC, hBitmap, 0, x_size, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
//    // 释放资源
//    SelectObject(hMemDC, hOldBitmap);
//    DeleteObject(hBitmap);
//    DeleteDC(hMemDC);
//    ReleaseDC(hwndDesktop, hwindowDC);
//
//    return src;
//}

std::vector<BYTE> ScreenShot::Screen_byte() {
    HWND hwndDesktop = GetDesktopWindow();
    HDC hwindowDC = GetDC(hwndDesktop);
    int screen_width = GetSystemMetrics(SM_CXSCREEN); // 获取屏幕宽度
    int screen_height = GetSystemMetrics(SM_CYSCREEN); // 获取屏幕高度

    int x = (screen_width - x_size) / 2; // 计算左上角的 x 坐标
    int y = (screen_height - y_size) / 2; // 计算左上角的 y 坐标

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = x_size;
    bi.biHeight = -y_size;  // 此行使其绘制上下颠倒
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;


    // 创建兼容的设备上下文
    HDC hMemDC = CreateCompatibleDC(hwindowDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hwindowDC, x_size, y_size);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // 从屏幕截取图像到内存设备上下文
    BitBlt(hMemDC, 0, 0, x_size, y_size, hwindowDC, x, y, SRCCOPY);

    std::vector<BYTE> byteStream = this->BitmapToByteStream(hBitmap, hMemDC);
    // 释放资源
    SelectObject(hMemDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(hwndDesktop, hwindowDC);

    return byteStream;
}


void ScreenShot::resize(int x_size, int y_size)
{
    x_size = x_size;
    y_size = y_size;
}


//cv::Mat ScreenShot::byteStreamToMat(const std::vector<BYTE>& byteStream) {
//    // 使用 OpenCV 的 imdecode 函数将字节流解码为 Mat 对象
//    cv::Mat img = cv::imdecode(cv::Mat(byteStream), cv::IMREAD_UNCHANGED);
//
//    if (img.empty()) {
//        std::cerr << "Error: Unable to decode image." << std::endl;
//    }
//
//    return img;
//}

ScreenShot::ScreenShot(int x_size_, int y_size_, int max_x_size_, int max_y_size_, int min_x_size_, int min_y_size_)
{
    x_size = x_size_;
    y_size = y_size_;
    max_y_size = max_y_size_;
    max_x_size = max_x_size_;
    min_y_size = min_y_size_;
    min_x_size = min_x_size_;
}

ScreenShot::ScreenShot() {
    x_size = 220;
    y_size = 220;
    max_y_size = 320;
    max_x_size = 320;
    min_y_size = 220;
    min_x_size = 220;
}

void ScreenShot::size_add() {
    x_size += 20;
    y_size += 20;
    x_size = min(x_size, max_x_size);
    y_size = min(y_size, max_y_size);
}

void ScreenShot::size_sub() {
    x_size -= 20;
    y_size -= 20;
    x_size = max(x_size, min_x_size);
    y_size = max(y_size, min_y_size);
}