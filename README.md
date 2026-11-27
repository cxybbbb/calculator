E:\Project C++\qt\calculator\
├── calculator.pro           # TEMPLATE=subdirs，唯一入口
├── interface\
│   └── IOperationPlugin.h   # 主程序与插件共享的唯一契约（header-only）
└── src\
    ├── core\                # 静态库，只做两件事：加载插件、按 id 分发
    │   ├── OperationRegistry.h/.cpp
    ├── plugins\             # 一个运算一个子工程
    │   ├── add\      add_operation.dll
    │   ├── subtract\ subtract_operation.dll
    │   ├── multiply\ multiply_operation.dll
    │   └── divide\   divide_operation.dll
    └── app\                 # 可执行程序（你的界面已迁到这里）
        ├── main.cpp
        ├── mainwindow.h/.cpp
        └── mainwindow.ui