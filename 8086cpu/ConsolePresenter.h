#pragma once
#include "AbstractPresenter.h"
#include "cpu8086.h"
#include "Memory.h"
#include <thread>
#include <iostream>

class ConsolePresenter : public AbstractPresenter {
public:
	virtual void notifyRegChange() override;
	virtual void notifyMemChange() override;
	virtual void notifyStkChange() override;

	ConsolePresenter(std::shared_ptr<cpu8086> p_cpu, std::shared_ptr<Memory> p_mem);
	virtual void Render() override;
};