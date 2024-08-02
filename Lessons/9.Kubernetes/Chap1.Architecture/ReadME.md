# Kubernetes架构

## 分层架构

Kubernetes架构图：

![Architecture](./assets/k8s_architecture.png)

Kubernetes 主要由以下几个核心组件组成：

* etcd 保存了整个集群的状态；
* apiserver 提供了资源操作的唯一入口，并提供认证、授权、访问控制、API 注册和发现等机制；
* controller manager 负责维护集群的状态，比如故障检测、自动扩展、滚动更新等；
* scheduler 负责资源的调度，按照预定的调度策略将 Pod 调度到相应的机器上；
* kubelet 负责维护容器的生命周期，同时也负责 Volume（CSI）和网络（CNI）的管理；
* Container runtime 负责镜像管理以及 Pod 和容器的真正运行（CRI）；
* kube-proxy 负责为 Service 提供 cluster 内部的服务发现和负载均衡；

## API 设计原则

对于云计算系统，系统 API 实际上处于系统设计的统领地位。Kubernetes 系统 API 的设计有以下几条原则：

- **所有 API 应该是声明式的**。 
- **API 对象是彼此互补而且可组合的**。 
- **高层 API 以操作意图为基础设计**。
- **低层 API 根据高层 API 的控制需要设计**。
- **尽量避免简单封装，不要有在外部 API 无法显式知道的内部隐藏的机制**。
- **API 操作复杂度与对象数量成正比**。
- **API 对象状态不能依赖于网络连接状态**。
- **尽量避免让操作机制依赖于全局状态，因为在分布式系统中要保证全局状态的同步是非常困难的**。

## 控制机制设计原则

- **控制逻辑应该只依赖于当前状态**。
- **假设任何错误的可能，并做容错处理**。
- **尽量避免复杂状态机，控制逻辑不要依赖无法监控的内部状态**。
- **假设任何操作都可能被任何操作对象拒绝，甚至被错误解析**。
- **每个模块都可以在出错后自动恢复**。
- **每个模块都可以在必要时优雅地降级服务**。

## Kubernetes 的核心技术概念和 API 对象

API 对象是 Kubernetes 集群中的管理操作单元。 Kubernetes 集群系统每支持一项新功能，引入一项新技术，一定会新引入对应的 API 对象，支持对该功能的管理操作。

每个 API 对象都有 3 大类属性：元数据 metadata、规范 spec 和状态 status。 元数据是用来标识 API 对象的，每个对象都至少有 3 个元数据：namespace，name 和 uid； 除此以外还有各种各样的标签 labels 用来标识和匹配不同的对象。规范描述了用户期望 Kubernetes 集群中的分布式系统达到的理想状态（Desired State）。规范描述了系统实际当前达到的状态（Status）。

Kubernetes 中所有的配置都是通过 API 对象的 spec 去设置的，也就是用户通过配置系统的理想状态来改变系统。

### Pod

Pod 是在 Kubernetes 集群中运行部署应用或服务的最小单元，它是可以支持多容器的。 Pod 的设计理念是支持多个容器在一个 Pod 中共享网络地址和文件系统，可以通过进程间通信和文件共享这种简单高效的方式组合完成服务。

### 副本控制器（Replication Controller，RC）

RC 是 Kubernetes 集群中最早的保证 Pod 高可用的 API 对象。 通过监控运行中的 Pod 来保证集群中运行指定数目的 Pod 副本。 指定的数目可以是多个也可以是 1 个； 少于指定数目，RC 就会启动运行新的 Pod 副本； 多于指定数目，RC 就会杀死多余的 Pod 副本。

RC 是 Kubernetes 较早期的技术概念，只适用于长期伺服型的业务类型。 

### 副本集（Replica Set，RS）

RS 是新一代 RC，提供同样的高可用能力，区别主要在于 RS 后来居上，能支持更多种类的匹配模式。

### 部署（Deployment）

部署表示用户对 Kubernetes 集群的一次更新操作。 部署是一个比 RS 应用模式更广的 API 对象，可以是创建一个新的服务，更新一个新的服务，也可以是滚动升级一个服务。 

### 服务（Service）

RC、RS 和 Deployment 只是保证了支撑服务的微服务 Pod 的数量，但是没有解决如何访问这些服务的问题。 要稳定地提供服务需要服务发现和负载均衡能力。

服务发现完成的工作，是针对客户端访问的服务，找到对应的的后端服务实例。

在 Kubernetes 集群中微服务的负载均衡是由 Kube-proxy 实现的。Kube-proxy 是 Kubernetes 集群内部的负载均衡器。它是一个分布式代理服务器，在 Kubernetes 的每个节点上都有一个；这一设计体现了它的伸缩性优势，需要访问服务的节点越多，提供负载均衡能力的 Kube-proxy 就越多，高可用节点也随之增多。

### 任务（Job）

Job 是 Kubernetes 用来控制批处理型任务的 API 对象。批处理业务与长期伺服业务的主要区别是批处理业务的运行有头有尾，而长期伺服业务在用户不停止的情况下永远运行。Job 管理的 Pod 根据用户的设置把任务成功完成就自动退出了。

### 后台支撑服务集（DaemonSet）

长期伺服型和批处理型服务的核心在业务应用，可能有些节点运行多个同类业务的 Pod，有些节点上又没有这类 Pod 运行；而后台支撑型服务的核心关注点在 Kubernetes 集群中的节点（物理机或虚拟机），要保证每个节点上都有一个此类 Pod 运行。

### 有状态服务集（StatefulSet）

StatefulSet 是用来控制有状态服务，StatefulSet 中的每个 Pod 的名字都是事先确定的，不能更改。StatefulSet 中 Pod 的名字的作用是关联与该 Pod 对应的状态。

### 存储卷（Volume）

Kubernetes 集群中的存储卷跟 Docker 的存储卷有些类似，只不过 Docker 的存储卷作用范围为一个容器，而 Kubernetes 的存储卷的生命周期和作用范围是一个 Pod。

### 持久存储卷（Persistent Volume，PV）和持久存储卷声明（Persistent Volume Claim，PVC）

PV 和 PVC 使得 Kubernetes 集群具备了存储的逻辑抽象能力，使得在配置 Pod 的逻辑里可以忽略对实际后台存储技术的配置，而把这项配置的工作交给 PV 的配置者，即集群的管理者。存储的 PV 和 PVC 的这种关系，跟计算的 Node 和 Pod 的关系是非常类似的；PV 和 Node 是资源的提供者，根据集群的基础设施变化而变化，由 Kubernetes 集群管理员配置；而 PVC 和 Pod 是资源的使用者，根据业务服务的需求变化而变化，由 Kubernetes 集群的使用者即服务的管理员来配置。

### 节点（Node）

Kubernetes 集群中的计算能力由 Node 提供，是所有 Pod 运行所在的工作主机，可以是物理机也可以是虚拟机。要运行 kubelet 管理节点上运行的容器。

### 用户帐户（User Account）和服务帐户（Service Account）

顾名思义，用户帐户为人提供账户标识，而服务账户为计算机进程和 Kubernetes 集群中运行的 Pod 提供账户标识。

### 命名空间（Namespace）

命名空间为 Kubernetes 集群提供虚拟的隔离作用，Kubernetes 集群初始有两个命名空间，分别是默认命名空间 default 和系统命名空间 kube-system。

