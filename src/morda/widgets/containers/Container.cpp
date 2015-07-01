#include "Container.hpp"

#include "../../App.hpp"

#include "../../util/util.hpp"



using namespace morda;



namespace{
class BlockedFlagGuard{
	bool& blocked;
public:
	BlockedFlagGuard(bool& blocked) :
			blocked(blocked)
	{
		this->blocked = true;
	}
	
	~BlockedFlagGuard()NOEXCEPT{
		this->blocked = false;
	}
};
}



Container::Container(const stob::Node* chain) :
		Widget(chain)
{
	if(chain){
		this->Add(*chain);
	}
}



Widget::LayoutParams& Container::GetLayoutParams(Widget& w){
	if(!w.layoutParams){
		w.layoutParams = this->CreateLayoutParams(w.layout.get());
	}
	
	return *w.layoutParams;
}



void Container::Add(const stob::Node& chain){
	for(const stob::Node* n = chain.ThisOrNextNonProperty().node(); n; n = n->NextNonProperty().node()){
		this->Add(morda::App::Inst().inflater.Inflate(*n));
	}
}



//override
void Container::Render(const morda::Matr4r& matrix)const{	
	for(auto& w: this->Children()){
		if(!w->IsVisible()){
			continue;
		}
		
		morda::Matr4r matr(matrix);
		matr.Translate(w->Rect().p);

		w->RenderInternal(matr);
	}
}



//override
bool Container::OnMouseButton(bool isDown, const morda::Vec2r& pos, EMouseButton button, unsigned pointerID){
//	TRACE(<< "Container::OnMouseButton(): isDown = " << isDown << ", button = " << button << ", pos = " << pos << std::endl)
	
	BlockedFlagGuard blockedFlagGuard(this->isBlocked);
	
	//check if mouse captured
	{
		T_MouseCaptureMap::iterator i = this->mouseCaptureMap.find(pointerID);
		if(i != this->mouseCaptureMap.end()){
			if(auto w = i->second.first.lock()){
				w->SetHovered(w->Rect().Overlaps(pos), pointerID);
				w->OnMouseButton(isDown, pos - w->Rect().p, button, pointerID);
				
				unsigned& n = i->second.second;
				if(isDown){
					++n;
				}else{
					--n;
				}
				if(n == 0){
					this->mouseCaptureMap.erase(i);
				}
				return true;//doesn't matter what to return
			}else{
				this->mouseCaptureMap.erase(i);
			}
		}
	}
	
	//call children in reverse order
	for(Widget::T_ChildrenList::const_reverse_iterator i = this->Children().rbegin(); i != this->Children().rend(); ++i){
		if(!(*i)->IsVisible() || !(*i)->IsEnabled()){
			continue;
		}
		
		if(!(*i)->Rect().Overlaps(pos)){
			continue;
		}
		
		//Sometimes mouse click event comes without prior mouse move,
		//but, since we get mouse click, then the widget was hovered before the click.
		(*i)->SetHovered(true, pointerID);
		if((*i)->OnMouseButton(isDown, pos - (*i)->Rect().p, button, pointerID)){
			ASSERT(this->mouseCaptureMap.find(pointerID) == this->mouseCaptureMap.end())
			
			if(isDown){//in theory, it can be button up event here, if some widget which captured mouse was removed from its parent
				this->mouseCaptureMap.insert(std::make_pair(pointerID, std::make_pair(std::weak_ptr<Widget>(*i), 1)));
			}
			
			return true;
		}
	}
	
	return this->Widget::OnMouseButton(isDown, pos, button, pointerID);
}



//override
bool Container::OnMouseMove(const morda::Vec2r& pos, unsigned pointerID){
//	TRACE(<< "Container::OnMouseMove(): pos = " << pos << std::endl)
	
	BlockedFlagGuard blockedFlagGuard(this->isBlocked);
	
	//check if mouse captured
	{
		T_MouseCaptureMap::iterator i = this->mouseCaptureMap.find(pointerID);
		if(i != this->mouseCaptureMap.end()){
			if(auto w = i->second.first.lock()){
				w->OnMouseMove(pos - w->Rect().p, pointerID);
		
				//set hovered goes after move notification because position of widget could change
				//during handling the notification, so need to check after that for hovering
				w->SetHovered(w->Rect().Overlaps(pos), pointerID);

				return true;//doesn't matter what to return
			}else{
				this->mouseCaptureMap.erase(i);
			}
		}
	}
	
	//call children in reverse order
	for(Widget::T_ChildrenList::const_reverse_iterator i = this->Children().rbegin(); i != this->Children().rend(); ++i){
		if(!(*i)->IsVisible() || !(*i)->IsEnabled()){
			ASSERT(!(*i)->IsHovered())
			continue;
		}
		
		if(!(*i)->Rect().Overlaps(pos)){
			(*i)->SetHovered(false, pointerID);
			continue;
		}
		
		(*i)->SetHovered(true, pointerID);
		
		if((*i)->OnMouseMove(pos - (*i)->Rect().p, pointerID)){//consumed mouse move event
			//un-hover rest of the children
			for(++i; i != this->Children().rend(); ++i){
				(*i)->SetHovered(false, pointerID);
			}
			return true;
		}		
	}
	
	return this->Widget::OnMouseMove(pos, pointerID);
}



void Container::OnHoverChanged(unsigned pointerID){
	//TODO: if some child removed during iterating?
	
	if(this->IsHovered(pointerID)){
		return;
	}
	
	//un-hover all the children if container became un-hovered
	BlockedFlagGuard blockedFlagGuard(this->isBlocked);
	for(auto& w : this->Children()){
		w->SetHovered(false, pointerID);
	}
}



void Container::OnResize(){
//	TRACE(<< "Container::OnResize(): invoked" << std::endl)
	BlockedFlagGuard blockedFlagGuard(this->isBlocked);
	for(auto& w : this->Children()){
		if(w->NeedsRelayout()){
			w->OnResize();
			w->relayoutNeeded = false;
		}
	}
}



Widget::T_ChildrenList::iterator Container::Add(const std::shared_ptr<Widget>& w,  T_ChildrenList::iterator insertBefore){
	ASSERT_INFO(w, "Container::Add(): widget pointer is 0")
	if(w->Parent()){
		throw morda::Exc("Container::Add(): cannot add widget, it is already added to some container");
	}

	if(this->isBlocked){
		throw morda::Exc("Container::Add(): cannot add child while iterating through children, try deferred adding.");
	}
	
	if(insertBefore != this->children.end() && (*insertBefore)->parent != this){
		throw morda::Exc("Container::Add(): cannot insert after provided iterator, it points to a different container");
	}

	T_ChildrenList::iterator ret = this->children.insert(insertBefore, w);
	
	w->parentIter = ret;
	w->parent = this;

	w->SetRelayoutNeeded();
	this->SetRelayoutNeeded();
	
	this->OnChildrenListChanged();
	
	if(this->children.size() > 1){
		(*(++this->children.rbegin()))->OnTopmostChanged();
	}
	
	w->OnTopmostChanged();
	
	ASSERT(!w->IsHovered())
	return ret;
}



void Container::Remove(Widget& w){
	if(w.parent != this){
		throw morda::Exc("Container::Remove(): widget is not added to this container");
	}
	
	if(this->isBlocked){
		throw morda::Exc("Container::Remove(): cannot remove child while iterating through children, try deferred removing.");
	}
//	TRACE(<< "Container::Remove(): w = " << (&w) << std::endl)
	
	ASSERT(w.parent == this)
	
	this->children.erase(w.parentIter);
	
	w.parent = nullptr;
	w.SetUnhovered();
	
	this->SetRelayoutNeeded();
	
	this->OnChildrenListChanged();
}



void Container::removeAll() {
	while(this->Children().size()){
//		TRACE(<< "Container::removeAll(): w = " << (this->Children().front().get()) << std::endl)
		this->Remove(*this->Children().front());
	}
}



//override
std::shared_ptr<Widget> Container::FindChildByName(const std::string& name)NOEXCEPT{
	if(auto r = this->Widget::FindChildByName(name)){
		return std::move(r);
	}
	
	for(auto& w : this->Children()){
		if(auto r = w->FindChildByName(name)){
			return r;
		}
	}
	return nullptr;
}



void Container::MakeChildTopmost(Widget& w){
	if(this->isBlocked){
		throw morda::Exc("Container::MakeChildTopmost(): cannot modify children list while iterating through children, try deferring the operation.");
	}
	
	ASSERT(w.Parent() == this)
	ASSERT(w.parentIter != this->children.end())
	
	ASSERT(this->children.size() != 0)
	
	if(&w == this->children.back().get()){
		return;//already topmost
	}
	
	ASSERT(this->children.size() > 1)
	
	auto p = std::move(*w.parentIter);
	
	this->children.erase(w.parentIter);

	this->children.push_back(std::move(p));
	w.parentIter = --this->children.end();
	
	(*(++this->children.rbegin()))->OnTopmostChanged();
	w.OnTopmostChanged();
	
	this->SetRelayoutNeeded();
}



Vec2r Container::dimForWidget(const Widget& w, const LayoutParams& lp)const{
	Vec2r d;
	for(unsigned i = 0; i != 2; ++i){
		if(lp.dim[i] == LayoutParams::D_Max){
			d[i] = this->Rect().d[i];
		}else if(lp.dim[i] == LayoutParams::D_Min || lp.dim[i] < 0){
			d[i] = -1;
		}else{
			d[i] = lp.dim[i];
		}
	}
	if(d.x < 0 || d.y < 0){
		Vec2r md = w.measure(d);
		for(unsigned i = 0; i != md.size(); ++i){
			if(d[i] < 0){
				d[i] = md[i];
			}
		}
		return d;
	}
	return d;
}