// A task scheduler that runs task at scheduled time

#include <boost/asio/io_service.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/heap/priority_queue.hpp>
#include <boost/shared_ptr.hpp>

#include <map>

#include <sys/time.h>

namespace unoekone { namespace interview {

class Task {
public:
    Task() {
    }
    
    virtual void run () = 0;
    
    virtual ~Task() {
    }
};

typedef boost::shared_ptr<Task> TaskPtr_t;

class DelayedTaskExecutor {
public:

    DelayedTaskExecutor(unsigned int threadPoolSize);

    void submitTask(TaskPtr_t task, unsigned int timeoffset);

    ~DelayedTaskExecutor();

private:
    bool mRun;
    boost::thread mDispatchThread;
    void dispatchTasks();

    void runTask(TaskPtr_t task);

    boost::mutex mTaskQueueMutex;
    boost::condition_variable mTaskWaitCondition;

    // Thread pool for running tasks
    boost::thread_group mTaskThreadPool;
    boost::asio::io_service mIoService;
    boost::asio::io_service::work mWork;

    typedef std::pair<TaskPtr_t, unsigned int> TaskDelayPair_t;

    class TaskTimeComparator {
        public:
            bool operator()(TaskDelayPair_t const& left, TaskDelayPair_t const& right) const {
                return left.second > right.second;
            }
    };

    void runIoService() {
        mIoService.run();
    }

    boost::heap::priority_queue<TaskDelayPair_t, boost::heap::compare<TaskTimeComparator> > mPq;
};

DelayedTaskExecutor::DelayedTaskExecutor(unsigned int threadPoolSize)
    : mRun(true), mDispatchThread(&DelayedTaskExecutor::dispatchTasks, this), mWork(mIoService) {

    for (unsigned int i = 0; i < threadPoolSize; ++i) {
            // Create threads in pool
            mTaskThreadPool.create_thread(boost::bind(&DelayedTaskExecutor::runIoService, this));
    }
}

DelayedTaskExecutor::~DelayedTaskExecutor() {
    {
        boost::mutex::scoped_lock lock(mTaskQueueMutex);

        mRun = false;

        mTaskWaitCondition.notify_all();
    }

    mDispatchThread.join();

    mPq.clear();

    // Stop thread pool
    mIoService.stop();
    mTaskThreadPool.join_all();
}

void DelayedTaskExecutor::submitTask(TaskPtr_t task, unsigned int timeoffset) {
    boost::mutex::scoped_lock lock(mTaskQueueMutex);

    timeval nowTval;
    gettimeofday(&nowTval, NULL);
    unsigned int now = nowTval.tv_sec * 1000 + nowTval.tv_usec/1000; // Millis

    mPq.push(std::make_pair(task, now + timeoffset));

    mTaskWaitCondition.notify_all();
}

void DelayedTaskExecutor::dispatchTasks() {
    boost::mutex::scoped_lock lock(mTaskQueueMutex);

    while (mRun) {
        timeval nowTval;
        gettimeofday(&nowTval, NULL);
        unsigned int now = nowTval.tv_sec * 1000 + nowTval.tv_usec/1000; // Millis

        while (!mPq.empty()) {
            TaskDelayPair_t const& topElem = mPq.top();
            if (topElem.second < now) {
                // runTask(topElem.first);
                mIoService.post(boost::bind(&DelayedTaskExecutor::runTask, this, topElem.first));

                mPq.pop();
            }
            else {
                break;
            }
        }

        if (!mPq.empty()) {
            TaskDelayPair_t const& topElem = mPq.top();

            if (mRun && topElem.second > now) {
                boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(topElem.second - now);
                mTaskWaitCondition.timed_wait(lock, timeout);
            }
        }
        else {
            while (mRun && mPq.empty()) {
                mTaskWaitCondition.wait(lock);
            }
        }
    }
}

void DelayedTaskExecutor::runTask(TaskPtr_t task) {

    std::cout << "Thread id: " << boost::this_thread::get_id() << std::endl;
    task->run();
}

} }

int main(int argc, char** argv) {

    class PrintTask : public unoekone::interview::Task {
    public:
        PrintTask(int id) 
            : mId(id) {
        }

        void run() {
            std::cout << "Running task id: " << mId << " at " << time(NULL) << std::endl;
        }

        ~PrintTask() {
            std::cout << "Task getting destroyed: " << mId << std::endl;
        }

    private:
        int mId;
    };


    unoekone::interview::DelayedTaskExecutor delayedTaskExecutor(2);

    std::cout << "Time = " << time(NULL) << std::endl;

    delayedTaskExecutor.submitTask(unoekone::interview::TaskPtr_t(new PrintTask(1)), 4000);

    sleep(1);

    delayedTaskExecutor.submitTask(unoekone::interview::TaskPtr_t(new PrintTask(2)), 1000);
    delayedTaskExecutor.submitTask(unoekone::interview::TaskPtr_t(new PrintTask(3)), 2000);
    
    sleep(5);

    std::cout << "Time = " << time(NULL) << std::endl;

    return 0;
}
