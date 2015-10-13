# GitHub训练营•配置Git


GitHub的核心是一个称为Git的开源版本控制系统（VCS），它负责所有本地计算机上发生的有关GitHub的事情。

如果你目前还不习惯使用命令行，GitHub让你在不使用命令行的情况下完成许多Git相关动作，包含：

* 创建一个代码库

* 分支代码库

* 社交群组化

不管怎样，如果你需要使用Git，我们就能帮助你创建！

小贴士：

* GitHub有一个桌面客户端！你能完全不碰命令行而来使用它。

* 了解更多Git，请参阅git-scm 网站上的“开始-Git基础”。

##输入：

出现在本页上的这些代码块是一种叫做Bash的脚本语言的某部分，
想要使用Bash脚本，我们需要用到与Git一同安装的名为Git Bash的应用程序，Git Bash可在开始目录“Git”下找到。

![Image of BootGitBash](https://github.s3.amazonaws.com/docs/bootcamp_1_win_gitbash.jpg)

以“$”开头的行表明你需要键入一行Bash脚本。在“$”后键入文本，并在每行末尾敲击回车键。将鼠标悬停在每行上时可以了解到脚本正在做什么。
![Image of Input](http://img4.douban.com/view/photo/photo/4BcbnsZjO44WMYQf0Tjlfw/135989898/x2274459548.jpg)

##输出：

未以“$”开头的行是输出行，是用来告知信息或让你知道下一步将做什么的，我们0已经在训练营教程中将输出文本标为了绿色。
![image of Output](http://img4.douban.com/view/photo/photo/4BcbnsZjO44WMYQf0Tjlfw/135989898/x2274459548.jpg)

##用户特定输入：
![image of UserInput](http://img4.douban.com/view/photo/photo/LKKV7A66nvlsrb7iGIENlQ/135989898/x2274462388.jpg)

黄色文本区表明你自己的个人信息，代码库等。如果它位于输入行（“$”行后）中，则需要你用自己的信息将此处替换掉。如果它出现在输出行，那么只是用做参考，在Git Bash中，个人信息将自动显示出来。

######需注意：在Git Bash中执行一些动作是不会有输出的，可能会出现，当你敲完代码，回车，显示另外提示的情况。没关系，这很正常，如果代码有问题，我们会及时提示的。
当然，为安全起见，我们不会显示你输入的密码，请放心输入。

##配置Git：

1. 下载并安装最新版本的GitHub桌面客户端。默认会自动安装并更新Git。

2. 打开Git Shell应用程序。

3. 输入你的姓名以便你的提交被正确标注。注意以$开头。
![image of Username](http://img3.douban.com/view/photo/photo/public/p2274462385.jpg)
 
4. 输入你提交代码时需要关联的邮箱地址。该邮箱需与邮箱设置中的地址保持一致。如需隐藏邮箱地址，请参阅“如何设置邮箱地址为私密”。
![image of UserEmail](http://img4.douban.com/view/photo/photo/public/p2274462386.jpg)

##下一步：从Git使GitHub生效

当你从Git系统连接GitHub代码库，需要通过HTTPS或SSH来使GitHub生效。

* 通过HTTPS连接（推荐）

如果通过HTTPS克隆库，可配合凭证助手来缓存GitHub密码。

* 通过SSH连接

如果通过SSH克隆库，必须在每台计算机安装SSH公钥，以便从GitHub 进行push或pull操作。

#####恭喜，你已经对Git基本入门了，接下来我们来学习如何创建一个代码库。
