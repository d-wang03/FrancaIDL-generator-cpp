#include "v4/com/bst/os/idl/libtest/libInfProxy.hpp"
#include <iostream>
using namespace v4::com::bst::os::idl::libtest;
int main()
{
    auto proxy = std::make_unique<v4::com::bst::os::idl::libtest::libInfProxy>();
    bool isSuccess;
    uint64_t id = 100;
    auto infor = libTypes::StaffInfor("Liu", "Tong", libTypes::MBTI::ISTJ, nullptr, nullptr);
    proxy->addNewStaff(id, infor, isSuccess);
    if (isSuccess)
        std::cout << "Successfully add new staff :" << infor.firstName << "." << infor.lastName << std::endl;
    else
        std::cout << "FAILED: for repeat staff id : " << id << std::endl;
}