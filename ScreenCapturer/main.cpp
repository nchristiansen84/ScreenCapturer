#include "resource.h"
#include "main.h"
#include "Utilities.h"
#include "Keyboard.h"

//  --Globals--  ///
HINSTANCE hInst;
HWND SaveDirBox = nullptr, FileNameBox = nullptr, KeyBox = nullptr;
WCHAR UserSaveDir[MAX_PATH] = { 0 }, UserFileName[MAX_PATH] = { 0 }, UserKey[USER_KEY_LEN] = { 0 };
PCWCH FileTypes[NUM_FILE_TYPES] = {L".bmp", L".jpg", L".png"};
DWORD UserFileTypeIndex = 1;
bool CtrlNeeded = false, AltNeeded = false;

///
// Processes window messages
// 
// Params:
//   window   - Handle to window
//   message  - Message being received and processed
//   wParam   - Additional mesasge information
//   lParam   - Additional message information
// 
// Return:
//   Returns the result of the message processed
///
LRESULT CALLBACK WndProc(
	_In_ HWND   window,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam )
{
	switch(message)
	{
	case WM_SIZE:
		InvalidateRect(window, nullptr, true);
		UpdateWindow(window);
		break;
	case WM_CREATE:
		{
		HWND dropdownWindow = nullptr;
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(window, &ps);

		// Add textbox for SaveDir
		SaveDirBox = CreateWindowW(EDIT_CLASS, UserSaveDir, ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, DIR_X, DIR_Y, DIR_WIDTH, DIR_HEIGHT, window, (HMENU)IDC_SAVE_DIR, hInst, 0);
		SendMessageW(SaveDirBox, EM_LIMITTEXT, MAX_PATH, 0L);

		// Add textbox for FileName
		FileNameBox = CreateWindowW(EDIT_CLASS, UserFileName, ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER, FILE_X, FILE_Y, FILE_WIDTH, FILE_HEIGHT, window, (HMENU)IDC_FILE_NAME, hInst, 0);
		SendMessageW(FileNameBox, EM_LIMITTEXT, MAX_PATH, 0L);

		// Add dropdown for file type, populate it, and set default
		dropdownWindow = CreateWindowW(WC_COMBOBOX, nullptr, CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, DROPDOWN_X, DROPDOWN_Y, DROPDOWN_WIDTH, DROPDOWN_HEIGHT, window, nullptr, hInst, 0);
		for(int i = 0; i < NUM_FILE_TYPES; i++){
			SendMessageW(dropdownWindow, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)FileTypes[i]);
		}
		SendMessageW(dropdownWindow, (UINT)CB_SETCURSEL, (WPARAM)UserFileTypeIndex, (LPARAM)0);

		// Add checkbox for CTRL
		CreateWindowW(BUTTON_CLASS, nullptr, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER, CTRL_X, CTRL_Y, CTRL_WIDTH, CTRL_HEIGHT, window, (HMENU)IDC_CTRL_BUTTON, hInst, 0);

		// Add checkbox for ALT
		CreateWindowW(BUTTON_CLASS, nullptr, BS_AUTOCHECKBOX | WS_CHILD | WS_VISIBLE | WS_BORDER, ALT_X, ALT_Y, ALT_WIDTH, ALT_HEIGHT, window, (HMENU)IDC_ALT_BUTTON, hInst, 0);

		// Add textbox for Key
		KeyBox = CreateWindowW(EDIT_CLASS, UserKey, WS_CHILD | WS_VISIBLE | WS_BORDER, KEY_X, KEY_Y, KEY_WIDTH, KEY_HEIGHT, window, (HMENU)IDC_KEY, hInst, 0);
		SendMessageW(KeyBox, EM_LIMITTEXT, USER_KEY_LEN, 0L);

		// Button to take screenshot
		CreateWindowW(BUTTON_CLASS, BUTTON_MESSAGE, WS_CHILD | WS_VISIBLE, BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, window, (HMENU)IDC_BUTTON, hInst, 0);

		EndPaint(window, &ps);
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON: // SS button
			takeSS();
			break;
		case IDC_CTRL_BUTTON: // Ctrl checkbox
			CtrlNeeded = !CtrlNeeded;
			break;
		case IDC_ALT_BUTTON: // Alt checkbox
			AltNeeded = !AltNeeded;
			break;
		}
		switch(HIWORD(wParam))
		{
		case CBN_SELCHANGE: // Dropdown
			UserFileTypeIndex = (int)SendMessageW((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			break;
		case EN_CHANGE: // Textbox changed
			if(LOWORD(wParam) == IDC_SAVE_DIR){ // Save dir edit box
				memset(UserSaveDir, 0, MAX_PATH);
				SendMessageW(SaveDirBox, WM_GETTEXT, MAX_PATH, (LPARAM)UserSaveDir);
			}else if(LOWORD(wParam) == IDC_FILE_NAME){ // File name edit box
				memset(UserFileName, 0, MAX_PATH);
				SendMessageW(FileNameBox, WM_GETTEXT, MAX_PATH, (LPARAM)UserFileName);
			}else if(LOWORD(wParam) == IDC_KEY){ // Key edit box
				WCHAR keyEntered[USER_KEY_LEN] = { 0 };
				SendMessageW(KeyBox, WM_GETTEXT, USER_KEY_LEN, (LPARAM)keyEntered);
				memset(UserKey, 0, USER_KEY_LEN);
				UserKey[0] = toupper(keyEntered[0]);
			}
			break;
		}
		break;
	case WM_PAINT:
		{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(window, &ps);

		// Add text for SaveDir
		TextOutW(dc, DIR_TEXT_X, DIR_TEXT_Y, SAVE_DIR_TEXT, (int)wcslen(SAVE_DIR_TEXT));

		// Add text for FileName
		TextOutW(dc, FILE_TEXT_X, FILE_TEXT_Y, FILE_NAME_TEXT, (int)wcslen(FILE_NAME_TEXT));

		// Add text for file type
		TextOutW(dc, DROPDOWN_TEXT_X, DROPDOWN_TEXT_Y, DROPDOWN_TEXT, (int)wcslen(DROPDOWN_TEXT));

		// Add text for CTRL
		TextOutW(dc, CTRL_TEXT_X, CTRL_TEXT_Y, CTRL_TEXT, (int)wcslen(CTRL_TEXT));

		// Add text for ALT
		TextOutW(dc, ALT_TEXT_X, ALT_TEXT_Y, ALT_TEXT, (int)wcslen(ALT_TEXT));

		// Add text for Key
		TextOutW(dc, KEY_TEXT_X, KEY_TEXT_Y, KEY_TEXT, (int)wcslen(KEY_TEXT));

		EndPaint(window, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProcW(window, message, wParam, lParam);
		break;
	}
	return 0;
}

///
// Initilizes the main window and some variables
// 
// Params:
//   Instance  - Handle to this instance
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed.
///
DWORD init(HINSTANCE Instance)
{
	HWND window = nullptr;
	hInst = Instance;

	window = CreateWindowW( WINDOW_CLASS,
	                        PROGRAM_NAME,
	                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,//WS_OVERLAPPEDWINDOW,
	                        CW_USEDEFAULT,
	                        0,
	                        WINDOW_WIDTH,
	                        WINDOW_HEIGHT,
	                        nullptr,
	                        nullptr,
	                        Instance,
	                        nullptr );
	if(!window){
		return ERROR_FUNCTION_FAILED;
	}

	ShowWindow(window, SW_SHOWNORMAL);
	UpdateWindow(window);

	memcpy(UserSaveDir, DEFAULT_SAVE_DIR, wcslen(DEFAULT_SAVE_DIR) * sizeof(WCHAR));

	return ERROR_SUCCESS;
}

///
// Cleans up anything that needs cleaned up
// 
// Params:
//   None
// 
// Return:
//   None
///
void cleanup()
{
	if(keyboardListenerThread){
		CloseHandle(keyboardListenerThread);
	}
}

///
// Registers a window class
// 
// Params:
//   Instance  - Handle to this instance
// 
// Return:
//   If success, return value is a class atom that uniquely identified the class ebing registered. If failed, return is zero.
///
ATOM registerClass(HINSTANCE Instance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = (HICON)LoadImageW(nullptr, ICON_NAME, IMAGE_ICON, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WINDOW_CLASS;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

///
// Entry point for this program that creates a window and processages messages for it and creates a thread that listens to keyboard input
// 
// Params:
//   Instance       - Handle to this instance
//   hPrevInstance  - Unused
//   lpCmdLine      - Unused
//   nCmdShow       - Unused
// 
// Return:
//   Returns the result of the last message processed
///
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	MSG message;

	// Create and register class
	if(!registerClass(hInstance)){
		MessageBoxW(nullptr, L"Failed to register class.", ERROR_MESSAGE, MB_OK| MB_ICONERROR);
		return ERROR_FUNCTION_FAILED;
	}

	// Initialization
	if(init(hInstance) != ERROR_SUCCESS){
		MessageBoxW(nullptr, L"Failed to create window.", ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return ERROR_FUNCTION_FAILED;
	}

	// Keybaord listener thread
	keyboardListenerThread = CreateThread(nullptr, 0, keyboardListener, nullptr, 0, nullptr);
	if(!keyboardListenerThread){
		MessageBoxW(nullptr, L"Failed to create thread.", ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return ERROR_FUNCTION_FAILED;
	}

	// Process messages
	while(GetMessageW(&message, nullptr, 0, 0)){
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}

	// Cleanup
	cleanup();

	return (int)message.wParam;
}