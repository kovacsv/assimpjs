# assimpjs

The [emscripten](https://emscripten.org) port of the [assimp](https://github.com/assimp/assimp) library. It runs entirely in the browser, and allows you to import assimp's 40+ 3D file formats, and access the result in JSON format.

[![Native Build](https://github.com/kovacsv/assimpjs/actions/workflows/native_build.yml/badge.svg)](https://github.com/kovacsv/assimpjs/actions/workflows/native_build.yml)
[![Emscripten Build](https://github.com/kovacsv/assimpjs/actions/workflows/emscripten_build.yml/badge.svg)](https://github.com/kovacsv/assimpjs/actions/workflows/emscripten_build.yml)

## How to use?

The library runs in the browser, and as a node module as well.

Given that browsers don't access the file system, you should provide all the files needed for import. Some 3D formats are coming in multiple files, so you should list all of them to import the model properly. The main file to import is called primary file, all the others are called secondary files.

You should provide two things for every file:
- **name:** The name of the file. It's used if files are referring each other.
- **content:** The content of the file as an `Uint8Array` object.

### Use from the browser

You will need the `assimpjs.js`, and the `assimpjs.wasm` files.

...

### Use as a node module

```js
let fs = require ('fs');
const assimpjs = require ('./assimpjs.js')();

assimpjs.then ((ajs) => {
    // let's create a file list object
    let fileList = new ajs.FileList ();
    
    // add primary file
    fileList.SetPrimaryFile ('cube_with_materials.obj', fs.readFileSync ('testfiles/cube_with_materials.obj'))
    
    // add secondary files if needed
    fileList.AddSecondaryFile ('cube_with_materials.mtl', fs.readFileSync ('testfiles/cube_with_materials.mtl'))
    
    // import the files
    let result = ajs.ImportFile (fileList);
    
    // parse the result as JSON
    let resultJson = JSON.parse (result);
});
```
