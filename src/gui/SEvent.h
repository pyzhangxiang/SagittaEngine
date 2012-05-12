//////////////////////////////////////////////////////
// file: SEvent.h @ 2008-12-7 by Zhang Xiang
// declares of the class SEvent
// SEvent is a class ...
//////////////////////////////////////////////////////
#ifndef __SEVENT_H__
#define __SEVENT_H__

// INCLUDES //////////////////////////////////////////
#include "GuiDef.h"

// DECLARES //////////////////////////////////////////

namespace Sagitta{

	class SListener;

	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SEvent{
	// enum defines
	public:
		enum EventType{
			ET_INVALID = 0,
			ET_NONE = 1,				// invalid event
			ET_TIMER = 2,				// timer event
			
			// mouse vent
			ET_MOUSE_MOVE,
			ET_LBUTTON_DOWN,
			ET_LBUTTON_UP,
			ET_LBUTTON_DBLCLK,
			ET_RBUTTON_DOWN,
			ET_RBUTTON_UP,
			ET_RBUTTON_DBLCLK,
			ET_MBUTTON_DOWN,
			ET_MBUTTON_UP,
			ET_MBUTTON_DBLCLK,
			// wheel
			ET_MOUSE_WHEEL,

			// key event
			ET_KEY_DOWN,
			ET_KEY_UP,
			ET_CHAR,

			// window event
			ET_CREATE,
			ET_DESTROY,
			ET_PAINT,
			ET_MOVE,
			ET_RESIZE,
			ET_SHOW,
			ET_HIDE,
			ET_CLOSE,
			ET_ACTIVATE,
			ET_DEACTIVATE,
			ET_ERASEBKGND,


			// app event
			ET_QUIT,

		};

	// member variables
	private:
		EventType m_Type;
		uLong m_iParam1;
		uLong m_iParam2;

		// Time is no need for implementation.
		//uLong m_iTime;

		/// the receiver of this event
		SListener *m_pReceiver;

	// constructors & destructor
	public:
		SEvent(void)
			: m_Type(ET_INVALID), m_iParam1(0), m_iParam2(0), m_pReceiver(0){

		}

		SEvent(EventType aType, uLong aParam1, uLong aParam2, SListener *aReceiver)
			: m_Type(aType), m_iParam1(aParam1), m_iParam2(aParam2), m_pReceiver(aReceiver){

		}

		SEvent(const SEvent &aEvent)
			: m_Type(aEvent.m_Type), m_iParam1(aEvent.m_iParam1), m_iParam2(aEvent.m_iParam2), m_pReceiver(aEvent.m_pReceiver){

		}

		virtual ~SEvent(){}

	// member functions
	public:
		SEvent &operator=(const SEvent aEvent){
			m_Type = aEvent.m_Type;
			m_iParam1 = aEvent.m_iParam1;
			m_iParam2 = aEvent.m_iParam2;
			m_pReceiver = aEvent.m_pReceiver;

			return *this;
		}

	public:
		EventType type(void) const{ return m_Type; }
		uLong param1(void) const{ return m_iParam1; }
		uLong param2(void) const{ return m_iParam2; }
		SListener *receiver(void) const{ return m_pReceiver; }


	}; //#### end class SEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SMouseEvent : public SEvent{
	// constructors & destructor
	public:
		SMouseEvent(EventType aType, uLong aButton, uLong aModifiers, const SPoint &aPoint, SListener *aReceiver)
			: SEvent(aType, (aButton << 16) + (aModifiers & 0xffff),
					(static_cast<uLong>(aPoint.x) << 16) + (static_cast<uLong>(aPoint.y) & 0xffff),
					aReceiver){
		}

	// member function
	public:
		SPoint point(void) const{ uLong point = SEvent::param2(); return SPoint(static_cast<float>(point >> 16), static_cast<float>(point & 0xffff)); }
		uLong button(void) const{ return SEvent::param1() >> 16; }
		uLong modifiers(void) const{ return SEvent::param1() & 0xffff; }

	}; //#### end class SMouseEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SWheelEvent : public SEvent{
	// constructors & destructor
	public:
		SWheelEvent(EventType aType, int aZDelta, uLong aModifiers, const SPoint &aPoint, SListener *aReceiver)
			: SEvent(aType, (((uLong)(short)aZDelta) << 16) + (aModifiers & 0xffff),
					(static_cast<uLong>(aPoint.x) << 16) + (static_cast<uLong>(aPoint.y) & 0xffff),
					aReceiver){
		}

	// member function
	public:
		SPoint point(void) const{ uLong point = SEvent::param2(); return SPoint(static_cast<float>(point >> 16), static_cast<float>(point & 0xffff)); }
		int zdelta(void) const{ return (int)(SEvent::param1() >> 16); }
		uLong modifiers(void) const{ return SEvent::param1() & 0xffff; }

	}; //#### end class SWheelEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SKeyEvent : public SEvent{
	// constructors & destructor
	public:
		SKeyEvent(EventType aType, uLong aKey, uLong aModifiers, uLong aCount, SListener *aReceiver)
			: SEvent(aType, aKey, (aCount << 16) + (aModifiers & 0xffff), aReceiver){
		}

	// member function
	public:
		uLong key(void) const{ return SEvent::param1(); }
		uLong modifiers(void) const{ return SEvent::param2() & 0xffff; }
		uLong repeatCount(void) const{ return SEvent::param2() >> 16; }

	}; //#### end class SKeyEvent


	// for future ...
	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SCreateEvent : public SEvent{
	// constructors & destructor
	public:
		SCreateEvent(EventType aType, SWindowHandle aHandle, SListener *aReceiver)
			: SEvent(aType, reinterpret_cast<uLong>(aHandle), 0, aReceiver){
		}

	// member function
	public:
		SWindowHandle handle(void) const{ return reinterpret_cast<SWindowHandle>(SEvent::param1()); }

	}; //#### end class SCreateEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SMoveEvent : public SEvent{
	// constructors & destructor
	public:
		SMoveEvent(EventType aType, int aX, int aY, SListener *aReceiver)
			: SEvent(aType, (uLong)aX, (uLong)aY, aReceiver){
		}

	// member function
	public:
		int x(void) const{ return static_cast<int>(SEvent::param1()); }
		int y(void) const{ return static_cast<int>(SEvent::param2()); }

	}; //#### end class SMoveEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SEraseBkgndEvent : public SEvent{
	// constructors & destructor
	public:
		SEraseBkgndEvent(EventType aType, SContextHandle aContextHandle, SListener *aReceiver)
			: SEvent(aType, reinterpret_cast<uLong>(aContextHandle), 0, aReceiver){
		}

	// member function
	public:
		SContextHandle contextHandle(void) const{ return reinterpret_cast<SContextHandle>(param1()); }

	}; //#### end class SEraseBkgndEvent


	/** class representation
	@remarks

	@note

	@par

	*/
	class _SG_GuiExport SResizeEvent : public SEvent{
		// constructors & destructor
	public:
		SResizeEvent(EventType aType, /*uLong aStatus, */uLong aWidth, uLong aHeight, SListener *aReceiver)
			: SEvent(aType, (uLong)aWidth, (uLong)aHeight, aReceiver){
		}

		// member function
	public:
		uLong width(void) const{ return SEvent::param1(); }
		uLong height(void) const{ return SEvent::param2(); }

	}; //#### end class SResizeEvent

} // namespace Sagitta

// DEFINES ///////////////////////////////////////////

#endif // __SEVENT_H__