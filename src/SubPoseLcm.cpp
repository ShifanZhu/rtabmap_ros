#include <stdio.h>
#include <lcm/lcm-cpp.hpp>
#include <rtabmap_ros/lcmPose.hpp>

class Handler 
{
    public:
        ~Handler() {}
        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const lcmPose* msg)
        {
            // printf("Received message on channel \"%s\":\n", chan.c_str());
            // printf("  position    = (%f, %f, %f)\n",
            //         msg->position[0], msg->position[1], msg->position[2]);
            // printf("  orientation = (%f, %f, %f, %f)\n",
            //         msg->orientation[0], msg->orientation[1], 
            //         msg->orientation[2], msg->orientation[3]);
        }
};

int main(int argc, char** argv)
{
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;
    Handler handlerObject;
    lcm.subscribe("lcm_pose", &Handler::handleMessage, &handlerObject);
    while(0 == lcm.handle());
    return 0;
}