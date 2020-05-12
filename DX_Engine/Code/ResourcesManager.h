#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_
#pragma once

#include "Interfaces\IBaseResource.h"
#include <unordered_map>

class ResourcesManager {
private:
	typedef std::unordered_map<std::string, std::unique_ptr<IBaseResource>> StrResourcePtrMap;

public:
									ResourcesManager();
									~ResourcesManager();

	void 							CreateResource(std::unique_ptr<IBaseResource>&& ppRes);
	IBaseResource*					GetResource(const std::string& name);

public:
	static ResourcesManager*		Create();

private:
	//std::vector<std::unique_ptr<IBaseResource>>		m_resources;
	StrResourcePtrMap				m_resources;

};

#endif //_RESOURCES_MANAGER_H_
