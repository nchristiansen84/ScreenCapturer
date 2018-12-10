# ScreenCapturer
Windows user mode tool to take a screenshot with specified key combination and save it to user specified file location.

Main purpose was to learn about basic GUI in Windows and get information from different Windows (dropdowns, checkboxes, edit fields, etc)

Initial version of program can take a screenshot by clicking a button or a user specified key combination. Currently key combination has to be alphabetical with optionally used with Ctrl and/or Alt

<Pre>
Run ScreenCapturer.exe and fill out fields:
  - "Directory Path:" User can enter an absolute or relative (from ScreenCapturer.exe) path for the directory where the screenshot will be saved. Default is C:\ScreenShots\
  - "File Name:" User can enter a file name to be prepended to an underscore then timestamp, which makes up the files full name. Default is only timestamp
  - "File Type:" User can select the file type (.bmp, .jpg, or .png) that the screenshot will be saved as from from dropdown. Default is .jpg
  - "CTRL" User can check this checkbox if they want to have Ctrl as part of the key combination to take a screenshot. Default is unchecked
  - "Alt" User can select this checkbox if they want to have Alt as part of the key combination to take a screenshot. Default is unchecked
  - "Key:" User can enter an alphabetical character which will take a screenshot (even when application is in background) unless needing to be in combination with Ctrl and/or Alt. Defaults is nothing
  - "ScreenShot" User can click this button to take a screenshot.
</Pre>
