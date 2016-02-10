#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

#include <deque>
#include <iostream>

template <typename T>
class BlockingQueue {

public:
	BlockingQueue(int size)
		: mSize(size) {
	}

	void add(T elem) {
		boost::mutex::scoped_lock lock(mQueueMutex);

		while (mQueue.size() >= mSize) {
			mFull.wait(lock);
		}

		mQueue.push_back(elem);
		mEmpty.notify_all();
	}

	T get() {
		boost::mutex::scoped_lock lock(mQueueMutex);

		while (mQueue.empty()) {
			mEmpty.wait(lock);
		}

		T retval = mQueue.front();
		mQueue.pop_front();

		mFull.notify_all();

		return retval;
	}

	void print() {
		boost::mutex::scoped_lock lock(mQueueMutex);

		int i = 0;
		for (auto const& it : mQueue) {
			std::cout << "mQueue[" << i++ << "] = " << it << std::endl;
		}
	}


private:
	boost::mutex mQueueMutex;
	boost::condition_variable mFull;
	boost::condition_variable mEmpty;

	int mSize;
	std::deque<T> mQueue;

};

int main(int argc, char **argv) {

	BlockingQueue<int> bq(5);

	for (int i = 0; i < 3; ++i) {
		bq.add(rand() % 100);
	}

	bq.print();

	for (int i = 0; i < 3; ++i) {
		std::cout << "bq[" << i << "] = " << bq.get() << std::endl;
	}

	return 0;
}
