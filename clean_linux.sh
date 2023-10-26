#!/bin/bash
# clean
echo "Clean du répertoire, nottament des fichiers ignoré par .gitignore"

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


