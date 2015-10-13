#第六章  创建脚本基础知识（原文请参考 [Chapter 6. Build Script Basics](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html)）

##6.1 工程和任务

Gradle中的所有内容都是基于**工程**和**任务**这两个基本概念之上的。

每一个Gradle的创建都是由一个或多个工程组成的。一个工程所代表的内容取决于你使用Gradle正在做的工作。比如：一个工程可能代表了一个Jar包或者是一个Web应用。它也可能是其他工程生成的一个由多个Jar包组成的ZIP发布文件。一个工程没有必要是一个已经被创建的东西。它可能是已经完成的，例如：把你的应用程序部署到一个站点或是生产环境。如果这看起来似乎有点模糊，别担心。Gradle的快捷创建支持(build-by-convention support)为工程的概念新增了一个更为具体的定义。

每一个工程是由一个或多个任务组成的。一个任务代表一个构建过程执行的某个不可分割的原子操作。这操作可能是编译某些类，创建一个Jar包，生成javadoc文件或者是向某个储存仓库发布某些文件。

现在，我们将要在一个工程的构建过程中定义一些简单的任务。接下来的章节将会着重介绍在工作中如何使用多工程以及多工程和多任务的结合。

##6.2 Hello World

你可以通过使用 `gradle` 命令来运行一个Gradle项目。`gradle` 命令将会在当前目录下寻找文件名为build.gradle的文件。我们通常将这个 `build.gradle` 文件称之为我们将要看到的构建脚本，尽管严格来说它应该是一个构建配置脚本.这个构建脚本定义了一个工程和这个工程所包含的任务。

赶紧来尝试下创建如下所示名为 `build.gradle` 的构建脚本吧。

**例 6.1. 你的第一个构建脚本**

**build.gradle**
```javascript
task hello {
    doLast {
        println 'Hello world!'
    }
}
```

在命令行模式下，进入到该脚本文件的目录，并通过使用命令 `gradle -q hello` 来执行：

**例 6.2. 执行一个构建脚本**

命令 **`gradle -q hello`** 的输出如下所示：
```javascript
> gradle -q hello
Hello world!
```

这究竟是如何运行的呢？这个构建脚本定义了一个叫做hello的单任务并且给它添加了一个动作。当你运行gradle hello命令时，Gradle就执行hello任务并且顺序执行其中包含的动作。而这个任务只是一个由一些可执行的groovy代码组成的终止命令。即打印出 `Hello world！` 。

如果你认为这看起来似乎和 `Ant's targets` 相似，那么恭喜你答对了。Gradle的任务等价于 `Ant's targets`，但是正如你将要看到的一样，`Gradle tasks` 功能更为强大。相比于 `Ant` 我们使用了一个不同的术语 `task` 而不是 `target`，因为我们认为 `task` 这个词相比于 `target` 会更加地令人记忆深刻。不幸的是这就引入了一个与 `Ant` 的术语冲突，因为 `Ant` 将它的命令 `commands` 比如 `javac` 或 `copy` 称为任务 `task` 。所以当谈到任务时，我们总是指的是与 `Ant's targets` 等价的 `Gradle tasks` 。如果我们要讨论 `Ant tasks(Ant commands)` ，我们会明确地说 `Ant task` 。

```
What does -q do?

Gradle 命令中 -q 意义为何？
在这个用户指导中的大部分例子中的运行都使用了-q这个命令行参数。这是为了屏蔽Gradle的日志信息，这样就可以只显示任务的输出内容。使得该用户指导中的例子输出显示更为简洁。当然你可以试试去掉-q运行上面的hello任务。
如果你不想用-q，那么就可以果断地不用。
```
想要了解更多关于影响Gradle输出的命令行参数可以查看 [Chapter 17, Logging](https://docs.gradle.org/current/userguide/logging.html) 。

##6.3 定义一个精简的任务

这有一个快速定义一个像我们上面的hello任务的方法，但它更为的简洁。如下所示：

**例 6.3. 定义简洁任务**

**build.gradle**
```
task hello << {
    println 'Hello world!'
}
```

又一次定义了一个只有一个可执行的单终止动作的 `hello ` 任务。我们将会在这个用户指导全程使用这种任务的定义方式。

##6.4 构建脚本的代码语言

Gradle构建脚本使你能够充分使用Groovy语言。作为一道开胃小菜，一起先来看看下面的例子：

**例 6.4. 在Gradle的任务中使用Groovy语言**

**build.gradle**
```
task upper << {
    String someString = 'mY_nAmE'
    println "Original: " + someString 
    println "Upper case: " + someString.toUpperCase()
}
```

命令 **`gradle -q upper`** 的输出如下所示：
```
> gradle -q upper
Original: mY_nAmE
Upper case: MY_NAME
```

**例 6.5. 在Gradle的任务中使用Groovy语言**

**build.gradle**
```
task count << {
    4.times { print "$it " }
}
```

命令 **`gradle -q count`** 的输出如下所示：
```
> gradle -q count
0 1 2 3 
```

##6.5 任务依赖

正如你可能已经猜想到的，你可以声明一个任务而它却可以依赖于其他的任务。

**例 6.6. 依赖任务的声明**

**build.gradle**
```
task hello << {
    println 'Hello world!'
}
task intro(dependsOn: hello) << {
    println "I'm Gradle"
}
```

命令 **`gradle -q intro`** 的输出如下所示：
```
> gradle -q intro
Hello world!
I'm Gradle
```

增加依赖时，相对应的任务没有必要是已经存在的，可以后面定义一个新的任务。

**例 6.7. 懒依赖 —— 依赖任务可以不必是已存在的**

**build.gradle**
```
task taskX(dependsOn: 'taskY') << {
    println 'taskX'
}
task taskY << {
    println 'taskY'
}
```

命令 **`gradle -q taskX`** 的输出如下所示：
```
> gradle -q taskX
taskY
taskX
```

任务taskY是taskX的依赖，但是taskX却是在taskY被定义之前声明的。这对于构建多工程是很重要的。想要了解更多关于任务依赖方面的知识可以查看 [Section 14.4, “Adding dependencies to a task”](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:adding_dependencies_to_tasks)。

**请注意**：当你要引用一个还未定义的任务时，绝不能使用快捷符号（详情请阅读 [Section 6.8, “Shortcut notations”](https://docs.gradle.org/current/userguide/tutorial_using_tasks.html#sec:shortcut_notations)）。

##6.6 动态任务

使用强大的 `Groovy` 语言不仅仅可以定义一个任务要完成的动作，还可做很多其他的事。比如，你可以使用它来动态创建任务。

**例 6.8. 任务的动态创建**

**build.gradle**
```
4.times { counter ->
    task "task$counter" << {
        println "I'm task number $counter"
    }
}
```

命令 **`gradle -q task1`** 的输出如下所示：
```
> gradle -q task1
I'm task number 1
```

##6.7 操作已定义的任务

任务一旦被创建就可以通过一个 `API` 来访问调用。比如，你可以使用它来动态给一个正在运行的任务添加依赖。然而Ant却不会允许像这样的事情发生。

**例 6.9. 通过API来访问调用一个任务 —— 添加依赖**

**build.gradle**
```
4.times { counter ->
    task "task$counter" << {
        println "I'm task number $counter"
    }
}
task0.dependsOn task2, task3
```

命令 **`gradle -q task0`** 的输出如下所示：
```
> gradle -q task0
I'm task number 2
I'm task number 3
I'm task number 0
```

或者你还可以给一个已定义的任务添加行为。

**例 6.10. 通过API来访问调用一个任务 —— 添加行为**

**build.gradle**
```
task hello << {
    println 'Hello Earth'
}
hello.doFirst {
    println 'Hello Venus'
}
hello.doLast {
    println 'Hello Mars'
}
hello << {
    println 'Hello Jupiter'
}
```

命令 **`gradle -q hello`** 的输出如下所示：
```
> gradle -q hello
Hello Venus
Hello Earth
Hello Mars
Hello Jupiter
```

这个 `doLast` 和 `doFirst` 可以被执行多次。它们向任务动作序列的开始或结尾添加一个动作。当这个任务被执行时，动作列表中的这些动作将会被顺序执行。这个 `<<` 符号只是 `doLast` 的一个别名。

##6.8 快捷符号

正如你可能在前面的例子中注意到的，有一个方便简洁的符号用来访问一个已存在的任务。每一个任务作为构建脚本的属性都是有效的。

**例 6.11. 访问调用作为构建脚本属性的任务**

**build.gradle**
```
task hello << {
    println 'Hello world!'
}
hello.doLast {
    println "Greetings from the $hello.name task."
}
```

命令 **`gradle -q hello`** 的输出如下所示：
```
> gradle -q hello
Hello world!
Greetings from the hello task.
```

这使得代码有很强的可读性，特别是当使用一些有插件提供的的任务时，比如编译任务。

##6.9 自定义任务属性

你可以给一个任务添加你自定义属性。添加一个名为myProperty的属性，使用 `ext.myProperty` 设置属性的初始值。从该定义行开始，此属性是可读的并且可以设置像一个预定义的任务属性。

**例 6.12. 添加任务自定义属性**

**build.gradle**
```
task myTask {
    ext.myProperty = "myValue"
}

task printTaskProperties << {
    println myTask.myProperty
}
```

命令 **`gradle -q printTaskProperties`** 的输出如下所示：
```
> gradle -q printTaskProperties
myValue
```

自定义属性对于任务是没有限制的。想要了解更多请阅读 [Section13.4.2,“Extra properties”](https://docs.gradle.org/current/userguide/writing_build_scripts.html#sec:extra_properties)。

##6.10 使用 Ant Tasks

在Gradle中 `Ant tasks` 是很优秀的，可以说是一流的技术。Gradle为 `Ant tasks` 提供了非常优秀的整合，而实现这样的整合只是简单的依赖了 `Groovy` 语言。非常优秀的 `AntBuilder` 为 `Groovy` 语言提供了很好的支持。从Gradle中使用 `Ant tasks` 相比于从一个 `build.xml` 文件中是一样的方便并且更加地功能强大。从下面例子中，你可以学会如何执行 `Ant tasks` 和 如何访问使用 `Ant properties`：

**例 6.13. 使用AntBuilder执行ant.loadfile文件**

**build.gradle**
```
task loadfile << {
    def files = file('../antLoadfileResources').listFiles().sort()
    files.each { File file ->
        if (file.isFile()) {
            ant.loadfile(srcFile: file, property: file.name)
            println " *** $file.name ***"
            println "${ant.properties[file.name]}"
        }
    }
}
```

命令 **`gradle -q loadfile`** 的输出如下所示：
```
> gradle -q loadfile
 *** agile.manifesto.txt ***
Individuals and interactions over processes and tools
Working software over comprehensive documentation
Customer collaboration  over contract negotiation
Responding to change over following a plan
 *** gradle.manifesto.txt ***
Make the impossible possible, make the possible easy and make the easy elegant.
(inspired by Moshe Feldenkrais)
```

当然在你的构建脚本中使用 `Ant` 你还可以做很多其它的工作，详细信息请阅读 [Chapter 16, Using Ant from Gradle](https://docs.gradle.org/current/userguide/ant.html) 。

##6.11 调用方法

Gradle在如何组织构建的逻辑方面有自己的衡量标准。上面的例子的构建逻辑的第一个水平就是抽取一个方法。

**例 6.14. 使用方法组织构建逻辑**

**build.gradle**
```
task checksum << {
    fileList('../antLoadfileResources').each {File file ->
        ant.checksum(file: file, property: "cs_$file.name")
        println "$file.name Checksum: ${ant.properties["cs_$file.name"]}"
    }
}

task loadfile << {
    fileList('../antLoadfileResources').each {File file ->
        ant.loadfile(srcFile: file, property: file.name)
        println "I'm fond of $file.name"
    }
}

File[] fileList(String dir) {
    file(dir).listFiles({file -> file.isFile() } as FileFilter).sort()
}
```

命令 **`gradle -q loadfile`** 的输出如下所示：
```
> gradle -q loadfile
I'm fond of agile.manifesto.txt
I'm fond of gradle.manifesto.txt
```

后面你将会看到在多工程的项目中像这样的方法是可以被子工程共享使用的。如果你的构建逻辑变得很复杂时，Gradle给你提供了其他更加方便的方法来组织它。为此我们一整章来详细阐述组织方法，详情请阅读 [Chapter 62, Organizing Build Logic](https://docs.gradle.org/current/userguide/organizing_build_logic.html) 。

##6.12 默认任务

Gradle允许你定义一个或多个默认任务，这些任务会在没有指定执行其他任务时执行。

**例 6.15. 定义一个默认任务**

**build.gradle**
```
defaultTasks 'clean', 'run'

task clean << {
    println 'Default Cleaning!'
}

task run << {
    println 'Default Running!'
}

task other << {
    println "I'm not a default task!"
}
```

命令 **`gradle -q`** 的输出如下所示：
```
> gradle -q
Default Cleaning!
Default Running!
```

上面这个命令相当于运行命令 **`gradle clean run`** 。在一个多工程的构建中，每一个子工程都可以定义它自己的默认任务。如果一个子工程没有指定执行的默认任务，那么将会使用它的父工程默认任务（如果父工程定义了默认任务）。

##6.13 DAG配置

正如我们后面会详细讨论的（详情请参考 [Chapter 58, The Build Lifecycle](https://docs.gradle.org/current/userguide/build_lifecycle.html)），Gradle有一个配置期和一个执行期。在Gradle配置期完成后，Gradle将会知道所有应该被执行的任务。Gradle为你提供了一个钩子函数来充分利用这些信息。这些信息使用的方案之一就是检查你指定将要运行的任务是否在这些将要被执行的任务当中。根据这些信息，你可以给一些变量赋予不同的值。

在下面这个例子当中，任务 `distribution` 和任务 `release` 的执行会导致版本参数具有不同的值。

**例 6.16. 根据选择任务的不同执行的结果亦不同**

**build.gradle**
```
task distribution << {
    println "We build the zip with version=$version"
}

task release(dependsOn: 'distribution') << {
    println 'We release now'
}

gradle.taskGraph.whenReady {taskGraph ->
    if (taskGraph.hasTask(release)) {
        version = '1.0'
    } else {
        version = '1.0-SNAPSHOT'
    }
}
```

命令 **`gradle -q distribution`** 的输出如下所示：
```
> gradle -q distribution
We build the zip with version=1.0-SNAPSHOT
```

命令 **`gradle -q release`** 的输出如下所示：
```
> gradle -q release
We build the zip with version=1.0
We release now
```

最重要的事情就是在任务release执行前任务release已被 `whenReady` 影响改变。即使当任务release不是主要任务，这也是有效的。

##6.14 接下来该学什么？

在本章中，我们简单的了解了一下gradle任务机制。但是这并不是任务这个概念的结束。如果你想要了解更多关于任务的知识，请参考 [Chapter 14, More about Tasks](https://docs.gradle.org/current/userguide/more_about_tasks.html) 。

还有，接下来你将要学到的是 [ Chapter 7, Java Quickstart](https://docs.gradle.org/current/userguide/tutorial_java_projects.html)，[Chapter 8, Dependency Management Basics](https://docs.gradle.org/current/userguide/artifact_dependencies_tutorial.html) 。