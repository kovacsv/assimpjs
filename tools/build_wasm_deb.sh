# activate the path for the terminal session
eval source ./emsdk_env.sh
#cd "emsdk"
eval emcmake cmake -B ./build_wasm -DEMSCRIPTEN=1 -DCMAKE_BUILD_TYPE=Release
#cd "./build_wasm"
eval emmake make -C build_wasm -j 12
# message for user
echo "Build Succeeded."
