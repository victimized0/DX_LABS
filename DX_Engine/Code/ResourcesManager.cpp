#include "pch.h"
#include "ResourcesManager.h"

ResourcesManager* ResourcesManager::Create() {
	return new ResourcesManager();
}

ResourcesManager::ResourcesManager() {

}

ResourcesManager::~ResourcesManager() {

}

void ResourcesManager::CreateResource(std::unique_ptr<IBaseResource>&& ppRes) {
	IBaseResource* res = GetResource( ppRes->GetName() );

	if ( res != nullptr && res->GetHash() == ppRes->GetHash() ) {
		ppRes.release();
	}
	else {
		IRenderer* rnd = gEnv.Renderer();
		if (ppRes->Initialise( rnd->GetDevice(), rnd->GetContext() )) {
			m_resources.insert({ ppRes->GetName(), std::move(ppRes) });
		}
	}
}

IBaseResource* ResourcesManager::GetResource(const std::string& name) {
	return m_resources.find( name )->second.get();
}
