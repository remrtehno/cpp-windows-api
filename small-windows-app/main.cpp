#include <Windows.h>
#include <string>
#include <iostream>

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnButtonClick(HWND hwnd, HWND editBox);

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ExtendedWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        L"ExtendedWindow",
        L"Extended Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 1;
    }

    // Create a button
    HWND button = CreateWindow(
        L"BUTTON",
        L"Click Me",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10, 10, 100, 30,
        hwnd, NULL, hInstance, NULL);

    std::cout << (WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON);

    // Create an input box (edit control)
    HWND editBox = CreateWindow(
        L"EDIT",
        L"",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
        10, 50, 200, 100, // Adjusted size for multiline input
        hwnd, (HMENU)2, hInstance, NULL); // 2 is the control ID for the edit box
    
    // My test element
    HWND myElem = CreateWindow(
        L"STATIC", // system class name
        L"Hello, world!", // window text
        WS_CHILD | WS_VISIBLE | SS_CENTER, // window style
        200, // x position
        100, // y position
        200, // width
        50, // height
        hwnd, // parent window handle
        NULL, // control identifier
        hInstance, // instance handle
        NULL // additional data
    );





    // Show and update the window
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        if (LOWORD(wParam) == BN_CLICKED) {
            // Button clicked
            OnButtonClick(hwnd, GetDlgItem(hwnd, 2)); // 2 is the control ID for the edit box
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// New Window Procedure
LRESULT CALLBACK NewWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        // Instead of PostQuitMessage(0), just hide the child window
        ShowWindow(hwnd, SW_HIDE);
        break;
    case WM_DESTROY:
        // Perform any additional cleanup if needed
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;

    /* closes parent window
    // Handle messages specific to the new window here
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
    */
}

void OnButtonClick(HWND hwnd, HWND editBox) {
    const int bufferSize = 256;
    wchar_t buffer[bufferSize];

    // Get the text from the edit box
    GetWindowText(editBox, buffer, bufferSize);

    // Display the entered text in a message box
    std::wstring message = L"You entered:\n" + std::wstring(buffer);
    MessageBox(hwnd, message.c_str(), L"Info", MB_OK | MB_ICONINFORMATION);

    /****************************************************************/
    // Create a new window when the button is clicked
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HWND newWindow = CreateWindowEx(
        0,
        L"ExtendedWindow",  // Use the existing window class or create a new one
        L"New Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300,
        NULL, NULL, hInstance, NULL);

    if (newWindow == NULL) {
        return;
    }

    // Set the new window procedure for the new window
    SetWindowLongPtr(newWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(NewWindowProc));

    // Show and update the new window
    ShowWindow(newWindow, SW_SHOWNORMAL);
    UpdateWindow(newWindow);
    /*****************************************/
}
