#include "Multithreading.h"

using namespace multiThread;

MultiThreader::MultiThreader()
{
	// start worker threads

	m_NumWorkers = std::thread::hardware_concurrency();

	if (m_NumWorkers == 0) m_NumWorkers = 8;	// default num of workers
	--m_NumWorkers; // main thread already running

	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		m_CollisionWorkers[i].first.thread = std::thread(&MultiThreader::CollisionsThread, this, i);
	}
}

MultiThreader::~MultiThreader()
{
	for (uint32_t i = 0; i < m_NumWorkers; ++i)
	{
		m_CollisionWorkers[i].first.thread.detach();
	}
}

void MultiThreader::CollisionsThread(uint32_t thread)
{
	auto& worker = m_CollisionWorkers[thread].first;
	auto& work = m_CollisionWorkers[thread].second;

	while (true)
	{
		// guard work from other threads
		{
			std::unique_lock<std::mutex> lock(worker.lock);
			worker.workReady.wait(lock, [&]() {return !work.complete; });
		}

		// run collision function
		ComponentCollisions(work.target, work.start, work.end);
		//SpriteCollisions(work.sprite, work.collidersStart, work.collidersEnd);

		{
			std::unique_lock<std::mutex> lock(worker.lock);
			work.complete = true;
		}
		worker.workReady.notify_one();
	}
}

void MultiThreader::ComponentCollisions(cColliderComponent* target, std::vector<cColliderComponent*>::iterator start, std::vector<cColliderComponent*>::iterator end)
{
	std::vector<cColliderComponent*>::iterator iter = start;
	for (iter; iter != end; ++iter)
	{
		if ((*iter) != target)
		{
			(*iter)->CheckCollisions(target);
		}
	}
}