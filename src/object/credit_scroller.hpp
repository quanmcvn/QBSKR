#ifndef HEADER_QBSKR_OBJECT_CREDIT_SCROLLER_HPP
#define HEADER_QBSKR_OBJECT_CREDIT_SCROLLER_HPP

#include <memory>
#include <vector>

#include "object/game_object.hpp"

class CrappyReaderData;
class Controller;
class DrawingContext;
class InfoBoxLine;

/**
 *  Class to scroll throught credit (obviously)
*/
class CreditScroller final : public GameObject {
private:
	CreditScroller(const CreditScroller&) = delete;
	CreditScroller& operator=(const CreditScroller&) = delete;

private:
	const Controller* m_controller;
	std::vector<std::unique_ptr<InfoBoxLine>> m_lines;
	float m_scroll;
	float m_default_speed;
	bool m_finished;
	bool m_fading;

public:
	CreditScroller(const CrappyReaderData* crd);

private:
	void parse_content(const CrappyReaderData* crd_content);

public:
	virtual void update(float dt_sec) override;
	virtual void draw(DrawingContext& drawing_context) override;

public:
	void set_default_speed(float default_speed);
};

#endif