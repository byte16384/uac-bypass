# UAC bypass

## PoC
![poc](https://github.com/user-attachments/assets/91731146-f76f-4499-84c5-506f852b0188)

## How it works
create a key in HKEY_CURRENT_USER\Software\Classes\ms-settings\Shell\Open\command and the default to cmd.exe
Set DeleteExecute to an empty value

Run fodhelper.exe
The reason is involved in Windows settings so its in the state of autoElevate find ms-settings in the registry and run the command Because of the key you modified earlier cmd.exe is automatically executed with administrator privileges




###### I will never take responsibility for your actions
###### Please dont hold me accountable even if it causes damage by exploiting it


