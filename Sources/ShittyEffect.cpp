DWORD WINAPI iloveskudding(LPVOID why) {
    int xxx = 0;
    while (1) {
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        int rndsize = 1 + rand() % (h / 2);
        HDC hdc = GetDC(0);
        HDC hcdc = CreateCompatibleDC(hdc);
        BITMAPINFO bmi = { 0 };
        PRGBQUAD rgbScreen = { 0 };
        bmi.bmiHeader = { sizeof(BITMAPINFOHEADER), w, h, 1, 32, BI_RGB };
        HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, NULL, (void**)&rgbScreen, NULL, NULL);
        SelectObject(hcdc, hBitmap);
        StretchBlt(hcdc, 0, 0, w, h, hcdc, 0, 0, 1, 1, WHITENESS);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h;
            rgbScreen[i].rgb <<= (xxx + ((xxx + x) ^ i)) | ((xxx + y) ^ i);
        }
        for (int yyy = 0; yyy < h; yyy += rndsize) {
            StretchBlt(hcdc, -2 + rand() % 5, yyy, w, rndsize, hcdc, 0, yyy, w, rndsize, SRCAND);
        }
        StretchBlt(hcdc, 0, 0, w + 1200, h + 850, hcdc, 0, 0, w - 1200, h - 850, SRCCOPY);
        BitBlt(hdc, 0, 0, w, h, hcdc, 0, 0, SRCCOPY);
        ReleaseDC(0, hdc);
        ReleaseDC(0, hcdc);
        DeleteObject(hBitmap);
        DeleteDC(hcdc);
        DeleteDC(hdc);
        Sleep(1);
        xxx++;
    }
}