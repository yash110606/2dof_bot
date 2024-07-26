#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"
#include <string>
#include <chrono>
#include <vector>
using namespace std::chrono_literals;
using namespace std;
class control_point : public rclcpp::Node
{
    public:
        control_point()
        :   Node("mycontroller")
        {
            joints_controller = this->create_publisher<sensor_msgs::msg::JointState>("/joint_states",10);
            timer_ = this->create_wall_timer(500ms,std::bind(&control_point::timercallback,this));
        }
    private: 
        std::vector<float> pos;
        void timercallback()
        {
            float num;
            cout<<"enter the position for base_arm_joint  : ";
            cin>>num;
            pos.emplace_back(num);
            cout<<"enter the position for part1_part2_joint : ";
            cin>>num;
            pos.clear();
            pos.emplace_back(num);
            auto msg = sensor_msgs::msg::JointState();
            
            msg.header.stamp = this->now();
            msg.name =  {"base_arm_joint","part1_part2_joint"};
            for(size_t i=0;i<pos.size();i++)
            {
                msg.position.emplace_back(pos[i]);
            }
            msg.velocity={1.0 , 1.0};
            msg.effort = {1000.0, 1000.0};
            joints_controller->publish(msg);
        }
        rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joints_controller;
        rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<control_point>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
