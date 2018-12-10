#include "Utilities.h"

///
// Builds a FilePath from DirPath, FileName, and FileType. FileName is optional because it will be prepended to timestamp.
// 
// Params:
//   DirPath   - The path to where the file will be saved
//   FileName  - The name of the file.
//     Optional: The FileName will be prepended to an underscore then timestamp if provided. If not provided it will only be the timestamp.
//   FileType  - The type of file it will be saved as. Supports: .bmp, .jpg, and .png
// 
// Return:
//   Returns nullptr on failure and a pointer to a string that represents the FilePath on success. On success, return must be freed by user.
///
PWCHAR buildFilePath(PCWCH DirPath, PCWCH FileName, PCWCH FileType)
{
	time_t timer;
	tm timerLocal;
	WCHAR timestamp[TIME_STAMP_LEN] = { 0 };
	PWCHAR filePath = nullptr;
	size_t filePathLen = 0;

	// Checks if FileName was provided, to determine if it should be prepended with underscore or not
	if(FileName[0] == 0){
		filePathLen = wcslen(DirPath) + TIME_STAMP_LEN + wcslen(FileType) + 2; // 1 for \ and null terminator
	}
	else{
		filePathLen = wcslen(DirPath) + wcslen(FileName) + TIME_STAMP_LEN + wcslen(FileType) + 3; // 1 for \, _, and null terminator
	}

	// Allocate and initialize memory for filePath
	filePath = (PWCHAR)calloc(filePathLen, sizeof(WCHAR));
	if(!filePath){
		return filePath;
	}

	// Get current time
	timer = time(nullptr);

	// Convert time to local time
	localtime_s(&timerLocal, &timer);

	// Convert time to local time and put into timestamp string
	wcsftime(timestamp, TIME_STAMP_LEN, L"%Y-%m-%d_%H-%M-%S", &timerLocal);

	// Build filePath: SaveDirPath\timestamp.png
	if(FileName[0] == 0){
		swprintf_s(filePath, filePathLen, L"%ws\\%ws%ws", DirPath, timestamp, FileType);
	}
	else{
		swprintf_s(filePath, filePathLen, L"%ws\\%ws_%ws%ws", DirPath, FileName, timestamp, FileType);
	}

	return filePath;
}

///
// Checks if the DirPath provided exists
// 
// Params:
//   DirPath  - The path to a directory
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed.
///
DWORD isValidDir(PCWCH DirPath)
{
	if(!PathFileExistsW(DirPath)){
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

///
// Checks if the FileName is a legal file name
// 
// Params:
//   FileName  - The name of a file
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed.
//
// Note: This is just initial version. Needs updated to better validate file name.
///
DWORD isValidFileName(PCWCH FileName)
{
	for(size_t i = 0; i < wcslen(FileName); i++){
		if( FileName[i] == '\\' || FileName[i] == '/' || FileName[i] == '\?' // Fails on \ / ?
		    || FileName[i] == '|' || FileName[i] == '>' || FileName[i] == '<' // Fails on | > <
		    || FileName[i] == ':' || FileName[i] == '*' || FileName[i] == '\"' // Fails on : * "
		    || FileName[i] == '+' || FileName[i] == ',' || FileName[i] == ';' // Fails on + , ;
		    || FileName[i] == '=' || FileName[i] == '[' || FileName[i] == ']' ){ // Fails on = [ ]
			return ERROR_INVALID_NAME;
		}
	}
	return ERROR_SUCCESS;
}

///
// Creates DirPath if it doesn't exist
// 
// Params:
//   DirPath  - The path to a directory
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed.
///
DWORD verifySaveDirExists(PCWCH DirPath){
	if(!CreateDirectoryW(DirPath, nullptr)){
		DWORD error = GetLastError();
		if(error != ERROR_ALREADY_EXISTS){
			return error;
		}
	}

	return ERROR_SUCCESS;
}

///
// Gets the dimensions of the entire screen
// 
// Params:
//   OutDesktopDim  - Variable to hold outputted dimensions of screen
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed.
///
DWORD getDesktopDim(PRECT OutDesktopDim)
{
	HWND desktop = GetDesktopWindow();

	if(!GetWindowRect(desktop, OutDesktopDim)){
		return GetLastError();
	}

	return ERROR_SUCCESS;
}

///
// Saves the Bitmap to the FilePath provided
// 
// Params:
//   FilePath  - The path the where the file should be saved
//   Bitmap    - A bitmap representing the area screenshotted
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed and creates MesasgeBox.
///
DWORD saveScreenShot(PCWCH FilePath, HBITMAP Bitmap)
{
	CImage image;
	WCHAR errorMessage[ERROR_MESSAGE_MAX] = { 0 };
	DWORD error = ERROR_SUCCESS;
	HRESULT hr = S_OK;

	// Attach bitmap to image
	image.Attach(Bitmap);

	// Save image
	hr = image.Save(FilePath);
	if(hr != S_OK){
		error = GetLastError();
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to save image. Error[%lu]. HR[0x%x]\n", SCREENSHOT_FAILED_MESSAGE, error, hr);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
	}

	// Cleanup
	image.Detach();

	return error;
}

///
// Captures a bitmap of the entire screen
// 
// Params:
//   FilePath  - The path the where the file should be saved
// 
// Return:
//   Returns DWORD of error. ERROR_SUCCESS means the function was successful, otherwise it failed and creates MessageBox.
///
DWORD takeScreenShot(PCWCH FilePath)
{
	HDC dcScreen = nullptr, dcMem = nullptr;
	HBITMAP bitmap = nullptr;
	RECT screenDim = { 0 };
	WCHAR errorMessage[ERROR_MESSAGE_MAX] = { 0 };
	DWORD error = ERROR_SUCCESS;

	// Gets the dimensions of the desktop
	error = getDesktopDim(&screenDim);
	if(error != ERROR_SUCCESS){
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to get desktop dimensions. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, error);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return error;
	}

	// Get DC for entire screen
	dcScreen = CreateDC(L"DISPLAY", nullptr, nullptr, nullptr);
	if(!dcScreen){
		error = ERROR_FUNCTION_FAILED;
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to create DC for screen. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, error);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return error;
	}

	// Get DC for entire screen
	dcMem = CreateCompatibleDC(dcScreen);
	if(!dcMem){
		error = ERROR_FUNCTION_FAILED;
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to create DC. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, error);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return error;
	}

	// Make bitmap in memory to store screenshot
	bitmap = CreateCompatibleBitmap(dcScreen, screenDim.right - screenDim.left, screenDim.bottom - screenDim.top);
	if(!bitmap){
		error = ERROR_FUNCTION_FAILED;
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to get bitmap. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, error);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		goto out;
	}

	// Join DC and bitmap
	SelectObject(dcMem, bitmap);

	// Copy from entire screen to bitmap
	if(!BitBlt( dcMem,
	            0, 0, // x,y for top left (destination)
	            screenDim.right - screenDim.left, screenDim.bottom - screenDim.top, // width,height (source and destination)
	            dcScreen,
	            screenDim.left, screenDim.top, // x,y for top left (source)
	            SRCCOPY | CAPTUREBLT) ){
		error = GetLastError();
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to transfer bits. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, error);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		goto out;
	}

	// Saves the screenshot as <TimeStamp>.PNG
	error = saveScreenShot(FilePath, bitmap);
	if(error != ERROR_SUCCESS){
		goto out;
	}

out:

	// Cleanup
	if(dcScreen){
		DeleteDC(dcScreen);
	}
	if(dcMem){
		DeleteDC(dcMem);
	}
	if(bitmap){
		DeleteObject(bitmap);
	}

	return error;
}

///
// Takes a screenshot of the entire screen and saves it
// 
// Params:
//   None
// 
// Return:
//   None. Creates MessageBox notifying user on failure.
///
void takeSS()
{
	PWCHAR filePath = nullptr;
	WCHAR errorMessage[ERROR_MESSAGE_MAX] = { 0 };
	DWORD error = ERROR_SUCCESS;

	// SaveDir was entered and taken away
	if(UserSaveDir[0] == 0){
		memcpy(UserSaveDir, DEFAULT_SAVE_DIR, wcslen(DEFAULT_SAVE_DIR) * sizeof(WCHAR));
	}

	// Checks if user specified a save directory
	if(!wcscmp(UserSaveDir, DEFAULT_SAVE_DIR)){
		// Check if directory exists and creates if it doesn't
		error = verifySaveDirExists(UserSaveDir);
		if(error != ERROR_SUCCESS){
			swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to create directory[%ws]. Error[%lu]\n", SCREENSHOT_FAILED_MESSAGE, UserSaveDir, error);
			MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
			return;
		}
	}
	else{
		// Check if directory exists
		if(isValidDir(UserSaveDir) != ERROR_SUCCESS){
			swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Invalid directory path[%ws]\n", SCREENSHOT_FAILED_MESSAGE, UserSaveDir);
			MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
			return;
		}
	}

	// Checks if the user has specified a file name
	if(UserFileName[0] != 0){
		// Check if valid file name
		if(isValidFileName(UserFileName) != ERROR_SUCCESS){
			swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Invalid file name[%ws]\n", SCREENSHOT_FAILED_MESSAGE, UserFileName);
			MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
			return;
		}
	}

	// Builds filePath
	filePath = buildFilePath(UserSaveDir, UserFileName, FileTypes[UserFileTypeIndex]);
	if(!filePath){
		swprintf_s(errorMessage, ERROR_MESSAGE_MAX, L"%ws Failed to allocate memory.\n", SCREENSHOT_FAILED_MESSAGE);
		MessageBoxW(nullptr, errorMessage, ERROR_MESSAGE, MB_OK | MB_ICONERROR);
		return;
	}

	// Takes screenshot
	takeScreenShot(filePath);

	free(filePath);
	return;
}