/**
* @file   : taskqueue.h
* @brief  : 任务队列，主要保证任务的执行和访问安全
* @version: 1.0
* @author : 陈鲁勇
* @create : 2017年04月
* @date   : 2017年6月3日
* @note   : 根据 Apache 许可证 2.0 版（以下简称“许可证”）授权;
*           除非遵守本许可，否则您不能使用这个文件。
* @remarks: 您可以获得该许可的副本：
*           http://www.apache.org/licenses/LICENSE-2.0
*           除非适用法律需要或者书面同意，按本许可分发的软件
*           要按“原样”分发，没有任何形式的，明确或隐含的担保条款。
*           参见按照本许可控制许可权限及限制的特定语言的许可证。
*
*   你可以获得该代码的最新版本：
*
*        http://git.oschina.net/Mr_ChenLuYong/oeasypool
*
*   开源社区的所有人都期待与你的共同维护。
*
*
*   如果你对这些代码还有不理解的地方可以通过最新的文章进行学习：
*
*        博客地址：http://blog.csdn.net/csnd_ayo
*
*        文章地址：http://blog.csdn.net/csnd_ayo/article/details/72457190
*
* 	 期待你提交Bug，欢迎Issues。
*
*/


#ifndef __OETASKQUEUE_H__
#define __OETASKQUEUE_H__

#include <deque>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <memory>
#include <thread>


/// 任务队列类
template<class T>
class OETaskQueue {

private:

    /// 就绪任务队列                               
    std::mutex                          mutexQueue_;
    std::deque<std::shared_ptr<T> >     queue_;
    /// 条件变量                                      
    std::mutex                          mutexConditPut_;
    std::condition_variable             conditPut_;
    /// 运行任务map                                   
    std::mutex                          mutexDoingTask_;
    std::unordered_map<size_t, std::shared_ptr<T> > mapDoingTask_;

public:

    /**
    * @brief ：向队列的末尾插入任务
    * @param ：task 任务类
    */
    void put_back(std::shared_ptr<T> &task) {
        std::unique_lock<std::mutex> lock(mutexQueue_);

        queue_.push_back(task);

        conditPut_.notify_one();
    }
    /**
    * @brief ：向队列的头部插入任务
    * @param ：task 任务类
    */
    void put_front(std::shared_ptr<T> &task) {
        std::unique_lock<std::mutex> lock(mutexQueue_);

        queue_.push_front(task);

        conditPut_.notify_one();
    }
    /**
    * @brief ：获取队首（并将任务加到运行任务列表中）
    * @return：任务类
    */
    std::shared_ptr<T> get(void) {

        std::unique_lock<std::mutex> lock(mutexQueue_);

        if (queue_.empty())
            return nullptr;


        std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);
        std::shared_ptr<T> &task = queue_.front();
        mapDoingTask_.insert(std::make_pair(task->getID(), task));
        queue_.pop_front();

        return task;
    }
    /**
    * @brief ：获取整个双向链表的大小
    * @return：大小
    */
    size_t size(void) {
        std::unique_lock<std::mutex> lock(mutexQueue_);
        return queue_.size();
    }

    /**
    * @brief ：释放队列
    */
    void release(void) {

        //将就绪队列中的任务删除
        {
            std::unique_lock<std::mutex> lock(mutexQueue_);

            if (!queue_.empty())
                queue_.clear();//清空
        }

        {
        std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);

        if (!mapDoingTask_.empty()) {

            // std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
            auto it_map = mapDoingTask_.begin();

            for (; it_map != mapDoingTask_.end(); ++it_map)
                it_map->second->setCancelRequired();

        }
    }


        conditPut_.notify_all();
    }
    /**
    * @brief ：删除任务（从就绪队列删除，如果就绪队列没有，则看执行队列有没有，有的话置下取消状态位）
    * @param ：nID 任务的编号
    * @return：成功返回0 失败返回非0
    */
    int deleteTask(size_t nID) {

        std::unique_lock<std::mutex> lock(mutexQueue_, std::defer_lock);
        lock.lock();


        // std::deque<std::shared_ptr<OETask>>::iterator
        auto it = queue_.begin();
        for (; it != queue_.end(); ++it) {
            if ((*it)->getID() == nID) {
                queue_.erase(it);
                lock.unlock();
                return 0;
            }
        }
        //下面的逻辑可能会造成死锁，这里要及时释放
        lock.unlock();

        // 试图取消正在执行的任务
        {
            std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);
            // std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
            auto it_map = mapDoingTask_.find(nID);
            if (it_map != mapDoingTask_.end())
                it_map->second->setCancelRequired();
        }




        //任务执行完后再返回
        while (mapDoingTask_.count(nID))
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

        return 0;
    }
    /**
    * @brief ：删除所有任务
    * @return：成功返回0 失败返回非0
    */
    int deleteAllTasks(void) {


        std::unique_lock<std::mutex> lock(mutexQueue_, std::defer_lock);
        lock.lock();

        if (!queue_.empty())
            queue_.clear();//清空


        {
            std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);
            if (!mapDoingTask_.empty()) {

                // std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
                auto it_map = mapDoingTask_.begin();

                for (; it_map != mapDoingTask_.end(); ++it_map)
                    it_map->second->setCancelRequired();

            }
        }

        lock.unlock();


        //任务执行完后再返回
        while (mapDoingTask_.empty())
            std::this_thread::sleep_for(std::chrono::milliseconds(50));


        return 0;
    }
    /**
    * @brief ：任务完成回调（从运行列表中删除指定任务）
    * @param ：nID 任务的编号
    * @return：成功返回0 失败返回非0
    */
    int onTaskFinished(size_t nID) {

        std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);

        // std::unordered_map<size_t, std::shared_ptr<OETask>>::iterator
        auto it_map = mapDoingTask_.find(nID);

        if (it_map != mapDoingTask_.end())
            mapDoingTask_.erase(it_map);

        return 0;
    }
    /**
    * @brief ：判断任务是否执行完毕
    * @param ：nID 任务的编号
    * @return：任务类
    */
    std::shared_ptr<T> isTaskProcessed(size_t nId) {
        std::lock_guard<std::mutex> lock_queue(mutexQueue_);
        // std::deque<std::shared_ptr<OETask>>::iterator
        auto it = queue_.begin();
        for (; it != queue_.end(); ++it) {

            if ((*it)->getID() == nId)
                return *it;

        }

        std::lock_guard<std::mutex> lock_doing_task(mutexDoingTask_);
        // std::unordered_map<int64_t, std::shared_ptr<OETask>>::iterator
        auto it_map = mapDoingTask_.find(nId);
        if (it_map != mapDoingTask_.end())
            return it_map->second;


        return nullptr;
    }

    /**
    * @brief ：等待有任务到达（带超时：超时自动唤醒）
    * @param ：millsec 超时时间（毫秒）
    * @return：成功返回true 失败返回false
    */
    bool wait(std::chrono::milliseconds millsec) {
        std::unique_lock<std::mutex> lock(mutexConditPut_);
        conditPut_.wait_for(lock, millsec);

        return true;
    }

};

#endif // __OETASKQUEUE_H__