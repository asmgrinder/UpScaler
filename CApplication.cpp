#include "CApplication.h"
#include "resource.h"

// #define IDM_NEW		101
// #define IDM_OPEN	102

#define MB_TITLE L"oRGB"

// #define ALPHA (1.75f)	// color strength multiplier

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

//     const int ImageListID    = 0;
//     const int numButtons     = 2;
//     const int bitmapSize     = 16;
//     
//     const DWORD buttonStyles = BTNS_AUTOSIZE;
// 
//     // Create the toolbar.
//     HWND hwndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, 
//                                       WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, 
//                                       mWindow, NULL, mInstance, NULL);
//     if (hwndToolbar != NULL)
//     {
// 		// Create the image list.
// 	    mhImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
// 	                                    ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
// 	                                    numButtons, 0);
// 
// 	    // Set the image list.
// 	    SendMessage(hwndToolbar, TB_SETIMAGELIST, 
// 	                WPARAM(ImageListID), 
// 	                LPARAM(mhImageList));
// 
// 	    // Load the button images.
// 	    SendMessage(hwndToolbar, TB_LOADIMAGES, 
// 	                WPARAM(IDB_STD_SMALL_COLOR), 
// 	                LPARAM(HINST_COMMCTRL));
// 
// 	    // Initialize button info.
// 	    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.
// 	    
// 	    TBBUTTON tbButtons[numButtons] = 
// 	    {
// 	        { MAKELONG(STD_FILENEW,  ImageListID), IDM_NEW,  TBSTATE_ENABLED, buttonStyles, {0}, 0, reinterpret_cast<INT_PTR>(L"New") },
// 	        { MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, {0}, 0, reinterpret_cast<INT_PTR>(L"Open")},
// 	        //{ MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0,               buttonStyles, {0}, 0, (INT_PTR)L"Save"}
// 	    };
// 
// 	    // Add buttons.
// 	    SendMessage(hwndToolbar, TB_BUTTONSTRUCTSIZE, WPARAM(sizeof(TBBUTTON)), 0);
// 	    SendMessage(hwndToolbar, TB_ADDBUTTONS, WPARAM(numButtons), reinterpret_cast<LPARAM>(&tbButtons));
// 
// 	    // Resize the toolbar, and then show it.
// 	    SendMessage(hwndToolbar, TB_AUTOSIZE, 0, 0); 
// 	    //ShowWindow(hwndToolbar,  TRUE);
// 
// 		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         // Create the rebar.
// 	    auto hwndRebar = mRebar = CreateWindowEx(WS_EX_TOOLWINDOW,
//                                                  REBARCLASSNAME,
//                                                  NULL,
//                                                  WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
//                                                  WS_CLIPCHILDREN | RBS_VARHEIGHT |
//                                                  CCS_NODIVIDER | RBS_BANDBORDERS,
//                                                  0, 0, 0, 0,
//                                                  mWindow,
//                                                  NULL,
//                                                  mInstance, // global instance handle
//                                                  NULL);
// 
// 	    if (nullptr != hwndRebar)
// 	    {
// 		    // Initialize band info used by both bands.
// 		    REBARBANDINFO rbBand;
//             rbBand.cbSize = sizeof(REBARBANDINFO);
// 		    rbBand.fMask  = 
// 		          RBBIM_STYLE       // fStyle is valid.
// 		        | RBBIM_TEXT        // lpText is valid.
// 		        | RBBIM_CHILD       // hwndChild is valid.
// 		        | RBBIM_CHILDSIZE   // child size members are valid.
// 		        | RBBIM_SIZE;       // cx is valid
// 		    rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;
// 		
// 		    // Get the height of the toolbar.
// 		    DWORD dwBtnSize = static_cast<DWORD>(SendMessage(hwndToolbar, TB_GETBUTTONSIZE, 0, 0));
// 		
// 		    // Set values unique to the band with the toolbar.
// 		    rbBand.lpText = TEXT("");
// 		    rbBand.hwndChild = hwndToolbar;
// 		    rbBand.cyChild = LOWORD(dwBtnSize);
// 		    rbBand.cxMinChild = numButtons * HIWORD(dwBtnSize);
// 		    rbBand.cyMinChild = LOWORD(dwBtnSize);
// 		    // The default width is the width of the buttons.
// 		    rbBand.cx = 0;
// 		
// 		    // Add the band that has the toolbar.
// 		    SendMessage(hwndRebar, RB_INSERTBAND, WPARAM(-1), reinterpret_cast<LPARAM>(&rbBand));
// 
// 		    // Set values unique to the band with the combo box.
// //		    RECT rc;
// //		    GetWindowRect(hwndCombo, &rc);
// //		    rbBand.lpText = TEXT("Font");
// //		    rbBand.hwndChild = hwndCombo;
// //		    rbBand.cxMinChild = 0;
// //		    rbBand.cyMinChild = rc.bottom - rc.top;
// //		    // The default width should be set to some value wider than the text. The combo 
// //		    // box itself will expand to fill the band.
// //		    rbBand.cx = 100;
// //		
// //		    // Add the band that has the combo box.
// //		    SendMessage(hwndRebar, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
// 	 	}
// 	}

	openImage(L"uqm_orig.png");
}

void CApplication::onOpen()
{
	WCHAR filename[MAX_PATH] = L"";
	OPENFILENAME ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = mWindow;
	ofn.lpstrFilter = L"All supported image files(*.bmp;*.png;*.jpg)\0*.bmp;*.png;*.jpg\0\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = ARRAYSIZE(filename);
	ofn.lpstrTitle = L"Open file";
	ofn.Flags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST;
	
	if (FALSE != GetOpenFileName(&ofn))
	{
		if (false != openImage(filename))
		{
			// redraw window
			InvalidateRect(mWindow, nullptr, TRUE);
		}
	}
}

void CApplication::onExit()
{
//	EndDialog(mWindow, IDOK);
//	PostQuitMessage(0);
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
            const float k[3][3] =
            {
                { 0.0714285714285714f, 0.1428571428571428f, 0.0714285714285714f },
                { 0.1428571428571428f, 0.1428571428571428f, 0.1428571428571428f },
                { 0.0714285714285714f, 0.1428571428571428f, 0.0714285714285714f }
            };
            for (int j = 0; j < int(mBigBitmap->GetHeight()); j ++)
            {
                for (int i = 0; i < int(mBigBitmap->GetWidth()); i ++)
                {
                    float sum[4] = { 0, 0, 0, 0 };
                    for (int dj = -1; dj <= 1; dj++)
                    {
                        for (int di = -1; di <= 1; di++)
                        {
                            ULONG quad = GetPixel(i + di, j + dj, bitmap.GetWidth(), bitmap.GetHeight(), bmpData);
                            float _k = k[di + 1][dj + 1];
                            sum[0] += ((quad >> 0) & 0xff) * _k;
                            sum[1] += ((quad >> 8) & 0xff) * _k;
                            sum[2] += ((quad >> 16) & 0xff) * _k;
                            sum[3] += ((quad >> 24) & 0xff) * _k;
                        }
                    }
                    niData[j * mBigBitmap->GetWidth() + i] = unsigned(sum[0])
                                                            | (unsigned(sum[1]) << 8)
                                                            | (unsigned(sum[2]) << 16)
                                                            | (unsigned(sum[3]) << 24);
//                     niData[j * mBigBitmap->GetWidth() + i] = bmpData[(j >> 1) * bitmap.GetWidth() + (i >> 1)];
                }
            }
            Graphics graphics(mBigBitmap.get());
            Bitmap bmp(mBigBitmap->GetWidth(),
                       mBigBitmap->GetHeight(),
                       mBigBitmap->GetWidth() * sizeof(long),
                       PixelFormat32bppARGB,
                       reinterpret_cast<BYTE*>(&niData[0]));
            graphics.DrawImage(&bmp, 0, 0);
// 			CoRGB oRgbSrc(bitmap.GetWidth(), bitmap.GetHeight(), &bmpData[0]);
// 
// 			const UINT rows = 3, columns = 3;
// 			mBigBitmap = make_shared<Bitmap>(bitmap.GetWidth() * columns, bitmap.GetHeight() * rows);
// 			Graphics graphics(mBigBitmap.get());
// 		    for (UINT row = 0; row < rows; row++)
// 		    {
// 		        for (UINT column = 0; column < columns; column++)
// 		        {
// 					CoRGB oRgbColored = oRgbSrc;
// 		            oRgbColored.ScaleColor(CoRGB::CF_C1_BLUEYELLOW, pow(ALPHA, column) / ALPHA);
// 		            oRgbColored.ScaleColor(CoRGB::CF_C2_GREENRED, ALPHA / pow(ALPHA, row));
// 		            vector<ULONG> imageData;
// 		            if (false != oRgbColored.GetImageData(imageData))
// 		            {
// 		            	Bitmap bmp(oRgbColored.GetWidth(),
// 									oRgbColored.GetHeight(),
// 									oRgbColored.GetWidth() * sizeof(ULONG),
// 									PixelFormat32bppARGB,
// 									reinterpret_cast<BYTE*>(&imageData[0]));
// 						graphics.DrawImage(&bmp, int(column * bmp.GetWidth()), int(row * bmp.GetHeight()));
// 					}
// 		        }
// 		    }
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

