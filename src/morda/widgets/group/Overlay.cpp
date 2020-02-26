#include "Overlay.hpp"
#include "../proxy/MouseProxy.hpp"


using namespace morda;

namespace{

const auto ContextMenuLayout_c = puu::read(R"qwertyuiop(
	layout{
		dx{fill} dy{fill}
	}
	@mouse_proxy{
		id{morda_overlay_mouseproxy}
		layout{
			dx{fill} dy{fill}
		}
	}
	@container{
		id{morda_overlay_container}
		layout{
			dx{fill} dy{fill}
		}
	}
)qwertyuiop");

}

Overlay::Overlay(std::shared_ptr<morda::context> c, const puu::forest& desc) :
		widget(std::move(c), desc),
		Pile(nullptr, desc)
{
	this->on_children_changed();
}

void Overlay::on_children_changed(){
	if(!this->overlayLayer || !this->overlayLayer->parent()){
		this->overlayLayer = std::make_shared<Pile>(this->context, ContextMenuLayout_c);
		this->push_back(this->overlayLayer);

		this->overlayContainer = this->overlayLayer->try_get_widget_as<Container>("morda_overlay_container");
		ASSERT(this->overlayContainer)

		auto mp = this->overlayLayer->try_get_widget_as<MouseProxy>("morda_overlay_mouseproxy");
		ASSERT(mp)

		mp->mouseButton = [this](widget& w, bool isDown, const Vec2r& pos, mouse_button button, unsigned id) -> bool{
			this->hideContextMenu();
			return false;
		};
	}

	ASSERT(this->overlayLayer)
	ASSERT(this->children().size() >= 1)

	if(this->children().back() != this->overlayLayer){
		auto w = this->overlayLayer->remove_from_parent();
		this->push_back(w);
	}
}



void Overlay::showContextMenu(std::shared_ptr<widget> w, Vec2r anchor){
	this->overlay().push_back(w);

	auto& lp = this->overlay().get_layout_params(*w);

	Vec2r dim = this->dims_for_widget(*w, lp);

	for(unsigned i = 0; i != 2; ++i){
		utki::clampTop(dim[i], this->overlay().rect().d[i]);
	}

	w->resize(dim);

	for(unsigned i = 0; i != 2; ++i){
		utki::clampRange(anchor[i], 0.0f, this->overlay().rect().d[i] - w->rect().d[i]);
	}

	w->move_to(anchor);
}

void Overlay::hideContextMenu(){
	if(this->overlayContainer->children().size() == 0){
		return;
	}
	ASSERT(this->overlayContainer->children().size() > 0)

	this->overlayContainer->remove(*this->overlayContainer->children().back());
}
