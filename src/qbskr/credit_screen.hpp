#ifndef HEADER_QBSKR_QBSKR_CREDIT_SCREEN_HPP
#define HEADER_QBSKR_QBSKR_CREDIT_SCREEN_HPP

#include <memory>
#include <string>

#include "qbskr/screen.hpp"

class CreditScroller;

class CreditScreen final : public Screen {
public:
	~CreditScreen() override;

private:
	CreditScreen(const CreditScreen&) = delete;
	CreditScreen& operator=(const CreditScreen&) = delete;

private:
	std::unique_ptr<CreditScroller> m_credit_scroller;

public:
	CreditScreen(const std::string& filename);

public:
	virtual void setup() override;
	virtual void leave() override;
	virtual void update(float dt_sec, const Controller& controller) override;
	virtual void draw(Compositor& compositor) override;

};

#endif