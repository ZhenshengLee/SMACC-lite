#pragma once

#include <ros_publisher_client/cl_ros_publisher.h>
#include <smacc/smacc_orthogonal.h>

namespace sm_dance_bot
{
class OrUpdatablePublisher : public smacc::Orthogonal
{
public:
    virtual void onInitialize() override
    {
        auto publisherClient_ = this->createClient<OrUpdatablePublisher, ros_publisher_client::ClRosPublisher>();
        publisherClient_->configure<std_msgs::String>("/updatable_string_publisher_out");
    }
};
} // namespace sm_dance_bot