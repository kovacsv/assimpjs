let fs = require ('fs');

const assimpjs = require ('./assimpjs.js')();
assimpjs.then ((ajs) => {
	let fileList = new ajs.FileList ();
	fileList.SetPrimaryFile ('cube_with_materials.obj', fs.readFileSync ('testfiles/cube_with_materials.obj'));
	fileList.AddSecondaryFile ('cube_with_materials.mtl', fs.readFileSync ('testfiles/cube_with_materials.mtl'));
	let result = ajs.ImportFile (fileList);
	let resultJson = JSON.parse (result);
	console.log (resultJson);
});
