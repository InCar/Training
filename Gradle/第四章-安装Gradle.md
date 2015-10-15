# 第四章
## 4.1 先决条件

安装JDK或者JRE，版本不要1.6，并配置JAVA_HOME环境变量。因为Gradle是用Groovy编写的，而Groovy基于JAVA。

## 4.2 下载

我们可以从[Gradle web site](http://gradle.org/gradle-download/)下载`Gradle`安装文件

## 4.3 解压

`Gradle`安装文件是一个ZIP压缩文件。完整的文件包含：
* 二进制文件。
* 用户手册（包括`HTML`和`PDF`两种版本）。
* DSL参考指南(`domain specific language` / `DSL`)。
* API手册（包括`Javadoc`和`Groovydoc`）。
* 样例。包含用户指南中提到的样例，有一些已经完成和更复杂的构建可以作为我们建立一个自己`Gradle`构建的起点。

## 4.4 环境变量

配置GRADLE_HOME到你的gradle根目录当中，然后把%GRADLE_HOME%/bin（linux或mac的是$GRADLE_HOME/bin）加到PATH的环境变量。

## 4.5 运行并测试是否工作正常

配置完成之后，运行gradle -v，检查一下是否安装无误。如果安装正确，它会打印出Gradle的版本信息，包括它的构建信息，Groovy, Ant, Ivy, 当前JVM和当前系统的版本信息。

## 4.6 JVM（Java虚拟机）参数

另外，可以通过GRADLE_OPTS或JAVA_OPTS来配置Gradle运行时的JVM参数。不过，JAVA_OPTS设置的参数也会影响到其他的JAVA应用程序。这些变量同样可以设置在gradle或者gradlew脚本的开头。

注意，目前我们还不能通过命令行来设置Gradle需要的JVM参数。
