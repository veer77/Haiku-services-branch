//----------------------------------------------------------------------
//  This software is part of the OpenBeOS distribution and is covered 
//  by the OpenBeOS license.
//
//  Copyright (c) 2003 Waldemar Kornewald, Waldemar.Kornewald@web.de
//---------------------------------------------------------------------

#ifndef _K_PPP_PROTOCOL__H
#define _K_PPP_PROTOCOL__H

#include <KPPPDefs.h>

class PPPInterface;
class PPPOptionHandler;


class PPPProtocol {
	public:
		PPPProtocol(const char *name, ppp_phase phase, uint16 protocol,
			int32 addressFamily, PPPInterface& interface,
			driver_parameter *settings, int32 flags = PPP_NO_FLAGS,
			const char *type = NULL, PPPOptionHandler *optionHandler = NULL);
		virtual ~PPPProtocol();
		
		virtual status_t InitCheck() const;
		
		const char *Name() const
			{ return fName; }
		
		ppp_phase Phase() const
			{ return fPhase; }
		
		PPPInterface& Interface() const
			{ return fInterface; }
		driver_parameter *Settings() const
			{ return fSettings; }
		
		uint16 Protocol() const
			{ return fProtocol; }
		int32 AddressFamily() const
			{ return fAddressFamily; }
				// negative values and values > 0xFF are ignored
		int32 Flags() const
			{ return fFlags; }
		ppp_side Side() const
			{ return fSide; }
				// which side this protocol works for
		
		// extensions
		const char *Type() const
			{ return fType; }
		PPPOptionHandler *OptionHandler() const
			{ return fOptionHandler; }
		
		void SetEnabled(bool enabled = true);
		bool IsEnabled() const
			{ return fEnabled; }
		
		bool IsUpRequested() const
			{ return fUpRequested; }
		
		virtual status_t Control(uint32 op, void *data, size_t length);
		virtual status_t StackControl(uint32 op, void *data);
			// called by netstack (forwarded by PPPInterface)
		
		virtual bool Up() = 0;
		virtual bool Down() = 0;
			// if DialOnDemand is supported check for DialOnDemand settings change
		bool IsUp() const
			{ return fConnectionStatus == PPP_ESTABLISHED_PHASE; }
		bool IsDown() const
			{ return fConnectionStatus == PPP_DOWN_PHASE; }
		bool IsGoingUp() const
			{ return fConnectionStatus == PPP_ESTABLISHMENT_PHASE; }
		bool IsGoingDown() const
			{ return fConnectionStatus == PPP_TERMINATION_PHASE; }
		
		virtual status_t Send(struct mbuf *packet) = 0;
		virtual status_t Receive(struct mbuf *packet, uint16 protocol) = 0;
		
		virtual void Pulse();

	protected:
		void SetUpRequested(bool requested = true)
			{ fUpRequested = requested; }
		
		void UpStarted();
		void DownStarted();
		
		void UpFailedEvent();
		void UpEvent();
		void DownEvent();
			// report up/down events

	protected:
		ppp_side fSide;
		status_t fInitStatus;

	private:
		char *fName;
		ppp_phase fPhase;
		uint16 fProtocol;
		int32 fAddressFamily;
		PPPInterface& fInterface;
		driver_parameter *fSettings;
		int32 fFlags;
		char *fType;
		PPPOptionHandler *fOptionHandler;
		
		bool fEnabled;
		bool fUpRequested;
		ppp_phase fConnectionStatus;
};


#endif
