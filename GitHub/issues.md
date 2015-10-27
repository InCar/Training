# 精通Issues（原文：[Master Issues](https://guides.github.com/features/issues/)）
Issues是一种很强大的方法，能给你的项目记录跟踪任务，功能和漏洞。他们就好像是电子邮件——只不过他们可以共享，可以与您的团队其他成员进行讨论。大多数软件项目都会有这样那样的bug跟踪系统。GitHub的这个跟踪系统被称为Issues，并且他在每一个库里都有自己的部分。
![Image of Yaktocat](https://guides.github.com/features/issues/navigation-highlight.png)
例如，让我们看一看 Bootstrap的issues部分：
![Image of Yaktocat](https://guides.github.com/features/issues/listing-screen.png)
GitHub的issues记录跟踪特殊就特殊在我们专注于协作、参考和优秀的文本格式。一个GitHub上典型的issues看起来有点像这个：
![Image of Yaktocat](https://guides.github.com/features/issues/example-issue.png)
* 用一个标题和描述来描述这个issue是关于什么的
* 彩色的编码标签帮你分类和过滤你的issues（就像电子邮件里的标签一样）
* 一个里程碑的作用就是它能像issues的容器一样。这对于关联项目的具体特征或者项目阶段来说是非常重要的（例如，每周Sprint9/5-9/16或发布1.0）。
* 一个接收到任务的人负责在任何给定的时间里完成issues。
* Comments允许任何人访问并提供反馈

## **里程碑(Milestone,以下均称为里程碑)、标签（Labels，以下均称为标签）和委托人（Assignee，以下均称为委托人）**
一旦你聚集了很多issues，你可能很难找到哪些是需要特别注意的。里程碑、标签和委托人对于筛选分类issues来说都是很给力的。

你可以通过点击右边边上相应的齿轮来修改或者添加时间表、委托人和标签。
![Image of Yaktocat](https://guides.github.com/features/issues/labels.png)
如果你没有找到编辑的按钮，那是因为你没有权限去编辑那个issue。你可以让repository owner加你为合作者来获得权限。
### **里程碑**
![Image of Yaktocat](https://guides.github.com/features/issues/milestones.png)

里程碑是一组一组的issues，相当于一个项目阶段，一个功能或者一个周期。人们用许多不同的方法来使用他们进行软件开发。GitHub上的一些里程碑例子包括：
* **测试版发行**——BUG文件，在你发行项目测试版之前必须先解决的文件BUG。这是一个很棒的方法没来确保你不会漏掉任何东西。
* **十月冲刺**——issues文件，会在10月影响到你。一个很伟大的方式让你在有很多事情要做的时候集中注意力。
* **设计**——文件issues，有关重新设计你的issues，是一种在工作中收集思路意见的有效途径

### **标签**
标签是一种高效的用来组织不同类型的issues的方法。issues可以有很多标签，只要你需要的情况，而且你可以一次过滤一个或多个标签。
![Image of Yaktocat](https://guides.github.com/features/issues/labels-listing.png)
### **委托人**
每一个isssues都会有一个委托人——一个负责处理issues的人。委托人的选择跟里程碑是一样的，是通过issues顶部的那个灰色的一栏。
## **通知（Notifications，以下均称为通知），@提到（某某）（@Mentions，以下均称为@提到（某某）），与参考文献（References，以下均称为参考文献）**
通过使用@提到（某某）和内部issues的参考，你可以通知其他GitHub的用户和组，来相互交叉连接issues。它们提供了一个灵活的方式来找到合适的人参与，从而高效地处理issues，而且很容易学习使用。他们可以在GitHub上贯穿所有的文本域来开展工作——它们是我们文本格式化语法的一部分，叫做GitHub Flavored Markdown。
![Image of Yaktocat](https://guides.github.com/features/issues/markdown-example.png)

如果你想了解更多，可以看看 Mastering Markdown。
### **通知**
通知是GitHub上跟上你的issues的方式，你可以用他们来了解库里的新issues，或者只是取了解到他们何时需要你去推进某个issues。

有2种接收通知的方式：通过邮件和通过网络。你可以在你的设置里面配置如何接收通知。如果你打算接收大量的通知，我们倾向于建议你这样，你接收网络+电子邮件通知时设置为Participating（参与），而网络通知设置为Watching（观看）。
![Image of Yaktocat](https://guides.github.com/features/issues/notifications.png)

这样设置，当有人特别提到你时你会接收到通知，然后紧跟上你感兴趣的网络接口。

你可以通过那个通知屏幕来访问你的通知。这个屏幕可以很好地立即看到很多通知，并立即标记他们为已读或者屏蔽。试着用键盘快捷键来加快你的工作流程——点击页面上的“？”来查看哪些快捷键是可用的。
![Image of Yaktocat](https://guides.github.com/features/issues/notification.png)

Muted threads不会在显示未读，直到你再次特别@提到了（某某）。这使得你很少感兴趣的主题得到有效的抑制（可能是一个你不了解的子系统）。如果你将一个issues标记为已读，那么它将这一一直保持下去，直到有人再次提到这个主题。

GitHub还能够同步你电子邮件通知的读/未读的状态——如果你在你的邮箱里读了某个通知，它将在网络端呗标记为已读（如果喜欢的话，确保你的邮箱能显示图片）。
### **@提到（某某）**
@提到（某某）是在GitHub issues中提到某某的方式。在这个issue的描述或者任何评论中，把@另一个GitHub的用户名列入其中，就会给那个人发送一个通知。这很类似于推特上使用的@提到（某某）。

我们喜欢用“/cc”的语法（副本抄送的简称）来把某人列入issues中：
> 在Safari浏览器上,新的小插件表单似乎坏掉了.当我尝试创建这个小插件时,Safari崩溃了.在10.8中这个可以再现,但在10.9中则不能.难道是浏览器的BUG？

> /cc @kneath @jresig

如果你能知道明确的用户并列入其中，那么就棒极了，然而，大多数情况，在整个团队里，我们并不真正的了解谁能帮到我们

 @提到（某某）也可以用于你们GitHub里的各种组。如果你在@acmeinc团队下创建一个组叫做browser-bugs，你可以这样@提到（某某）来提及那个组：
> /cc @acmeinc/browser-bugs

这将会给browser-bugs组里的每一个成员发送通知。
### 引用
通常，issues都会依赖于其他的issues，或者至少有些联系，你想把他们关联起来。你可以在#号后接上issue编号来引用issue。
> 嗨， @kneath，我觉得问题是从#42开始的

当你这样做时，我们将在issue #42中创建一个事件，就像这样：
![Image of Yaktocat](https://guides.github.com/features/issues/reference.png)

如果issues在另外一个库里时呢？那么就在名字前加上库的名字，就像这样：“kneath/example-project#42”。

一个使用GitHub更有趣的方式是，在评论中直接提及某个issues。在评论信息里加入该issue号。
![Image of Yaktocat](https://guides.github.com/features/issues/commit.png)

当你把评论合并到主分支时，通过在评论的前缀加上“Fixes”， “Fixed”， “Fix”， “Closes”， “Closed”， 或者 “Close”，它也将自动关闭issue。

引用，能够实现所做的工作与要跟踪的BUG的紧密相连，并且还是一个很棒的方式，给你的项目的历史添加可见性。
## **搜索**
在每一页的顶部有一个搜索框，能让你在所有的issues中进行搜索。
![Image of Yaktocat](https://guides.github.com/features/issues/search.png)

你可以通过以下方式审视搜索结果：
* 键盘，例如，[所有提到了侧边栏的issues](https://github.com/twbs/bootstrap/issues?q=sidebar)
* 状态，例如，[所有提到了侧边栏的关闭的issues](https://github.com/twbs/bootstrap/issues?q=sidebar+is%3Aclosed)
* 委托人，例如，[所有提到了侧边栏的委托给了@mdo的issues](https://github.com/twbs/bootstrap/issues?q=sidebar+is%3Aclosed+assignee%3Amdo)

[我们的有关搜索issues的帮助文档](https://help.github.com/articles/using-search-to-filter-issues-and-pull-requests/)会给你展示其他的搜索方式：使用创建/修改日期，标签，作者，评论次数，代码库的所有者，等等。
## **概述&报告**
在issues的部分之外，还有其他两个页面，帮你概述你的所有代码库里的issues发生了什么。
### 指示板issue
如果你正在比较多的项目里寻找有关你的更多的issues，这个指示板issue将会是一个很棒的工具。这个指示板用起来跟issus部分非常相似，但它的狗获取不同的issues：
* 代码库里你拥有的和需要协助的所有的issues
* 委托给你的issues
* 你创建的issues

如果你使用的组织，每个都有自己的issues，指示板将会从组织中把这些issues分离出来。
### **脉动**
在每个库的底部，是一个叫做脉动的部分——脉动就相当于过去一周（或是一天，或者过去的三个月，等等）在代码库里发生的所有的事情。
![Image of Yaktocat](https://guides.github.com/features/issues/pulse.png)

它是一个有效的途径，然你在离开的时候紧跟代码库，并且不想查看库里提供的小通知。
## **issues的其他用途**
issues是用来跟踪所有事情的有效方式——GitHub是一个很棒的平台，能让你轻松分享并在issues上进行协作。这里有一些让我们爱不释手的东西：
* 你房屋装修的差错跟踪,比如类似这种事情大门装错了
* 你的开源项目的BUG跟踪系统
* 关于食谱的请求（或许你有一个不错的面筋披萨面团配方？）

## 结束
现在，祝贺你自己——阅读了相当大一段内容！issues管理器是任何开发人员在开发过程中的一个有力工具之一。我想，现在所有剩下的工作就是去真正修复BUG了。

