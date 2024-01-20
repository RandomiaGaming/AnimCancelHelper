#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <thread>
using namespace std;

HHOOK mouseHook;

int main();
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
void PressKey(BYTE vkCode, bool extendedScanCode);
void ReleaseKey(BYTE vkCode, bool extendedScanCode);
void MouseDown();
void MouseUp();

int main() {
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0 && wParam == WM_XBUTTONDOWN) {
		MSLLHOOKSTRUCT* msllStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
		int button = HIWORD(msllStruct->mouseData);
		if (button == 1) {
			//MouseDown();
			cout << "Action button down." << endl;
		}
		else if (button == 2) {
			PressKey(VK_RSHIFT, false);
			PressKey(VK_DELETE, true);
			PressKey(0x52 /*R*/, false);
			cout << "Anim cancel button down." << endl;
		}
	}
	if (nCode >= 0 && wParam == WM_XBUTTONUP) {
		MSLLHOOKSTRUCT* msllStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
		int button = HIWORD(msllStruct->mouseData);
		if (button == 2) {
			MouseUp();
			cout << "Action button up." << endl;
		}
		else if (button == 2) {
			ReleaseKey(VK_RSHIFT, false);
			ReleaseKey(VK_DELETE, true);
			ReleaseKey(0x52 /*R*/, false);
			cout << "Anim cancel button up." << endl;
		}
	}
	return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}
void PressKey(BYTE vkCode, bool extendedScanCode) {
	BYTE scanCode = (BYTE)MapVirtualKey(vkCode, 0);
	DWORD flags = 0;
	if (extendedScanCode) {
		flags |= KEYEVENTF_EXTENDEDKEY;
	}
	keybd_event(vkCode, scanCode, flags, 0);
}
void ReleaseKey(BYTE vkCode, bool extendedScanCode) {
	BYTE scanCode = (BYTE)MapVirtualKey(vkCode, 0);
	DWORD flags = KEYEVENTF_KEYUP;
	if (extendedScanCode) {
		flags |= KEYEVENTF_EXTENDEDKEY;
	}
	keybd_event(vkCode, scanCode, flags, 0);
}
void MouseDown() {
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}
void MouseUp() {
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}