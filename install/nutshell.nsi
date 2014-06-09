;NSIS Modern User Interface
;Welcome/Finish Page Example Script
;Written by Joost Verburg
;HM5-8HNR4F-33VX-DY8ZEA Code: AC3498
;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

   !define VERSION "4.84"
   !define DATE "[140223]"

  ;Name and file
  Name "NutShell  ${VERSION} for PCRaster 4.0.0"
  OutFile "D:\prgc\PCR\pcraster-nutshell\install\nutshell${VERSION}setup.exe"
  
  ;Default installation folder
  InstallDir "c:\praster4.0.0\bin"
  ;$PROGRAMFILES\pcraster\apps
   
  ;Get installation folder from registry if available
  ;InstallDirRegKey HKLM "Software\PCRaster\PCRaster\" "" 
  ;InstallDirRegKey HKLM "Software\NutShell" "installPath"
  ;InstallDirRegKey HKLM "Software\PCRaster\Install\" "appPath"
  
  ;Request application privileges for Windows Vista
  RequestExecutionLevel user
;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages
  !define MUI_ICON "nutshell1.ico"
	!define MUI_WELCOMEFINISHPAGE_BITMAP "nutshell1.bmp"
	!define MUI_WELCOMEFINISHPAGE_BITMAP_NOSTRETCH
  !define MUI_WELCOMEPAGE_TEXT "Installation of NutShell version ${VERSION}, compile date ${DATE} for PCRaster. Contains also mapedit version 1.11. \
  NutShell is a windows shell for the freeware PCRaster environmental modelling language and raster GIS. \
  The PCRaster software can be found here: http://pcraster.geo.uu.nl. \
  This version is updated for PCRaster 4.0.0"

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE license.txt
  !define MUI_DIRECTORYPAGE_TEXT_TOP "Please select the [...]/PCRaster4.0.0/bin folder. NutShell will be copied in a subdirectory nutshell."
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
    ;Start Menu Folder Page Configuration
  ;!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
  ;!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Nutshell" 
  ;!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
    ;!insertmacro MUI_PAGE_STARTMENU "NutShell" $StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES
  !define MUI_FINISHPAGE_NOAUTOCLOSE
  !define MUI_FINISHPAGE_SHOWREADME "README.txt" ;nutshellhelp.pdf"
  !define MUI_FINISHPAGE_SHOWREADME_NOTCHECKED
  !insertmacro MUI_PAGE_FINISH

  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

  
;--------------------------------
;Installer Sections

Section "Main files" SecDummy

  SetOutPath "$INSTDIR\nutshell\"
  
  ;ADD YOUR OWN FILES HERE...
  File D:\prgc\PCR\pcraster-nutshell\release\vc\nutshell.exe
  File d:\prgc\PCR\pcraster-nutshell\release\vc\*.dll
  File d:\prgc\PCR\pcraster-nutshell\install\nutshellhelp.pdf
  File d:\prgc\PCR\pcraster-nutshell\install\README.txt

;  SetOutPath "$INSTDIR\"
  File mapedit.exe

  
 ; CopyFiles "$PROFILE\application data\nutshell\nutshell.ini" "$PROFILE\application data\nutshell\nutshell.${VERSION}"
  
  CreateShortCut "$DESKTOP\PCRaster NutShell.lnk" "$INSTDIR\nutshell\NutShell.exe"
  
SectionEnd

Section /o "Delete ini" SecDummy1

  SetOutPath "$INSTDIR\nutshell\"  

  CopyFiles "$PROFILE\application data\nutshell\nutshell.ini" "$PROFILE\application data\nutshell\nutshell.${VERSION}"
  
  CreateShortCut "$DESKTOP\PCRaster NutShell.lnk" "$INSTDIR\nutshell\NutShell.exe"
  
SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
 ; LangString DESC_SecDummy ${LANG_ENGLISH} "NutShell exe and help files"
 ; LangString DESC_SecDummy1 ${LANG_ENGLISH} "Backup and delete old ini file"

  ;Assign language strings to sections
;  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;  !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
;  !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy1} $(DESC_SecDummy1)
 ; !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
 
