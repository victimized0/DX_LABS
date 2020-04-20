#ifndef _INTERFACE_MESH_LOADER_H_
#define _INTERFACE_MESH_LOADER_H_

#pragma once

struct IMeshLoader {
	virtual bool LoadFile(std::string path) = 0;
};

#endif //_INTERFACE_MESH_LOADER_H_
