#!/bin/bash

OS=$(uname)

if [ "$OS" != "Linux" ]; then
    echo "Ce script doit être exécuté uniquement sur Linux!"
    exit 1
fi

# Lire les informations depuis le fichier .env
APP_NAME=$(grep "APP_NAME" .env | cut -d '=' -f2)
APP_VERSION=$(grep "APP_VERSION" .env | cut -d '=' -f2)

# clean
echo "Clean du répertoire, notamment des fichiers ignoré par .gitignore"

if [ -d "_deps" ]; then
    rm -rf _deps
fi
if [ -d "WaveGlitch_BressaniDev_artefacts" ]; then
    rm -rf WaveGlitch_BressaniDev_artefacts
fi

if [[ -f "./build.ninja" ]]; then
    rm "./build.ninja"
fi
if [[ -f "./cmake_install.cmake" ]]; then
    rm "./cmake_install.cmake"
fi
if [[ -f "./CMakeCache.txt" ]]; then
    rm "./CMakeCache.txt"
fi
if [[ -f "./CPackConfig.cmake" ]]; then
    rm "./CPackConfig.cmake"
fi
if [[ -f "./CPackSourceConfig.cmake" ]]; then
    rm "./CPackSourceConfig.cmake"
fi


# Supprimer le dossier de build s'il existe
if [ -d "build" ]; then
  rm -rf build
fi

# Créer un dossier de build si il n'existe pas
mkdir -p build
cd build

# Supprimer le répertoire 'bin' s'il existe
if [ -d "bin" ]; then
  rm -r bin
fi

# Générer les fichiers de projet avec CMake
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64" ..
#cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" ..

# Compiler le projet
cmake --build . --config Release

# Si vous souhaitez copier le fichier exécutable dans un emplacement spécifique, ajoutez la commande ici
# par exemple :
# cp WaveGlitch.app /chemin/vers/emplacement/souhaité
#/build/WaveGlitch_BressaniDev_artefacts/Release



mkdir bin
#cp -rf ./WaveGlitch_BressaniDev_artefacts/Release ./bin/opt/WaveGlitch
cd bin

mkdir -p WaveGlitch-deb/DEBIAN
#mkdir -p WaveGlitch-deb/usr/bin
mkdir -p WaveGlitch-deb/usr/share/applications
mkdir -p WaveGlitch-deb/usr/share/pixmaps
mkdir -p WaveGlitch-deb/opt/
cp -rf ../WaveGlitch_BressaniDev_artefacts/Release ./WaveGlitch-deb/opt/WaveGlitch
chmod +x ./WaveGlitch-deb/opt/WaveGlitch/WaveGlitch
cp ../../.env WaveGlitch-deb/opt/WaveGlitch/
cp -r ../../Assets/ WaveGlitch-deb/opt/WaveGlitch/
#cp WaveGlitch WaveGlitch-deb/usr/bin
cp ../../icon/icon.png WaveGlitch-deb/usr/share/pixmaps/WaveGlitch.png

# Créer ou écraser le fichier WaveGlitch.desktop avec le contenu approprié
cat <<EOL > WaveGlitch-deb/usr/share/applications/WaveGlitch.desktop
[Desktop Entry]
Version=$APP_VERSION
Name=$APP_NAME
Comment=Description de $APP_NAME
Exec=/opt/WaveGlitch/WaveGlitch
Icon=/usr/share/pixmaps/WaveGlitch.png
Terminal=false
Type=Application
Categories=Utility;
EOL

echo "Fichier .desktop créé avec succès!"

cat <<EOL > WaveGlitch-deb/DEBIAN/control
Package: $APP_NAME
Version: $APP_VERSION
Section: base
Priority: optional
Architecture: amd64
Maintainer: Stéphane Bressani <stephane@bressani.dev>
Description: WaveGlitch est un utilitaire pour générer depuis un fichier audio les formats répété (pour faire du glitch): 1/1 1/2 1/4 ... 1/128.
EOL

echo "Fichier control créé avec succès!"

cd ..
dpkg-deb --build ./bin/WaveGlitch-deb
#sudo dpkg -i WaveGlitch-deb.deb

echo "Compilation terminée."
