#include <iostream>
#include <fstream>
#include "contacts.pb.h"

using namespace std;

void PrintContacts(contacts3::Contacts& contacts){
    for(int i = 0; i < contacts.contacts_size(); i++){
        const contacts3::PeopleInfo& people = contacts.contacts(i);
        cout << "name: " << people.name() << endl;
        cout << "age: " << people.age() << endl;
        for(int j = 0; j < people.phone_size(); j++){
            const contacts3::PeopleInfo_Phone& phone = people.phone(j);
            cout << "phone" << j+1 << ": " << phone.number();
            cout << "(" << phone.PhoneType_Name(phone.type()) << ")" << endl;
        }
        if(people.has_data() && people.data().Is<contacts3::Address>()){
            contacts3::Address address;
            // UnpackTo():转换成Any对象
            people.data().UnpackTo(&address);
            if(!address.home_address().empty()){
                cout << "HomeAddress: " << address.home_address() << endl;
            }
            if(!address.unit_address().empty()){
                cout << "UnitAddress: " << address.unit_address() << endl;
            }
        }
    }

}

int main(){
    contacts3::Contacts contacts;
    // 读取文件
    fstream input("contacts.bin", ios::in | ios::binary);
    if(!contacts.ParseFromIstream(&input)){
        cerr << "prase error!" << endl;
        input.close();
        return -1;
    }
    // 打印通讯录列表
    PrintContacts(contacts);
    return 0;
}