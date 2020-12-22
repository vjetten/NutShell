==============================================================================================================
nutshell for PCRaster version 5.10
==============================================================================================================
Date: 10 Sep 2020
Author: Victor Jetten
uses pcraster 4.3.0 or lower (pcraster.geo.uu.nl)
can use GDAL (gdal.org)
---------------------------------------------------------------------------------------------------------------

NOTE DO *NOT* copy the contents of this zip file in the pcraster/bin directory. PCRaster uses different versions of the Qt dll files.

Changes 5.10
- fixed a bug in finding the conda installation
- added a save button next to the commandline dropdon list to save commands in between in nutshell.ini (in case of crashes)

Changes 5.9
- Automaticaly find Conda installation of PCRaster, which includes GDAL (much easier)
- fixed batchfiles

Changes 5.4-5.7
- fixed bugs to better work with GDAL
- better works with very high res screens
- works with latest PCRaster versions
- includes mapedit

Changes 5.1-5.4
= font control for high res screens
- fixed minor interface bugs
- fixed bug in executing CMD file 
- added platforms in install zip file

Changes 4.86-5.1
- added surpport for GDAL operations
- added support for windows/DOS CMD batchfiles, run from explorer, or run from editor window like a script (incl argument substitution)
- added conversion button for map to tiff (using gdal)
- map attributes on a tif file uses gdalinfo
- simplified links to PCRaster and GDAL directories
- assumed MapEdit is in the smae directory as NutShell
- Work directory lists dropbox editable, copy-paste explorer path in it
- fixed a bug in delete script report files
- fixed behaviour of find-replace dialog
 
Changes 4.85-4.86
- Added option to show TIF maps
- Added option to include path to GDAL installation
- Added option GDAL commands fror the command window

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

