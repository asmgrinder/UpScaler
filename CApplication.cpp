#include "CApplication.h"
#include "resource.h"

#define MB_TITLE L"UpScaler"

using namespace std;
using namespace Gdiplus;

CApplication::CApplication()
{
    mWindow = nullptr;
}

CApplication::~CApplication()
{
}

void CApplication::onInitDialog()
{
    HICON icon = LoadIcon(mInstance, IDI_APPLICATION);
    SendMessage(mWindow, WM_SETICON, ICON_SMALL, LPARAM(icon));
    SendMessage(mWindow, WM_SETICON, ICON_BIG, LPARAM(icon));

    // Initialize common controls.
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC   = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
    InitCommonControlsEx(&icex);
}

void CApplication::onOpen()
{
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = mWindow;
    ofn.lpstrFilter = L"All supported image files(*.bmp;*.png;*.jpg)\0*.bmp;*.png;*.jpg;*.jpeg\0\0";
    ofn.lpstrFile = mFileName;
    ofn.nMaxFile = ARRAYSIZE(mFileName);
    ofn.lpstrTitle = L"Open file";
    ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST;
    
    if (FALSE != GetOpenFileName(&ofn))
    {
        if (false != openImage(mFileName))
        {
            // redraw window
            InvalidateRect(mWindow, nullptr, TRUE);
        }
    }
}

int GetEncoderClsid(const wstring formatStr, CLSID *pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size != 0)
    {
        vector<char> buffer(size);
        ImageCodecInfo* pImageCodecInfo = reinterpret_cast<ImageCodecInfo *>(&buffer[0]);
        if (pImageCodecInfo != nullptr)
        {
            Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

            for (UINT j = 0; j < num; ++j)
            {
                if (pImageCodecInfo[j].MimeType == formatStr)
                {
                    *pClsid = pImageCodecInfo[j].Clsid;
                    return j;  // Success
                }
            }
        }
    }
    return -1;  // Failure
}

void CApplication::onSaveAs()
{
    OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = mWindow;
    ofn.lpstrFilter = L"Png files(*.png)\0*.png\0\0";
    ofn.lpstrFile = mFileName;
    ofn.nMaxFile = ARRAYSIZE(mFileName);
    ofn.lpstrTitle = L"Save file";
    ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"png";
    
    if (FALSE != GetSaveFileName(&ofn))
    {
        CLSID pngClsid;
        GetEncoderClsid(L"image/png", &pngClsid);
        mBigBitmap->Save(mFileName, &pngClsid, nullptr);
    }
}

void CApplication::onExit()
{
    DestroyWindow(mWindow);
}

void CApplication::doPaint(HDC hdc, const RECT &rect)
{
    Graphics graphics(hdc);
    DWORD colorRef = GetSysColor(COLOR_BTNFACE);
    Color color;
    color.SetFromCOLORREF(colorRef);
    graphics.Clear(color);
    if (nullptr != mBigBitmap)
    {
        float ratioX = float(rect.right - rect.left) / mBigBitmap->GetWidth();
        float ratioY = float(rect.bottom - rect.top) / mBigBitmap->GetHeight();
        float ratio = min(ratioX, ratioY);
        ratio = min(ratio, 1.f);
        graphics.DrawImage(mBigBitmap.get(),
                            rect.left,
                            rect.top,
                            int(ratio * mBigBitmap->GetWidth()),
                            int(ratio * mBigBitmap->GetHeight()));
    }
}

void CApplication::onEraseBkgnd(HDC hdc, const RECT &rect)
{
    doPaint(hdc, rect);
}

void CApplication::onPaint(const RECT &rect)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(mWindow, &ps);
    doPaint(hdc, rect);
    EndPaint(mWindow, &ps);
}

void CApplication::onSize()
{
    // redraw window
    InvalidateRect(mWindow, nullptr, TRUE);
}

bool CApplication::getBitmapData(Bitmap &Bmp, std::vector<ULONG> &Data)
{
    try
    {
        Data.resize(Bmp.GetWidth() * Bmp.GetHeight());
        Rect rectangle(0, 0, Bmp.GetWidth(), Bmp.GetHeight());
        BitmapData bitmapData;
        if (Ok == Bmp.LockBits(&rectangle, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData))
        {
            UINT* pixels = reinterpret_cast<UINT*>(bitmapData.Scan0);
            UINT lineOffset = 0;
            for (UINT row = 0; row < Bmp.GetHeight(); row ++)
            {
                for (UINT col = 0; col < Bmp.GetWidth(); col ++)
                {
                    Data[lineOffset + col] = pixels[col + row * bitmapData.Stride / sizeof(long)];
                }
                lineOffset += Bmp.GetWidth();
            }
            Bmp.UnlockBits(&bitmapData);
            return true;
        }
    }
    catch (...)
    {
    }
    return false;
}

float clamp(float Value, float Minimum, float Maximum)
{
    return min(max(Value, Minimum), Maximum);
}

ULONG GetPixel(int i, int j, int Width, int Height, const vector<ULONG> &Data)
{
    i >>= 1;
    j >>= 1;
    while (i < 0)
    {
        i += Width;
    }
    while (i >= Width)
    {
        i -= Width;
    }
    while (j < 0)
    {
        j += Height;
    }
    while (j >= Height)
    {
        j -= Height;
    }
    return Data[j * Width + i];
}

bool CApplication::openImage(const WCHAR *FilenameStr)
{
    if (nullptr == FilenameStr)
    {
        return false;
    }
    try
    {
        // create image from file
        Bitmap bitmap(FilenameStr);
        // get image data
        vector<ULONG> bmpData;
        if (false != getBitmapData(bitmap, bmpData))
        {
            mBigBitmap = make_shared<Bitmap>(bitmap.GetWidth() * 2, bitmap.GetHeight() * 2);
            vector<ULONG> niData(mBigBitmap->GetWidth() * mBigBitmap->GetHeight());
            float w1 = 0.1715728752538093f, w2 = sqrtf(2) * 0.5f * w1, w3 = 0.5f * w1;
//             float w1 = 0.1428571428571428f, w2 = 2 * 0.5f * w1, w3 = 0.5f * w1;
//             float w1 = 5/27.f, w3 = 0.5f * w1, w2 = 1.2f * w3;
            const float k[3][3] =
            {
                { w3, w2, w3 },
                { w2, w1, w2 },
                { w3, w2, w3 }
            };
            int width = bitmap.GetWidth();
            int height = bitmap.GetHeight();
            for (int j = 0; j < int(mBigBitmap->GetHeight()); j++)
            {
                for (int i = 0; i < int(mBigBitmap->GetWidth()); i++)
                {
                    float sum[4] = { 0, 0, 0, 0 };
                    for (int dj = -1; dj <= 1; dj++)
                    {
                        for (int di = -1; di <= 1; di++)
                        {
                            ULONG quad = GetPixel(i + di, j + dj, width, height, bmpData);
                            float _k = k[di + 1][dj + 1];
                            sum[0] += ((quad >>  0) & 0xff) * _k;
                            sum[1] += ((quad >>  8) & 0xff) * _k;
                            sum[2] += ((quad >> 16) & 0xff) * _k;
                            sum[3] += ((quad >> 24) & 0xff) * _k;
                        }
                    }
                    niData[j * mBigBitmap->GetWidth() + i] = (unsigned(clamp(sum[0], 0.f, 255.f)) <<  0)
                                                        | (unsigned(clamp(sum[1], 0.f, 255.f)) <<  8)
                                                        | (unsigned(clamp(sum[2], 0.f, 255.f)) << 16)
                                                        | (unsigned(clamp(sum[3], 0.f, 255.f)) << 24);
                }
            }
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    ULONG baseQuad = bmpData[j * width + i];
                    float baseVal[4] =
                    {
                        float((baseQuad >>  0) & 0xff),
                        float((baseQuad >>  8) & 0xff),
                        float((baseQuad >> 16) & 0xff),
                        float((baseQuad >> 24) & 0xff),
                    };
                    float avg[4] = { 0, 0, 0, 0 };
                    for (int dj = 0; dj <= 1; dj++)
                    {
                        for (int di = 0; di <= 1; di++)
                        {
                            int _j = j * 2 + dj;
                            int _i = i * 2 + di;
                            ULONG quad = niData[_j * mBigBitmap->GetWidth() + _i];
                            avg[0] += ((quad >>  0) & 0xff) * 0.25f;
                            avg[1] += ((quad >>  8) & 0xff) * 0.25f;
                            avg[2] += ((quad >> 16) & 0xff) * 0.25f;
                            avg[3] += ((quad >> 24) & 0xff) * 0.25f;
                        }
                    }
                    float mult[ARRAYSIZE(avg)];
                    for (unsigned q = 0; q < ARRAYSIZE(mult); q++)
                    {
                        mult[q] = 1.f;
                        if (0.f != avg[q])
                        {
                            mult[q] = baseVal[q] / avg[q];
                        }
                        mult[q] = (0.8f * 1.f + 0.2 * mult[q]);
                    }
                    for (int dj = 0; dj <= 1; dj++)
                    {
                        for (int di = 0; di <= 1; di++)
                        {
                            int _j = j * 2 + dj;
                            int _i = i * 2 + di;
                            ULONG quad = niData[_j * mBigBitmap->GetWidth() + _i];
                            float r = clamp(((quad >>  0) & 0xff) * mult[0], 0.f, 255.f);
                            float g = clamp(((quad >>  8) & 0xff) * mult[1], 0.f, 255.f);
                            float b = clamp(((quad >> 16) & 0xff) * mult[2], 0.f, 255.f);
                            float a = clamp(((quad >> 24) & 0xff) * mult[3], 0.f, 255.f);
                            niData[_j * mBigBitmap->GetWidth() + _i] = (unsigned(r) <<  0)
                                                                    | (unsigned(g) <<  8)
                                                                    | (unsigned(b) << 16)
                                                                    | (unsigned(a) << 24);
                        }
                    }
                }
            }
            Graphics graphics(mBigBitmap.get());
            Bitmap bmp(mBigBitmap->GetWidth(),
                    mBigBitmap->GetHeight(),
                    mBigBitmap->GetWidth() * sizeof(long),
                    PixelFormat32bppARGB,
                    reinterpret_cast<BYTE*>(&niData[0]));
            graphics.DrawImage(&bmp, 0, 0);
        }
        
        return true;
    }
    catch (...)
    {
        MessageBox(mWindow, L"Error opening or processing image file.", MB_TITLE, MB_OK);
    }
    
    return false;
}

INT_PTR CALLBACK CApplication::dialogProc(HWND Window, UINT Message, WPARAM WordParam, LPARAM LongParam)
{
    RECT rc;
    // get pointer to class instance from window
    CApplication *thisPtr = reinterpret_cast<CApplication *>(GetWindowLongPtr(Window, GWLP_USERDATA));
    switch (Message)
    {
        case WM_COMMAND:
            if (nullptr != thisPtr)
            {
                // recognize command identifier
                switch (LOWORD(WordParam))
                {
                    case IDM_OPEN:
                        thisPtr->onOpen();
                        break;
                    case IDM_SAVEAS:
                        thisPtr->onSaveAs();
                        break;
                    case IDM_EXIT:
                        thisPtr->onExit();
                        break;
                }
            }
            break;
            
        case WM_PAINT:
            GetClientRect(Window, &rc);
            if (nullptr != thisPtr)
            {
                thisPtr->onPaint(rc);
            }
            return TRUE;

        case WM_ERASEBKGND:
            GetClientRect(Window, &rc);
            if (nullptr != thisPtr)
            {
                thisPtr->onEraseBkgnd(reinterpret_cast<HDC>(WordParam), rc);
            }
            return FALSE;

        case WM_SIZE:
            if (nullptr != thisPtr)
            {
                thisPtr->onSize();
            }
            break;

        case WM_INITDIALOG:
            // pointer to class instance not attached yet
            thisPtr = reinterpret_cast<CApplication *>(LongParam);
            thisPtr->mWindow = Window;
            // attach it
            SetWindowLongPtr(Window, GWLP_USERDATA, LongParam);
            // call initialization code
            if (nullptr != thisPtr)
            {
                thisPtr->onInitDialog();
            }
            return TRUE;

        case WM_CLOSE:
            if (nullptr != thisPtr)
            {
                thisPtr->onExit();
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    // do default message handling
    return FALSE;
}

void CApplication::Run(HINSTANCE Instance)
{
    // initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    mInstance = Instance;
    // create main window
    if (0 == CreateDialogParam(Instance, MAKEINTRESOURCE(IDD_DLGMAIN), nullptr, dialogProc, LPARAM(this)))
    {
        reportLastError();
        return;
    }
    // message loop
    HACCEL accel = LoadAccelerators(Instance, MAKEINTRESOURCE(IDR_ACCELERATOR));
    MSG message;
    // retrieve message
    while (FALSE != GetMessage(&message, NULL, 0, 0))
    {
        // first of all translate keyboard accelerators
        if (FALSE == TranslateAccelerator(mWindow, accel, &message))
        {
            // then test if it's message for dialog
            if (FALSE == IsDialogMessage(mWindow, &message))
            {
                // translate & dispatch all other messages
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }
    // uninitialize GDI+
    GdiplusShutdown(gdiplusToken);
}

void CApplication::reportLastError()
{
    // convert error code to message and display it
    LPCWSTR message;
    if (0 != FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&message),
        0,
        nullptr))
    {
        MessageBox(nullptr, message, MB_TITLE, MB_OK);
    }
}

