@echo off
echo Suppression des répertoires...
rd /s /q "_deps" "CMakeFiles" "WaveGlitch_BressaniDev.dir" "WaveGlitch_BressaniDev_artefacts" "x64" ".vs" "build" "cmake-build-debug"

echo Suppression des fichiers...
del /q ALL_BUILD.vcxproj ALL_BUILD.vcxproj.filters ALL_BUILD.vcxproj.user cmake_install.cmake CMakeCache.txt CPackConfig.cmake CPackSourceConfig.cmake INSTALL.vcxproj INSTALL.vcxproj.filters WaveGlitch.sln OctaSplit.vcxproj OctaSplit.vcxproj.filters PACKAGE.vcxproj PACKAGE.vcxproj.filters ZERO_CHECK.vcxproj ZERO_CHECK.vcxproj.filters

echo Le nettoyage est terminé.
