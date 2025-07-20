#include <iostream>
#include "contacts.pb.h"

int main()
{
    std::string people_str;
    {
        // 序列化
        contacts::PeopleInfo people;
        people.set_name("linnaid");
        people.set_age(19);
        if(!people.SerializeToString(&people_str)){
            std::cerr << "序列化联系人失败" << std::endl;
            return -1;
        }
        std::cout << "序列化成功，结果：" << people_str << std::endl;
    }
    {
        // 反序列化
        contacts::PeopleInfo people;
        if(!people.ParseFromString(people_str)){
            std::cerr << "反序列化联系人失败" << std::endl;
            return -1;
        }
        std::cout << "反序列化成功" << std::endl
                  << "姓名：" << people.name() <<std::endl
                  << "年龄：" << people.age() << std::endl;
    }

    return 0;
}