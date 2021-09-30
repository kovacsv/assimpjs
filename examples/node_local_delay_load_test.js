let fs = require ('fs');
const assimpjs = require ('../dist/assimpjs.js')();

assimpjs.then ((ajs) => {
    // import model
    let result = ajs.ImportFile (
		// file name
		'cube_with_materials.obj',
		// file content as arraybuffer
		fs.readFileSync ('testfiles/cube_with_materials.obj'),
		// check if file exists by name
		function (fileName) {
			return fs.existsSync ('testfiles/' + fileName);
		},
		// get file content as arraybuffer by name
		function (fileName) {
			return fs.readFileSync ('testfiles/' + fileName);
		}
	);
    
    // parse the result json
    let resultJson = JSON.parse (result);
    
    console.log (resultJson);
});
