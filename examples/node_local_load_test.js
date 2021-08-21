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
    
    console.log (resultJson);
});
