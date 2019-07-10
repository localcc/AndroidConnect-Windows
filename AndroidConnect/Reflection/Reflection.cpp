#include "Reflection.hpp"

//For those, who want to add plugins, here you add your plugins to reflection, that's temporary(maybe)
Reflection::Reflection() {
	AddChildrenRaw("Notification", Notification::Notify);
}

void Reflection::AddChildren(char* name, Reflection::ReflectType src) {
	this->parent.children[name] = src;
}

void Reflection::AddChildrenDst(char* name, Reflection::ReflectType src, Reflection::ReflectType dst) {
	dst.children[name] = src;
}

void Reflection::AddChildrenRaw(const char* name, FnPtr func) {
	this->parent.children[name] = ReflectType{
		func
	};
}

void Reflection::AddChildrenRawDst(char* name, FnPtr func, Reflection::ReflectType dst) {
	dst.children[name] = ReflectType{
		func
	};
}


void Reflection::SetParent(Reflection::ReflectType parent) {
	this->parent = parent;
}

Reflection::FnPtr Reflection::GetByName(char* name) {
	return this->parent.children[name].functionPointer;
}