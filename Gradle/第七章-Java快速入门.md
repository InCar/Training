# 第七章 Java快速入门[Chapter 7. Java Quickstart](https://docs.gradle.org/current/userguide/tutorial_java_projects.html)
##7.1 Java插件

正如我们所知，Gradle是一个通用的构建工具。它几乎可以构建任何你想在构建脚本中实现的东西。然而，除非你在构建脚本中添加代码否则Gradle不会做任何事情。

就基础部分来说，大部分Java项目都十分相似：你需要编译你的Java源码文件，运行一些单元测试并且创建一个JAR文件来包含你的class文件。如果你不需要为每一个项目做这些事情，那将是一件美妙的事情。幸运的是，你真的不需要。Groovy通过使用插件来解决这个问题。插件是Gradle的扩展，它以某种方式配置你的项目，典型的做法是添加一些预先配置好用于做某些有用事情的task。Gradle和许多插件运行在一起，并且你可以很简单地编写你自己的插件并分享给别人。其中一种插件就是 **Java插件**。这个插件添加了许多task到你的项目中，这些task会编译和测试你的Java源码，并且把它打包进JAR文件。

Java插件是基于约定的。这意味着在项目的许多方面该插件都定了默认值，比如Java源码应该放在哪里。如果你的项目遵守约定，在你的构建脚本中你通常不需要做太多的事情就可以完成一个有用的构建。如果在某些地方你不想或无法遵循这个约定，Gradle允许你定制你的项目。事实上，因为对Java项目的支持是以一个插件的形式来实现的，如果你不想的话，你根本不需要在构建Java项目时使用这个插件。

在之后的章节中，我们有许多有关Java插件、依赖管理和多项目构建例子的深入探讨。在本章中我们想要给你一个有关如何使用Java插件构建Java项目的初步概念。

##7.2 一个基础的Java项目

让我们来看一个简单的例子。想要使用Java插件，添加以下内容到你的构建文件中：

**示例 7.1. 使用Java插件**

**build.gradle**

```groovy
    apply plugin: 'java'
```
注意：这个示例所使用的源码可以在Gradle安装目录下的samples/java/quickstart目录下找到。

这就是你定义一个Java项目所需要做的。这将会应用Java插件到你的项目中，它会添加一些task到你的项目中。

Gradle会期望在src/main/java目录下找到你的项目源码，在src/test/java目录下找到测试源码。另外，在src/main/resources目录下的任何文件都会被作为资源文件添加到JAR文件里，在src/test/resources目录下的任何文件都会被添加到classpath中用于运行测试。所有的输出文件都被创建在build目录里，其中JAR文件在build/libs目录里。

###7.2.1 构建项目

Java插件可以添加相当多的task到你们项目。然而，你构建项目的时候只需要少数的task。用的最普遍得task就是build task，它可以用来构建完整的项目。当你运行 **gradle build**，Greadle将会编译和测试你的代码，创建一个JAR文件包含你的主要class和资源文件：
 
**示例7.2.构建一个java项目**

**运行gradle build后输出**

```groovy
    > gradle build
    :compileJava
    :processResources
    :classes
    :jar
    :assemble
    :compileTestJava
    :processTestResources
    :testClasses
    :test
    :check
    :build
```
 ```groovy   
    BUILD SUCCESSFUL
```
```groovy   
    Total time: 1 secs
```

一些其它有用的task是：

**clean**
    
    删除build目录，移除所有的构建文件。
    
**assemble**
    
    编译和打包你的代码，但是不会运行单元测试。其它的插件会添加更多的artifacts到这个task里。例如，如果你使用War插件，这个task就会构建WAR文件到你的项目里。
    
**check**

    编译和测试你的代码。其它的插件会添加更多的check到这个task里。例如，如果你使用checkstyle插件，这个task还会对你的源代码运行Checkstyle。
    

###7.2.2 外部依赖

通常，一个Java项目将会依赖很多外部的JAR文件。涉及到的这个JAR文件，你需要告诉Gradle在哪里找到它们。在Gradle里面，artifact位于一个仓库中就像JAR文件，这个仓库可以用来提取项目需要的外部依赖文件，或者发布项目的artifact，或者两者一起。作为例子，我们将会使用公共的Maven库：

**示例7.3.添加Maven库**

**build.gradle**

```groovy
    repositories {
        mavenCentral()
    }
```

让我们来添加一些外部依赖。在这里，我们会声明我们的class会有一个来自commons collections的编译时依赖，并且我们的test class会有一个来自junit的编译时依赖：

**示例7.4.添加依赖**

**build.gradle**

```groovy
    dependencies {
        compile group: 'commons-collections', name: 'commons-collections', version: '3.2'
        testCompile group: 'junit', name: 'junit', version: '4.+'
    }
```

你可以找到更多资料在第八章，依赖管理基础。[Chapter 8, Dependency Management Basics](https://docs.gradle.org/current/userguide/artifact_dependencies_tutorial.html)

###7.2.3 定制项目

Java项目添加了很多属性到你的项目中去。这些属性的会有默认值通常就足够启动项目。如果这些默认值不适合的话也很容易去更改。让我们来看一下这个例子。在这里我们会声明我们的Java项目版本号，以及我们的源码是哪个版本的Java编写的。我们还添加了一些JAR manifest属性。

**示例7.5.定制MANIFEST.MF**

**build.gradle**

```groovy
    sourceCompatibility = 1.5
    version = '1.0'
    jar {
        manifest {
            attributes 'Implementation-Title': 'Gradle Quickstart',
                       'Implementation-Version': version
        }
    }
```

ava plugin添加的task是有规律的task，确切的就如它们好像是在构建文件中声明的一样。这意味着你可以使用任何在之前文章所示的结构来定义这些task。例如，你可以设置task的属性，添加task的行为，修改task的依赖，或者完全替换掉task。在我们的例子中，我们将会配置test task，它是Test的一种，通过这个方式在运行测试的时候添加一个系统属性：

**示例7.6.添加一个系统属性**

**build.gradle**

```groovy
    test {
        systemProperties 'property': 'value'
    }
```

###7.2.4 发布一个JAR文件

通常JAR文件需要发布在某一个地方。这样的话，你就需要告诉Gradle在哪里发布这些JAR文件。在Gradel里面，artifacts（比如JAR文件）被发布到仓库里。在下面的例子里，我们将会发布一个本地目录，你也可以发布到远程，或者多个地址。

**示例7.7.发布JAR文件**

**build.gradle**

```groovy
uploadArchives {
    repositories {
       flatDir {
           dirs 'repos'
       }
    }
}
```

运行 **gradle uploadArchives**命令来发布JAR文件。


###7.2.5 创建一个Eclipse项目

想要创建Eclipse的项目说明文件，比如.project，你需要添加其他插件到你的构建文件中：

**示例 7.8. Eclipse插件**

**build.gradle**

```groovy
    apply plugin: 'eclipse'
```

运行gradle eclipse命令来生成Eclipse项目文件。想要获取更多有关eclipse task的信息可以查阅[第38章，Eclipse插件](https://docs.gradle.org/current/userguide/eclipse_plugin.html)。

###7.2.6 总结

下面是我们的例子的完整构件文件：

**示例 7.9. Java例子 - 完整的构件文件**

**build.gradle**

```groovy
    apply plugin: 'java'
    apply plugin: 'eclipse'
    
    sourceCompatibility = 1.5
    version = '1.0'
    jar {
        manifest {
            attributes 'Implementation-Title': 'Gradle Quickstart',
                       'Implementation-Version': version
        }
    }
    
    repositories {
        mavenCentral()
    }
    
    dependencies {
        compile group: 'commons-collections', name: 'commons-collections', version: '3.2'
        testCompile group: 'junit', name: 'junit', version: '4.+'
    }
    
    test {
        systemProperties 'property': 'value'
    }
    
    uploadArchives {
        repositories {
           flatDir {
               dirs 'repos'
           }
        }
    }
```

##7.3 构建多项目

让我们来看一个典型的多项目构建。以下是项目的结构：

**示例 7.10. 多项目构建 - 分层布局**

Build layout（构建结构）

```groovy
    multiproject/
      api/
      services/webservice/
      shared/
      services/shared/
```

注意：在Gradle安装目录下的samples/java/multiproject目录里可以找到源码。

在例子中，我们有四个项目。项目api编译出一个JAR文件供webservice使用。项目webservice是一个返回XML文件的webapp。项目shared包含了api和webservice要用到的代码。项目services/shared/的代码依赖于shared。

###7.3.1 定义一个多项目构建

想要定义一个多项目构建，你需要创建一个设置文件。这个设置文件位于项目树的根目录，并且指定哪些项目被包含在构建中。该设置文件必须被命名为settings.gradle。在本例子中，我们使用了一个简单的分层布局。下面有与该层次相一致的设置文件

**示例 7.11. 多项目构建 - settings.gradle文件**

**settings.gradle**

```groovy
    include "shared", "api", "services:webservice", "services:shared"
```

你可以在[第59章， 多项目构建](https://docs.gradle.org/current/userguide/multi_project_builds.html)中找到更多有关设置文件的信息。

###7.3.2 通用配置

对于大多数多项目构建来说，里面有一些配置对于每一个项目都是通用的。在我们的例子里，我们将会使用一种被称作配置注射技术在根项目里定义通用配置。根项目（多项目构建的根目录）就像一个容器，而subprojects方法会遍历容器内的每一个元素（即子项目）然后将配置信息注入其中。通过这个方式，我们可以很容易地为每一个文件注入配置和一些通用依赖。

**示例 7.12. 多项目构建 - 通用配置**

**build.gradle**

```groovy
    subprojects {
        apply plugin: 'java'
        apply plugin: 'eclipse-wtp'
    
        repositories {
           mavenCentral()
        }
    
        dependencies {
            testCompile 'junit:junit:4.12'
        }
    
        version = '1.0'
    
        jar {
            manifest.attributes provider: 'gradle'
        }
    }
```

需要注意的是我们的例子里将Java插件应用于每一个子项目。这意味着所有我们在上面看到的task和配置属性在每一个子项目里都是可用的。所以你可以在根目录运行gradle build命令来编译、测试和打包所有的子项目。

你还要注意的是这些插件仅仅应用于子项目内部，不包括根目录，所以你不应该该根目录放置任何的Java源文件，只能在子项目里放源文件。

###7.3.3 项目之间的依赖关系

你可以在同一个构建之中添加子项目之间的依赖，例如一个项目生成的JAR文件被用来编译另一个项目。在api项目的构件文件里我们将会添加一个shared项目的依赖。由于这个依赖的声明，Gradle将会知道在构建api项目之前必须先构建shared项目。

**示例 7.13. 多项目构建 - 项目间的依赖**

**api/build.gradle**

```groovy
    dependencies {
        compile project(':shared')
    }
```

查阅[第59章 7.1节，使项目依赖无效](https://docs.gradle.org/current/userguide/multi_project_builds.html#disable_dependency_projects)来了解如何让该功能无效。

###7.3.4 创建发行包

我们也需要添加一个发行包发送到客户端：

**示例7.14.多项目构建发行包文件**

**api/build.gradle**

```groovy
    task dist(type: Zip) {
        dependsOn spiJar
        from 'src/dist'
        into('libs') {
            from spiJar.archivePath
            from configurations.runtime
        }
    }
    artifacts {
       archives dist
    }
```

##7.4 下一步去哪？

在本章，你已经知道了构建一个基础的Java基本项目普遍要做的事情，这个章节还不够详尽，针对Java项目在Gradle里面还可以做很多事情的。你可以找到更多关于Java插件的知识在第二十三章 Java插件[Chapter 23, The Java Plugin](https://docs.gradle.org/current/userguide/java_plugin.html),而且你可以在Gradle安装目录 *samples/java*下找到更多java项目的例子。

另外，让我们继续学习[第八章，依赖管理基础](https://docs.gradle.org/current/userguide/artifact_dependencies_tutorial.html)。