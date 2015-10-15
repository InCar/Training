#Hello World (原文地址：[hello-world](https://guides.github.com/activities/hello-world/))

在计算机编程世界中，**Hello World**是一个历史悠久的传统。我们所有的新知识的学习都是从Hello World开始，现在我们从Hello World开始学习GitHub 。

GitHub是一个用来托管项目、协同工作的平台；使用GitHub，你将再无需担心你的硬盘中的数据丢失，也无需担心在多台电脑间同步文件；更为重要的是，在软件开发中，GitHub可以帮助你更好的协同工作、异步处理流程。

本指南通过GitHub的五个要点：**Repositories**，**Branches**，**Commits**，**Issues**及**Pull Requests**，来指导你。

当你理解了GitHub的这五项要点，你将可以学得更多的东西，在GitHub上走的更远。 同时，在此`hello-world`知识库(如：[holman/feedback](https://github.com/holman/ama),将来的资料库：[jlord/hello-world](https://github.com/jlord/hello-world))，你可以存储此项目相关的想法、资源、笔记及讨论或者其他相关的事情。

**安装**

我们准备在此教程中使用GitHub.com完成这些任务，所以你无需使用任何命令行，也无需安装Git（GitHub是基于Git的）。你甚至无需懂得任何编码的知识。你仅仅需要一个GitHub账号，所以如果你没有GitHub账号，请申请一个。

>提示：请在一个单独的浏览器窗口（或者页签）中打开此教程，这样可以方便你在完成此教程的过程中查阅它。（这提示也忒弱智了）

##创建一个版本库

**版本库**是通常情况下GitHub中一个项目的基本单位。版本库中可以包含文件夹、文件、图片以及你的项目所需的任何东西。我们推荐在每一个版本库中包含一个**README**文件，或者一个描述此项目的文件。GitHub能够让你在创建新的版本库的时候很容易的增加一个这种文件。*GitHub还提供了其它常见的选项，比如许可文件，并且你也可以跳过这些*

**去新建一个版本库**

1.点击左上角你的用户名后的 **+** 按钮

2.将你的版本库命名为`hello-world`

3.编写一段简单的描述

4.勾选**Initialize this repository with a README**

![](https://guides.github.com/activities/hello-world/create-new-repo.png)

点击**Create repository**按钮。 Duang , 版本库建好了! :boom:

##建一个ISSUE



**ISSUE**是一个版本库中关于某个需要注意的东西的说明。它可能是一个错误，一个功能请求，一个问题或其它的一些东西。你可以在GitHub上给这些问题打上标签、搜索和分配问题，使管理活动项目更容易。



**新建一个ISSUE完成README文件的编写**



1.点击标签栏的**ISSUE**页签



2.点击**New Issue**按钮



3.为你的Issue写上标题和描述



![](https://guides.github.com/activities/hello-world/issue.png)



完成了之后点击**Submit new Issue**按钮。那么，这个Issue就有了一个可以供你引用的永久URL，即使这个Issue被关闭了。



##创建一个分支



**分支**用来在同一时间段内在版本库的不同部分工作。



当你创建了一个版本库，它默认有一个名为`master`的分支。你可以一直在这个唯一的分支上工作，这很好。如果你想处理另外一个功能或者想法，你可以从`maste`分支创建另一个分支，这样的话你就能从`master`分支的工作中释放出来。



当你创建一个分支，你就会在这个时间点上复制原始分支（就像图片快照）。当你在新分支上工作时，原始的分支发生了变化，不用担心，你可以拉取这些更新。



![](https://guides.github.com/activities/hello-world/branching.png)



你可能曾经通过将文件命名为类似于“story.txt”，“story-joe-edit.txt”，“story-sue-edit.txt”的方式保存文件的不同版本。GitHub版本库则通过更易于管理的方式来实现相同的目的。



在GitHub上，开发者、程序员和设计人员使用分支使bug修复和新功能的开发从`master`主分支上分离。当新功能和bug修复完成后，这些分支会被合并到`maste`主分支。



**现在创建一个新的分支**



1.打开你的`hello-world`版本库



2.点击文件列表上方叫做 **branch: master** 的下拉列表框



3.将分支名称`readme-edits`敲入新分支文本框



4.选择蓝色的**Create branch**选项，或者敲击键盘上的**Enter**键



![](https://guides.github.com/activities/hello-world/readme-edits.gif)

现在你有了两个分支：`master`和` readme-edits`。他们看起来几乎是一样的，但是不会太久。下一步我们将会将我们的修改增加到新分支中。



##提交


