#include "ResCursor.hpp"

#include "../context.hpp"

#include "../util/util.hpp"


using namespace morda;

ResCursor::ResCursor(std::shared_ptr<morda::context> c, ResImage& image, const Vec2r& hotspot) :
		Resource(std::move(c)),
		image_v(std::dynamic_pointer_cast<ResImage>(image.shared_from_this())),
		hotspot_v(hotspot)
{}


std::shared_ptr<ResCursor> ResCursor::load(morda::context& ctx, const puu::forest& desc, const papki::file& fi) {
	std::shared_ptr<ResImage> image;
	Vec2r hotspot;
	bool hotspot_set = false;

	for(auto& p : desc){
		if(p.value == "image"){
			image = ctx.loader.load<ResImage>(get_property_value(p).to_string());
		}else if(p.value == "hotspot"){
			hotspot = parse_vec2(p.children);
			hotspot_set = true;
		}
	}

	if(!image){
		throw std::logic_error("ResCursor::load(): resource description does not contain 'image' property");
	}
	
	if(!hotspot_set){
		throw std::logic_error("ResCursor::load(): resource description does not contain 'hotspot' property");
	}
	
	return std::make_shared<ResCursor>(ctx.shared_from_this(), *image, hotspot);
}
