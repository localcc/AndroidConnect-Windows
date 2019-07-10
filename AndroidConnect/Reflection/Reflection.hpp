#pragma once
#include <map>
#include <functional>

#include "../Plugins/PluginsIncludes.hpp"

//TODO: add normal reflection, but that's for later!



class Reflection {
public:
	typedef void(*FnPtr)(void*);
	struct ReflectType {
		FnPtr functionPointer;
		std::map<char*, ReflectType> children;
	};
	Reflection();
	void SetParent(ReflectType);
	void AddChildren(char* name, ReflectType src);
	void AddChildrenDst(char* name, ReflectType src, ReflectType dst);
	void AddChildrenRaw(const char* name, FnPtr func);
	void AddChildrenRawDst(char* name, FnPtr func, ReflectType dst);

	FnPtr GetByName(char*);
private:
	ReflectType parent;
};