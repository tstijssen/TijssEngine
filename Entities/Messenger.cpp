#include "Messenger.h"

namespace Tijss
{
	// single global messenger object

	cMessenger Messenger;

	void cMessenger::SendMsg(TEntityUID to, const sMessage& msg)
	{
		m_Messages.insert(Msg_UIDPair(to, msg));
	}

	bool cMessenger::FetchMsg(TEntityUID to, sMessage* msg)
	{
		// find first message for given target
		Msg_Iterator iter = m_Messages.find(to);

		// if none found; exit
		if (iter == m_Messages.end())
		{
			return false;
		}

		// otherwise assign message value to pointer and remove from messages list
		*msg = iter->second;
		m_Messages.erase(iter);

		return true;
	}

}