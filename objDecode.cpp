struct Object {
	float* v; //vertices
	uint n_v;
	float* vt; //texture mappings
	uint n_vt;	
	float* vn; //normals (not necessarily unit)
	uint n_vn;
	float* vp; //parameter space vertices
	uint n_vp;
	float* f; //faces
	uint n_f;
};

Object* decodeObj(String filename){
	Object decoded = new Object;
	

	return &decoded;
}