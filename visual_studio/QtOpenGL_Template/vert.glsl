#version 430
in layout(location = 0) vec3 vp;
in layout(location = 1) float offset;
/*uniform mat4 model_matrix; // ModelToWorld matrix
uniform mat4 view_matrix; // WorldToCamera matrix
uniform mat4 proj_matrix; // Projection matrix
*/
//uniform mat4 model_matrix, view_matrix, proj_matrix;
//out vec3 normal;
//flat out int InstanceID;

void main () {
	//normal = vn;
	gl_Position = vec4(vp.x + offset, vp.y, vp.z, 1.0);
	//gl_Position = vec4(vp, 1.0);
	//InstanceID = gl_InstanceID;
}