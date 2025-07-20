#include <iostream>
#include <fstream>
#include "contacts.pb.h"

using namespace std;


void AddPeopleInfo(contacts3::PeopleInfo* people){
    cout << "-------------新增联系人-------------" << endl;
    cout << "name: ";
    string name;
    getline(cin, name);
    people->set_name(name);
    cout << "age: ";
    int age;
    cin >> age;
    people->set_age(age);
    cin.ignore(256, '\n');
    for(int i = 0;;i++){
        cout << "phone" << i+1 << ": ";
        string ph;
        getline(cin, ph);
        if(ph.empty()){
            break;
        }
        contacts3::PeopleInfo_Phone* phone = people->add_phone();
        phone->set_number(ph);
        cout << "PhoneType(1. MP; 2. TEL): ";
        int type;
        cin >> type;
        cin.ignore(256, '\n');
        switch(type){
            case 1:
                phone->set_type(contacts3::PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_MP);
                break;
            case 2:
                phone->set_type(contacts3::PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_TEL);
                break;
            default:
                cout << "select error!" << endl;
        }
    }

    contacts3::Address address;
    cout << "HomeAddress: ";
    string home_address;
    getline(cin, home_address);
    address.set_home_address(home_address);
    cout << "UnitAddress: ";
    string unit_address;
    getline(cin, unit_address);
    address.set_unit_address(unit_address);

    people->mutable_data()->PackFrom(address);
 
    cout << "-----------添加联系人成功------------" << endl;
}

int main()
{
    contacts3::Contacts contacts;
    // 读取存在的联系人
    fstream input("contacts.bin", ios::in | ios::binary);
    if(!input){
        cout << "contacts.bin not find, creat new file!" << endl;
    } else if(!contacts.ParseFromIstream(&input)){
        cerr << "prase error!" << endl;
        input.close();
        return -1;
    }
    // 添加联系人
    AddPeopleInfo(contacts.add_contacts());
    // 写入文件
    fstream output("contacts.bin", ios::out | ios::binary | ios::trunc);
    if(!contacts.SerializeToOstream(&output)){
        cerr << "write error" << endl;
        input.close();
        output.close();
        return -1;
    }
    cout << "write succes!" << endl;
    input.close();
    output.close();

    return 0;
}