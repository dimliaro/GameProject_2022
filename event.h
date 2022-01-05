#pragma once

class Event
{
protected:
	float m_pos_x;
	float m_pos_y;


	float m_duration = 2.0f;
	float m_delay = 0.0f;
	float m_elapsed_time = 0.0f;
	float m_elapsed_delay = 0.0f;

	bool m_active = true;
public:
	virtual void draw() {};
	virtual void update();
	Event(float x = 0.0f, float y = 0.0f, float dur = 2.0f , float del=0.0f);

	virtual ~Event() {};

	bool active() { return m_active; }
	void disable() { m_active = false; }
	bool waiting();
};

class SmokeEvent : public Event
{
	float m_orientation;
	float m_scale;

public:
	void draw() override;
	SmokeEvent(float x, float y);
};