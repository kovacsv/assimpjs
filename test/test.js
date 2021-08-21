var fs = require ('fs');
var path = require ('path');
var assert = require ('assert');

var config = 'Debug'
if (process.env.TEST_CONFIG !== undefined) {
	config = process.env.TEST_CONFIG;
}
var assimpjs = require ('../em_build/' + config + '/assimpjs.js')();

var ajs = null;
before (async function () {
	if (ajs !== null) {
		return;
	}
	ajs = await assimpjs;
});

function LoadModel (files)
{
	let fileList = new ajs.FileList ();
	for (let i = 0; i < files.length; i++) {
		let filePath = path.join (__dirname, '../assimp/test/models/' + files[i]);
		fileList.AddFile (filePath, fs.readFileSync (filePath))
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

it ('Empty file list', function () {
	assert (IsError ([]));
});

it ('Not importable file', function () {
	assert (IsError (['3DS/test.png']));
});

it ('Independent order', function () {
	assert (IsSuccess (['OBJ/cube_usemtl.obj', 'OBJ/cube_usemtl.mtl']));
	assert (IsSuccess (['OBJ/cube_usemtl.mtl', 'OBJ/cube_usemtl.obj']));
});

it ('3D', function () {
	assert (IsSuccess (['3D/box.uc', '3D/box_a.3d', '3D/box_d.3d']));
});

it ('3DS', function () {
	assert (IsSuccess (['3DS/test1.3ds']));
	assert (IsSuccess (['3DS/fels.3ds']));
	assert (IsSuccess (['3DS/cubes_with_alpha.3DS']));
	assert (IsSuccess (['3DS/cube_with_specular_texture.3DS']));
	assert (IsSuccess (['3DS/cube_with_diffuse_texture.3DS']));
});

it ('3MF', function () {
	assert (IsSuccess (['3MF/box.3mf']));
});

it ('AC', function () {
	assert (IsSuccess (['AC/SphereWithLight.ac']));
});

it ('AMF', function () {
	assert (IsSuccess (['AMF/test_with_mat.amf']));
});

it ('ASE', function () {
	assert (IsSuccess (['ASE/ThreeCubesGreen.ASE']));
});

it ('B3D', function () {
	assert (IsSuccess (['B3D/WusonBlitz.b3d']));
});

it ('BLEND', function () {
	// TODO
	assert (IsError (['BLEND/box.blend']));
});

it ('BVH', function () {
	assert (IsSuccess (['BVH/Boxing_Toes.bvh']));
});

it ('COB', function () {
	assert (IsSuccess (['COB/molecule.cob']));
});

it ('COLLADA', function () {
	assert (IsSuccess (['COLLADA/duck.dae']));
	assert (IsSuccess (['COLLADA/duck.zae']));
});

it ('CSM', function () {
	assert (IsSuccess (['CSM/ThomasFechten.csm']));
});

it ('DXF', function () {
	// TODO
	assert (IsError (['DXF/PinkEggFromLW.dxf']));
});

it ('OBJ', function () {
	assert (IsSuccess (['OBJ/spider.obj']));
	assert (IsSuccess (['OBJ/cube_usemtl.obj', 'OBJ/cube_usemtl.mtl']));
});

});
