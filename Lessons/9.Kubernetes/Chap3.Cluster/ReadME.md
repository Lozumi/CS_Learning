# 集群资源管理

为了管理异构和不同配置的主机，便于 Pod 的运维管理，Kubernetes 中提供了很多集群管理的配置和管理功能，通过 namespace 划分的空间，通过为 node 节点创建 label 和 taint 用于 pod 的调度等。

## Node

Node 是 Kubernetes 集群的工作节点，可以是物理机也可以是虚拟机。

## Namespace

在一个 Kubernetes 集群中可以使用 namespace 创建多个“虚拟集群”，这些 namespace 之间可以完全隔离，也可以通过某种方式，让一个 namespace 中的 service 可以访问到其他的 namespace 中的服务。

当项目和人员众多的时候可以考虑根据项目属性，例如生产、测试、开发划分不同的 namespace。

### Namespace 使用

**获取集群中有哪些 namespace**

```
kubectl get ns
```

集群中默认会有 `default` 和 `kube-system` 这两个 namespace。

在执行 `kubectl` 命令时可以使用 `-n` 指定操作的 namespace。

用户的普通应用默认是在 `default` 下，与集群管理相关的为整个集群提供服务的应用一般部署在 `kube-system` 的 namespace 下，例如我们在安装 kubernetes 集群时部署的 `kubedns`、`heapseter`、`EFK` 等都是在这个 namespace 下面。

## Label

Label 是附着到 object 上（例如 Pod）的键值对。可以在创建 object 的时候指定，也可以在 object 创建后随时指定。

```yaml
"labels": {
  "key1" : "value1",
  "key2" : "value2"
}
```

不要在 label 中使用大型、非标识的结构化数据，记录这样的数据应该用 annotation。

### 动机

Label 能够将组织架构映射到系统架构上（就像是康威定律），这样能够更便于微服务的管理，你可以给 object 打上如下类型的 label：

- `"release" : "stable"`, `"release" : "canary"`
- `"environment" : "dev"`, `"environment" : "qa"`, `"environment" : "production"`
- `"tier" : "frontend"`, `"tier" : "backend"`, `"tier" : "cache"`

## Label selector

Label 不是唯一的，很多 object 可能有相同的 label。

通过 label selector，客户端／用户可以指定一个 object 集合，通过 label selector 对 object 的集合进行操作。

Label selector 有两种类型：

- *equality-based* ：可以使用 `=`、`==`、`!=` 操作符，可以使用逗号分隔多个表达式
- *set-based* ：可以使用 `in`、`notin`、`!` 操作符，另外还可以没有操作符，直接写出某个 label 的 key，表示过滤有某个 key 的 object 而不管该 key 的 value 是何值，`!` 表示没有该 label 的 object

## 示例

```bash
$ kubectl get pods -l environment=production,tier=frontend
$ kubectl get pods -l 'environment in (production),tier in (frontend)'
$ kubectl get pods -l 'environment in (production, qa)'
$ kubectl get pods -l 'environment,environment notin (frontend)'
```

## Annotation

Annotation 可以将 Kubernetes 资源对象关联到任意的非标识性元数据。使用客户端（如工具和库）可以检索到这些元数据。

### 关联元数据到对象

Label 和 Annotation 都可以将元数据关联到 Kubernetes 资源对象。Label 主要用于选择对象，可以挑选出满足特定条件的对象。相比之下，annotation 不能用于标识及选择对象。annotation 中的元数据可多可少，可以是结构化的或非结构化的，也可以包含 label 中不允许出现的字符。

Annotation 和 label 一样都是 key/value 键值对映射结构：

```json
"annotations": {"key1":"value1","key2":"value2"}
```

## 示例

如 Istio 的 Deployment 配置中就使用到了 annotation：

```yaml
apiVersion: extensions/v1beta1
kind: Deployment
metadata:
  name: istio-manager
spec:
  replicas: 1
  template:
    metadata:
      annotations:
        alpha.istio.io/sidecar: ignore
      labels:
        istio: manager
    spec:
      serviceAccountName: istio-manager-service-account
      containers:
      - name: discovery
        image: harbor-001.jimmysong.io/library/manager:0.1.5
        imagePullPolicy: Always
        args: ["discovery", "-v", "2"]
        ports:
        - containerPort: 8080
        env:
        - name: POD_NAMESPACE
          valueFrom:
            fieldRef:
              apiVersion: v1
              fieldPath: metadata.namespace
      - name: apiserver
        image: harbor-001.jimmysong.io/library/manager:0.1.5
        imagePullPolicy: Always
        args: ["apiserver", "-v", "2"]
        ports:
        - containerPort: 8081
        env:
        - name: POD_NAMESPACE
          valueFrom:
            fieldRef:
              apiVersion: v1
              fieldPath: metadata.namespace
```

## Taint和Toleration

Taint 和 toleration 相互配合，可以用来避免 pod 被分配到不合适的节点上。每个节点上都可以应用**一个或多个** taint，这表示对于那些不能容忍这些 taint 的 pod，是不会被该节点接受的。如果将 toleration 应用于 pod 上，则表示这些 pod 可以（但不要求）被调度到具有相应 taint 的节点上。

### 为 node 设置 taint

为 node1 设置 taint：

```bash
kubectl taint nodes node1 key1=value1:NoSchedule
kubectl taint nodes node1 key1=value1:NoExecute
kubectl taint nodes node1 key2=value2:NoSchedule
```



删除上面的 taint：

```bash
kubectl taint nodes node1 key1:NoSchedule-
kubectl taint nodes node1 key1:NoExecute-
kubectl taint nodes node1 key2:NoSchedule-
```



查看 node1 上的 taint：

```bash
kubectl describe nodes node1
```

### 为 pod 设置 toleration

只要在 pod 的 spec 中设置 tolerations 字段即可，可以有多个 `key`，如下所示：

```yaml
tolerations:
- key: "key1"
  operator: "Equal"
  value: "value1"
  effect: "NoSchedule"
- key: "key1"
  operator: "Equal"
  value: "value1"
  effect: "NoExecute"
- key: "node.alpha.kubernetes.io/unreachable"
  operator: "Exists"
  effect: "NoExecute"
  tolerationSeconds: 6000
```

- `value` 的值可以为 `NoSchedule`、` PreferNoSchedule` 或 `NoExecute`。
- `tolerationSeconds` 是当 pod 需要被驱逐时，可以继续在 node 上运行的时间。

## QoS

QoS（Quality of Service），大部分译为“服务质量等级”，又译作“服务质量保证”，是作用在 Pod 上的一个配置，当 Kubernetes 创建一个 Pod 时，它就会给这个 Pod 分配一个 QoS 等级，可以是以下等级之一：

- **Guaranteed**：Pod 里的每个容器都必须有内存/CPU 限制和请求，而且**值必须相等**。
- **Burstable**：Pod 里至少有一个容器有内存或者 CPU 请求且不满足 Guarantee 等级的要求，即内存/CPU 的值设置的不同。
- **BestEffort**：容器必须没有任何内存或者 CPU 的限制或请求。

该配置不是通过一个配置项来配置的，而是通过配置 CPU/内存的 `limits` 与 `requests` 值的大小来确认服务质量等级的。使用 `kubectl get pod -o yaml` 可以看到 pod 的配置输出中有 `qosClass` 一项。该配置的作用是为了给资源调度提供策略支持，调度算法根据不同的服务质量等级可以确定将 pod 调度到哪些节点上。

例如，下面这个 YAML 配置中的 Pod 资源配置部分设置的服务质量等级就是 `Guarantee`。

```yaml
spec:
  containers:
    ...
    resources:
      limits:
        cpu: 100m
        memory: 128Mi
      requests:
        cpu: 100m
        memory: 128Mi
```

下面的 YAML 配置的 Pod 的服务质量等级是 `Burstable`。

```yaml
spec:
  containers:
    ...
    resources:
      limits:
        memory: "180Mi"
      requests:
        memory: "100Mi"
```