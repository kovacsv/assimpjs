var fs = require ('fs');
var assert = require ('assert');

var config = 'Debug'
if (process.env.TEST_CONFIG !== undefined) {
	config = process.env.TEST_CONFIG;
}
console.log (config);

var assimpjs = require ('../em_build/' + config + '/assimpjs.js')();

var ajs = null;
before (async function () {
	ajs = await assimpjs;
});

function LoadModel (files)
{
	let fileList = new ajs.FileList ();
	for (let i = 0; i < files.length; i++) {
		fileList.AddFile (files[i], fs.readFileSync (files[i]))
	}
	return ajs.ImportModel (fileList);
}

function IsError (files)
{
	return LoadModel (files) === "error";
}

function IsSuccess (files)
{
	return LoadModel (files) !== "error";
}

describe ('Importer', function () {

it ('Error', function () {
	assert (IsError ([]));
});

it ('3DS', function () {
	assert (IsSuccess (['../assimp/test/models/3DS/cubes_with_alpha.3DS']));
});

});
