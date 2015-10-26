# 精通Markdown（原文请参考[Mastering Markdown](https://guides.github.com/features/mastering-markdown))

Markdown是一种在GitHub上可以用来设计各种形式写作的轻量级且易于使用的语法

##### 你将会学到： #####

* Markdown格式如何使得协作设计变得容易

* Markdown与传统格式化方法的区别

* 如何使用Markdown来格式化文本

* 如何利用GitHub的Markdown自动渲染

* 如何使用GitHub独有的Markdown扩展功能

## Markdown是什么？

[Markdown](http://daringfireball.net/projects/markdown)能在网络上设计文本。你可以分分钟用Markdow管理文档的显示，以粗体或者斜体排版，插图，创建列表~Markdown主要是些常规文本，只加入了非常少量的非字母字符，比如`#`或`*`。

你能在GitHub的大部分场景使用Markdown：

* [Gists](https://gist.github.com/)

* 在Issue中评释或是Pull Requests时

* `.md`或`.markdown`文件的延伸

## 举例 ##

### 文本 ###

```
It's very easy to make some words **bold** and other words *italic* with Markdown. You can even [link to Google!](http://google.com）
```

It's very easy to make some words **bold** and other words *italic* with Markdown. You can even [link to Google!](http://google.com)

### 列表 ###


```
有时你想要有序的列表：

1. 1

2. 2

3. 3

有时你会想要点状列表:

* 以星号符开头Start a line with a star

* 搞定！

又或者,
 
- 用破折号也可以
- 如果你要使用二级要点，在星号符或破折号前空两格：
  - 像这样
  - 还有这样
```

有时你想要有序的列表：

1. 1

2. 2

3. 3

有时你会想要点状列表:

* 以星号符开头Start a line with a star

* 搞定！

又或者,
 
- 用破折号也可以
- 如果你要使用二级要点，在星号符或破折号前空两格：
  - 像这样
  - 还有这样

### 图片 ###

```
想要插图，需要这样做：

![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)
```

想要插图，需要这样做：

![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)

### 标题和引用 ###

```
# 使文档结构化

有时通过设置不同级别标题有效的结构化你的文档。用一个`#`打头来创建标题。排列多个`##`则表示更小大小的标题。

### 这是一个三级标题

你能用1个`#`到6个`######`标出不同大小的标题。

如果要引用某人的话，可以在句首使用 > 符号

> 咖啡，被发明出来最好的有机悬浮液。。。我用它击败了博格。
> - 舰长詹妮弗
```

# 使文档结构化

有时通过设置不同级别标题有效的结构化你的文档。用一个`#`打头来创建标题。排列多个`##`则表示更小大小的标题。

### 这是一个三级标题

你能用1个`#`到6个`######`标出不同大小的标题。

如果要引用某人的话，可以在句首使用 > 符号

> 咖啡，被发明出来最好的有机悬浮液。。。我用它击败了博格。
> -舰长詹妮弗

### 代码 ###

```
GitHub上的Markdown提供多种设计代码的方式。如果是行内代码，将其放在反引号内：`var example = true`。更长的代码块，则可以用缩进四个空格的方式处理：
    if (isAwesome){
    return true
    }
    
GitHub也支持不需要缩进就能实现多行代码设计的代码围栏：                                                              ```if (isAwesome){
  return true
}                                                                                                                     ```
还可以加入语言来实现语法高亮：                                                                                        ```javascript
if (isAwesome){
  return true
}
```

GitHub上的Markdown提供多种设计代码的方式。如果是行内代码，将其放在反引号内：`var example = true`。更长的代码块，则可以用缩进四个空格的方式处理：
    if (isAwesome){
    return true
    }
    
GitHub也支持不需要缩进就能实现多行代码设计的代码围栏：

```
if (isAwesome){
  return true
}
```

还可以加入语言来实现语法高亮：

```javascript
if (isAwesome){
  return true
}
```

### 其他 ###

```
GitHub还支持许多其他Markdown中的功能，例如提及并链接别人，如果你想要直接点评某人，在@符后写上他们的名字：嗨 @Acebear - 再来点赏金任务呗~ 

O(∩_∩)O, 不过我得承认，任务列表是我的最爱~

- [x] 这是一个已完成项目
- [ ] 这是一个尚未完成的项目

噢噢，当然还有我们的emoji表情，也可以表示喔~~ :sparkles: :camel: :boom:
```

GitHub还支持许多其他Markdown中的功能，例如提及并链接别人，如果你想要直接点评某人，在@符后写上他们的名字：嗨 @Acebear - 再来点赏金任务呗~ 

O(∩_∩)O, 不过我得承认，任务清单是我的最爱~

- [x] 这是一个已完成项目
- [ ] 这是一个尚未完成的项目

噢噢，当然还有我们的emoji表情，也可以表示喔~~ :sparkles: :camel: :boom:

## 语法指导 ##

这是一份Markdown语法总览，你能在GitHub.com或是你自己的文本文件中任意使用~

### Headers ###

    # 这是一个<h1>标题
    ## 这是一个<h2>标题
    ###### 这是一个<h6>标题

### 强调 ###

    *这句话将是斜体*
    _这句话将是斜体_
    
    **这句话将被加粗**
    __这句话将被加粗__
    
    _你 **可以** 混用喔_

### 列表 ###

#### 无序列表 ####

    * 项1
    * 项2
      *项2a
      *项2b

#### 有序列表 ####

    1.项1
    2.项2
    3.项3
      项3a
      项3b

### 图片 ###

    ![GitHub Logo](/images/logo.png)
    Format: ![Alt Text](url)

### 链接 ###

    http://github.com - automatic!
    [GitHub](http://github.com)

### 块引用 ###

    就像坎耶·维斯特说的：
    
    > 我们正活在将来，所以
    > 当下其实是我们的过去。

### 行内代码 ###

    我认为你在这里应该使用一个
    `<addr>  元素。

## GitHub Flavored Markdown(GFM) ##

GitHub.com使用了其特有的Markdown语法版本，新加入一些有用的特性，让GitHub.com的内容更便于使用。

需要注意的是，有一些GFM特性只能自爱藐视或是评释Issues和Pull Requests时可以使用。其中包括@某人和引用SHA-1 hashes，issues和Pull Request的情况。任务列表也可在Gist评释和在Gist Markdown文件中有效。 

### 语法高亮 ###

这是一个[GFM](https://help.github.com/articles/github-flavored-markdown)中如何使用语法高亮的例子：

    ```javasctipt
    function fancyAlert(arg) {
      if(arg) {
        $.facebox({div:'#foo'})
      }
    }
    ```


或是使用4个空格的缩进来实现：

    function fancyAlert（arg） {
      if(arg) {
        $.facebox({div:'#foo'})
      }
    }

这是一个没有用语法高亮的Python代码例子：

    def foo():
      if not bar:
        return True

### 任务列表 ###

    - [x] 支持 @某人, #参考, [链接](), 以及<del>标签</del> 
    - [x] 需要列表语法的情况 (支持任何无序或者有序的列表)
    - [x] 这是这个已完成项
    - [ ] 这是一个尚未完成项

### 表格 ###

我们可以创建表格！！！(Amazing! but...略复杂)通过列表内文字并用连字符`-`划分这些文字来组合成第一行，再用管道符`|` 隔出每一列：


```
标题一 | 标题二
------- | ------
第一格内容 | 第二格内容
第一列内容 | 第二列内容
```

效果如下～

标题一 | 标题二
------- | ------
第一格内容 | 第二格内容
第一列内容 | 第二列内容

### 引用SHA ###

任何提交中提到的[SHA-1 hash内容](http://en.wikipedia.org/wiki/SHA-1)将被自动转化成链接，提交到GitHub上。


```
16c999e8c71134401a78d4d46435517b2271d6ac
mojombo@16c999e8c71134401a78d4d46435517b2271d6ac
mojombo/github-flavored-markdown@16c999e8c71134401a78d4d46435517b2271d6ac
```

### 同代码仓库中引用issue ###

任何提到issue和Pull request的数字，都会被自动转化成链接。


```
#1
mojombo#1
mojombo/github-flavored-markdown#1
```

### 用户名 @点名 ###

输入一个@符，紧跟一个用户名，将通知此人关注该评释，这就是一次@点名，因为你正被单独提到。你也可以用@点名同组织内各组人员。

### URL的自动链接 ###

任何URL（比如`http://www.github.com/`）将被自动转化成可点击的链接。

### 删除线 ###

任何用两条飘飘符括起来的文字（像 `~~这样~~`）将显示成带删除线效果。

### Emoji ###

GitHub支持emoji表情符啦:sparkles: :camel: :boom:

了解所有支持的Emoji表情，请参阅“[Emoji cheat sheet](http://www.emoji-cheat-sheet.com/)”
