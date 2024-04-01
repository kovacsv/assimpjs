# check if emsdk path is there
if [ ! -d "emsdk" ]; then
  # clone if path is missing
  eval git clone --recursive https://github.com/emscripten-core/emsdk.git
  # enter emsdk path
  cd "emsdk"
  # install the version 3.1.56
  eval ./emsdk install 3.1.56
  # activate the version 3.1.56
  eval ./emsdk activate 3.1.56
  # activate the path for the terminal session
  eval source ./emsdk_env.sh
else
  echo "Emscripten already set up!"
  # enter emsdk path
  cd "emsdk"
  # activate the path for the terminal session
  eval source ./emsdk_env.sh
fi
