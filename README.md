# OEasyPool

 The thread pool, is so simple and easy to use.
 Do you have any reason to refuse?

# Presentation

 这里主要介绍关于任务线程池的使用及结构。

## 类

- Task （任务基类）

  该类主要实现一个任务类
  virtual int doWork() = 0;

- TaskQueue （任务队列）
  
  该类主要针对任务的存储、删除、撤回等状态做管理

- ThreadPool （线程池）

  整个线程池的核心业务处理类

## 扩展

 关于任务线程池的扩展，我们已经提供了很好的解决方案。
 例如继承 `Task` 类，来完成线程任务的扩展。

 例如案例中 `TaskTest.h` 中使用的那样，将主要的业务逻辑放在 doWork 中即可。

## 配置

 关于线程池的配置，我们也留出了接口。
 通过 `OEThreadPool` 类中的 `tagThreadPoolConfig` 结构体与 `init` 函数，进行线程池的资源配置。

 代码中已经有了比较详尽的注释，但是在这里我还是想贴出来再强调一遍，虽然他很简单，但是看起来非常重要。
 
```
/// 线程池配置参数
typedef struct tagThreadPoolConfig {
	int nMaxThreadsNum;		    /// 最大线程数量
	int nMinThreadsNum;		    /// 最小线程数量
	double dbTaskAddThreadRate;   /// 增 最大线程任务比 (任务数量与线程数量，什么比例的时候才加)
	double dbTaskSubThreadRate;   /// 减 最小线程任务比 (任务数量与线程数量，什么比例的时候才减)
} ThreadPoolConfig;
```

 线程池在没有合理的init之前，是不会开展工作的。

 

# 其他

 You didn't do anything, You get it.

 I just hope find the insufficient, if you get wrong anything, please tell me. thanks.

 Before this, you should star to it, I believe that it can keep in touch between you and me.

 If you are an overseas Chinese who can contact the server in we home, you may need this article [[Go](http://blog.csdn.net/csnd_ayo/article/details/72457190)].


