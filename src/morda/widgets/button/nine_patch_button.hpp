#pragma once

#include "PushButton.hpp"
#include "../label/NinePatch.hpp"

namespace morda{

/**
 * @brief Simple push button.
 * Implementation of a push button as a nine patch which can have children.
 * In GUI scripts can be instantiated as "SimpleButton". Only available after
 * initializing standard morda widgets.
 * Since the button is a NinePatch it can have children.
 */
class nine_patch_button :
		virtual public button,
		public NinePatch
{
	std::shared_ptr<const res_ninepatch> unpressedNinePatch_v;
	std::shared_ptr<const res_ninepatch> pressedNinePatch_v;
	
public:
	nine_patch_button(const nine_patch_button&) = delete;
	nine_patch_button& operator=(const nine_patch_button&) = delete;
	
	void set_unpressed_nine_patch(std::shared_ptr<const res_ninepatch> np);
	
	const decltype(unpressedNinePatch_v)& get_unpressed_nine_patch()const noexcept{
		return this->unpressedNinePatch_v;
	}
	
	void set_pressed_nine_patch(std::shared_ptr<const res_ninepatch> np);
	
	const decltype(pressedNinePatch_v)& get_pressed_nine_patch()const noexcept{
		return this->pressedNinePatch_v;
	}
	
protected:
	nine_patch_button(std::shared_ptr<morda::context> c, const puu::forest& desc);
	
	void on_pressed_changed()override;
};

}
