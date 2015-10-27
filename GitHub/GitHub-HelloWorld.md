#Hello World (原文地址：[hello-world](https://guides.github.com/activities/hello-world/))

在计算机编程世界中，**Hello World**是一个历史悠久的传统。我们所有的新知识的学习都是从Hello World开始，现在我们从Hello World开始学习GitHub 。

GitHub是一个用来托管项目、协同工作的平台；使用GitHub，你将再无需担心你的硬盘中的数据丢失，也无需担心在多台电脑间同步文件；更为重要的是，在软件开发中，GitHub可以帮助你更好的协同工作、异步处理流程。

本指南通过GitHub的五个要点：**Repositories**，**Branches**，**Commits**，**Issues**及**Pull Requests**，来指导你。

当你理解了GitHub的这五项要点，你将可以学得更多的东西，走的更远。 而且，在此`hello-world`仓库中，你可以存储想法(或未来创建的仓库,如：[jlord/hello-world](https://github.com/jlord/hello-world))、资源或一般性的笔记甚至和其它人讨论事情(如：[holman/feedback](https://github.com/holman/ama))。

**安装&零代码编写**

我们准备在此教程中使用GitHub.com完成这些任务，所以你无需使用任何命令行，也无需安装Git（GitHub是基于Git的）。你甚至无需懂得任何编码的知识。你仅仅需要一个GitHub账号，所以如果你没有GitHub账号，请申请一个。

>提示：请在一个单独的浏览器窗口（或者页签）中打开此教程，这样可以方便你在完成此教程的过程中查阅它。

##创建一个版本库（Repository）

**版本库**是通常情况下GitHub中一个项目的基本单位。版本库中可以包含文件夹、文件、图片以及你的项目所需的任何东西。我们推荐在每一个版本库中包含一个**README**文件，或者一个描述此项目的文件。GitHub能够让你在创建新的版本库的时候很容易的增加一个这种文件。*GitHub还提供了其它常见的选项，比如许可文件，但现在我们可以忽略这个*。

**去新建一个版本库**

1.点击左上角你的用户名后的 **+** 按钮

2.将你的版本库命名为`hello-world`

3.编写一段简单的描述

4.勾选**Initialize this repository with a README**

![](https://guides.github.com/activities/hello-world/create-new-repo.png)

点击**Create repository**按钮。 Duang , 版本库建好了! :boom:

##创建一个问题（ISSUE）



**ISSUE**是一个版本库中关于某个需要注意的东西的说明。它可能是一个错误，一个功能请求，一个问题或其它的一些东西。你可以在GitHub上给这些问题打上标签、搜索和分配问题，使管理活动项目更容易。



**新建一个ISSUE完成README文件的编写**



1.点击标签栏的**ISSUE**页签



2.点击**New Issue**按钮



3.为你的Issue写上标题和描述



![](https://guides.github.com/activities/hello-world/issue.png)



完成了之后点击**Submit new Issue**按钮。那么，这个Issue就有了一个可以供你引用的永久URL，即使这个Issue被关闭了。

接下来，编辑你的README然后关闭这个Issue。


##创建一个分支

（Branch）

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



##提交（Commit）

在GitHub上，保存修改被称作**提交**。提交是非常光荣的，一堆提交在一起读起来就像你项目的历史。

每一个提交都有一个相关的**提交说明**，这是一个描述，解释为什么会有这些特定的变化。感谢这些说明吧，你和其他人可以通过阅读这些提交明白你做了什么、为什么要这么做。

你现在应该依然在``readme-edits``分支的代码编辑页面，现在做一些改变。

**提交修改**

1.点击README文件

2.点击页面右上角的铅笔图标开始编辑

3.在编辑器中写一些文字介绍你自己

4.写一点提交说明描述你的修改

![](https://guides.github.com/activities/hello-world/commit.png)

点击 **Commit changes** 按钮。现在这些修改已经在你的``readme-edits``分支的README文件中了，而且现在这个分支有了相对于``master``分支不同的内容和提交（应该是这样！）。

##创建一个拉取请求（Pull Request）

拉取请求是GitHub协同工作的核心。当你创建一个**拉取请求**，表示你提供你的修改并请求某人去拉取你的贡献—也就是将这些修改合并到他们的分支。GitHub的消息推送功能允许你比较两个分支上的内容。这些增加、删除、修改的内容会以绿色、红色以及所谓的**差异**的形式显示。

一旦你做了修改，你就可以创建一个拉取请求.人们使用拉取请求来开始一个关于提交（代码审查）的讨论，即使是在代码完成之前。这样的话，当你被卡住时可以得到反馈和帮助。

通过在你的拉取请求信息中使用GitHub的[@mention system](https://github.com/blog/821-mention-somebody-they-re-notified),你可以向特定的团队或个人请求反馈，无论他们正坐在这栋大楼或者是10个时区以外。

你甚至可以像我们这里做的一样，在自己的版本库中开一个拉取请求，然后自己合并它。

**为README文件的改变开一个拉取请求**

*点击图片查看大图*

Step | Screenshot
------------ | -------------
点击侧边栏的拉取请求图标, 然后点击拉取请求页面的绿色**New pull request**按钮|![](https://cloud.githubusercontent.com/assets/1305617/2543808/73059f0a-b5f7-11e3-928e-745e9f9a5c75.gif)
选择您所做的分支``readme-edits``与原始分支``master``作比较|![](https://guides.github.com/activities/hello-world/pick-branch.png)
在差异比较页面浏览你的修改，确保这是你想要的提交|![](https://guides.github.com/activities/hello-world/diff.png)
当你确信这些是你所想要提交的，点击绿色大按钮**Create Pull Request**|![](https://guides.github.com/activities/hello-world/create-pr.png)
给你的拉取请求写一个标题，如果它直接关联到一个开放的问题，标题中包括“fixes #”标记和问题编号。写一个简短的说明描述你的修改|![](https://guides.github.com/activities/hello-world/pr-form.png)

当你写好了这些信息，点击**Create pull request** ！

>提示：你可以使用[表情符号](https://github.com/blog/1289-emoji-autocomplete)、[拖放图片和GIF图像](https://github.com/blog/1347-issue-attachments)到你的评论和拉取请求中。

##合并你的拉取请求

是时候把你的修改和到一起了 — 将你的``readme-edits``分支合并到``master``分支。

1.点击绿色按钮将修改合并到``master``分支

2.点击**Confirm merge**

3.当这些修改被包含到主分支后，继续使用紫色框中的** Delete branch**按钮删除子分支。

![](https://guides.github.com/activities/hello-world/merge-button.png)

![](https://guides.github.com/activities/hello-world/delete-button.png)

如果你重新审视你开的这个现在已经关闭的问题。由于你在拉取请求的标题中包含“fixes #1”标记,当这个拉取请求被合并后，GitHub将处理关闭这个问题。

##庆祝一下！

你已经学会了再GitHub上创建一个拉取请求！ :tada: :octocat: :zap:

你可以谦虚的吹牛了，这些是你在本指南中学到的东西：创建一个版本库、 分支, 问题和拉取请求, 然后合并拉取请求！炫耀一下你的[contribution squares](https://help.github.com/articles/viewing-contributions) 。

如果你想学习更多关于拉取请求的强大功能，我们推荐你阅读[GitHub Flow Guide](http://guides.github.com/overviews/flow/)。你也可以访问[GitHub Explore](http://github.com/explore)来参与一个开源项目。

>提示：查阅我们的其它[指南](http://guides.github.com/)和[YouTube Channel](http://youtube.com/githubguides)了解更多的GitHub使用指引。

