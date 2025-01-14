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

#include "gradient.hpp"

#include "../../util/util.hpp"

#include "../../context.hpp"

using namespace morda;

gradient::gradient(std::shared_ptr<morda::context> c, const treeml::forest& desc) :
		widget(std::move(c), desc)
{
	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "gradient"){
			this->res = this->context->loader.load<res::gradient>(get_property_value(p).to_string());
		}
	}
}

void gradient::render(const matrix4& matrix)const{
	set_simple_alpha_blending(*this->context->renderer);
	
	morda::matrix4 matr(matrix);
	matr.scale(this->rect().d);

	if(this->res){
//		TRACE(<< "this->rect().d = " << this->rect().d << std::endl)
		this->res->render(matr);
	}
}
