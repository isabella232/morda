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

#include "util.hpp"

#include <utki/debug.hpp>
#include <utki/config.hpp>

#include "../context.hpp"

#include "raster_image.hpp"

using namespace morda;


morda::vector2 morda::parse_vec2(treeml::forest::const_iterator begin, treeml::forest::const_iterator end){
	morda::vector2 ret;

	unsigned n = 0;
	real v = 0;
	for(auto i = begin; n != 2 && i != end; ++n, ++i){
		v = real(i->value.to_float());
		ret[n] = v;
	}

	for(; n != 2; ++n){
		ret[n] = v;
	}

	return ret;
}

morda::rectangle morda::parse_rect(const treeml::forest& desc){
	using std::min;
	vector2 p = parse_vec2(desc.begin(), desc.end());
	vector2 d = parse_vec2(std::next(desc.begin(), min(size_t(2), desc.size())), desc.end());
	return rectangle(p, d);
}

morda::sides<real> morda::parse_sides(const treeml::forest& desc){
	using std::min;
	vector2 p = parse_vec2(desc.begin(), desc.end());
	vector2 d = parse_vec2(std::next(desc.begin(), min(size_t(2), desc.size())), desc.end());
	return sides<real>(p.x(), p.y(), d.x(), d.y());
}

real morda::parse_dimension_value(const treeml::leaf& l, const morda::units& units){
	// check if millimeters
	if(l.length() >= 2 && l[l.length() - 1] == 'm' && l[l.length() - 2] == 'm'){
		return units.mm_to_px(l.to_float());
	}else if(l.length() >= 2 && l[l.length() - 1] == 'p' && l[l.length() - 2] == 'd'){ //check if in density pixels
		return units.dp_to_px(l.to_float());
	}

	if(l.empty()){
		return real(0);
	}

	return real(l.to_float());
}


real morda::parse_layout_dimension_value(const treeml::leaf& l, const morda::units& units){
	if(l == "min"){
		return widget::layout_params::min;
	}else if(l == "fill"){
		return widget::layout_params::fill;
	}else if(l == "max"){
		return widget::layout_params::max;
	}
	return parse_dimension_value(l, units);
}


morda::texture_2d::type morda::num_channels_to_texture_type(unsigned numChannels){
	switch(numChannels){
		default:
			ASSERT(false)
		case 1:
			return morda::texture_2d::type::grey;
			break;
		case 2:
			return morda::texture_2d::type::grey_alpha;
			break;
		case 3:
			return morda::texture_2d::type::rgb;
			break;
		case 4:
			return morda::texture_2d::type::rgba;
			break;
	}
}

std::shared_ptr<texture_2d> morda::load_texture(renderer& r, const papki::file& fi){
	raster_image image(fi);
//	TRACE(<< "ResTexture::Load(): image loaded" << std::endl)

	return r.factory->create_texture_2d(
			num_channels_to_texture_type(image.num_channels()),
			image.dims(),
			image.pixels()
		);
}

void morda::set_simple_alpha_blending(renderer& r){
	r.set_blend_enabled(true);
	r.set_blend_func(
			renderer::blend_factor::src_alpha,
			renderer::blend_factor::one_minus_src_alpha,
			renderer::blend_factor::one,
			renderer::blend_factor::one_minus_src_alpha
		);
}


r4::vector4<float> morda::color_to_vec4f(uint32_t color){
	return r4::vector4<float>(
			float(color & 0xff) / float(0xff),
			float((color >> 8) & 0xff) / float(0xff),
			float((color >> 16) & 0xff) / float(0xff),
			float((color >> 24) & 0xff) / float(0xff)
		);
}

bool morda::is_leaf_property(const treeml::leaf& l){
	return l.empty() || l[0] != '@';
}

bool morda::is_property(const treeml::tree& t){
	return is_leaf_property(t.value) && t.children.size() != 0;
}

bool morda::is_leaf_child(const treeml::leaf& l){
	return !is_leaf_property(l);
}
