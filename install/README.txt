nutshell for PCRaster version 4.0
========================================
programmed in Qt and compiled with MSVC2010
Opensource on request and can be compiled in linux (not tried yet!)

Changes since 3.5:
Script editor:
- context sensitive help when connected to the web.
- Improved find (Ctrl-F) and find-repace (Ctrl-H): press F3 and the word under the cursor will be found, F3 for next and Shift-F3 for previous
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

Date: 14 Oct 2012