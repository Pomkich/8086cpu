#pragma once
#include <memory>
class cpu8086;
class Memory;

class AbstractPresenter {
protected:
	std::shared_ptr<cpu8086> cpu_pt;
	std::shared_ptr<Memory> mem_pt;

public:
	virtual void notifyRegChange() = 0;	// register changed
	virtual void notifyMemChange() = 0;	// memory changed
	virtual void notifyStkChange() = 0;	// stack changed

	virtual void Render() = 0;
};