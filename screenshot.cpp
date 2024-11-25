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

    // ��ȡλͼ��Ϣ
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

    // Ϊλͼ��Ϣ�ṹ������ڴ�ռ�
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

    // ����λͼ�ֽ�����
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbmih->biSizeImage);

    // ��ȡλͼ����
    GetDIBits(hdc, hBitmap, 0, (WORD)pbmih->biHeight, lpBits, pbmi, DIB_RGB_COLORS);

    // �����ֽ���
    std::vector<BYTE> byteStream;
    byteStream.resize(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmih->biSizeImage);

    // ���BITMAPFILEHEADER
    hdr.bfType = 0x4D42;  // 'BM'
    hdr.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pbmih->biSizeImage;
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // ����BITMAPFILEHEADER���ֽ���
    memcpy(&byteStream[0], &hdr, sizeof(BITMAPFILEHEADER));

    // ����BITMAPINFOHEADER���ֽ���
    memcpy(&byteStream[sizeof(BITMAPFILEHEADER)], pbmih, sizeof(BITMAPINFOHEADER));

    // �����������ݵ��ֽ���
    memcpy(&byteStream[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)], lpBits, pbmih->biSizeImage);

    // �����ڴ�
    GlobalFree((HGLOBAL)lpBits);
    LocalFree(pbmi);

    return byteStream;
}

//cv::Mat ScreenShot::Screen_mat(){
//    HWND hwndDesktop = GetDesktopWindow();
//    HDC hwindowDC = GetDC(hwndDesktop);
//    int screen_width = GetSystemMetrics(SM_CXSCREEN); // ��ȡ��Ļ���
//    int screen_height = GetSystemMetrics(SM_CYSCREEN); // ��ȡ��Ļ�߶�
//
//    int x = (screen_width - x_size) / 2; // �������Ͻǵ� x ����
//    int y = (screen_height - y_size) / 2; // �������Ͻǵ� y ����
//
//    cv::Mat src(x_size, y_size, CV_8UC4); // �����յ� Mat �������ڴ洢ͼ��
//
//    BITMAPINFOHEADER bi;
//    bi.biSize = sizeof(BITMAPINFOHEADER);
//    bi.biWidth = x_size;
//    bi.biHeight = -y_size;  // ����ʹ��������µߵ�
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
//    // �������ݵ��豸������
//    HDC hMemDC = CreateCompatibleDC(hwindowDC);
//    HBITMAP hBitmap = CreateCompatibleBitmap(hwindowDC, x_size, y_size);
//    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
//
//    // ����Ļ��ȡͼ���ڴ��豸������
//    BitBlt(hMemDC, 0, 0, x_size, y_size, hwindowDC, x, y, SRCCOPY);
//
//    // ���ڴ��豸�����ĸ���ͼ��Mat����
//    GetDIBits(hMemDC, hBitmap, 0, x_size, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
//    // �ͷ���Դ
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
    int screen_width = GetSystemMetrics(SM_CXSCREEN); // ��ȡ��Ļ���
    int screen_height = GetSystemMetrics(SM_CYSCREEN); // ��ȡ��Ļ�߶�

    int x = (screen_width - x_size) / 2; // �������Ͻǵ� x ����
    int y = (screen_height - y_size) / 2; // �������Ͻǵ� y ����

    BITMAPINFOHEADER bi;
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = x_size;
    bi.biHeight = -y_size;  // ����ʹ��������µߵ�
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;


    // �������ݵ��豸������
    HDC hMemDC = CreateCompatibleDC(hwindowDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hwindowDC, x_size, y_size);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

    // ����Ļ��ȡͼ���ڴ��豸������
    BitBlt(hMemDC, 0, 0, x_size, y_size, hwindowDC, x, y, SRCCOPY);

    std::vector<BYTE> byteStream = this->BitmapToByteStream(hBitmap, hMemDC);
    // �ͷ���Դ
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
//    // ʹ�� OpenCV �� imdecode �������ֽ�������Ϊ Mat ����
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