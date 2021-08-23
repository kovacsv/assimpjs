pushd %~dp0

call %1\emsdk_env.bat
rem -DASSIMP_NO_EXPORT=1
call emcmake cmake -B em_build -G "Unix Makefiles" -DEMSCRIPTEN=1 -DCMAKE_MAKE_PROGRAM=mingw32-make -DCMAKE_BUILD_TYPE=%2 -DASSIMP_BUILD_TESTS=0 -DASSIMP_BUILD_ASSIMP_TOOLS=0 -DBUILD_SHARED_LIBS=0 . || goto :error
call emmake mingw32-make -C em_build || goto :error
popd
echo Build Succeeded.

popd
exit /b 0

:error
echo Build Failed with Error %errorlevel%.
popd
popd
exit /b 1
