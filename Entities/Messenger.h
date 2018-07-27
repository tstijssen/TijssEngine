#pragma once
#include <map>
#include "EntityTypes.h"
using namespace std;

namespace Tijss
{

	// default messages
	enum eMessageType
	{
		Msg_Die,			// destroy entity/component
		Msg_SetupAnimation,	// instruct renderer to setup for rendering animated sprites
		Msg_Collision		// sends the UID of an entity that has collided
	};

	struct sMessage
	{
		sMessage() {};
		sMessage(const sMessage& message)
		{
			memcpy(this, &message, sizeof(sMessage));
		}
		sMessage& operator=(const sMessage& message)
		{
			memcpy(this, &message, sizeof(sMessage));
			return *this;
		}

		eMessageType type;
		TEntityUID from;

		// should all messages have this?
		string text;
		sFloat3 coordinate;	// use z value as layer number
		TEntityUID target;

	};


	class cMessenger
	{
	public:
		cMessenger() {};

		void SendMsg(TEntityUID to, const sMessage& msg);

		bool FetchMsg(TEntityUID to, sMessage* msg);

	private:
		cMessenger(const cMessenger&);
		cMessenger& operator=(const cMessenger&);

		// data
		typedef std::multimap<TEntityUID, sMessage> Msg_Multimap;
		typedef Msg_Multimap::iterator Msg_Iterator;
		typedef std::pair<TEntityUID, sMessage> Msg_UIDPair;

		Msg_Multimap m_Messages;
	};

}
