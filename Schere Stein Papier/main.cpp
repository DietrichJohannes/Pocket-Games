#include <windows.h>
#include <cassert>
#include <ctime>
#include <commctrl.h>
#include <string>
#include <iostream>

#define Schere 100
#define Stein 101
#define Papier 102

WNDCLASS wc;
HWND hWnd;
MSG msg;

HWND SchereButton;
HWND SteinButton;
HWND PapierButton;

HWND PcText;
HWND PlayerText;
HWND GameAlert;

HWND PCPoints;
HWND PlayerPoints;

int PointsPC;
int PointsPlayer;

int PCChoice;
int PlayerChoice;

using namespace std;

void CPUPlay();
void getWinner();
void updatePoints();

LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN:
            if (wParam == VK_NUMPAD1) {
                SetWindowText(PlayerText, "Schere");
                PlayerChoice = 0;
                CPUPlay();
            } else if (wParam == VK_NUMPAD2) {
                SetWindowText(PlayerText, "Stein");
                PlayerChoice = 1;
                CPUPlay();
            } else if (wParam == VK_NUMPAD3) {
                SetWindowText(PlayerText, "Papier");
                PlayerChoice = 2;
                CPUPlay();
            }
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == Schere) {
                SetWindowText(PlayerText, "Schere");
                PlayerChoice = 0;
                CPUPlay();
            } else if (LOWORD(wParam) == Stein) {
                SetWindowText(PlayerText, "Stein");
                PlayerChoice = 1;
                CPUPlay();
            } else if (LOWORD(wParam) == Papier) {
                SetWindowText(PlayerText, "Papier");
                PlayerChoice = 2;
                CPUPlay();
            }
            break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CPUPlay() {
    srand(time(0));
    int PCIndex = 1;

    PCIndex = PCChoice;

    do {
        PCChoice = rand() % 3;
    } while (PCChoice == PCIndex);

    if (PCChoice == 0) {
        SetWindowText(PcText, "Schere");
    } else if (PCChoice == 1) {
        SetWindowText(PcText, "Stein");
    } else if (PCChoice == 2) {
        SetWindowText(PcText, "Papier");
    }

    getWinner();
}

void getWinner() {
    if ((PCChoice == 0 && PlayerChoice == 1) || (PCChoice == 1 && PlayerChoice == 2) || (PCChoice == 2 && PlayerChoice == 0)) {
        SetWindowText(GameAlert, "Du hast gewonnen!");
        Beep(250, 150);
        Beep(250, 150);
        PointsPlayer++;
    } else if ((PCChoice == 0 && PlayerChoice == 2) || (PCChoice == 1 && PlayerChoice == 0) || (PCChoice == 2 && PlayerChoice == 1)) {
        SetWindowText(GameAlert, "Du hast verloren!");
        Beep(150, 150);
        Beep(150, 150);
        PointsPC++;
    } else if (PCChoice == PlayerChoice) {
        SetWindowText(GameAlert, "Unentschieden");
        Beep(200, 500);
    }
    cout << "Pc Punkte: " << PointsPC << "\n";
    cout << "Spieler Punkte: " << PointsPlayer << "\n\n";
    cout << "Spieler: " << PlayerChoice << "\n";
    cout << "PC: " << PCChoice << "\n\n";
    updatePoints();
}

void updatePoints() {
    string pointPl = std::to_string(PointsPlayer);
    string pointpc = std::to_string(PointsPC);

    LPCSTR PPl = pointPl.c_str();
    LPCSTR PPC = pointpc.c_str();

    SetWindowText(PlayerPoints, PPl);
    SetWindowText(PCPoints, PPC);

    int result;

    if (PointsPC >= 5) {
        Beep(500, 250);
        Beep(450, 250);
        Beep(300, 250);
        Beep(150, 350);
        result = MessageBox(hWnd, "Die CPU hat dich besiegt!\n Nochmal Spielen?", "Verloren!", MB_YESNO);
        PointsPlayer = 0;
        PointsPC = 0;
        updatePoints();
    }

    if (PointsPlayer >= 5) {
        Beep(800, 250);
        Beep(900, 250);
        Beep(1000, 250);
        Beep(1100, 250);
        result = MessageBox(hWnd, "Du hast die CPU besiegt!\n Nochmal Spielen?", "Gewonnen!", MB_YESNO);
        PointsPlayer = 0;
        PointsPC = 0;
        updatePoints();
    }

    if (result == IDYES) {
        PointsPlayer = 0;
        PointsPC = 0;
        updatePoints();
    }

    if (result == IDNO) {
        PostQuitMessage(0);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = MessageHandler;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(0, IDI_WINLOGO);
    wc.hCursor = LoadCursor(0, IDC_CROSS);
    wc.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = "RetroGame";

    assert(RegisterClass(&wc));

    hWnd = CreateWindow("RetroGame", "Schere Stein Papier", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, hInstance, 0);

    ShowWindow(GetConsoleWindow(), SW_HIDE);

    ShowWindow(hWnd, nCmdShow);
    SetForegroundWindow(hWnd);
    UpdateWindow(hWnd);

    CreateWindow("static", "CPU:", WS_VISIBLE | WS_CHILD | SS_CENTER, 382, 50, 35, 15, hWnd, 0, hInstance, 0);
    PcText = CreateWindow("static", "-", WS_VISIBLE | WS_CHILD | SS_CENTER, 375, 70, 50, 15, hWnd, 0, hInstance, 0);
    PCPoints = CreateWindow("static", "0", WS_VISIBLE | WS_CHILD | SS_CENTER, 770, 10, 15, 15, hWnd, 0, hInstance, 0);

    CreateWindow("static", "Du:", WS_VISIBLE | WS_CHILD | SS_CENTER, 382, 120, 35, 15, hWnd, 0, hInstance, 0);
    PlayerText = CreateWindow("static", "-", WS_VISIBLE | WS_CHILD | SS_CENTER, 375, 140, 50, 15, hWnd, 0, hInstance, 0);
    PlayerPoints = CreateWindow("static", "0", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 15, 15, hWnd, 0, hInstance, 0);

    GameAlert = CreateWindow("static", "-", WS_VISIBLE | WS_CHILD | SS_CENTER, 350, 250, 100, 30, hWnd, 0, hInstance, 0);

    SchereButton = CreateWindow("button", "Schere", WS_CHILD | WS_VISIBLE | BS_FLAT, 200, 400, 100, 50, hWnd, (HMENU) Schere, hInstance, 0);
    SteinButton = CreateWindow("button", "Stein", WS_CHILD | WS_VISIBLE | BS_FLAT, 350, 400, 100, 50, hWnd, (HMENU) Stein, hInstance, 0);
    PapierButton = CreateWindow("button", "Papier", WS_CHILD | WS_VISIBLE | BS_FLAT, 500, 400, 100, 50, hWnd, (HMENU) Papier, hInstance, 0);

    bool running = true;
    while (running) {
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // GameStuff
    }

    return msg.wParam; // Füge dies hinzu, um die Rückgabe zu beheben
}
