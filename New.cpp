#include <iostream>
#include <Windows.h>

HHOOK focusHook = NULL;
HWND robloxWindow = NULL;
RECT robloxRect = { 0 };

LRESULT CALLBACK FocusChangeHook(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
        {
            HWND activatedWindow = (HWND)lParam;
            char windowTitle[256];
            GetWindowTextA(activatedWindow, windowTitle, sizeof(windowTitle));

            if (strcmp(windowTitle, "Roblox") == 0)
            {
                ClipCursor(NULL);
            }
        }
    }

    return CallNextHookEx(focusHook, nCode, wParam, lParam);
}

bool isRobloxOpen()
{
    HWND hwnd = FindWindow(NULL, L"Roblox");
    return (hwnd != NULL);
}

POINT getMousePosition()
{
    POINT pos;
    GetCursorPos(&pos);
    return pos;
}

bool isRightClickPressed()
{
    SHORT state = GetAsyncKeyState(VK_RBUTTON);
    return (state & 0x8000) != 0;
}

void lockMouse()
{
    if (GetClientRect(robloxWindow, &robloxRect))
    {
        MapWindowPoints(robloxWindow, NULL, reinterpret_cast<POINT*>(&robloxRect), 2);

        int taskbarHeight = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYMAXIMIZED);
        robloxRect.bottom -= taskbarHeight;

        ClipCursor(&robloxRect);
    }
}

void unlockMouse()
{
    ClipCursor(NULL);
}

void setMousePosition(int x, int y)
{
    SetCursorPos(x, y);
}

bool isMouseInTitleBar()
{
    RECT windowRect;
    GetWindowRect(robloxWindow, &windowRect);
    POINT cursorPos = getMousePosition();

    if (cursorPos.y >= windowRect.top && cursorPos.y < windowRect.top + GetSystemMetrics(SM_CYCAPTION))
    {
        return true;
    }
    return false;
}

int main()
{
    bool isMouseLocked = false;
    POINT rightClickPos = { 0, 0 };
    bool isInsertPressed = false;
    bool isRobloxActive = false;

    focusHook = SetWindowsHookEx(WH_CALLWNDPROC, FocusChangeHook, NULL, GetCurrentThreadId());

    while (!isRobloxOpen())
    {
        std::cout << "Roblox is not open. Waiting for it to open..." << std::endl;
        Sleep(500);
    }

    robloxWindow = FindWindow(NULL, L"Roblox");
    GetClientRect(robloxWindow, &robloxRect);

    while (true)
    {
        bool isRightClickPressedNow = isRightClickPressed();
        bool isInsertKeyPressed = (GetAsyncKeyState(VK_INSERT) & 0x8000) != 0;

        if (isInsertKeyPressed && !isInsertPressed)
        {
            isMouseLocked = !isMouseLocked;
            if (isMouseLocked)
            {
                lockMouse();
                std::cout << "Mouse locked inside Roblox screen." << std::endl;
            }
            else
            {
                unlockMouse();
                std::cout << "Mouse unlocked." << std::endl;
            }
            isInsertPressed = true;
        }
        else if (!isInsertKeyPressed)
        {
            isInsertPressed = false;
        }

        if (isMouseLocked)
        {
            POINT        if (isRobloxActive && robloxWindow != GetForegroundWindow())
        {
            unlockMouse();
            isRobloxActive = false;
            std::cout << "Roblox window is no longer active." << std::endl;
        }

        if (!isRobloxActive && robloxWindow == GetForegroundWindow())
        {
            lockMouse();
            isRobloxActive = true;
            std::cout << "Roblox window is now active." << std::endl;
        }

        if (currentMousePos.x < robloxRect.left || currentMousePos.x > robloxRect.right - 1 ||
            currentMousePos.y < robloxRect.top || currentMousePos.y > robloxRect.bottom - 1)
        {
            setMousePosition(rightClickPos.x, rightClickPos.y);
        }

        if (isMouseInTitleBar())
        {
            HWND activeWindow = GetForegroundWindow();
            if (activeWindow != robloxWindow)
            {
                RECT activeWindowRect;
                GetWindowRect(activeWindow, &activeWindowRect);
                setMousePosition(currentMousePos.x, activeWindowRect.bottom);
            }
            else
            {
                RECT windowRect;
                GetWindowRect(robloxWindow, &windowRect);
                int desiredY = windowRect.top + GetSystemMetrics(SM_CYCAPTION) + 5;
                setMousePosition(rightClickPos.x, desiredY);
            }
        }
    }

    if (isRightClickPressedNow && !isMouseLocked)
    {
        isMouseLocked = true;
        rightClickPos = getMousePosition();
    }
    else if (!isRightClickPressedNow && isMouseLocked)
    {
        isMouseLocked = false;
        setMousePosition(rightClickPos.x, rightClickPos.y);
    }

    Sleep(10);
}

UnhookWindowsHookEx(focusHook);

return 0;
}
