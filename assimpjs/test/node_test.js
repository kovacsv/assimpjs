const ajs = require ('./assimpjs.js')();
ajs.then ((a) => {
	let fileList = new a.FileList ();
	console.log (a.ImportFile (fileList));
});
