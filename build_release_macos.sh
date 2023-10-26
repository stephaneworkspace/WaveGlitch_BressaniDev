#!/bin/bash

OS=$(uname)

if [ "$OS" != "Darwin" ]; then
    echo "Ce script doit être exécuté uniquement sur macOS!"
    exit 1
fi

# clean
echo "Clean du répertoire, nottament des fichiers ignoré par .gitignore"

if [ -d "_deps" ]; then
    rm -rf _deps
fi
if [ -d "WaveGlitch_BressaniDev_artefacts" ]; then
    rm -rf WaveGlitch_BressaniDev_artefacts
fi
if [ -d "CMakeFiles" ]; then
    rm -rf CMakeFiles
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

mkdir bin
cp -rf ./WaveGlitch_BressaniDev_artefacts/Release/WaveGlitch.app ./bin/WaveGlitch.app
cd bin
mkdir installer
create-dmg 'WaveGlitch.app' ./installer

echo "Compilation terminée."
