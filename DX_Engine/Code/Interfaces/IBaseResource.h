#ifndef _INTERFACE_BASE_RESOURCE_H_
#define _INTERFACE_BASE_RESOURCE_H_
#pragma once

struct IBaseResource {
public:
	IBaseResource(const std::string& name)
		: m_name(name)
	{}

	virtual bool				Initialise(IDevice* pDev, IDevCon* pCon) = 0;
	virtual void				GenerateHash() = 0;
	virtual uint32_t			GetHash() { return m_crc32c; }
	virtual const std::string&	GetName() { return m_name; }

protected:
	std::string					m_name;
	uint32_t					m_crc32c;

};

#endif //_INTERFACE_BASE_RESOURCE_H_
