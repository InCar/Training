# 掌握Issues
###### Issues是一种很强大的方法，能给你的项目记录跟踪任务，功能和漏洞。他们就好像是电子邮件——只不过他们可以共享，可以与您的团队其他成员进行讨论。大多数软件项目都会有这样那样的bug跟踪系统。GitHub的这个跟踪系统被称为Issues，并且他在每一个库里都有自己的部分。
![Image of Yaktocat](https://guides.github.com/features/issues/navigation-highlight.png)
###### 例如，让我们看一看 Bootstrap的issues部分：
![Image of Yaktocat](https://guides.github.com/features/issues/listing-screen.png)
###### GitHub的issues记录跟踪特殊就特殊在我们专注于协作、参考和优秀的文本格式。一个GitHub上典型的issues看起来有点像这个：
![Image of Yaktocat](https://guides.github.com/features/issues/example-issue.png)
* 用一个标题和描述来描述issues的全部
* 彩色的编码标签帮你分类和过滤你的issues（就像电子邮件里的标签一样）
* 一个里程碑的作用就是它能像issues的容器一样。这对于关联项目的具体特征或者项目阶段来说是非常重要的（例如，每周Sprint9/5-9/16或运行1.0）。
* 一个接收到任务的人负责在任何给定的时间里完成issues。
* Comments允许任何人访问并提供反馈

## **里程碑(Milestone,以下均称为里程碑)、标签（Labels，以下均称为标签）和委托人（Assignee，以下均称为委托人）**
###### 一旦你聚集了很多issues，你可能很难找到哪些是需要特别注意的。里程碑、标签和委托人对于筛选分类issues来说都是很给力的。
###### 你可以通过点击右边边上相应的齿轮来修改或者添加时间表、委托人和标签。
![Image of Yaktocat](https://guides.github.com/features/issues/labels.png)
###### 如果你没有找到编辑的按钮，那是因为你没有权限去编辑那个issue。你可以让repository owner加你为合作者来获得权限。
### **里程碑**
![Image of Yaktocat](https://guides.github.com/features/issues/milestones.png)
###### 里程碑是一组一组的issues，相当于一个项目阶段，一个功能或者一个周期。人们用许多不同的方法来使用他们进行软件开发。GitHub上的一些里程碑例子包括：
* **测试版发行**——BUG文件，在你发行项目测试版之前必须先解决的文件BUG。这是一个很棒的方法没来确保你不会漏掉任何东西。
* **十月冲刺**——issues文件，会在10月影响到你。一个很伟大的方式让你在有很多事情要做的时候集中注意力。
* **设计**——文件issues，有关重新设计你的issues，是一种在工作中收集思路意见的有效途径

### **标签**
###### 标签是一种高效的用来组织不同类型的issues的方法。issues可以有很多标签，只要你需要的情况，而且你可以一次过滤一个或多个标签。
![Image of Yaktocat](https://guides.github.com/features/issues/labels-listing.png)
### **委托人**
###### 每一个isssues都会有一个委托人——一个负责处理issues的人。委托人的选择跟里程碑是一样的，是通过issues顶部的那个灰色的一栏。
## **通知（Notifications，以下均称为通知），@提到（某某）（@Mentions，以下均称为@提到（某某）），与参考文献（References，以下均称为参考文献）**
###### 通过使用@提到（某某）和内部issues的参考，你可以通知其他GitHub的用户和组，来相互交叉连接issues。它们提供了一个灵活的方式来找到合适的人参与，从而高效地处理issues，而且很容易学习使用。他们可以在GitHub上贯穿所有的文本域来开展工作——它们是我们文本格式化语法的一部分，叫做GitHub Flavored Markdown。
![Image of Yaktocat](https://guides.github.com/features/issues/markdown-example.png)
###### 如果你想了解更多，可以看看 Mastering Markdown。
### **通知**
###### 通知是GitHub上跟上你的issues的方式，你可以用他们来了解库里的新issues，或者只是取了解到他们何时需要你去推进某个issues。
###### 有2种接收通知的方式：通过邮件和通过网络。你可以在你的设置里面配置如何接收通知。如果你打算接收大量的通知，我们倾向于建议你这样，你接收网络+电子邮件通知时设置为Participating（参与），而网络通知设置为Watching（观看）。
![Image of Yaktocat](https://guides.github.com/features/issues/notifications.png)
###### 这样设置，当有人特别提到你时你会接收到通知，然后紧跟上你感兴趣的网络接口。
###### 你可以通过那个通知屏幕来访问你的通知。这个屏幕可以很好地立即看到很多通知，并立即标记他们为已读或者屏蔽。试着用键盘快捷键来加快你的工作流程——点击页面上的“？”来查看哪些快捷键是可用的。
![Image of Yaktocat](https://guides.github.com/features/issues/notification.png)
###### Muted threads不会在显示未读，直到你再次特别@提到了（某某）。这使得你很少感兴趣的主题得到有效的抑制（可能是一个你不了解的子系统）。如果你将一个issues标记为已读，那么它将这一一直保持下去，直到有人再次提到这个主题。
### **@提到（某某）**
###### @提到（某某）是在GitHub issues中提到某某的方式。在这个issue的描述或者任何评论中，把@另一个GitHub的用户名列入其中，就会给那个人发送一个通知。这很类似于推特上使用的@提到（某某）。
###### 我们喜欢用“/cc”的语法（副本抄送的简称）来把某人列入issues中：
> 它看起来就像Safari上破碎的插件形式。当我试着创建此控件，Safari就崩溃了。这是可再生的10.8，而并不是10.9.难道是浏览器的BUG？

> /cc @kneath @jresig
###### 如果你能知道明确的用户并列入其中，那么就棒极了，然而，大多数情况，在整个团队里，我们并不真正的了解谁能帮到我们
###### @提到（某某）也可以用于你们GitHub里的各种组。如果你在@acmeinc团队下创建一个组叫做browser-bugs，你可以这样@提到（某某）来提及那个组：
> /cc @acmeinc/browser-bugs

###### 这将会给browser-bugs组里的每一个成员发送通知。
### 参考文献
###### 通常，issues都会依赖于其他的issues，或者至少有些联系，你想把他们关联起来。你可以通过在标签中加入issue号来提及此issues。
> 嗨， @kneath，我觉得问题的开始在#42

###### 当你这样做时，我们将在issue #42中创建一个事件，就像这样：
![Image of Yaktocat](https://guides.github.com/features/issues/reference.png)
###### 如果issues在另外一个库里时呢？

