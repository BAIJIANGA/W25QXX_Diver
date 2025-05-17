# W25QXX_Driver_v1.0
W25QXX驱动以及一些demo

W25QXX_Driver文件夹为 W25QXX的驱动源文件 把头文件改了 直接加进去编译就行 如果库有问题或者有什么意见请在B站及时反馈

1_demo_W25QXX读写测试_文件系统 使用SPI2 和 USART2 引脚定义以及修改请在工程中自行查看

2_demo_W25QXX串口烧写数据 使用SPI2 和 USART2 引脚定义以及修改请在工程中自行查看 默认全片擦除 不推荐使用这种方式烧录数据

3_demo_野火标准库_文件系统 使用SPI1 和 USART1 引脚定义以及修改请在工程中自行查看

4_demo_标准库_SPI+DMA_文件系统 使用SPI1 和 USART1 引脚定义以及修改请在工程中自行查看 注意视频中提到的波特率分频的问题

5_demo_标准库_硬件SPI_烧录算法文件生成 使用SPI1 引脚定义以及修改请在工程中自行查看 注意视频中提到的全部变量的问题 

6_demo_标准库_硬件SPI_DMA_烧录算法文件生成 使用SPI1 引脚定义以及修改请在工程中自行查看 注意视频中提到的全部变量的问题

Keil中 stldr生成指令 更改路径到 复制到KEIL USER中即可
cmd.exe /C copy "!L" "C:\Program Files (x86)\STMicroelectronics\@L.stldr"

有些工程是MDK4生成的请自行百度解决
stldr文件生成不了Keil 请使用管理员权限打开

算法生成的教程链接
https://blog.csdn.net/u010351030/article/details/106384068
