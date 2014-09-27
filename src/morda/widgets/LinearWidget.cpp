#include "LinearWidget.hpp"


using namespace morda;



LinearWidget::LinearWidget(const stob::Node* desc) :
		Widget(desc)
{
	if(!desc){
		return;
	}
	
	if(const stob::Node* n = desc->GetProperty("vertical")){
		this->isVertical = n->AsBool();
	}
}


