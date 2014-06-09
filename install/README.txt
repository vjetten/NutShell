==============================================================================================================
nutshell for PCRaster version 4.83
==============================================================================================================
Date: 23 Feb 2014
Author: Victor Jetten
programmed in Qt (http://qt-project.org) and compiled with MSVC 2010 (freeware)
Opensource on request and can be compiled in linux (not tried yet!)

Default install location(!):  ../pcraster4.0.0/bin/nutshell
Do *NOT* install in the pcraster4.0.0/bin directory, nutshell uses different Qt dlls which
If you are blocked because of admin rights, NutShell can be installed anywhere else.
---------------------------------------------------------------------------------------------------------------
Changes 4.8-4.84
- Synced with PCRaster 4.0.0 install, help system etc
- free splitter window movement
- nutshell.ini now in nutshell.exe directory, no more default windows user locations
- correct resizing of columns in explorer

Changes 4.6-4.8:
- Added cmd file to list of pcraster files
- Added open new script with basic key words

Changes 4.5:
- Fixed error causing a crash when wrong command is typed
- Command resulting in file not found now gives back an error (e.g. typing pcralc instead of pcrcalc)
- Command causing a windows error no longer added to the commandlist
- Fixed bug removing directories form the workdir list
- TODO: legend editing sometimes still gives crashes

Changes 4.4 - 4.2:
- Double click no longer displays variable (too irritating), only F4
- Bug fixes report script variable and delete files and series
- Bug fixes display variables from script

Changes 4.2 - 4.1:
- Double click in editor nolonger displays variable (use F4)
- Ctrl-D or Ctrl-Y deletes line in editor
- Fixed bug in map attributes and new map
- Fixed bug in accidentally deleting wrong mapseries
- Added highlighting repeat-until loops (blue)

Changes 3.5 - 4.1:
- Added graphical NutShellhelp (ALt-F1)
Script editor:
- context sensitive help when connected to the web.
- Improved find (Ctrl-F) and find-repace (Ctrl-H): press F3 and the word under the cursor will be found, 
  F3 for next and Shift-F3 for previous
- toggle for highlighted section: indent/unindent, "##", "report" (Alt-I, Alt-U, Alt-3, Alt-4) 
- link variables in script to filenames and show directly (F4 and F6)
- (un)fold sections binding,areamap,timer,initial,dynamic by double clicking on the section header
- better argument substitution functionality
Explorer:
- show only reported files from current script in explorer (including argument substitution and map series)
- delete all reports from current script (including argument substitution and map series)
- drag and drop functionality between files and directories (default move or ctrl-mouse copy), including map series
- better integration of mapseries in explorer, such as mapseries by pressing delete on first file (e.g. name0000.001)
- save a list of workdirectories
- numerous small bug fixes

