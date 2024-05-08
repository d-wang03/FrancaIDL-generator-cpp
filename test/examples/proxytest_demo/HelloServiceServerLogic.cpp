
/**
 * This file is auto generated from template. Please add your own logic in this class.
 **/
#include "HelloServiceServerLogic.h"

namespace AdvancedServiceFramework
{
HelloServiceServerLogic::HelloServiceServerLogic() : HelloServiceServerLogicDefault(__func__)
{
}
HelloServiceServerLogic::~HelloServiceServerLogic()
{
}

void HelloServiceServerLogic::openCamera(
    const std::shared_ptr<CommonAPI::ClientId> _client, uint32_t _cameraID, bool _ifConfig,
    v2::com::bst::os::services::hello::HelloServiceStubDefault::openCameraReply_t _reply)
{
    // TODO: Add the stub processing logic here.
    std::cout << "Enter HelloServiceServerLogic openCamera" << std::endl;
    if (_cameraID % 2 == 0)
    {
        std::cout << "camera do not have even number." << std::endl;
        setStatusAttribute(false);
        fireFirstBcEvent("hi! First selective broadcast!");
        _reply(2, true);
    }
    else
    {
        std::cout << "Camera " << _cameraID << " is open." << std::endl;
        setStatusAttribute(true);
        fireSecondBcEvent("Bye! Second broadcast!");
        _reply(1, false);
    }
}

void HelloServiceServerLogic::startRecord(
    const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path,
    v2::com::bst::os::services::hello::HelloServiceStubDefault::startRecordReply_t _reply)
{
    // TODO: Add the stub processing logic here.
    std::cout << "Enter HelloServiceServerLogic startRecord" << std::endl;
}

void HelloServiceServerLogic::stopRecord(
    const std::shared_ptr<CommonAPI::ClientId> _client,
    v2::com::bst::os::services::hello::HelloServiceStubDefault::stopRecordReply_t _reply)
{
    // TODO: Add the stub processing logic here.
    std::cout << "Enter HelloServiceServerLogic stopRecord" << std::endl;
}

void HelloServiceServerLogic::readRecord(
    const std::shared_ptr<CommonAPI::ClientId> _client, std::string _path,
    v2::com::bst::os::services::hello::HelloServiceStubDefault::readRecordReply_t _reply)
{
    // TODO: Add the stub processing logic here.
    std::cout << "Enter HelloServiceServerLogic readRecord" << std::endl;
}
void HelloServiceServerLogic::fireFirstBcEvent(const std::string &_hi,
                                               const std::shared_ptr<CommonAPI::ClientIdList> _receivers)
{
    if (!m_service.expired())
        (m_service.lock())->fireFirstBcSelective(_hi, _receivers);
}
void HelloServiceServerLogic::fireSecondBcEvent(const std::string &_bye)
{
    if (!m_service.expired())
        (m_service.lock())->fireSecondBcEvent(_bye);
}
std::string HelloServiceServerLogic::getNameAttribute()
{
    if (!m_service.expired())
        return (m_service.lock())->getNameAttribute();
    else
        return std::string();
}
void HelloServiceServerLogic::setNameAttribute(const std::string &_value)
{
    if (!m_service.expired())
        (m_service.lock())->setNameAttribute(_value);
}
uint32_t HelloServiceServerLogic::getPositionAttribute()
{
    if (!m_service.expired())
        return (m_service.lock())->getPositionAttribute();
    else
        return uint32_t();
}
void HelloServiceServerLogic::setPositionAttribute(const uint32_t &_value)
{
    if (!m_service.expired())
        (m_service.lock())->setPositionAttribute(_value);
}
bool HelloServiceServerLogic::getStatusAttribute()
{
    if (!m_service.expired())
        return (m_service.lock())->getStatusAttribute();
    else
        return bool();
}
void HelloServiceServerLogic::setStatusAttribute(const bool &_value)
{
    if (!m_service.expired())
        (m_service.lock())->setStatusAttribute(_value);
}

} // namespace AdvancedServiceFramework
