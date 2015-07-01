/* The MIT License:

Copyright (c) 2015 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://morda.googlecode.com

/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include "Widget.hpp"
#include "containers/Container.hpp"


namespace morda{

class List : public Container{
	size_t addedIndex = 0;
	
	size_t posIndex = 0;
	real posOffset = real(0);
	
	bool isVertical;
	
	size_t numTailItems = 0;//Zero means that number of tail items has to be recomputed
	real firstTailItemOffset = real(0);
	
	
protected:
	List(bool isVertical, const stob::Node* chain);
public:
	List(const List&) = delete;
	List& operator=(const List&) = delete;
	
	
	class ItemsProvider : virtual public ting::Shared{
		friend class List;
		
		List* list = nullptr;
	protected:
		ItemsProvider(){}
	public:
		virtual ~ItemsProvider()NOEXCEPT{}
		
		virtual size_t count()const NOEXCEPT = 0;
		
		virtual std::shared_ptr<Widget> getWidget(size_t index) = 0;
		
		virtual void recycle(size_t index, std::shared_ptr<Widget> w){}
		
		void notifyDataSetChanged(){
			if(this->list){
				this->list->notifyDataSetChanged();
			}
		}
	};
	
	void setItemsProvider(std::shared_ptr<ItemsProvider> provider = nullptr);
	

	void OnResize()override;
	
	size_t visibleCount()const{
		return this->Children().size();
	}
	
	void setScrollPosAsFactor(real factor);
	
	real scrollFactor()const NOEXCEPT;
	
private:
	std::shared_ptr<ItemsProvider> provider;
	
	void updateChildrenList();
	
	bool arrangeWidget(std::shared_ptr<Widget>& w, real& pos, bool add, size_t index);//returns true if it was the last visible widget
	
	
	void updateTailItemsInfo();
	
	void notifyDataSetChanged(){
		this->numTailItems = 0;//means that it needs to be recomputed
		
		this->removeAll();
		this->addedIndex = 0;
		
		this->updateChildrenList();
	}
};



class HorizontalList : public List{
public:
	HorizontalList(const stob::Node* chain = nullptr) :
			Widget(chain),
			List(false, chain)
	{}
	
	HorizontalList(const HorizontalList&) = delete;
	HorizontalList& operator=(const HorizontalList&) = delete;
};


class VerticalList : public List{
public:
	VerticalList(const stob::Node* chain = nullptr) :
			Widget(chain),
			List(true, chain)
	{}
	
	VerticalList(const VerticalList&) = delete;
	VerticalList& operator=(const VerticalList&) = delete;
};

}