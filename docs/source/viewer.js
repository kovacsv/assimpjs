class Viewer
{
    constructor ()
    {
        this.renderer = null;
        this.scene = null;
        this.camera = null;
        this.light = null;
        this.controls = null;
        this.mainObject = null;
    }

    Init (parentElement)
    {
        let width = 800;
        let height = 600;
        this.renderer = new THREE.WebGLRenderer ({
            antialias : true
        });
        parentElement.appendChild (this.renderer.domElement);
    
        this.renderer.setClearColor (0x222222, 1);
        this.renderer.setSize (width, height);
    
        this.scene = new THREE.Scene ();
    
        this.light = new THREE.DirectionalLight (0x555555);
        this.light.position.set (0.0, 0.0, 0.0);
        this.scene.add (this.light);
    
        this.camera = new THREE.PerspectiveCamera (45.0, width / height, 0.1, 1000.0);
        this.camera.position.set (1.5, 2.0, 3.0);
        this.camera.up.set (0.0, 1.0, 0.0);
        this.camera.lookAt (new THREE.Vector3 (0.0, 0.0, 0.0));
        this.scene.add (this.camera);
    
        this.controls = new THREE.OrbitControls (this.camera, this.renderer.domElement);
        this.controls.addEventListener ('change', () => {
            this.Render ();
        });
    
        let textureLoader = new THREE.CubeTextureLoader ();
        let textures = [
            'images/fishermans_bastion/posx.jpg',
            'images/fishermans_bastion/negx.jpg',
            'images/fishermans_bastion/posy.jpg',
            'images/fishermans_bastion/negy.jpg',
            'images/fishermans_bastion/posz.jpg',
            'images/fishermans_bastion/negz.jpg'
        ];
        this.scene.environment = textureLoader.load (textures, () => {
            this.Render ();
        });
    }

    LoadGlbBuffer (glbBuffer, onReady)
    {
        if (this.mainObject !== null) {
            this.scene.remove (this.mainObject);
        }

        let blob = new Blob ([glbBuffer]);
        let url = URL.createObjectURL (blob);
        let gltfLoader = new THREE.GLTFLoader ();
        gltfLoader.load (url,
            (gltf) => {
                this.mainObject = gltf.scene;
                this.scene.add (this.mainObject);

                this.mainObject.updateWorldMatrix (true, true);
                let boundingBox = new THREE.Box3 ().setFromObject (this.mainObject);
                let boundingSphere = new THREE.Sphere ();
                boundingBox.getBoundingSphere (boundingSphere);                
        
                this.FitToSphere (boundingSphere);
                this.AdjustClippingPlanes (boundingSphere);
                onReady ();
            }
        );
    }

    FitToSphere (boundingSphere)
    {
        let fieldOfView = this.camera.fov / 2.0;
        if (this.camera.aspect < 1.0) {
            fieldOfView = fieldOfView * this.camera.aspect;
        }

        let cameraDir = this.camera.position.clone ().sub (this.controls.target).normalize ();

        this.controls.target = boundingSphere.center;
        let distance = boundingSphere.radius / Math.sin (THREE.Math.degToRad (fieldOfView));
    
        let cameraPos = this.controls.target.clone ().add (cameraDir.clone ().multiplyScalar (distance));
        this.camera.position.set (cameraPos.x, cameraPos.y, cameraPos.z);
        this.controls.update();
    }

    AdjustClippingPlanes (boundingSphere)
    {
        if (boundingSphere.radius < 10.0) {
            this.camera.near = 0.01;
            this.camera.far = 100.0;
        } else if (boundingSphere.radius < 100.0) {
            this.camera.near = 0.1;
            this.camera.far = 1000.0;
        } else if (boundingSphere.radius < 1000.0) {
            this.camera.near = 10.0;
            this.camera.far = 10000.0;
        } else {
            this.camera.near = 100.0;
            this.camera.far = 1000000.0;
        }

        this.camera.updateProjectionMatrix ();        
    }

    Resize (width, height)
    {
        this.camera.aspect = width / height;
        this.camera.updateProjectionMatrix ();
        this.renderer.setSize (width, height);
        this.Render ();
    }

    Render ()
    {
        var lightDir = this.camera.position.clone ().sub (this.controls.target);
        this.light.position.set (lightDir.x, lightDir.y, lightDir.z);
        this.renderer.render (this.scene, this.camera);
    }
};
