#include <windows.h>
#include <cmath>
#include <vector>

#define ID_IMAGE_DRAW   901
#define ID_TRANS_COLOR  902
#define ID_TRANS_MOVE   903 
#define ID_INFO_ABOUT   904
#define ID_INFO_EXIT    905

static bool isDrawn = false;        
static int redVal = 135;            
static int greenVal = 206;          
static int blueVal = 235;          
static int offsetX = 0;
static int offsetY = 0;

class Sun {
public:
    void show(HDC hdc, int x, int y, int radius = 40) {
        HBRUSH brush = CreateSolidBrush(RGB(255, 220, 0));
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(230, 180, 0));
        HGDIOBJ oldBrush = SelectObject(hdc, brush);
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        for (int i = 0; i < 8; ++i) {
            double angle = i * 3.14159 / 4.0;
            int x1 = x + (int)((radius + 5) * cos(angle));
            int y1 = y + (int)((radius + 5) * sin(angle));
            int x2 = x + (int)((radius + 20) * cos(angle));
            int y2 = y + (int)((radius + 20) * sin(angle));
            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
        }

        Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
};

class Cloud {
public:
    void show(HDC hdc, int x, int y) {
        HBRUSH brush = CreateSolidBrush(RGB(240, 248, 255));
        HPEN pen = CreatePen(PS_SOLID, 1, RGB(180, 200, 220));
        HGDIOBJ oldBrush = SelectObject(hdc, brush);
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        Ellipse(hdc, x, y, x + 60, y + 40);
        Ellipse(hdc, x + 25, y - 15, x + 85, y + 35);
        Ellipse(hdc, x + 50, y, x + 110, y + 40);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
};

class Flower {
public:
    void show(HDC hdc, int x, int y) {
        HPEN stemPen = CreatePen(PS_SOLID, 3, RGB(34, 139, 34));
        HGDIOBJ oldPen = SelectObject(hdc, stemPen);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x, y + 50);

        HBRUSH petalBrush = CreateSolidBrush(RGB(255, 105, 180));
        HGDIOBJ oldBrush = SelectObject(hdc, petalBrush);

        Ellipse(hdc, x - 18, y - 8, x - 2, y + 8);
        Ellipse(hdc, x + 2, y - 8, x + 18, y + 8);
        Ellipse(hdc, x - 8, y - 18, x + 8, y - 2);
        Ellipse(hdc, x - 8, y + 2, x + 8, y + 18);

        HBRUSH coreBrush = CreateSolidBrush(RGB(255, 215, 0));
        SelectObject(hdc, coreBrush);
        Ellipse(hdc, x - 7, y - 7, x + 7, y + 7);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(stemPen);
        DeleteObject(petalBrush);
        DeleteObject(coreBrush);
    }
};

class House {
public:
    void show(HDC hdc, int x, int y) {
        HBRUSH wallBrush = CreateSolidBrush(RGB(222, 184, 135));
        HGDIOBJ oldBrush = SelectObject(hdc, wallBrush);
        Rectangle(hdc, x, y, x + 140, y + 120);

        HBRUSH roofBrush = CreateSolidBrush(RGB(200, 50, 50));
        SelectObject(hdc, roofBrush);
        POINT roofPoints[3] = {
            { x - 10, y },
            { x + 150, y },
            { x + 70, y - 60 }
        };
        Polygon(hdc, roofPoints, 3);

        HBRUSH doorBrush = CreateSolidBrush(RGB(101, 67, 33));
        SelectObject(hdc, doorBrush);
        Rectangle(hdc, x + 15, y + 50, x + 50, y + 120);

        HBRUSH winBrush = CreateSolidBrush(RGB(173, 216, 230));
        SelectObject(hdc, winBrush);
        Rectangle(hdc, x + 75, y + 30, x + 120, y + 70);

        SelectObject(hdc, oldBrush);
        DeleteObject(wallBrush);
        DeleteObject(roofBrush);
        DeleteObject(doorBrush);
        DeleteObject(winBrush);
    }
};

void DrawScene(HDC hdc, int width, int height) {
    // Динамічний колір неба на основі поточних змінних
    HBRUSH skyBrush = CreateSolidBrush(RGB(redVal, greenVal, blueVal));
    HGDIOBJ oldBrush = SelectObject(hdc, skyBrush);
    Rectangle(hdc, 0, 0, width, (int)(height * 0.65));

    HBRUSH grassBrush = CreateSolidBrush(RGB(50, 205, 50));
    SelectObject(hdc, grassBrush);
    Rectangle(hdc, 0, (int)(height * 0.65), width, height);

    SelectObject(hdc, oldBrush);
    DeleteObject(skyBrush);
    DeleteObject(grassBrush);

    // Малюємо композицію тільки тоді, коли користувач обрав пункт меню "Draw..."
    if (isDrawn) {
        Sun sun;
        sun.show(hdc, width - 100 + offsetX, 90 + offsetY, 45);

        Cloud cloud1, cloud2, cloud3;
        cloud1.show(hdc, 60 + offsetX, 50 + offsetY);
        cloud2.show(hdc, 250 + offsetX, 80 + offsetY);
        cloud3.show(hdc, 480 + offsetX, 40 + offsetY);

        House house;
        house.show(hdc, 150 + offsetX, (int)(height * 0.65) - 120 + offsetY);

        Flower f1, f2, f3, f4;
        f1.show(hdc, 360 + offsetX, (int)(height * 0.65) + 40 + offsetY);
        f2.show(hdc, 430 + offsetX, (int)(height * 0.65) + 60 + offsetY);
        f3.show(hdc, 500 + offsetX, (int)(height * 0.65) + 30 + offsetY);
        f4.show(hdc, 580 + offsetX, (int)(height * 0.65) + 50 + offsetY);
    }
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Lab3Task2WindowClass";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc)) return 0;

    HMENU hMenu = CreateMenu();
    HMENU hSubImage = CreatePopupMenu();
    HMENU hSubTrans = CreatePopupMenu();
    HMENU hSubInfo = CreatePopupMenu();

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubImage, L"Image");
    AppendMenuW(hSubImage, MF_STRING, ID_IMAGE_DRAW, L"Draw...");

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubTrans, L"Transformations");
    AppendMenuW(hSubTrans, MF_STRING, ID_TRANS_COLOR, L"Change Color");
    AppendMenuW(hSubTrans, MF_STRING, ID_TRANS_MOVE, L"Move");

    AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hSubInfo, L"Information");
    AppendMenuW(hSubInfo, MF_STRING, ID_INFO_ABOUT, L"About");
    AppendMenuW(hSubInfo, MF_STRING, ID_INFO_EXIT, L"Exit");

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, L"Laboratory Work #3 - Task 2 (Variant 1: Movement)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 950, 650,
        NULL, hMenu, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case ID_IMAGE_DRAW:
            isDrawn = true;     
            offsetX = 0;
            offsetY = 0;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case ID_TRANS_COLOR:
            // Збільшуємо значення компонентів кольору на певну величину (з циклічним обмеженням до 255)
            redVal = (redVal + 30) % 256;
            greenVal = (greenVal - 20 + 256) % 256;
            blueVal = (blueVal + 15) % 256;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case ID_TRANS_MOVE:
            offsetX += 45;
            offsetY += 15;
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case ID_INFO_ABOUT:
            MessageBoxW(hwnd, L"Laboratory Work #3\nTask 2: Menu and Transformations\nVariant 1: Movement", L"About Program", MB_OK | MB_ICONINFORMATION);
            break;
        case ID_INFO_EXIT:
            DestroyWindow(hwnd);
            break;
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);
        DrawScene(hdc, rect.right, rect.bottom);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_SIZE:
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        MessageBox(hwnd, L"Left mouse button clicked on the composition!", L"WM_LBUTTONDOWN", MB_OK | MB_ICONINFORMATION);
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hwnd);
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, message, wParam, lParam);
}