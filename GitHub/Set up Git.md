# GitHub训练营•配置 *Git*（原文请参考 [Set Up Git](https://help.github.com/articles/set-up-git))


GitHub的核心是一个称为Git的开源版本控制系统（VCS），它负责所有本地计算机上发生的有关GitHub的事情。

如果你目前还不习惯使用命令行，GitHub让你在*不* 使用命令行的情况下完成许多Git相关动作，包含：

* [创建一个代码库](https://help.github.com/articles/create-a-repo)

* [分支代码库](https://help.github.com/articles/fork-a-repo)

* [社交群组化](https://help.github.com/articles/be-social)

不管怎样，如果你需要使用Git，我们就能帮助你创建！

小贴士：

* GitHub有一个[桌面客户端](https://desktop.github.com)！你能完全不碰命令行而来使用它。

* 了解更多Git，请参阅[git-scm](http://git-scm.com/)网站上的“[开始-Git基础](http://git-scm.com/book/en/Getting-Started-Git-Basics)”。

##输入：

出现在本页上的这些代码块是一种叫做Bash的脚本语言的某部分，
想要使用Bash脚本，我们需要用到与Git一同安装的名为Git Bash的应用程序，Git Bash可在系统开始目录“Git”下找到。

![Image of BootGitBash](https://github.s3.amazonaws.com/docs/bootcamp_1_win_gitbash.jpg)

以“$”开头的行表明你需要键入一行Bash脚本。在“$”后键入文本，并在每行末尾敲击回车键。将鼠标悬停在每行上时可以了解到脚本正在做什么。

![Image of Input](http://img4.douban.com/view/photo/photo/4BcbnsZjO44WMYQf0Tjlfw/135989898/x2274459548.jpg)

##输出：

未以“$”开头的行是输出行，是用来告知信息或让你知道下一步将做什么的，我们已经在训练营教程中将输出文本标为了绿色。

![image of Output](http://img4.douban.com/view/photo/photo/4BcbnsZjO44WMYQf0Tjlfw/135989898/x2274459548.jpg)

##用户特定输入：

![image of UserInput](http://img4.douban.com/view/photo/photo/LKKV7A66nvlsrb7iGIENlQ/135989898/x2274462388.jpg)

黄色文本区表明你自己的个人信息，代码库等。如果它位于输入行（“$”行后）中，则需要你用自己的信息将此处替换掉。如果它出现在输出行，那么只是用做参考，在Git Bash中，个人信息将自动显示出来。

######需注意：在Git Bash中执行一些动作是不会有输出的，可能会出现，当你敲完代码，回车，显示另外提示的情况。没关系，这很正常，如果代码有问题，我们会及时提示的。
当然，为安全起见，我们不会显示你输入的密码，请放心输入。

##配置Git：

1. 下载并安装最新版本的[GitHub桌面客户端](https://desktop.github.com)。默认会自动安装并更新Git。

2. 打开**Git Shell**应用程序。

3. 输入你的*姓名*以便你的提交被正确标注。注意以`$`开头。

 ![image of Username](http://img3.douban.com/view/photo/photo/public/p2274462385.jpg)
 
4. 输入你提交代码时需要关联的*邮箱地址*。该邮箱需与[邮箱设置](https://help.github.com/articles/adding-an-email-address-to-your-github-account)中的地址保持一致。如需隐藏邮箱地址，请参阅“[如何设置邮箱地址为私密](https://help.github.com/articles/keeping-your-email-address-private)”。

![image of UserEmail](http://img4.douban.com/view/photo/photo/public/p2274462386.jpg)

##下一步：从Git使GitHub生效

当你从Git系统连接GitHub代码库，需要通过HTTPS或SSH来使GitHub生效。

* 通过HTTPS连接（推荐）

如果通过[HTTPS克隆](https://help.github.com/articles/which-remote-url-should-i-use#cloning-with-https-recommended)库，可配合凭证助手来[缓存GitHub密码](https://help.github.com/articles/caching-your-github-password-in-git)。

* 通过SSH连接

如果通过[SSH克隆库](https://help.github.com/articles/which-remote-url-should-i-use#cloning-with-ssh)，必须在每台计算机安装[SSH公钥](https://help.github.com/articles/generating-ssh-keys)，以便从GitHub 进行push或pull操作。

#####恭喜，你已经对Git基本入门了，接下来我们来学习[如何创建一个代码库](https://github.com/HP-Enterprise/Training/blob/master/GitHub/Creat%20a%20Repo.md)
* **配置Git**
* [创建代码仓库](https://github.com/HP-Enterprise/Training/blob/master/GitHub/Creat%20a%20Repo.md)
* [Fork A Repository](https://help.github.com/articles/fork-a-repo)
* [Be Social](https://help.github.com/articles/be-social)
