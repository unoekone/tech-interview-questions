// A queue that blocks to get an element until the queue has one
// and similarly blocks to add an element to the queue until the queue has room

#include <condition_variable>
#include <mutex>

#include <deque>
#include <iostream>

template <typename T>
class BlockingQueue {

public:
	BlockingQueue(int size)
		: mSize(size) {
	}

	void add(T elem) {
		std::unique_lock<std::mutex> lock(mQueueMutex);

		while (mQueue.size() >= mSize) {
			mFull.wait(lock);
		}

		mQueue.push_back(elem);
		mEmpty.notify_all();
	}

	T get() {
		std::unique_lock<std::mutex> lock(mQueueMutex);

		while (mQueue.empty()) {
			mEmpty.wait(lock);
		}

		T retval = mQueue.front();
		mQueue.pop_front();

		mFull.notify_all();

		return retval;
	}

	void print() {
		std::unique_lock<std::mutex> lock(mQueueMutex);

		int i = 0;
		// for (typename std::deque<T>::const_iterator it = mQueue.begin(), itend = mQueue.end();
				// it != itend; ++it) {
		for (auto const& it : mQueue) {
			std::cout << "mQueue[" << i++ << "] = " << it << std::endl;
		}
	}


private:
	std::mutex mQueueMutex;
	std::condition_variable mFull;
	std::condition_variable mEmpty;

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
