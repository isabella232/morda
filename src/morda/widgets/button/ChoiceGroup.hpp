#pragma once

#include "../group/Pile.hpp"

#include "choice_button.hpp"

namespace morda{

/**
 * @brief Choice group container.
 * This is a container which is used to group several choice buttons to the same
 * choice group.
 * In the GUI script can be instantiated as "ChoiceGroup". Only available after
 * initializing standard morda widgets.
 */
class ChoiceGroup : public Pile{
	friend class choice_button;
	
	std::shared_ptr<choice_button> activeChoiceButton_v;
public:
	ChoiceGroup(std::shared_ptr<morda::context> c, const puu::forest& desc);
	
	ChoiceGroup(const ChoiceGroup&) = delete;
	ChoiceGroup& operator=(const ChoiceGroup&) = delete;
	
	bool isWidgetActive(const widget& w)const noexcept;
	
	const decltype(activeChoiceButton_v)& getActiveButton()const noexcept{
		return this->activeChoiceButton_v;
	}
	
private:
	void setActiveChoiceButton(const std::shared_ptr<choice_button>& rb);
};

}
