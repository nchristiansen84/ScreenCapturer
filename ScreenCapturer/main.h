#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <CommCtrl.h>

///  --Constant Strings--  ///
#define PROGRAM_NAME       L"Screen Capturer"
#define ICON_NAME          L"Icon.ico"
#define WINDOW_CLASS       L"win32app"
#define BUTTON_CLASS       L"BUTTON"
#define EDIT_CLASS         L"EDIT"
#define BUTTON_MESSAGE     L"ScreenShot"
#define SAVE_DIR_TEXT      L"Directory Path: "
#define FILE_NAME_TEXT     L"File Name: "
#define DROPDOWN_TEXT      L"File Type: "
#define CTRL_TEXT          L"CTRL "
#define ALT_TEXT           L"ALT "
#define KEY_TEXT           L"Key: "
#define ERROR_MESSAGE      L"Error"
#define DEFAULT_SAVE_DIR   L"C:\\ScreenShots"
#define DEFAULT_FILE_TYPE  L"jpg"

///  --Constant Numbers--  ///
#define TIME_STAMP_LEN        20
#define DROPDOWN_SINGLE       25
#define CHECK_BOX_LEN         15
#define NUM_FILE_TYPES        3
#define USER_KEY_LEN          2 // Updated later when allow more than alpha characters
#define ERROR_MESSAGE_MAX     256

// Main Window
#define WINDOW_WIDTH          260
#define WINDOW_HEIGHT         250
#define WINDOW_BORDER_TOP     30
#define WINDOW_BORDER_BOTTOM  8
#define WINDOW_BORDER_SIDE    8
#define WINDOW_GAP            10

// Dir Text and Textbox
#define DIR_TEXT_X            WINDOW_GAP
#define DIR_TEXT_Y            WINDOW_GAP
#define DIR_WIDTH             WINDOW_WIDTH - WINDOW_BORDER_SIDE - WINDOW_BORDER_SIDE -WINDOW_GAP - WINDOW_GAP
#define DIR_HEIGHT            DROPDOWN_SINGLE
#define DIR_X                 DIR_TEXT_X
#define DIR_Y                 DIR_TEXT_Y + (DROPDOWN_SINGLE - WINDOW_GAP)

// File Text and Textbox
#define FILE_TEXT_X           DIR_X
#define FILE_TEXT_Y           DIR_Y + DIR_HEIGHT + WINDOW_GAP
#define FILE_WIDTH            WINDOW_WIDTH/2
#define FILE_HEIGHT           DROPDOWN_SINGLE
#define FILE_X                FILE_TEXT_X
#define FILE_Y                FILE_TEXT_Y + (DROPDOWN_SINGLE - WINDOW_GAP)

// Dropdown Text and Dropdown
#define DROPDOWN_TEXT_X       FILE_TEXT_X + FILE_WIDTH + WINDOW_GAP
#define DROPDOWN_TEXT_Y       FILE_TEXT_Y
#define DROPDOWN_WIDTH        WINDOW_WIDTH/3
#define DROPDOWN_HEIGHT       DROPDOWN_SINGLE*(NUM_FILE_TYPES+1)
#define DROPDOWN_X            DROPDOWN_TEXT_X
#define DROPDOWN_Y            DROPDOWN_TEXT_Y + (DROPDOWN_SINGLE - WINDOW_GAP)

// CTRL Text and Checkbox
#define CTRL_TEXT_X           FILE_X
#define CTRL_TEXT_Y           FILE_Y + FILE_HEIGHT + WINDOW_GAP
#define CTRL_WIDTH            CHECK_BOX_LEN
#define CTRL_HEIGHT           CHECK_BOX_LEN
#define CTRL_X                CTRL_TEXT_X + WINDOW_GAP
#define CTRL_Y                CTRL_TEXT_Y + (DROPDOWN_SINGLE - (WINDOW_GAP/2))

// ALT Text and Checkbox
#define ALT_TEXT_X            CTRL_TEXT_X + (FILE_WIDTH/2) + WINDOW_GAP
#define ALT_TEXT_Y            CTRL_TEXT_Y
#define ALT_WIDTH             CHECK_BOX_LEN
#define ALT_HEIGHT            CHECK_BOX_LEN
#define ALT_X                 CTRL_X + (FILE_WIDTH/2) + (WINDOW_GAP/2)
#define ALT_Y                 CTRL_Y

// KEY Text and Textbox
#define KEY_TEXT_X            DROPDOWN_TEXT_X
#define KEY_TEXT_Y            ALT_TEXT_Y
#define KEY_WIDTH             WINDOW_WIDTH/3
#define KEY_HEIGHT            DROPDOWN_SINGLE
#define KEY_X                 KEY_TEXT_X
#define KEY_Y                 CTRL_Y - (WINDOW_GAP/2)

// Button for ScreenShot
#define BUTTON_WIDTH          WINDOW_WIDTH - WINDOW_BORDER_SIDE - WINDOW_BORDER_SIDE
#define BUTTON_HEIGHT         50
#define BUTTON_X              0
#define BUTTON_Y              WINDOW_HEIGHT - BUTTON_HEIGHT - WINDOW_BORDER_TOP - WINDOW_BORDER_BOTTOM

///  --Externals--  ///
extern WCHAR UserSaveDir[MAX_PATH];
extern WCHAR UserFileName[MAX_PATH];
extern WCHAR UserKey[USER_KEY_LEN];
extern PCWCH FileTypes[NUM_FILE_TYPES];
extern DWORD UserFileTypeIndex;
extern bool CtrlNeeded;
extern bool AltNeeded;

#endif