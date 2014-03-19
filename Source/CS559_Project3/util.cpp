#include "util.h"

vec3 get_norm_from_yaw_pitch(GLfloat yaw, GLfloat pitch)
{
	GLfloat n_y = glm::sin(pitch); 
	GLfloat n_x = glm::cos(pitch)*glm::cos(yaw);
	GLfloat n_z = glm::cos(pitch)*glm::sin(yaw);

	return vec3(n_x, n_y, n_z);
}

void add_cylinder_vertices(
	GLfloat lower_radius,
	GLfloat upper_radius,
	GLfloat height,
	GLfloat offset,
	GLuint slices,
	GLuint stacks,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices,
	bool is_out_ward_norm
) {
	// It will make the normal computation complicated when one of them is zero
	assert(0.0 < lower_radius && 0.0 < upper_radius);

	// Note: we are adding vertices and vertex indices into the vector
	GLuint index_offset = vertices.size();

	GLfloat height_interval = height/stacks;
	GLfloat radius_interval = (lower_radius-upper_radius)/stacks;
	GLfloat yaw_interval = 360.0f/slices;
	GLfloat pitch = glm::atan((lower_radius-upper_radius)/height);

	// Push in all the vertices
	for (GLuint i = 0; i <= stacks; i ++) {
		GLfloat h = offset + i*height_interval;
		GLfloat r = lower_radius - i*radius_interval;
		for (GLuint j = 0; j <= slices; j ++) {
			GLfloat yaw = radians(j*yaw_interval);
			vec3 normal;
			if (is_out_ward_norm) {
				normal = get_norm_from_yaw_pitch(yaw, pitch);
			} else {
				normal = get_norm_from_yaw_pitch(-yaw, (GLfloat)(radians(180.0)+pitch));
			}
			GLfloat x = r*cos(yaw); GLfloat z = r*sin(yaw); GLfloat y = h;
			GLfloat tc_x = (GLfloat)(yaw/radians(360.0));
			GLfloat tc_y = (GLfloat)(i/stacks);
			vertices.push_back(VertexAttributes(vec3(x,y,z), color, normal, vec2(tc_x, tc_y)));
		}
	}

	// Push in all the vertex indices
	for (GLuint i = 0; i < stacks; i ++) {
		GLuint level_offset = (slices+1) * i;
		for (GLuint j = 0; j < slices; j ++) {
			// triangle 1 of the rectangle
			vertex_indices.push_back(index_offset + level_offset + j);
			vertex_indices.push_back(index_offset + level_offset + j+1);
			vertex_indices.push_back(index_offset + level_offset + slices + j);
			// triangle 2 of the rectangle
			vertex_indices.push_back(index_offset + level_offset + j+1);
			vertex_indices.push_back(index_offset + level_offset + slices + j);
			vertex_indices.push_back(index_offset + level_offset + slices + j+1);
		}
	}

}

void add_disk_vertices(
	GLfloat inner_radius,
	GLfloat outer_radius,
	GLfloat offset,
	GLuint slices,
	GLuint loops,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices,
	bool is_upward_norm
) {
	assert(0 <= inner_radius && 0 < outer_radius);

	// 
	vec3 normal;
	if (is_upward_norm) {
		normal = vec3(0, 1.0, 0);
	}else{
		normal = vec3(0,-1.0,0);
	}

	GLfloat angle_interval = radians(360.0f/slices);
	GLfloat radius_interval = (outer_radius-inner_radius)/loops;
	color = vec3(1.0, 0, 0);

	GLuint index_offset = vertices.size();

	for (GLuint i = 0; i <= loops; i ++) {
		GLfloat r = outer_radius - radius_interval*i;
		for (GLuint j = 0; j < slices; j ++) {
			GLfloat angle = angle_interval*j;
			GLfloat x = r*cos(angle); GLfloat y = offset; GLfloat z = r*sin(angle);
			GLfloat tc_x = (GLfloat)(x/outer_radius*0.5+0.5);
			GLfloat tc_y = (GLfloat)(z/outer_radius*0.5+0.5);
			vertices.push_back(VertexAttributes(vec3(x,y,z), color, normal, vec2(1-tc_x, 1-tc_y)));
		}
	}

	for (GLuint i = 0; i < loops; i ++) {
		GLuint level_offset = slices * i;
		for (GLuint j = 0; j < slices; j ++) {
			// triangle 1 of the rectangle
			vertex_indices.push_back(index_offset + level_offset + j);
			vertex_indices.push_back(index_offset + level_offset + (j+1)%slices);
			vertex_indices.push_back(index_offset + level_offset + slices + j);
			// triangle 2 of the rectangle
			vertex_indices.push_back(index_offset + level_offset + (j+1)%slices);
			vertex_indices.push_back(index_offset + level_offset + slices + j);
			vertex_indices.push_back(index_offset + level_offset + slices + (j+1)%slices);
			
		}
	}
}

void add_rectangle_vertices(
	vec3 a,
	vec3 b,
	vec3 c,
	vec3 d,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
) {
	// First compute the norm
	vec3 d1 = a-b;
	vec3 d2 = d-a;
	vec3 normal = normalize(cross(d1, d2));

	GLuint index_offset = vertices.size();

	// Push in vertices
	vertices.push_back(VertexAttributes(a,color,normal,vec2(0.0f, 0.0f)));
	vertices.push_back(VertexAttributes(b,color,normal,vec2(1.0f, 0.0f)));
	vertices.push_back(VertexAttributes(c,color,normal,vec2(1.0f, 1.0f)));
	vertices.push_back(VertexAttributes(d,color,normal,vec2(0.0f, 1.0f)));

	// Push in vertex indices
	vertex_indices.push_back(index_offset);
	vertex_indices.push_back(index_offset+1);
	vertex_indices.push_back(index_offset+3);
	vertex_indices.push_back(index_offset+1);
	vertex_indices.push_back(index_offset+2);
	vertex_indices.push_back(index_offset+3);
}

void add_torus_vertices(
	GLfloat inner_radius,
	GLfloat outer_radius,
	GLfloat offset,
	GLuint sides,
	GLuint rings,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
	) {
	assert(0 < inner_radius && inner_radius < outer_radius);
	GLuint index_offset = vertices.size();

	// Push in vertices
	GLfloat yaw_interval = (GLfloat)radians(360.0/rings);
	GLfloat pitch_interval = (GLfloat)radians(366.0/sides);

	// Push in vertices
	for (GLuint i = 0; i <= rings; i ++) {
		GLfloat yaw = i*yaw_interval;
		for (GLuint j = 0; j <= sides; j ++) {
			GLfloat pitch = j*pitch_interval;
			GLfloat ring_radius = (GLfloat)((outer_radius-inner_radius)/2.0);
			GLfloat y = ring_radius*sin(pitch)+offset;
			GLfloat x = (outer_radius-ring_radius*cos(pitch))*cos(yaw);
			GLfloat z = (outer_radius-ring_radius*cos(pitch))*sin(yaw);
			GLfloat tc_x = (GLfloat)(pitch/radians(360.0));
			GLfloat tc_y = (GLfloat)(yaw/radians(360.0));
			vertices.push_back(VertexAttributes(vec3(x,y,z), color, get_norm_from_yaw_pitch(-yaw,pitch), vec2(1-tc_x,1-tc_y)));
		}
	}

	// Push in indices to form triangles
	for (GLuint i = 0; i < rings; i ++) {
		for (GLuint j = 0; j < sides; j ++) {
			// Triangle 1
			vertex_indices.push_back(index_offset+i*(sides+1)+j);
			vertex_indices.push_back(index_offset+i*(sides+1)+j+1);
			vertex_indices.push_back(index_offset+(i+1)*(sides+1)+j);
			// Triangle 2
			vertex_indices.push_back(index_offset+i*(sides+1)+j+1);
			vertex_indices.push_back(index_offset+(i+1)*(sides+1)+j+1);
			vertex_indices.push_back(index_offset+(i+1)*(sides+1)+j);
		}
	}
}


void add_ball_vertices(
	vec3 center,
	GLfloat radius,
	GLuint stacks,
	GLuint slices,
	vec3 color,
	vector<VertexAttributes> & vertices,
	vector<GLuint> & vertex_indices
) {

	GLuint index_offset = vertices.size();

	GLfloat yaw_interval = (GLfloat)radians(360.0/stacks);
	GLfloat pitch_interval = (GLfloat)radians(180.0/slices);

	
	for (GLuint i = 0; i <= stacks; i ++) {
		GLfloat yaw = i * yaw_interval;
		for (GLuint j = 0; j <= slices; j ++) {
			GLfloat pitch = (GLfloat)(radians(-90.0)+j * pitch_interval);
			GLfloat y = radius * sin(pitch)+center.y;
			GLfloat x = radius * cos(pitch) * cos(yaw) + center.x;
			GLfloat z = radius * cos(pitch) * sin(yaw) + center.z;
			GLfloat tc_x = (GLfloat)(yaw/radians(360.0));
			GLfloat tc_y = (GLfloat)(0.5-0.5*(y-center.y)/radius);
			vertices.push_back(VertexAttributes(vec3(x,y,z), color, get_norm_from_yaw_pitch(yaw, pitch),vec2(1-tc_x,1-tc_y)));
		}
	}
	for (GLuint i = 0; i < stacks; i ++)
	{
		for (GLuint j = 0; j < slices; j ++)
		{
			// Triangle 1
			vertex_indices.push_back(index_offset+i*(slices+1)+j);
			vertex_indices.push_back(index_offset+i*(slices+1)+j+1);
			vertex_indices.push_back(index_offset+(i+1)*(slices+1)+j);
			// Triangle 2
			if (j+1 != slices) {
				vertex_indices.push_back(index_offset+i*(slices+1)+(j+1)%slices);
				vertex_indices.push_back(index_offset+(i+1)*(slices+1)+j+1);
				vertex_indices.push_back(index_offset+(i+1)*(slices+1)+j);
			}
		}
	}
	
}


