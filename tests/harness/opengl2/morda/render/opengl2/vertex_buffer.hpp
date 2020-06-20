#pragma once

#include <utki/span.hpp>

#include <r4/vector2.hpp>

#include <morda/render/vertex_buffer.hpp>

#include "buffer.hpp"

namespace morda{ namespace render_opengl2{

class OpenGL2VertexBuffer : public morda::vertex_buffer, public opengl_buffer{
public:
	const GLint numComponents;
	const GLenum type;
	
	OpenGL2VertexBuffer(utki::span<const r4::vec4f> vertices);
	
	OpenGL2VertexBuffer(utki::span<const r4::vec3f> vertices);
	
	OpenGL2VertexBuffer(utki::span<const r4::vec2f> vertices);
	
	OpenGL2VertexBuffer(utki::span<const float> vertices);
	
	OpenGL2VertexBuffer(const OpenGL2VertexBuffer&) = delete;
	OpenGL2VertexBuffer& operator=(const OpenGL2VertexBuffer&) = delete;

private:
	void init(GLsizeiptr size, const GLvoid* data);
};

}}
