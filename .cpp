void fixCursor(HWND handle)
{
    if (mode == 1)
    { 
        RECT rect = { NULL };
        if (GetWindowRect(robloxHWND, &rect)) {
            int sizeX = (rect.right - rect.left);
            int sizeY = (rect.bottom - rect.top);

            int centerX = (rect.right - rect.left) / 2;
            int centerY = (rect.bottom - rect.top) / 2;

            if (!isMouseInTitleBar()) {
                SetCursorPos(rect.left + centerX, rect.top + centerY);
            }
        }
    }
    else if (mode == 2)
    {
        POINT p;
        if (GetCursorPos(&p))
        {
            if (ScreenToClient(handle, &p))
            {
                RECT rect = { NULL };
                if (GetWindowRect(robloxHWND, &rect)) {
                    int sizeX = (rect.right - rect.left);
                    int sizeY = (rect.bottom - rect.top);

                    int centerX = (rect.right - rect.left) / 2;
                    int centerY = (rect.bottom - rect.top) / 2;

                    POINT realP;
                    if (GetCursorPos(&realP))
                    {
                        if (isMouseInTitleBar())
                        {
                            int desiredY = rect.top + GetSystemMetrics(SM_CYCAPTION) + 5; 
                            SetCursorPos(rightClickPos.x, desiredY);
                        }
                        else if (p.x < 30)
                        {
                            SetCursorPos(rect.left + 100, realP.y);
                        }
                        else if (p.x > sizeX - 40)
                        {
                            SetCursorPos(rect.right - 100, realP.y);
                        }
                        else if (p.y < 70)
                        {
                            SetCursorPos(realP.x, rect.top + 100);
                        }
                        else if (p.y > sizeY - 40)
                        {
                            SetCursorPos(realP.x, rect.bottom - 100);
                        }
                    }
                }
            }
        }
    }
}
