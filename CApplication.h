#ifndef CAPPLICATION_H
#define CAPPLICATION_H
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <new>
#include <vector>
#include <commctrl.h>
// #include "CoRGB.h"

class CApplication
{
	public:
		CApplication();
		~CApplication();
		void Run(HINSTANCE Instance);
	protected:
		void onInitDialog();
		void onOpen();
		void onExit();
		void onPaint(const RECT &rect);
		void onEraseBkgnd(HDC hdc, const RECT &rect);
		void doPaint(HDC hdc, const RECT &rect);
		void onSize();
		void reportLastError();
		bool openImage(const WCHAR *FilenameStr);
		static bool getBitmapData(Gdiplus::Bitmap &Bmp, std::vector<ULONG> &Data);
		static INT_PTR CALLBACK dialogProc(HWND Window, UINT Message, WPARAM WordParam, LPARAM LongParam);

		HWND mWindow, mRebar;
		HINSTANCE mInstance;
		HIMAGELIST mhImageList;
		std::shared_ptr<Gdiplus::Bitmap> mBigBitmap;
};

#endif
