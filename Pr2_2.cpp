#include <windows.h>
#include <cmath>
#include <vector>

// Клас для малювання сонця
class Sun {
public:
    void show(HDC hdc, int x, int y, int radius = 40) {
        HBRUSH brush = CreateSolidBrush(RGB(255, 220, 0));
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(230, 180, 0));
        HGDIOBJ oldBrush = SelectObject(hdc, brush);
        HGDIOBJ oldPen = SelectObject(hdc, pen);

        // Промені сонячного диска
        for (int i = 0; i < 8; ++i) {
            double angle = i * 3.14159 / 4.0;
            int x1 = x + (int)((radius + 5) * cos(angle));
            int y1 = y + (int)((radius + 5) * sin(angle));
            int x2 = x + (int)((radius + 20) * cos(angle));
            int y2 = y + (int)((radius + 20) * sin(angle));
            MoveToEx(hdc, x1, y1, NULL);
            LineTo(hdc, x2, y2);
        }

        // Диск сонця
        Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }
};

// Клас для малювання хмари
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

// Клас для малювання квітки
class Flower {
public:
    void show(HDC hdc, int x, int y) {
        // Стебло
        HPEN stemPen = CreatePen(PS_SOLID, 3, RGB(34, 139, 34));
        HGDIOBJ oldPen = SelectObject(hdc, stemPen);
        MoveToEx(hdc, x, y, NULL);
        LineTo(hdc, x, y + 50);

        // Пелюстки
        HBRUSH petalBrush = CreateSolidBrush(RGB(255, 105, 180));
        HGDIOBJ oldBrush = SelectObject(hdc, petalBrush);

        Ellipse(hdc, x - 18, y - 8, x - 2, y + 8);
        Ellipse(hdc, x + 2, y - 8, x + 18, y + 8);
        Ellipse(hdc, x - 8, y - 18, x + 8, y - 2);
        Ellipse(hdc, x - 8, y + 2, x + 8, y + 18);

        // Серцевина квітки
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

// Клас для малювання Будинку
class House {
public:
    void show(HDC hdc, int x, int y) {
        // Стіни будинку
        HBRUSH wallBrush = CreateSolidBrush(RGB(222, 184, 135));
        HGDIOBJ oldBrush = SelectObject(hdc, wallBrush);
        Rectangle(hdc, x, y, x + 140, y + 120);

        // Дах
        HBRUSH roofBrush = CreateSolidBrush(RGB(200, 50, 50));
        SelectObject(hdc, roofBrush);
        POINT roofPoints[3] = {
            { x - 10, y },
            { x + 150, y },
            { x + 70, y - 60 }
        };
        Polygon(hdc, roofPoints, 3);

        // Двері
        HBRUSH doorBrush = CreateSolidBrush(RGB(101, 67, 33));
        SelectObject(hdc, doorBrush);
        Rectangle(hdc, x + 15, y + 50, x + 50, y + 120);

        // Вікно
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
    // Тло: Небо та Трава
    HBRUSH skyBrush = CreateSolidBrush(RGB(135, 206, 235));
    HGDIOBJ oldBrush = SelectObject(hdc, skyBrush);
    Rectangle(hdc, 0, 0, width, height * 0.65); 

    HBRUSH grassBrush = CreateSolidBrush(RGB(50, 205, 50));
    SelectObject(hdc, grassBrush);
    Rectangle(hdc, 0, height * 0.65, width, height); 

    SelectObject(hdc, oldBrush);
    DeleteObject(skyBrush);
    DeleteObject(grassBrush);

    // Сонце
    Sun sun;
    sun.show(hdc, width - 100, 90, 45);

    // Хмари 
    Cloud cloud;
    cloud.show(hdc, 60, 50);
    cloud.show(hdc, 250, 80);
    cloud.show(hdc, 480, 40);

    // Будинок
    House house;
    house.show(hdc, 150, height * 0.65 - 120);

    // Квіти на галявині 
    Flower flower;
    flower.show(hdc, 360, height * 0.65 + 40);
    flower.show(hdc, 430, height * 0.65 + 60);
    flower.show(hdc, 500, height * 0.65 + 30);
    flower.show(hdc, 580, height * 0.65 + 50);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
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
        MessageBox(hwnd, L"Натиснуто ліву кнопку миші на композиції!", L"WM_LBUTTONDOWN", MB_OK | MB_ICONINFORMATION);
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
    return DefWindowProc(hwnd, message, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"CompositionWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) return 0;

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Завдання 2 — Композиція (Варіант 1)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 950, 650,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}