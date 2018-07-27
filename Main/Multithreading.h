#pragma once
#include <thread>
#include <condition_variable>
#include "TypeDefines.h"
#include "CollisionComponent.h"

using namespace Tijss;

namespace multiThread
{
	struct WorkerThread
	{
		std::thread				thread;
		std::condition_variable workReady;
		std::mutex				lock;
	};

	struct ThreadParameters
	{
		bool complete = true;
		Tijss::cColliderComponent* target;
		std::vector<Tijss::cColliderComponent*>::iterator start;
		std::vector<Tijss::cColliderComponent*>::iterator end;
	};

	class MultiThreader
	{
	public:
		MultiThreader();
		~MultiThreader();

		void CollisionsThread(uint32_t thread);
		void ComponentCollisions(cColliderComponent* target, std::vector<cColliderComponent*>::iterator start, std::vector<cColliderComponent*>::iterator end);

		static const uint32_t MAX_WORKERS = 31;
		std::pair<WorkerThread, ThreadParameters> m_CollisionWorkers[MAX_WORKERS];
		uint32_t m_NumWorkers;

	};
}