#!/bin/bash

# Set variables
APPNAME="nutshell"
EXECUTABLE="release/nutshell"  # Your compiled binary
ICON="resources/nutshell1.png"
DESKTOPFILE="nutshell.desktop"

# User-local install locations
APPDIR="$HOME/.local/bin"
DESKTOPDIR="$HOME/.local/share/applications"
ICONSIZEDIR="$HOME/.local/share/icons/hicolor/128x128/apps"

# Create directories if needed
mkdir -p "$APPDIR" "$DESKTOPDIR" "$ICONSIZEDIR"

# Copy executable
cp "$EXECUTABLE" "$APPDIR/"

# Copy icon
cp "$ICON" "$ICONSIZEDIR/$APPNAME.png"

# Copy .desktop file and fix Exec path
sed "s|Exec=.*|Exec=$APPDIR/$APPNAME|" "$DESKTOPFILE" > "$DESKTOPDIR/$APPNAME.desktop"
chmod +x "$APPDIR/$APPNAME"
chmod +r "$ICONSIZEDIR/$APPNAME.png"
chmod +r "$DESKTOPDIR/$APPNAME.desktop"

# Update icon cache (optional, may require gtk-update-icon-cache)
if command -v gtk-update-icon-cache >/dev/null 2>&1; then
    gtk-update-icon-cache "$HOME/.local/share/icons/hicolor"
fi

echo "NutShell installed! You can find it in your application menu."