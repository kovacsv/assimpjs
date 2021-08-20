let fs = require ('fs');

const assimpjs = require ('./assimpjs.js')();
assimpjs.then ((ajs) => {
	let fileList = new ajs.FileList ();
	fileList.AddFile ('cube_with_materials.3ds', fs.readFileSync ('testfiles/cube_with_materials.3ds'))
	let result = ajs.ImportFile (fileList);
	let resultJson = JSON.parse (result);
	console.log (resultJson);
});
