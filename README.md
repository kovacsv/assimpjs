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

## How to build on Windows?

A set of batch scripts are prepared for building on Windows.

### 1. Install Prerequisites

Install [CMake](https://cmake.org) (3.6 minimum version is needed). Make sure that the cmake executable is in the PATH.

### 2. Install Emscripten SDK

Run the Emscripten setup script.

```
tools\build_setup_emscripten_win.bat
```

### 3. Compile the WASM library

Run the release build script.

```
tools\build_wasm_win_release.bat
```

### 4. Build the native project (optional)

If you want to debug the code, it's useful to build a native project. To do that, just use cmake to generate the project of your choice.
