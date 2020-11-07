# clipboard
`clipboard`是一个用于编辑剪切板的Windows控制台应用程序。
他还有对应的`libclipboard`

**欢迎** Issues 和 Pull Request!
## 快速开始
### clipboard
#### 前提条件
* C++编译器
* Windows操作系统
* git
#### 构建
1. git clone git@github.com:Haceau-Zoac/clipboard
2. 使用Visual Studio2019打开解决方案项目 或 运行build.bat

    * 若无msvc编译器，请自行修改build.bat
3. 按Ctrl+F5（Visual Studio），跳转到exe文件夹，打开控制台运行
#### 示例
    clipboard
可能输出：Hello, world!

    clipboard edit "qwq" & clipboard
输出：qwq
### libclipboard
#### 前提条件
* git
#### 构建
1. git clone git@github.com:Haceau-Zoac/clipboard
2. 将`clipboard.hpp`放到你的项目目录下
3. #include "clipboard"
#### 示例
    #include <iostream>
    #include <string>
    #include "clipboard.hpp"

    int main(void)
    {
        try
        {
            hac::clipboard clip;
            std::cout << "当前剪切板内容为" << clip.get() << '\n';
            std::cout << "请输入：";
            std::string str;
            std::getline(std::cin, str);
            clip.set(str);
            std::cout << "当前剪切板内容为" << clip.get();
        }
        catch (hac::clipboard_exception ex)
        {
            std::cerr << "Clipboard exception:" << ex.what();
        }

        return 0;
    }
