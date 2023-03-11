#pragma once

class AbstractPresenter {
private:
	virtual void notifyRegChange() = 0;
	virtual void notifyMemChange() = 0;
	virtual void notifyStkChange() = 0;

public:
	virtual void Render() = 0;
};