#ifndef _I_SCENE_OBJECT_FACTORY_H_
#define _I_SCENE_OBJECT_FACTORY_H_

#include "..\SceneObject.h"

class ISceneObjectCreator {
public:
	virtual SceneObject* Create(const std::string&) = 0;
};

#endif //_I_SCENE_OBJECT_FACTORY_H_