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

#include "linear_container.hpp"

namespace morda{

/**
 * @brief Horizontal container widget.
 * Row is a horizontal variant of linear container. From GUI scripts it can be instantiated as "row".
 */
class row : public linear_container{
public:
	row(std::shared_ptr<morda::context> c, const treeml::forest& desc) :
			widget(std::move(c), desc),
			linear_container(this->context, desc, false)
	{}

	row(const row&) = delete;
	row& operator=(const row&) = delete;
};

}
