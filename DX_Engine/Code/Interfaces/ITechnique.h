#ifndef _INTERFACE_TECHNIQUE_H_
#define _INTERFACE_TECHNIQUE_H_
#pragma once

struct ITechnique {
	virtual ~ITechnique();
	virtual bool Initialise(IDevice* device) = 0;
	virtual void Draw(IDevCon* context) = 0;
	//virtual void SetData(T const& data);
};

#endif //_INTERFACE_TECHNIQUE_H_
