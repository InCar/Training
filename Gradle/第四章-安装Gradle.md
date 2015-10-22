# 第四章 Gradle安装[Chapter 4. Installing Gradle](https://docs.gradle.org/current/userguide/installation.html)
## 4.1 先决条件

`Gradle`需要安装`JDK`或者`JRE`，版本不低于1.6（使用`java -version`查看）。`Gradle`搭载自己的`Groovy`库,因此不需要安装，已经存在的`Groovy`的安装会被`Gradle`忽视。

`Gradle`会使用你本来的路径下的`JDK` 。或者，你可以把`JAVA_HOME`环境变量指向你所需的`JDK`安装目录。

## 4.2 下载

我们可以从[Gradle web site](http://gradle.org/gradle-download/)下载`Gradle`发行包

## 4.3 解压

`Gradle`安装文件是一个ZIP压缩文件。完整的文件包含：

* 执行程序
* 用户手册（包括`HTML`和`PDF`两种版本）
* `DSL`参考指南
* API手册（包括`Javadoc`和`Groovydoc`）
* 大量的样例。包含用户指南中提到的样例，有一些已经完成和更复杂的构建可以作为我们建立一个自己`Gradle`构建的起点
* 执行程序源代码。只是用来参考的，如果你想构建`Gradle`的话需要下载源代码，或者从源代码库迁出分支，详情请见[Gradle web site](http://gradle.org/gradle-download/)

## 4.4 环境变量

为了运行`Gradle`,需要添加 *GRADLE_HOME*/bin到PATH的环境变量里去。通常，这样就足够来运行`Gradle`了。

## 4.5 运行并测试是否工作正常

配置完成之后，运行`gradle -v`，检查一下是否安装无误。如果安装正确，它会打印出`Gradle`的版本信息，和一些本地环境配置（`Groovy`, `JVM version`, `OS`等...)。这里的`Gradle`版本信息将会匹配你下载的版本。

## 4.6 JVM（Java虚拟机）参数

运行`Gradle`需要的`JVM`参数可以通过环境变量设置。我们可以使用`GRADLE_OPTS`或`JAVA_OPTS`来配置，或者两个一起。`JAVA_OPTS`设置的参数也会影响到其他的`JAVA`应用程序。典型的就是在`JAVA_OPTS`里设置`HTTP`代理而在`GRADLE_OPTS`里设置内存参数。这些变量同样可以设置在`gradle`或者`gradlew`脚本的开头。

注意，目前我们还不能通过命令行来设置`Gradle`需要的`JVM`参数。
