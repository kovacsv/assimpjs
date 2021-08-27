# assimpjs

The [emscripten](https://emscripten.org) interface for the [assimp](https://github.com/assimp/assimp) library. It runs entirely in the browser, and allows you to import 40+ 3D file formats and access the result in JSON format. This is not a full port of assimp, but an easy to use interface to access it's functionality.

[![Native Build](https://github.com/kovacsv/assimpjs/actions/workflows/native_build.yml/badge.svg)](https://github.com/kovacsv/assimpjs/actions/workflows/native_build.yml)
[![Emscripten Build](https://github.com/kovacsv/assimpjs/actions/workflows/emscripten_build.yml/badge.svg)](https://github.com/kovacsv/assimpjs/actions/workflows/emscripten_build.yml)

## How to use?

The library runs in the browser and as a node.js module as well.

You will need two files: `assimpjs.js` and `assimpjs.wasm`. The wasm file is loaded runtime by the js file.

Given that browsers don't access the file system, you should provide all the files needed for import. Some 3D formats are coming in multiple files, so you should list all of them to import the model properly. The main file to import should always be the first one in the list.

You should provide two things for every file:
- **name:** The name of the file. It's used if files are referring to each other.
- **content:** The content of the file as an `Uint8Array` object.

### Use from the browser

First, include the `assimpjs.js` file in your website.

```html
<script type="text/javascript" src="assimpjs.js"></script>
```

After that, download the model files, and pass them to assimpjs.

```js
assimpjs ().then (function (ajs) {
    // fetch the files to import (make sure that the main file is the first)
    let files = [
        'testfiles/cube_with_materials.obj',
        'testfiles/cube_with_materials.mtl'
    ];
    Promise.all (files.map ((file) => fetch (file))).then ((responses) => {
        return Promise.all (responses.map ((res) => res.arrayBuffer ()));
    }).then ((arrayBuffers) => {
        // create new file list object
        let fileList = new ajs.FileList ();
        for (let i = 0; i < files.length; i++) {
            fileList.AddFile (files[i], new Uint8Array (arrayBuffers[i]));
        }
        
        // import model
        let result = ajs.ImportModel (fileList);
        
        // parse the result json
        let resultJson = JSON.parse (result);
    });
});
```

### Use as a node.js module

You should require the `assimpjs.js` module in your script. In node.js you can use the file system module to get the buffer of each file.

```js
let fs = require ('fs');
const assimpjs = require ('./assimpjs.js')();

assimpjs.then ((ajs) => {
    // create new file list object
    let fileList = new ajs.FileList ();
    
    // add model files (make sure that the main file is the first)
    fileList.AddFile (
        'cube_with_materials.obj',
        fs.readFileSync ('testfiles/cube_with_materials.obj')
    );
    fileList.AddFile (
        'cube_with_materials.mtl',
        fs.readFileSync ('testfiles/cube_with_materials.mtl')
    );
    
    // import model
    let result = ajs.ImportModel (fileList);
    
    // parse the result json
    let resultJson = JSON.parse (result);
});
```

## How to build?

Build instructions are set up for windows, but they are basically the same for all platforms.

### 1. Install Prerequisites

You have to install the following software:
- [CMake](https://cmake.org) (3.6 minimum version is needed).

### 2. Install Emscripten SDK

You can find detailed instruction on [Emscripten Download and install](https://emscripten.org/docs/getting_started/downloads.html) page. You have to use the following commands to set up the environment:

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
git pull
emsdk install latest
emsdk activate latest
emsdk install mingw-4.6.2-32bit
emsdk activate mingw-4.6.2-32bit
```

### 3. Generate the Visual Studio Project (Optional)

You can create the project with CMake, and build it with Visual Studio. This step is optional. The Visual Studio project is useful if you would like to debug the functionalities, but it is not necessary for building the web application.

```
cmake -B build -G "Visual Studio 16 2019"
```

### 4. Generate the Web Application

The easiest way is to run the `em_build_win.bat` file from the root directory. It needs two command line parameters:
- The Emscripten SDK folder (e.g. "C:\Emscripten\emsdk").
- The build configuration type (Debug or Release).

```
em_build_win.bat C:\Emscripten\emsdk Release
```

