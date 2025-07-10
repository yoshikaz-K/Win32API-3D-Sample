#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>

/* M_PI を使う*/
#define _USE_MATH_DEFINES

/* 数学用ヘッダ */
#include <math.h>

/* 時間計測　取得 */
#pragma comment(lib, "Winmm.lib")

/* ウィンドウサイズ */
#define MAX_WIDTH 640
#define MAX_HEIGHT 480

/* DIB */
static LPBYTE lpDIB;
static LPDWORD lpPixel;
static BITMAPINFO bmpInfo;

/* ウィンドウ　ハンドル */
HWND windowHandle;

/* モード管理　フラグ */
BOOL TitleScene = FALSE;
BOOL MainScene = FALSE;

/* モード */
void DrawTitleScene(HDC hdc);
void DrawMainScene();

/* ウィンドウ　プロシジャ */
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

/* 3次元位置　座標構造体 */
/* 透視投影用　構造体 */
typedef struct T3D
{
    float View_x = 100.0f;
    float View_y = 100.0f;
    float View_x2 = 80.0f;
    float View_y2 = 80.0f;
    float Screen_z = 5.0f;

    float z1 = 6.0f;
    float z2 = 8.0f;

    float x = 1.0f;
    float y = 1.0f;
    float z = 1.0f;

    float camera_x = 1.0f;
    float camera_y = 1.0f;
    float camera_z = 1.0f;

    /* 角度をラジアンに変換 */
    float angle = 15.0f;
    float deg = angle * M_PI / 180.0f;

} T3D;
T3D Point;

/* 内積　を返す関数 */
/* ベクトルの内積　*/
//static float Dot(Point3D a, Point3D b);
/* 外積　を返す関数 */
/* ベクトルの外積 */
//static Point3D Cross(Point3D a, Point3D b);
/* 回転 */
static void Rotate();
/* 透視投影 */
static void Pas_Trans();
/* 座標　を返す　関数？ (点を打つ関数）　*/
static void SetDrawingPoint(int x, int y, BYTE b, BYTE g, BYTE r);
/* FPS */
static void CallFps();
/* 線を描画する　関数 不完全 */
static void SetDrawingLine(unsigned int xs, unsigned int ys, unsigned int xe, unsigned int ye, BYTE b, BYTE g, BYTE r);
/* 画面クリア　関数　*/
static void ClearDrawingScreen();

/* タイトルモード */
void DrawTitleScene(HDC hdc)
{
    RECT rect = { 0 };

    rect.right = 420;
    rect.bottom = 200;
    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(255, 255, 255));
    DrawText(hdc, TEXT("3D like Sample Program by K."),
        -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}
/* メインモード */
void DrawMainScene()
{
    /* FPS */
    CallFps();
    /* シーンをクリアする */
    ClearDrawingScreen();
    /* 描画 */
    Pas_Trans();
}

/* 回転 */
static void Rotate()
{
   Point.View_y += (cos(Point.deg) * Point.y) - (sin(Point.deg) * Point.y);
   Point.z1 += (sin(Point.deg) * Point.z) + (cos(Point.deg) * Point.z);
   Point.z2 += (sin(Point.deg) * Point.z) + (cos(Point.deg) * Point.z);
   Point.View_x += 0.0f;
}

/* 透視投影 */
/*(立方体の描画)*/
static void Pas_Trans()
{
    //////////////////// 一個目の四角形 //////////////////////
           /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x, 
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y, 

               
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                0, 0, 255
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

              
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 0, 0
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

         
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                0, 255, 0
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

             
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 0, 255
            );

            /////////////// 二個目の四角形 ///////////////
            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x, 
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,

           
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                0, 0, 255
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,

              
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 0, 0
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,

               
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                0, 255, 0
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,

                
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 0, 255
            );

            ////////////////// 側面 ///////////////////
            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

               
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 255, 255
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

               
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 255, 255
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                200 + Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

               
                200 + Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 255, 255
            );

            /* 線分を描画する　関数 */
            SetDrawingLine(
                /* 式は　透視投影写像 */
                Point.View_x - (Point.z1 / Point.Screen_z) * Point.camera_x,
                200 + Point.View_y - (Point.z1 / Point.Screen_z) * Point.camera_y,

             
                Point.View_x2 - (Point.z2 / Point.Screen_z) * Point.camera_x,// + (cos(Point.deg) * Point.camera_x) - (sin(Point.deg) * Point.camera_y),
                200 + Point.View_y2 - (Point.z2 / Point.Screen_z) * Point.camera_y,// + (sin(Point.deg) * Point.camera_x) + (cos(Point.deg) * Point.camera_y),
                255, 255, 255
            );
}

/*　ベクトルの大きさ */
/**
static POINT3 Distance(POINT3 a, POINT3 b)
{
    d.x = a.x - b.x;
    d.y = a.y - b.y;
    d.z = a.z - b.z;
    POINT3 result = { sqrt(d.x * d.x + d.y * d.y + d.z * d.z) };
    return result;
}
/* ベクトルの内積　*/
/*
static float Dot(Point3D a, Point3D b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
*/
/* ベクトルの外積 */
/**
static Point3D Cross(Point3D a, Point3D b)
{
    Point3D c = {};

    c.x = a.y * b.z - a.z * b.y;
    c.y = a.z * b.x - a.x * b.z;
    c.z = a.x * b.y - a.y * b.x;

    return c;
}
*/
/* 座標（点）を返す　関数　*/
static void SetDrawingPoint(int x, int y, BYTE b, BYTE g, BYTE r)
{
            if ((x >= 0 && x < MAX_WIDTH) && (y >= 0 && y < MAX_HEIGHT))
            {
                if (lpPixel != NULL)
                    lpPixel[x + y * MAX_WIDTH] = RGB(b, g, r);
            }
}
/* 線を描画する　関数 不完全 */
static void SetDrawingLine(unsigned int xs, unsigned int ys, unsigned int xe, unsigned int ye, BYTE b, BYTE g, BYTE r)
{
    unsigned int x, y;
    int dx, dy;
    float rad;
    unsigned int length;
    unsigned int l;

    dx = xe - xs;
    dy = ye - ys;

    length = (double)sqrt(dx * dx + dy * dy);

    rad = (double)atan2(dy, dx);

    for (l = 0; l < length; l++)
    {
        x = xs + l * (double)cos(rad);
        y = ys + l * (double)sin(rad);

        /* 点を描画　*/
        SetDrawingPoint(
            x, 
            y,
            b, g, r
        );
    }
}
/* 画面クリア　関数 */
static void ClearDrawingScreen()
{
    if (lpPixel != NULL)
    {
        for (int i = 0; i < MAX_HEIGHT; ++i)
        {
            for (int j = 0; j < MAX_WIDTH; ++j)
            {
                    lpPixel[j + i * MAX_WIDTH] = 0x00000000;
            }
        }
    }
}
/* FPS */
static void CallFps()
{
    HDC hdc = GetDC(windowHandle);

    static TCHAR str[255];
    static DWORD frames = 0;
    static DWORD before = 0;
    DWORD now, progress;
    static DWORD fps = 60;

    if (before == 0) before = timeGetTime();

    now = timeGetTime();

    progress = now - before;

    if ((frames * (1000.0 / fps)) > progress)
    {
        Sleep((frames * (1000.0 / fps)) - progress);
    }
    if (progress >= 1000)
    {
        before = now;
        frames = 0;
    }
    frames++;

    SetBkColor(hdc, RGB(0, 0, 0));
    SetTextColor(hdc, RGB(255, 255, 255));
    wsprintf(str, TEXT("FPS=:%d"), frames);
    TextOut(hdc, 5, 5, str, lstrlen(str));

    ReleaseDC(windowHandle, hdc);
}
//Entry point.
int main(void)
{
    HINSTANCE hInst = GetModuleHandle(nullptr);

    WNDCLASSEX w = {};
    w.cbSize = sizeof(WNDCLASSEX);
    w.lpfnWndProc = WindowProcedure;
    w.lpszClassName = _T("3D Test");
    w.hInstance = GetModuleHandle(0);
    w.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassEx(&w);

    RECT wrc = { 0,0, MAX_WIDTH, MAX_HEIGHT };
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

    windowHandle = CreateWindow(
        w.lpszClassName,
        _T("3D Sample Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wrc.right - wrc.left,
        wrc.bottom - wrc.top,
        nullptr,
        nullptr,
        w.hInstance,
        nullptr);

    ShowWindow(windowHandle, SW_SHOW);
    //UpdateWindow(windowHandle);

    MSG msg = {};

    /* メインループ */
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            /* メインモードのとき　メインモードを描画*/
            if (MainScene) DrawMainScene();
        }
    }
    //ウィンドウクラス　解放
    UnregisterClass(w.lpszClassName, w.hInstance);

    return 0;
}
//ウィンドウ プロシジャ
LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;

    /* 準備-取得メッセージ */
    if (msg == WM_CREATE)
    {
        /* 最初は タイトルシーン */
        TitleScene = TRUE;

            /* DIB 作成 */
            lpPixel = (LPDWORD)HeapAlloc(GetProcessHeap(), 
                HEAP_ZERO_MEMORY, 
                (static_cast<SIZE_T>(MAX_WIDTH)) * MAX_HEIGHT * 4);

            bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmpInfo.bmiHeader.biWidth = MAX_WIDTH;
            bmpInfo.bmiHeader.biHeight = -MAX_HEIGHT;
            bmpInfo.bmiHeader.biPlanes = 1;
            bmpInfo.bmiHeader.biBitCount = 32;
            bmpInfo.bmiHeader.biCompression = BI_RGB;

        return 0;
    }
    /* 解放メッセージ */
    if (msg == WM_DESTROY)
    {
        HeapFree(GetProcessHeap(), 0, lpPixel);

        PostQuitMessage(0);
        return 0;
    }
    /* 描画メッセージ */
    if (msg == WM_PAINT)
    {
        GetClientRect(hwnd, &rect);

        hdc = BeginPaint(hwnd, &ps);

        /* モード管理　描画 */
        if (TitleScene) DrawTitleScene(hdc);
        else if (MainScene) DrawMainScene();

        /* メインシーンのとき 描画 */
        if (MainScene == TRUE)
        {
            StretchDIBits(
                hdc,
                0, 0,
                MAX_WIDTH,
                MAX_HEIGHT,
                0, 0,
                MAX_WIDTH,
                MAX_HEIGHT,
                lpPixel,
                &bmpInfo,
                DIB_RGB_COLORS,
                SRCCOPY
            );
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    /* 左クリックされたら・・ */
    if (msg == WM_LBUTTONUP)
    {
        /* タイトルシーンのとき */
        if (TitleScene)
        {
            /* タイトルモードを消し、メインモードを描画 */
            TitleScene = FALSE;
            MainScene = TRUE;
        }
        /* 再描画 */
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    /* キー入力 */
    if (msg == WM_KEYDOWN)
    {
        switch (wparam)
        {
        case VK_RIGHT:
            Point.camera_x += 2.0f;
            break;
        case VK_LEFT:
            Point.camera_x -= 2.0f;
            break;
        case VK_UP:
            Point.View_y -= 1.0f;
            Point.z1 -= 1.0f;
            Point.z2 -= 1.0f;
            break;
        case VK_DOWN:
            Point.View_y += 1.0f;
            Point.z1 += 1.0f;
            Point.z2 += 1.0f;
            break;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wparam, lparam);
}
