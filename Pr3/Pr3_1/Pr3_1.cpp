#include <windows.h>
#include <string>
#include <cmath>

#define ID_EDIT_X 101
#define ID_EDIT_Y 102
#define ID_BUTTON 103
#define ID_STATIC_RESULT 104

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
    WNDCLASSEXA wc{};
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = "Task1WindowClass";

    if (!RegisterClassExA(&wc)) return 0;

    HWND hwnd = CreateWindowExA(
        0, "Task1WindowClass", "f(x,y) = x * y^2",
        WS_OVERLAPPEDWINDOW, 400, 250, 480, 320,
        nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hEditX;
    static HWND hEditY;
    static HWND hButton;
    static HWND hResult;

    switch (message) {
    case WM_CREATE: {
        HINSTANCE hInstance = reinterpret_cast<LPCREATESTRUCTA>(lParam)->hInstance;

        hEditX = CreateWindowExA(0, "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            50, 70, 100, 25, hwnd, reinterpret_cast<HMENU>(ID_EDIT_X), hInstance, nullptr);

        hEditY = CreateWindowExA(0, "EDIT", "0", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT,
            180, 70, 100, 25, hwnd, reinterpret_cast<HMENU>(ID_EDIT_Y), hInstance, nullptr);

        hButton = CreateWindowExA(0, "BUTTON", "Calculate", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            50, 120, 130, 30, hwnd, reinterpret_cast<HMENU>(ID_BUTTON), hInstance, nullptr);

        hResult = CreateWindowExA(0, "STATIC", "0", WS_CHILD | WS_VISIBLE,
            180, 170, 220, 25, hwnd, reinterpret_cast<HMENU>(ID_STATIC_RESULT), hInstance, nullptr);
        break;
    }
    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_BUTTON) {
            char bufferX[50]{};
            char bufferY[50]{};

            GetWindowTextA(hEditX, bufferX, sizeof(bufferX));
            GetWindowTextA(hEditY, bufferY, sizeof(bufferY));

            try {
                double x = std::stod(bufferX);
                double y = std::stod(bufferY);
                double resultVal = x * std::pow(y, 2);

                std::string resultStr = std::to_string(resultVal);
                SetWindowTextA(hResult, resultStr.c_str());
            }
            catch (...) {
                SetWindowTextA(hResult, "Error: enter numbers!");
            }
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        TextOutA(hdc, 50, 35, "Enter values for x and y:", 25);
        TextOutA(hdc, 50, 170, "Result f(x,y):", 14);
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, message, wParam, lParam);
    }
    return 0;
}