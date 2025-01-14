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

#include "text_widget.hpp"

#include "../../context.hpp"

#include "../../util/util.hpp"

using namespace morda;

void text_widget::set_font(std::shared_ptr<res::font> font){
	if(!font){
		throw std::invalid_argument("text_widget::SetFont(): passed argument is null");
	}

	this->font = std::move(font);

	this->invalidate_layout();

	this->on_font_change();
}

text_widget::text_widget(std::shared_ptr<morda::context> c, const treeml::forest& desc) :
		widget(std::move(c), desc)
{
	for(const auto& p : desc){
		if(!is_property(p)){
			continue;
		}

		if(p.value == "font"){
			this->font = this->context->loader.load<morda::res::font>(get_property_value(p).to_string().c_str());
		}
	}

	// load default font if needed
	if(!this->font){
		this->font = this->context->loader.load<res::font>("morda_fnt_text");
	}
}
