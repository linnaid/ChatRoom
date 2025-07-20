# <font color="yellow">在这里我将学习Redis的简单使用</font>

### 一、配置
1. 安装\
sudo apt update\
sudo apt install redis-server
2. 检查状态\
sudo systemctl status redis
3. Redis 设置为开机自启动\
sudo systemctl enable redis
4. 取消 Redis 的开机自启\
sudo systemctl disable redis-server
5. 手动启动或停止\
sudo systemctl start redis-server\
sudo systemctl stop redis-server
6. 启动Redis\
redis-server
7. 连接\
redis-cli
8. 测试网络连接是否成功\
ping(返回PONG则成功)


### 二、字符串(String)
1. <font color="red">SET</font> 命令，创建变量，也就是键(Redis中的变量均采取字符串形式储存，也就是二进制存储)；\
eg. SET name linnaid 或set name linnaid\
等同于 std::string name = "linnaid";
2. <font color="red">GET</font> 命令，获取变量，也就是键的内容；\
eg. GET name 或 get name\
等同于输出name;
3. <font color="red">DEL</font> 命令，删除一个变量，也就是键；\
eg. DEL name 或 del name\
等同于删除name变量;
4. <font color="red">EXISTS</font> 命令，判断一个键是否存在；\
eg. EXISTS name\
输出为0,则不存在，输出为1,则存在；
5. <font color="red">KEYS</font> 命令，查看数据库中有哪些键；\
KEYS * 表示查找所有键；\
KEYS *me 表示查找结尾为me的键；
6. <font color="red">FLUSHALL</font> 命令，删除数据库中所有键；\
7. <font color="red">QUIT</font> 命令，退出客户端；
8. <font color="red">--raw</font> 命令，在登陆时加，表示以原始的形式显示内容；\
eg. redis-cli --raw
可以正常显示中文(除二进制外的进制内容)
9. <font color="red">clear</font> 命令，清空屏幕；
10. <font color="red">TTL</font> 命令，查看一个键的过期时间；\
eg. TTL name\
输出-1表示未设置过期时间；
11. <font color="red">EXPIRE</font> 命令，设置一个键的过期时间；\
eg. EXPIRE name 10 （10是秒数）
12. <font color="red">SETEX</font> 命令，设置一个带有过期时间的键；\
eg. SETEX name 5
13. <font color="red">SETNX</font> 命令，对不存在的键则添加，对存在的键则不做任何处理；\
eg. SETNX name linnaid


### 三、列表(List)
1. <font color="orange">LPUSH</font> 命令，将元素添加到列表的头部；\
eg. LPUSH letter a\
(可一次添加多个，从左至右依次添加到表头)
2. <font color="orange">LRANGE</font> 命令，获取列表内容；\
eg. LRANGE lettter 0 -1\
0表示起始位置，-1表示到最后一个字符；
3. <font color="orange">RPUSH</font> 命令，向表尾加入元素，与LPUSH的使用相同；
4. <font color="orange">LPOP</font> 命令，头删；
eg. LPOP letter 或 LPOP letter 2\
数字表示要删几个元素；
5. <font color="orange">RPOP</font> 命令，尾删；
6. <font color="orange">LLEN</font> 命令，查看列表中有几个元素；\
eg. LLEN letter
7. <font color="orange">LTRIM</font> 命令，删除列表中指定范围以外的元素；\
eg. LTRIM letter 1 3\
表示只保留下标1～3的元素；


### 四、集合(Set)
- set是一个不能有重复元素的集合
1. <font color="LimeGreen">SADD</font> 命令，向一个集合中添加元素；\
eg. SADD course Redis\
向course中添加一个"Redis";
2. <font color="LimeGreen">SMEMBERS</font> 命令，查看一个集合中的元素；\
eg. SMEMBERS course
3. <font color="LimeGreen">SISMEMBER</font> 命令，判断一个元素是否在集合中；\
eg. SISMEMBER course Redis
4. <font color="LimeGreen">SREM</font> 命令，删除集合中的元素；\
eg. SREM course Redis
5. <font color="LimeGreen">SINTER</font> 
6. <font color="LimeGreen">SUNION</font>
7. <font color="LimeGreen">SDIFF</font>


### 五、有序集合(SortedSet)
- 有序集合的每一个元素都与一个浮点数关联，按照此数对集合中的元素进行排序(由小到大)，分数可以重复；
1. <font color="Magenta">ZADD</font> 命令，添加一个元素到集合,每个元素由分数和成员构成，分数在前；\
eg. ZADD result 121 math 98 english 23 history
2. <font color="Magenta">ZRANGE</font> 命令，查看集合中的部分元素；\
eg. ZRANGE result 0 -1\
查看从下标0开始到结束的元素；
3. <font color="Magenta">WITHSCORES</font> 命令，可以同时查看分数；\
eg. ZRANGE result 0 -1 WITHSCORES
4. <font color="Magenta">ZSCORE</font> 命令，查看某个成员的分数；\
eg. ZSCORE result math
5. <font color="Magenta">ZRANK</font> 命令，查看某个成员的排名(按由小到大的排名方式)；\
eg. ZRANK result math
6. <font color="Magenta">ZREVRANK</font> 命令，查看某个成员的正向排名；\
eg. ZREVRANK result math
7. <font color="Magenta">SREM</font> 命令，删除集合中的元素；\
eg. ZREM result math


### 六、哈希(Hash)
1. <font color="BlueViolet">HSET</font> 命令，向哈希中添加一个键值对；\
eg. HSET person name linnaid
2. <font color="BlueViolet">HGET</font> 命令，获取哈希中的某个键值对；\
eg. HGET person name
3. <font color="BlueViolet">HGETALL</font> 命令，获取哈希中的所有键值对；\
eg. HGETALL person
4. <font color="BlueViolet">HDEL</font> 命令，删除哈希中的某个键值对；\
eg. HDEL person name
5. <font color="BlueViolet">HEXISTS</font> 命令，判断某个键值对是否存在；\
eg. HEXISTS person name
6. <font color="BlueViolet">HKEYS</font> 命令，获取哈希中的所有键；\
eg. HKEYS person
7. <font color="BlueViolet">HLEN</font> 命令，获取哈希中所有键值对的数量；\
eg. HLEN person

<font color="BlueViolet"></font>