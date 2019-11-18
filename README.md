# 18xiangyuanbei
**18年翔远杯差速PID程序**
*（使用位置型PID计算）*  
硬件为 370电机差速底盘 主控板：Arduino Mega2560 电机驱动：TB6612   
实物：  
![循迹小车1](https://github.com/1812598631/18xiangyuanbei/blob/master/PNG/%E5%9B%BE1.jpg)   
![循迹小车2](https://github.com/1812598631/18xiangyuanbei/blob/master/PNG/%E5%9B%BE2.jpg)   
---
图中全部为差速车。
舵机车将PID输出值从左右电机速度差值  更改为舵机转角即可    
翔远杯程序可以用 if-else 将每个遇到黑线的情况列举出来 来对应不同的处理    
例如：https://www.geek-workshop.com/forum.php?mod=viewthread&tid=29080&highlight=PID 这里面用switch case语句来对应不同情况进行处理，适当参考。
但是实际循迹效果不好，而PID算法非常适合循迹使用。   

不过，至少需要六个循迹模块才能体现出PID算法的优势。

>**资料整理**：
>><https://www.cnblogs.com/20135319zl/p/4941049.html> Arduino的基础知识    
>><http://mc.dfrobot.com.cn/thread-14783-1-1.html> PID原理解释 （推荐先看这一篇把算法主要思想看懂）   
>><https://github.com/br3ttb/Arduino-PID-Library/> PID库GitHub下载地址   
>><https://blog.csdn.net/ie_Jeton/article/details/76913229> Arduino官方PID库   
>><https://www.arduino.cn/thread-15176-1-2.html> PID库的调用    
>><https://www.geek-workshop.com/thread-197-1-1.html> PID进阶   
