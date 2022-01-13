function GetFileBuffer (file)
{
	return new Promise ((resolve, reject) => {
		let reader = new FileReader ();
		reader.onloadend = function (event) {
			if (event.target.readyState === FileReader.DONE) {
				resolve (event.target.result);
			}
		};
		reader.onerror = function () {
			reject ();
		};
		reader.readAsArrayBuffer (file);
	});		
}

function LoadModel (ajs, files, onSuccess, onError)
{
	let downloadFiles = [];
	for (let i = 0; i < files.length; i++) {
		downloadFiles.push (GetFileBuffer (files[i]));
	}
	Promise.all (downloadFiles).then ((arrayBuffers) => {
		let fileList = new ajs.FileList ();
		for (let i = 0; i < arrayBuffers.length; i++) {
			fileList.AddFile (files[i].name, new Uint8Array (arrayBuffers[i]));
		}
		let result = ajs.ConvertFileList (fileList, 'glb2');
		if (!result.IsSuccess () || result.FileCount () != 1) {
			onError (result.GetErrorCode ());
		} else {
			let resultFile = result.GetFile (0);
			let resultFileContent = resultFile.GetContent ();
			onSuccess (resultFileContent);
		}
	}).catch (() => {
		onError ('failed_to_load_file');
	});
}

function ResizeViewer (viewer, resultDiv)
{
	let width = resultDiv.clientWidth - 20;
	let height = window.innerHeight - resultDiv.offsetTop - 40;
	viewer.Resize (width, height);
}

window.onload = function ()
{
	let dragDropDiv = document.getElementById ('dragdrop');
	let resultModelDiv = document.getElementById ('result_model');
	let resultErrorDiv = document.getElementById ('result_error');
	resultModelDiv.style.display = 'none';
	resultErrorDiv.style.display = 'none';

	let viewer = new Viewer ();
	viewer.Init (resultModelDiv);

    window.addEventListener ('resize', () => {
        ResizeViewer (viewer, resultModelDiv);
    });

	let texts = {
		loadingAssimpJS : 'LOADING ASSIMPJS...',
		loadingModel : 'LOADING MODEL...',
		dragDrop : 'DRAG & DROP YOUR FILES'
	};
	dragDropDiv.innerHTML = texts.loadingAssimpJS;
	assimpjs ().then (function (ajs) {
		dragDropDiv.innerHTML = texts.dragDrop;
		window.addEventListener ('dragstart', (ev) => {
			ev.preventDefault ();
		}, false);
		window.addEventListener ('dragover', (ev) => {
			ev.stopPropagation ();
			ev.preventDefault ();
			ev.dataTransfer.dropEffect = 'copy';
		}, false);
		window.addEventListener ('drop', (ev) => {
			ev.stopPropagation ();
			ev.preventDefault ();
			let files = ev.dataTransfer.files;
			if (files.length > 0) {
				dragDropDiv.innerHTML = texts.loadingModel;
				resultModelDiv.style.display = 'none';
				resultErrorDiv.style.display = 'none';
				setTimeout (() => {
					LoadModel (ajs, files,
						(resultBuffer) => {
							dragDropDiv.innerHTML = texts.dragDrop;
							viewer.LoadGlbBuffer (resultBuffer, () => {
								resultModelDiv.style.display = 'block';
								ResizeViewer (viewer, resultModelDiv);
								viewer.Render ();
							});
						},
						(errorCode) => {
							dragDropDiv.innerHTML = texts.dragDrop;
							resultErrorDiv.style.display = 'block';
							resultErrorDiv.innerHTML = '<div>Import failed with error code: ' + errorCode + '.</div>';
						}
					);
				}, 10);
			}
		}, false);
	});
};
