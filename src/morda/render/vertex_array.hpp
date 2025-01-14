/*
morda - GUI framework

Copyright (C) 2012-2021  Ivan Gagis <igagis@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* ================ LICENSE END ================ */

#pragma once

#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

#include <vector>
#include <memory>

namespace morda{

class vertex_array{
public:
	const std::vector<std::shared_ptr<vertex_buffer>> buffers;
	
	const std::shared_ptr<index_buffer> indices;
	
	/**
	 * @brief Vertex data rendering mode.
	 * Enumeration defining how to interpret vertex data when rendering.
	 */
	enum class mode{
		// NOTE: do not change order!!!
		
		triangles,
		triangle_fan,
		line_loop,
		triangle_strip
	};
	
	const mode rendering_mode;
	
	vertex_array(decltype(buffers)&& buffers, std::shared_ptr<morda::index_buffer> indices, mode rendering_mode);

	virtual ~vertex_array(){}
};

}
