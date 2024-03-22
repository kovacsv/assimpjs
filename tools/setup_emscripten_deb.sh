# check if emsdk path is there
if [ ! -d "emsdk" ]; then
  # clone if path is missing
  eval git clone --recursive https://github.com/emscripten-core/emsdk.git
  wait
  # enter emsdk path
  cd "emsdk"
  # install the latest version
  eval ./emsdk install latest
  # activate the latest version
  eval ./emsdk activate latest
  # activate the path for the terminal session
  eval source ./emsdk_env.sh
else
  echo "Emscripten already set up!"
  # enter emsdk path
  cd "emsdk"
  # activate the path for the terminal session
  eval source ./emsdk_env.sh
fi
wait


