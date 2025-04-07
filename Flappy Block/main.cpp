#include <windows.h>
#include <vector>
#include <string>
#include <ctime>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

struct GameObject {
    int x, y, width, height;
    COLORREF color;
};

HWND hwndButton;
RECT canvasRect = { 0, 0, 1575, 700 };
GameObject player = { 10, 120, 30, 30, RGB(255, 165, 0) };
std::vector<GameObject> obstacles;
int frameNo = 0;
float gravity = 0.1f;
float gravitySpeed = -1.0f;
bool gameOver;

void DrawObject(HDC hdc, GameObject obj) {
    HBRUSH brush = CreateSolidBrush(obj.color);
    RECT r = { obj.x, obj.y, obj.x + obj.width, obj.y + obj.height };
    FillRect(hdc, &r, brush);
    DeleteObject(brush);
}

void UpdateGame(HWND hwnd) {
    if (gameOver) return;

    frameNo++;

    // Hinderniskollision prüfen
    for (size_t i = 0; i < obstacles.size(); ++i) {
        if (player.x < obstacles[i].x + obstacles[i].width &&
            player.x + player.width > obstacles[i].x &&
            player.y < obstacles[i].y + obstacles[i].height &&
            player.y + player.height > obstacles[i].y) {
            gameOver = true;
            InvalidateRect(hwnd, &canvasRect, TRUE); // Letztes Bild mit Game Over zeichnen
            return;
        }
    }

    // Neue Hindernisse generieren
    if (frameNo == 1 || frameNo % 150 == 0) {
        int minHeight = 70, maxHeight = 200;
        int height = rand() % (maxHeight - minHeight + 1) + minHeight;

        int minGap = 80, maxGap = 200;
        int gap = rand() % (maxGap - minGap + 1) + minGap;

        GameObject top = {
                canvasRect.right, 0,
                51, height,
                RGB(50, 205, 50) // limegreen
        };
        GameObject bottom = {
                canvasRect.right,
                height + gap,
                51,
                canvasRect.bottom - (height + gap),
                RGB(64, 224, 208) // turquoise
        };

        obstacles.push_back(top);
        obstacles.push_back(bottom);
    }

    // Hindernisse bewegen
    for (auto& obs : obstacles) {
        obs.x -= 2; // Geschwindigkeit hier anpassbar
    }

    // Schwerkraft anwenden
    gravitySpeed += gravity;
    player.y += (int)gravitySpeed;

    // Bodenbegrenzung
    if (player.y + player.height > canvasRect.bottom) {
        player.y = canvasRect.bottom - player.height;
        gravitySpeed = 0;
    }

    // obere Begrenzung
    if (player.y < 0) {
        player.y = 0;
        gravitySpeed = 0;
    }

    // Neu zeichnen
    InvalidateRect(hwnd, &canvasRect, TRUE);
}


void DrawGame(HDC hdc) {
    DrawObject(hdc, player);
    for (auto& obs : obstacles) {
        DrawObject(hdc, obs);
    }

    std::wstring score = L"SCORE: " + std::to_wstring(frameNo);
    TextOutW(hdc, 1300, 20, score.c_str(), score.length());
}

void ResetGame() {
    frameNo = 0;
    gravitySpeed = 0;
    player.y = 120;
    obstacles.clear();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    srand((unsigned)time(NULL));
    const wchar_t CLASS_NAME[] = L"Flappy Block";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
            0,                     // dwExStyle
            CLASS_NAME,            // lpClassName (wchar_t*)
            L"Flappy Block",      // lpWindowName
            WS_OVERLAPPEDWINDOW,   // dwStyle
            CW_USEDEFAULT, CW_USEDEFAULT, 1600, 800,
            NULL, NULL, hInstance, NULL
    );


    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);

    SetTimer(hwnd, 1, 10, NULL); // 50 FPS

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            hwndButton = CreateWindowW(L"BUTTON", L"Neues Spiel",
                                      WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                                      20, 720, 100, 30, hwnd, (HMENU)1, NULL, NULL);
            break;

        case WM_TIMER:
            UpdateGame(hwnd);
            break;

        case WM_KEYDOWN:
            if (wParam == VK_SPACE && !gameOver) {
                gravitySpeed = -3.0f;  // Sprung-Impuls
            } else if (wParam == VK_F5) {
                ResetGame();
            }
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            DrawGame(hdc);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                ResetGame(); // Dein Spiel neu starten
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
