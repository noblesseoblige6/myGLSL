#include "VBOPlane.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <cstdio>
#include <cmath>

VBOPlane::VBOPlane(float xsize, float zsize, int xdivs, int zdivs)
{
	m_xsize = xsize;
	m_zsize = zsize;
	m_xdivs = xdivs;
	m_zdivs = zdivs;

	glGenVertexArrays( 1, &vaoHandle );
	glBindVertexArray(vaoHandle);
	faces = xdivs * zdivs;
	float * v = new float[3 * (xdivs + 1) * (zdivs + 1)];
	float * tex = new float[2 * (xdivs + 1) * (zdivs + 1)];
	unsigned int * el = new unsigned int[6 * xdivs * zdivs];

	float x2 = xsize / 2.0f;
	float z2 = zsize / 2.0f;
	float iFactor = (float)zsize / zdivs;
	float jFactor = (float)xsize / xdivs;
	float texi = 1.0f / zdivs;
	float texj = 1.0f / xdivs;
	float x, z;
	int vidx = 0, tidx = 0;
	for( int i = 0; i <= zdivs; i++ ) {
		z = iFactor * i - z2;
		for( int j = 0; j <= xdivs; j++ ) {
			x = jFactor * j - x2;
			v[vidx] = x;
			v[vidx+1] = 0.0f;
			v[vidx+2] = z;
			vidx += 3;
			tex[tidx] = j * texj;
			tex[tidx+1] = i * texi;
			tidx += 2;
		}
	}

	unsigned int rowStart, nextRowStart;
	int idx = 0;
	for( int i = 0; i < zdivs; i++ ) {
		rowStart = i * (xdivs+1);
		nextRowStart = (i+1) * (xdivs+1);
		for( int j = 0; j < xdivs; j++ ) {
			el[idx] = rowStart + j;
			el[idx+1] = nextRowStart + j;
			el[idx+2] = nextRowStart + j + 1;
			el[idx+3] = rowStart + j;
			el[idx+4] = nextRowStart + j + 1;
			el[idx+5] = rowStart + j + 1;
			idx += 6;
		}
	}

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * (xdivs+1) * (zdivs+1) * sizeof(float), v, GL_STATIC_DRAW);
	glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * (xdivs+1) * (zdivs+1) * sizeof(float), tex, GL_STATIC_DRAW);
	glVertexAttribPointer( (GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
	glEnableVertexAttribArray(2);  // Texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xdivs * zdivs * sizeof(unsigned int), el, GL_STATIC_DRAW);

	glBindVertexArray(0);
	delete [] v;
	delete [] tex;
	delete [] el;
}

void VBOPlane::getParam(float& _xsize, float& _zsize, int& _xdivs, int& _zdivs)
{
	_xsize = m_xsize;
	_zsize = m_zsize;
	_xdivs = m_xdivs;
	_zdivs = m_zdivs;
}
void VBOPlane::render() const {
	glBindVertexArray(vaoHandle);
	glVertexAttrib3f(1, 0.0f, 1.0f, 0.0f);  // Constant normal for all verts
	glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
}