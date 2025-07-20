# <font color="yellow">Protobuf</font>

### 一、初识Protobuf

#### 1. 序列化
- 将所要传输的对象转化为二进制的过程是序列化；反之则是反序列化；
- 序列化使用：存储数据、网络传输；
- 实现序列化的方法：JSON、XML、ProtoBuf；

#### 2. 什么是PB(ProtoBuf)
- 将结构化数据进行序列化的一种方式；

#### 3. PB的特点
- **语言无关、平台无关**：即PB支持Java、C++、Python等多种语言，支持多平台；
- **高效**：比XML更小、更快、更简单；
- **扩展性、兼容性好**：可以更新数据结构，不影响和破坏原有旧程序；
- **使用特点**：PB需要依赖通过*编译生成的头文件和源文件*来使用；


### 二、ProtoBuf安装

#### 1. 下载ProtoBuf编译器
sudo apt-get install autoconf automake libtool curl make g++ unzip -y\
wget https://github.com/protocolbuffers/protobuf/releases/download/v21.11/protobuf-all-21.11.zip\
unzip protobuf-all-21.11.zip
#### 2. 安装ProtoBuf
./autogen.sh（如果安装的是具体语言，不需要这一步）\
./configure --prefix=/usr/local/protobuf\
make\
make check\
sudo make install
- 如果使用的是“./configure --prefix=/usr/local/protobuf“，则需要再执行\
sudo vim /etc/profile\


### 三、快速上手
#### 1. 目的
完整使用ProtoBuf(以下称为PB)；\
学习基本语法；
#### 2. 需求
编写第一版本通讯录1.0；
- 对一个联系人的信息使用PB序列化，并打印；
- 对序列化的内容反序列化，打印；
- 联系人包含姓名和年龄；
#### 3.定义消息字段
格式：字段类型 字段名 = 字段唯一编号；
- 字段名均小写（规范）；
- 字段类型：标量数据类型、特殊类型（枚举、其他消息类型等）；
- 字段唯一编号：标识字段，不可更改(19000~19999不可用)；
#### 4. 编译
protoc --cpp_out=. contacts.proto

g++ -o TestPB main.cc contacts.pb.cc -std=c++11 -lprotobuf
(经过我的尝试，发现c++17也可以，可能是向下兼容的原因)


### 四、proto3语法
#### 1. 字段规则
- repeated：消息中可以包含该字段任意多次（包括0），其中重复值的顺序会被保留，可以理解为定义了一个数组；
- singular：消息中可以包含该字段1/0次，proto3语法中，字段默认使用该规则；
#### 2. 写入/读取 文件
-  消息序列化为二进制数据并写入指定的输出流\
bool SerializeToOstream(std::ostream* output) const;

- 反序列化 数据，即将存储在输入流（如文件流、内存流等）中的二进制数据解析成对应的 Protobuf 消息对象\
bool ParseFromIstream(std::istream* input);

- 验证链接的 Protobuf 库版本与编译时使用的头文件版本是否匹配\
GOOGLE_PROTOBUF_VERIFY_VERSION
#### 3. decode命令
- protoc --decode(从标准输入中读取二进制消息)
#### 4. enum类型
- 0值常量必须存在且作为第一个枚举值；
- 枚举常量值必须在32位整形范围内，不为负；
- 同级枚举类型的枚举量名称不能相同；

#### 5. Any对象
mutable_data()\
(相当于开辟一个Any空间)；\
bool PackFrom(const ::PROTOBUF_NAMESPACE_ID::Message& message)\
将message转化为Any对象并存储在data内；

