#define WH_KEYBOARD_LL 13
#define WH_MOUSE_LL 14

typedef struct {
DWORD vkCode;
DWORD scanCode;
DWORD flags;
DWORD time;
ULONG_PTR dwExtraInfo;
} KBDLLHOOKSTRUCT, *PKBDLLHOOKSTRUCT;

typedef struct {
POINT pt;
DWORD mouseData;
DWORD flags;
DWORD time;
ULONG_PTR dwExtraInfo;
} MSLLHOOKSTRUCT, *PMSLLHOOKSTRUCT;




LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT *kbHook = (KBDLLHOOKSTRUCT *)lParam;
	if(kbHook->vkCode != 0)
	{
        return 1;
	}
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


HINSTANCE hInst = GetModuleHandle(NULL);
HHOOK KeyHook;

class HookClass
{
public:

	bool SetHook()
	{
		KeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, hInst, 0);
		if(KeyHook == NULL) return false;
		return true;
	}

	bool UnHook()
	{
		if(UnhookWindowsHookEx(KeyHook) == NULL) return false;
		return true;
	}

};
