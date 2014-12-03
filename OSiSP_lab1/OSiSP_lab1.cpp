// OSiSP_lab1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "OSiSP_lab1.h"

#define MAX_LOADSTRING 100

// Global Variables:
HDC     winDC, //контекст окна
		hMemoryDC,
		cwinDC; //контекст в памяти (копия окна) - прослойка между ресурсными контекстами и окном - на нем будем рисовать всё
HWND hWnd;
HBITMAP bmpMem;
HDC tempDC;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
void				SaveBMP();
void				CaptureScreen(HWND);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_OSISP_LAB1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	HDC hdcRef = GetDC(hWnd); 

	int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE); 
	int iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE); 
	int iWidthPels = GetDeviceCaps(hdcRef, HORZRES); 
	int iHeightPels = GetDeviceCaps(hdcRef, VERTRES);
	int realHeight =iHeightPels -  iHeightMM;
	int realWindth = iWidthPels -  iWidthMM;
	bmpMem = CreateCompatibleBitmap (GetDC(hWnd), realWindth, realHeight);
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_OSISP_LAB1));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_OSISP_LAB1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OSISP_LAB1);
	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
 //  hMemoryDC = CreateCompatibleDC(hScreenDC); // buffer
	//if(hMemoryDC == NULL){
	//	ReleaseDC(hw, hScreenDC);
	//	return false;
	//}	

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
HPEN hPen = NULL;
BOOL fDraw= FALSE;
POINT ptPrevious = {0};
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	cwinDC = GetDC(hWnd);
	static PAINTSTRUCT paintStruct;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);
	switch (message)
	{
	case WM_INITDIALOG:
       hPen = CreatePen(PS_SOLID, 3, RGB(128, 0, 0));
        //bRet = TRUE;
        break;
	case WM_COMMAND:
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_SAVE:
			//tempDC = GetDC(hWnd);//получаем все
			//bmpMem = CreateCompatibleBitmap (tempDC, 800, 600);
			//ReleaseDC (hWnd, tempDC); //Отпускаем контекст окна
			//SaveBMP();
			CaptureScreen(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//winDC = BeginPaint (hWnd, &paintStruct);
			StretchBlt (
				hdc, 
				paintStruct.rcPaint.left, 
				paintStruct.rcPaint.top, 
				paintStruct.rcPaint.right, 
				paintStruct.rcPaint.bottom, 
				cwinDC, 
				paintStruct.rcPaint.left, 
				paintStruct.rcPaint.top, 
				paintStruct.rcPaint.right, 
				paintStruct.rcPaint.bottom,
				SRCCOPY
			);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN: 
        fDraw = TRUE; 
        ptPrevious.x = LOWORD(lParam); 
        ptPrevious.y = HIWORD(lParam); 
        break;
    case WM_LBUTTONUP: 
        if(fDraw) 
        { 
            hdc = GetDC(hWnd); 
			SelectObject(hdc,hPen);
            MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL); 
            LineTo(hdc, LOWORD(lParam), HIWORD(lParam)); 
            ReleaseDC(hWnd, hdc);
            fDraw = FALSE; 
        } 
        break;
    case WM_MOUSEMOVE: 
        if (fDraw) 
        { 
            hdc = GetDC(hWnd); 
			SelectObject(hdc,hPen);
            MoveToEx(hdc, ptPrevious.x, ptPrevious.y, NULL); 
            LineTo  
            (
                hdc, 
                ptPrevious.x = LOWORD(lParam), 
                ptPrevious.y = HIWORD(lParam)
            ); 
            ReleaseDC(hWnd, hdc); 
        } 
        break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//void SaveBMP () {
//	BITMAPFILEHEADER bfh; //структура с заголовком файла
//	BITMAPINFOHEADER bih;	//структура с описанием формата картинки
//	BITMAPINFO bi;	//описание формата для функции GetDIBits ()
//	void *pdwBits = NULL; //указатель на буфер с будущим рисунком
//	DWORD dwWB = 0; //переменная для хранения записанных байт в файл
//	HANDLE hFile = NULL; //хэндл открытого файла
//	HDC hdcRef = GetDC(hWnd); 
//
//	int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE); 
//	int iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE); 
//	int iWidthPels = GetDeviceCaps(hdcRef, HORZRES); 
//	int iHeightPels = GetDeviceCaps(hdcRef, VERTRES);
//	int realHeight =iHeightPels -  iHeightMM;
//	int realWindth = iWidthPels -  iWidthMM;
//	//Создаем пустой буфер, в который потом скопируется картинка
//	pdwBits = (void *)GlobalAlloc (GMEM_FIXED, realWindth * realHeight * 4);
//	
//	//Обнуляем структуры
//	memset (&bfh, 0, sizeof (BITMAPFILEHEADER));
//	memset (&bih, 0, sizeof (BITMAPINFOHEADER));
//	memset (&bi, 0, sizeof (BITMAPINFO));
//	
//	//Заполняем заголовочную структуру
//	bfh.bfType = 0x4D42;                           // Обозначим, что это bmp 'BM'
//	bfh.bfOffBits = sizeof(bfh) + sizeof(bih);    
//	bfh.bfSize = bfh.bfOffBits + realWindth * realHeight * 4;   // Посчитаем размер конечного файла
//	
//	//Заполняем описание картинки
//	bih.biSize = sizeof(bih);                      // Так положено
//	bih.biBitCount = 32;                           // 16 бит на пиксель
//	bih.biCompression = BI_RGB;                    // Без сжатия
//	bih.biHeight = realHeight;
//	bih.biWidth = realWindth;
//	bih.biPlanes = 1; 
//	
//	//второй тип инфы
//	//третяя попытка - делаем скрин битмапа если это ху*** не хочет записывать битмап, мразь
//	BYTE* memory;
//	BITMAPINFO info;
//    info.bmiHeader = bih;
//    HDC winDC = GetWindowDC(hWnd);
//    HDC bmpDC = CreateCompatibleDC(winDC);
//    //Создаем битмэп
//    HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, NULL, 0);
//    SelectObject(bmpDC, bitmap);//Выбираем в контекст битмэп
//    BitBlt(bmpDC, 0, 0, realHeight, realWindth, winDC, 0, 0, SRCCOPY);
//    ReleaseDC(hWnd, winDC);
//	RECT rect;
//	RECT lprect;
//	GetClientRect(hWnd, &lprect);
//	
//	//Копируем в структуру BITMAPINFO
//	CopyMemory (&bi, &bih, sizeof (BITMAPINFOHEADER));   
//	HDC memDC = CreateCompatibleDC(GetDC(hWnd));
//	//Получаем с помощью функции саму картинку из контекста в памяти
//	GetDIBits (cwinDC, bmpMem, 0, realHeight, pdwBits, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
//	
//	//Создаем файл для записи
//	hFile = CreateFile ( _T("E:\\save.bmp"), 
//											GENERIC_WRITE, 
//											0,
//											NULL,
//											CREATE_ALWAYS,
//											FILE_ATTRIBUTE_NORMAL,
//											(HANDLE)NULL);
//	if (hFile == INVALID_HANDLE_VALUE) {
//			if (pdwBits) GlobalFree (pdwBits);
//			//SetWindowText (hWnd, "Ошибка при открытии файла");
//			return;
//		}
//	
//	//Записываем структуру с заголовком файла
//	if (!WriteFile (hFile, (void *)&bfh, sizeof (BITMAPFILEHEADER), &dwWB, NULL)) {
//		//SetWindowText (hWnd, "Ошибка при записи BITMAPFILEHEADER");
//		if (pdwBits) GlobalFree (pdwBits);
//		return;
//	}
//	
//	//Записываем структуру с описанием формата картинки
//	if (!WriteFile (hFile, (void *)&bih, sizeof (BITMAPINFOHEADER), &dwWB, NULL)) {
//		//SetWindowText (hWnd, "Ошибка при записи BITMAPINFOHEADER");
//		if (pdwBits) GlobalFree (pdwBits);
//		return;
//	}	
//	
//	//Записываем собсна саму картинку после двух предыдущих структур
//	//if (! WriteFile(hFile, memory, bih., &dwWB, NULL)) {
//	//	SetWindowText (hWnd, "Ошибка при записи DIBits");
//		if (pdwBits) GlobalFree (pdwBits);
//		return;
//	}	
//	
//	//Закрываем хэндл файла и очищаем буфер
//	CloseHandle (hFile);
//	//if (pdwBits) GlobalFree (pdwBits);
//	return;
//}

void CaptureScreen(HWND window)
{
    //Получаем прямоугольную область экрана
    RECT windowRect;
    //GetWindowRect(window, &windowRect);
    GetClientRect(window, &windowRect); // Это изменяет только коордианаты правого угла =(
 
 	HDC hdcRef = GetDC(hWnd); 

	int iWidthMM = GetDeviceCaps(hdcRef, HORZSIZE); 
	int iHeightMM = GetDeviceCaps(hdcRef, VERTSIZE); 
	int iWidthPels = GetDeviceCaps(hdcRef, HORZRES); 
	int iHeightPels = GetDeviceCaps(hdcRef, VERTRES);
	int realHeight =iHeightPels -  iHeightMM;
	int realWindth = iWidthPels -  iWidthMM;
    //Размеры битмэпа
    int bitmap_dx = windowRect.right-windowRect.left;
    int bitmap_dy = windowRect.bottom-windowRect.top;
 
    BITMAPINFOHEADER bmpInfoHeader;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAP* pBitmap;
 
    bmpFileHeader.bfType = 0x4d42;
    bmpFileHeader.bfSize = 0;
    bmpFileHeader.bfReserved1 = 0;
    bmpFileHeader.bfReserved2 = 0;
    bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
 
    bmpInfoHeader.biSize = sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth = bitmap_dx;
    bmpInfoHeader.biHeight = bitmap_dy;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 32;
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = bitmap_dx*bitmap_dy*(32/8);
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;
 
    BITMAPINFO info;
    info.bmiHeader = bmpInfoHeader;
 
    BYTE* memory;
    HDC winDC = GetWindowDC(window);
    HDC bmpDC = CreateCompatibleDC(winDC);
    //Создаем битмэп
    HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, NULL, 0);
    SelectObject(bmpDC, bitmap);//Выбираем в контекст битмэп
    BitBlt(bmpDC, -8, -50, bitmap_dx+16, bitmap_dy+50, winDC, 0, 0, SRCCOPY);
    ReleaseDC(window, winDC);
 
//    OPENFILENAME ofn;//Указатель на структуру с данными инициализации диалогового окна
    //char strFilter[] = "Файлы данных *.bmp\0";
    //char strFileName[MAX_PATH] = "";
    //memset(&ofn, 0, sizeof(OPENFILENAME));//Обнуление ofn
    //ofn.lStructSize = sizeof(OPENFILENAME);
    //ofn.hwndOwner = window;
    //ofn.lpstrFilter = strFilter;
    //ofn.lpstrFile = strFileName;//Буфер для имени файла
    //ofn.nMaxFile = MAX_PATH;//Размер файла
    //ofn.lpstrInitialDir = NULL;
    //ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
    //strcpy(strFileName, ofn.lpstrFile);
    //GetSaveFileName(&ofn); //MessageBox(hwnd,"Невозможно сохранить файл", "О программе...",MB_ICONINFORMATION);
 
    HANDLE hFile = CreateFile(
        _T("E:\\save.bmp"), GENERIC_WRITE, 
        0, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,NULL);
 
    if (hFile == INVALID_HANDLE_VALUE) 
        return;
 
    DWORD dwWritten = 0;
    WriteFile(hFile, &bmpFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, &bmpInfoHeader, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, memory, bmpInfoHeader.biSizeImage, &dwWritten, NULL);
    CloseHandle(hFile);
}