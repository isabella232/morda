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

#include "choice_group.hpp"

using namespace morda;

choice_group::choice_group(std::shared_ptr<morda::context> c, const treeml::forest& desc) :
		widget(std::move(c), desc),
		pile(this->context, desc)
{}

bool choice_group::is_active(const widget& w)const noexcept{
	widget* aw = this->active_choice_button.lock().get();
	return aw == &w;
}

void choice_group::set_active_choice_button(std::weak_ptr<choice_button> cb){
	auto oldactive = this->active_choice_button.lock();

	if(cb.lock() == oldactive){
		return;
	}
	
	this->active_choice_button = cb;
	
	if(oldactive){
		oldactive->set_pressed(false);
	}
}
