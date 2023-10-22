#!/bin/sh

# Créez un répertoire temporaire pour les icônes
mkdir MyIcon.iconset

# Utilisez sips pour redimensionner votre image en différentes tailles requises pour un fichier .icns
sips -z 16 16     ./icon.png --out MyIcon.iconset/icon_16x16.png
sips -z 32 32     ./icon.png --out MyIcon.iconset/icon_16x16@2x.png
sips -z 32 32     ./icon.png --out MyIcon.iconset/icon_32x32.png
sips -z 64 64     ./icon.png --out MyIcon.iconset/icon_32x32@2x.png
sips -z 128 128   ./icon.png --out MyIcon.iconset/icon_128x128.png
sips -z 256 256   ./icon.png --out MyIcon.iconset/icon_128x128@2x.png
sips -z 256 256   ./icon.png --out MyIcon.iconset/icon_256x256.png
sips -z 512 512   ./icon.png --out MyIcon.iconset/icon_256x256@2x.png
sips -z 512 512   ./icon.png --out MyIcon.iconset/icon_512x512.png
sips -z 1024 1024 ./icon.png --out MyIcon.iconset/icon_512x512@2x.png

# Utilisez iconutil pour convertir le répertoire d'icônes en un fichier .icns
iconutil -c icns MyIcon.iconset

# Supprimez le répertoire temporaire des icônes
rm -R MyIcon.iconset