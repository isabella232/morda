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

#include "treeml.hpp"
#include "../resource_loader.hpp"

#include "../context.hpp"

using namespace morda::res;

morda::res::treeml::treeml(std::shared_ptr<morda::context> c, ::treeml::forest&& forest) :
		resource(std::move(c)),
		s(std::move(forest))
{}

std::shared_ptr<morda::res::treeml> morda::res::treeml::load(morda::context& ctx, const ::treeml::forest& desc, const papki::file& fi){
	for(auto& p : desc){
		if(p.value == "file"){
			fi.set_path(get_property_value(p).to_string());
		}
	}

	return std::make_shared<treeml>(utki::make_shared_from(ctx), ::treeml::read(fi));
}