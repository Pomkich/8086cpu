#pragma once
#include "cpu8086.h"
#include "Memory.h"

class AbstractPresenter {
protected:
	std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<Memory> mem_pt;

	virtual void notifyRegChange() = 0;	// register changed
	virtual void notifyMemChange() = 0;	// memory changed
	virtual void notifyStkChange() = 0;	// stack changed

public:
	virtual void Render() = 0;
};