#include <windows.h>
#include <cmath>
#include <cwchar>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"GraphWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) return 0;

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Завдання 1 — Графік y = x^2 / e^x (Варіант 1)",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        100, 100, 850, 650,
        NULL, NULL, hInstance, NULL
    );

    if (!hwnd) return 0;

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int cxClient, cyClient;

    switch (msg) {
    case WM_SIZE:
        cxClient = LOWORD(lParam);
        cyClient = HIWORD(lParam);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (cxClient < 200 || cyClient < 200) {
            EndPaint(hwnd, &ps);
            return 0;
        }

        // Відступи від країв вікна для осей
        int paddingLeft = 70;
        int paddingBottom = 60;
        int paddingTop = 60;
        int paddingRight = 50;

        int graphWidth = cxClient - paddingLeft - paddingRight;
        int graphHeight = cyClient - paddingTop - paddingBottom;

        if (graphWidth <= 0 || graphHeight <= 0) {
            EndPaint(hwnd, &ps);
            return 0;
        }

        int x0 = paddingLeft;
        int y0 = cyClient - paddingBottom;

        // Межі координат для побудови
        double xMinAxis = 0.0;  // Початок осі X з 0
        double xMaxAxis = 6.5;  // Максимум осі X 

        double xMinGraph = 0.4; // Графік будується від x = 0.4
        double xMaxGraph = 6.0; // Графік будується до x = 6.0

        // Пошук max значення Y у діапазоні x ∈ [0.4, 6]
        int numPoints = 300;
        double yMaxVal = 0.0;
        for (int i = 0; i <= numPoints; ++i) {
            double x = xMinGraph + i * (xMaxGraph - xMinGraph) / numPoints;
            double y = (x * x) / std::exp(x);
            if (y > yMaxVal) yMaxVal = y;
        }

        double yMinAxis = 0.0;
        double yMaxAxis = yMaxVal * 1.15;

        // Малювання осей координат 
        HPEN hAxisPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hAxisPen);

        // Вісь X 
        MoveToEx(hdc, x0, y0, NULL);
        LineTo(hdc, cxClient - paddingRight + 20, y0);

        // Стрілка осі X
        LineTo(hdc, cxClient - paddingRight + 10, y0 - 5);
        MoveToEx(hdc, cxClient - paddingRight + 20, y0, NULL);
        LineTo(hdc, cxClient - paddingRight + 10, y0 + 5);

        // Вісь Y 
        MoveToEx(hdc, x0, y0, NULL);
        LineTo(hdc, x0, paddingTop - 20);

        // Стрілка осі Y
        LineTo(hdc, x0 - 5, paddingTop - 10);
        MoveToEx(hdc, x0, paddingTop - 20, NULL);
        LineTo(hdc, x0 + 5, paddingTop - 10);

        // Підписи осей X та Y
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, cxClient - paddingRight + 25, y0 - 10, L"X", 1);
        TextOut(hdc, x0 - 15, paddingTop - 25, L"Y", 1);

        TextOut(hdc, x0 - 18, y0 + 8, L"0", 1);

        // Засічки та підписи на осі X 
        int xTicks = 6;
        for (int i = 1; i <= xTicks; ++i) {
            double val = i * 1.0;
            int px = x0 + (int)((val - xMinAxis) / (xMaxAxis - xMinAxis) * graphWidth);

            MoveToEx(hdc, px, y0 - 4, NULL);
            LineTo(hdc, px, y0 + 4);

            wchar_t buf[16];
            swprintf_s(buf, L"%.1f", val);
            TextOut(hdc, px - 10, y0 + 8, buf, (int)wcslen(buf));
        }

        // Засічки та підписи на осі Y 
        int yTicks = 5;
        for (int i = 1; i <= yTicks; ++i) {
            double val = yMinAxis + i * (yMaxAxis - yMinAxis) / yTicks;
            int py = y0 - (int)((val - yMinAxis) / (yMaxAxis - yMinAxis) * graphHeight);

            MoveToEx(hdc, x0 - 4, py, NULL);
            LineTo(hdc, x0 + 4, py);

            wchar_t buf[16];
            swprintf_s(buf, L"%.2f", val);
            TextOut(hdc, x0 - 45, py - 8, buf, (int)wcslen(buf));
        }

        // Малювання графіку функції y = x^2 / e^x
        HPEN hGraphPen = CreatePen(PS_SOLID, 3, RGB(0, 102, 204));
        SelectObject(hdc, hGraphPen);

        for (int i = 0; i <= numPoints; ++i) {
            double x = xMinGraph + i * (xMaxGraph - xMinGraph) / numPoints;
            double y = (x * x) / std::exp(x);

            int px = x0 + (int)((x - xMinAxis) / (xMaxAxis - xMinAxis) * graphWidth);
            int py = y0 - (int)((y - yMinAxis) / (yMaxAxis - yMinAxis) * graphHeight);

            if (i == 0) MoveToEx(hdc, px, py, NULL);
            else LineTo(hdc, px, py);
        }

        // Виведення назви функції
        SetTextColor(hdc, RGB(0, 102, 204));
        wchar_t titleText[] = L"y = x^2 / e^x,  x in [0.4, 6]";
        TextOut(hdc, cxClient / 2 - 80, 15, titleText, (int)wcslen(titleText));

        // Очищення ресурсів GDI
        SelectObject(hdc, hOldPen);
        DeleteObject(hAxisPen);
        DeleteObject(hGraphPen);

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}