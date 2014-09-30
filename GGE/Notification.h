#ifndef __GGE_NOTIFICATION__
# define __GGE_NOTIFICATION__

# include <list>

namespace GGE
{
	class NotificationHolder
	{
	public:
		typedef size_t Notification;	// as undefined enum

		bool	PushNotification(const Notification & n)
		{
			if (this->_pendingNotifications.size() > MAX_QUEUED_NOTIFICATION_QTY)
				return false;
			this->_pendingNotifications.push_back(n);
			return true;
		}
	protected:

		bool	PullNotification(Notification & n)	// return IsTherePendingNotification ?
		{
			if (this->_pendingNotifications.empty()) return false;
			n = this->_pendingNotifications.back();
			this->_pendingNotifications.pop_back();
			return true;
		}

		std::list<Notification>	_pendingNotifications;
		static const size_t MAX_QUEUED_NOTIFICATION_QTY = 128;
	};
}

#endif // __GGE_NOTIFICATION__