#pragma once

#include "../widget.hpp"
#include "../group/List.hpp"
#include "../base/TextWidget.hpp"

namespace morda{

class TextInputWrap :
		public virtual Widget,
		public TextWidget,
		private List
{
public:
	TextInputWrap(const puu::forest& desc);

	TextInputWrap(const TextInputWrap&) = delete;
	TextInputWrap& operator=(const TextInputWrap&) = delete;

private:

};

}
