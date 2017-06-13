# OEasyPool

 The thread pool, is so simple and easy to use.
 Do you have any reason to refuse?

# Presentation

 Here are mainly introduced about the use of task thread pool and structure.

![thread pool uml](https://github.com/chenluyong/OEasyPool/blob/master/doc/chart/task%20thread%20pool%20chart%20uml%20-%202017-06-13.jpg)


## Class

- OETask

  task base class, pool call `doWork` function.

  derived class: main logic function.

  virtual int doWork() = 0;

- OETaskQueue
  
  The management of the task，for example: add/sub/recall status.

- OEThreadPool

  The processing of the thread pool

## Extend
 
 task thread pool extend, we provide a good solution.
 inherit `OETask`, complete the main logic function.

 for example: see `TaskTest.h`.

## Config

 Regarding the thread pool configuration, we also set aside the interface


 use class `OEThreadPool` :: struct `tagThreadPoolConfig` and function `init`, can config thread pool.

 Code has more detailed comments, but I still want to post here again, though he is very simple, but it seems very important.
 
```
/// thread config
typedef struct tagThreadPoolConfig {
	int nMaxThreadsNum;		    /// max thread number
	int nMinThreadsNum;		    /// min thread number
	double dbTaskAddThreadRate;   /// add - max thread task rate （decide when to increase）
	double dbTaskSubThreadRate;   /// sub - max thread task rate （decide when to reduce）
} ThreadPoolConfig;
```

 before init, thread pool didn't do anything.
 

# Other

 You didn't do anything, You get it.

 I just hope find the insufficient, if you get wrong anything, please tell me. thanks.

 Before this, you should star to it, I believe that it can keep in touch between you and me.

 If you are an overseas Chinese who can contact the server in we home, you may need this article [[Go](http://blog.csdn.net/csnd_ayo/article/details/72457190)].


