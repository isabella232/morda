#include "OpenGL2IndexBuffer.hpp"

#include "OpenGL2_util.hpp"

#ifdef M_RENDER_OPENGLES2
#	include <GLES2/gl2.h>
#else
#	include <GL/glew.h>
#endif

OpenGL2IndexBuffer::OpenGL2IndexBuffer(const utki::Buf<std::uint16_t> indices) :
		elementType(GL_UNSIGNED_SHORT),
		elementsCount(GLsizei(indices.size()))
{	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer);
	assertOpenGLNoError();
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.sizeInBytes(), &*indices.begin(), GL_STATIC_DRAW);
	assertOpenGLNoError();
	
	//TODO: remove this
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	assertOpenGLNoError();
}
