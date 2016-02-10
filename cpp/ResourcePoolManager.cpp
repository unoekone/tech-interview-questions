// ResourcePoolManager to manage a generic pool of resources

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include <iostream>

#include <vector>
#include <set>

template<typename T>
class ResourcePoolManager {
public:

	typedef int ResourceId;

	class ResourceContainer {
	public:
	
		ResourceContainer(T &resource, ResourceId id, ResourcePoolManager &poolManager)
				: mResource(resource), mResourceId(id), mPoolManager(poolManager) {
		}
	
		~ResourceContainer() {
			mPoolManager.checkIn(mResourceId);
		}
	
		T const& getResource() {
			return mResource;
		}
	
		ResourceId getResourceId() {
			return mResourceId;
		}
	
	private:
		T &mResource;
		ResourceId mResourceId;
		ResourcePoolManager &mPoolManager;
	};

	ResourcePoolManager() {
	}
	
	ResourcePoolManager& addResource(T resource) {
		boost::mutex::scoped_lock lock(mResourceMutex);

		mResources.push_back(resource);
		mCheckIn.notify_all();

		return *this;
	}

	ResourceContainer checkOut() {
		boost::mutex::scoped_lock lock(mResourceMutex);

		while(true) {
			for (int i = 0; i < mResources.size(); ++i) {
				if (mCheckedoutResources.find(i) == mCheckedoutResources.end()) {
					mCheckedoutResources.insert(i);
					return ResourceContainer(mResources[i], i, *this);
				}
			}

			mCheckIn.wait(lock);
		}
	}

	void checkIn(ResourceId id) {
		boost::mutex::scoped_lock lock(mResourceMutex);

		if (mCheckedoutResources.find(id) != mCheckedoutResources.end()) {
			mCheckedoutResources.erase(id);

			mCheckIn.notify_all();
		}
	}

	void shutdown() {
		boost::mutex::scoped_lock lock(mResourceMutex);

		while (!mCheckedoutResources.empty()) {
			mCheckIn.wait(lock);
		}
	}

	~ResourcePoolManager() {
		shutdown();
	}

	void print() {
		boost::mutex::scoped_lock lock(mResourceMutex);

		int i = 0;
		for (auto const& e : mResources) {
			std::cout << "Resource[" << i++ << "]: " << e << std::endl;
		}
	}

private:
	boost::mutex mResourceMutex;
	boost::condition_variable mCheckIn;

	std::vector<T> mResources;
	std::set<ResourceId> mCheckedoutResources;
	
};

int main(int argc, char** argv) {
	ResourcePoolManager<int> primeRM;

	primeRM.addResource(1).addResource(3).addResource(5).addResource(7).addResource(11);

	primeRM.print();

	ResourcePoolManager<int>::ResourceContainer container = primeRM.checkOut();

	std::cout << "Resource id: " << container.getResourceId() << ", resource : " << container.getResource() << std::endl;

	return 0;
}
