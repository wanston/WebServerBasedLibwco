# WebServerBasedLibwco
基于[Libwco](https://github.com/wanston/libwco)协程库实现的web静态服务器。

# 特点
由于采用了Libwco协程库，省去了繁琐的回调，代码逻辑及其清晰，显著提高了开发速度。

# 设计
多线程+协程：
n条工作线程，每条线程开始时，运行一条accept协程，accept协程负责为每条连接创建一条io协程。

# 测试
使用webbench简单地测了一下，结果如图。
![测试结果](https://github.com/wanston/WebServerBasedLibwco/blob/master/image/%E5%8D%8F%E7%A8%8BServer%E6%B5%8B%E8%AF%95.png?raw=true)
