// SPDX-FileCopyrightText: NVIDIA CORPORATION & AFFILIATES
// Copyright (c) 2023 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef ISAAC_ROS_NITROS_BRIDGE_ROS2__IMAGE_CONVERTER_NODE_HPP_
#define ISAAC_ROS_NITROS_BRIDGE_ROS2__IMAGE_CONVERTER_NODE_HPP_

#include <map>
#include <memory>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "isaac_ros_managed_nitros/managed_nitros_publisher.hpp"
#include "isaac_ros_managed_nitros/managed_nitros_subscriber.hpp"

#include "ipc_buffer_manager.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "isaac_ros_nitros_bridge_interfaces/msg/nitros_bridge_image.hpp"
#include "isaac_ros_nitros_image_type/nitros_image.hpp"
#include "isaac_ros_nitros_image_type/nitros_image_builder.hpp"
#include "isaac_ros_nitros_image_type/nitros_image_view.hpp"


namespace nvidia
{
namespace isaac_ros
{
namespace nitros_bridge
{

class ImageConverterNode : public rclcpp::Node
{
public:
  explicit ImageConverterNode(const rclcpp::NodeOptions options = rclcpp::NodeOptions());

  ~ImageConverterNode();

private:
  // Convert stub message into managed NITROS message
  void ROS1To2Callback(
    const isaac_ros_nitros_bridge_interfaces::msg::NitrosBridgeImage::SharedPtr msg);

  // Copy managed NITROS message data into IPC memory pool and convert to bridge message
  void ROS2To1Callback(const nvidia::isaac_ros::nitros::NitrosImageView msg);

  // Publisher for output NitrosImage messages
  std::shared_ptr<nvidia::isaac_ros::nitros::ManagedNitrosPublisher<
      nvidia::isaac_ros::nitros::NitrosImage>> nitros_pub_;
  // Publisher for output bridge messages
  rclcpp::Publisher<isaac_ros_nitros_bridge_interfaces::msg::NitrosBridgeImage>::SharedPtr
    bridge_image_pub_;

  // Subscription to input NitrosImage messages
  std::shared_ptr<nvidia::isaac_ros::nitros::ManagedNitrosSubscriber<
      nvidia::isaac_ros::nitros::NitrosImageView>> nitros_sub_;
  // Subscription to input bridge messages
  rclcpp::Subscription<isaac_ros_nitros_bridge_interfaces::msg::NitrosBridgeImage>::SharedPtr
    bridge_image_sub_;

  // Number of blocks of the device memory pool
  int64_t num_blocks_;
  // Map between CUDA memory handle bytes to device pointer
  std::map<std::vector<uint8_t>, void *> handle_ptr_map_;
  // CUDA IPC memory pool manager
  std::shared_ptr<IPCBufferManager> ipc_buffer_manager_;
  // If received first message
  bool first_msg_received_ = false;
};

}  // namespace nitros_bridge
}  // namespace isaac_ros
}  // namespace nvidia

#endif  // ISAAC_ROS_NITROS_BRIDGE_ROS2__IMAGE_CONVERTER_NODE_HPP_
