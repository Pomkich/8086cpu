#pragma once
#include "AbstractPresenter.h"
#include <iostream>

class ConsolePresenter : public AbstractPresenter {
private:
	void notifyRegChange() override;
	void notifyMemChange() override;
	void notifyStkChange() override;

public:
	ConsolePresenter(std::shared_ptr<cpu8086> p_cpu, std::shared_ptr<Memory> p_mem);
	void Render() override;
};