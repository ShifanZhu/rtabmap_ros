#include<ros/ros.h>
#include<tf/transform_listener.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <lcm/lcm-cpp.hpp>
#include <rtabmap_ros/lcmPose.hpp>

 int main(int argc, char *argv[])
{
    //初始化节点
    ros::init(argc, argv, "tf_listener");
    
    ros::NodeHandle node;

    tf::TransformListener listener;
	ros::Publisher localizationPosePub_;
	localizationPosePub_ = node.advertise<geometry_msgs::PoseStamped>("camera_pose", 1);

    int seq = 0;
    ros::Rate rate(10.0);

    lcm::LCM lcm;
    
    while (node.ok())
    {
       tf::StampedTransform transform;
       try
       {
           // 查找turtle2与turtle1的坐标变换
            listener.waitForTransform("/map", "/camera_link", ros::Time(0), ros::Duration(3.0));
            listener.lookupTransform("/map", "/camera_link", ros::Time(0), transform);

            geometry_msgs::PoseStamped poseMsg;
            poseMsg.header.seq = seq; seq++;
            poseMsg.header.stamp = ros::Time::now();
            poseMsg.header.frame_id = "map";

            tf::Quaternion quat_tf = transform.getRotation();
            tf::Vector3 vec_tf = transform.getOrigin();

            poseMsg.pose.position.x = vec_tf[0];
            poseMsg.pose.position.y = vec_tf[1];
            poseMsg.pose.position.z = vec_tf[2];
            poseMsg.pose.orientation.x = quat_tf.x();
            poseMsg.pose.orientation.y = quat_tf.y();
            poseMsg.pose.orientation.z = quat_tf.z();
            poseMsg.pose.orientation.w = quat_tf.w();

            localizationPosePub_.publish(poseMsg);

            if(!lcm.good()) continue;
            lcmPose lcm_pose;
            lcm_pose.position[0] = vec_tf[0];
            lcm_pose.position[1] = vec_tf[1];
            lcm_pose.position[2] = vec_tf[2];

            lcm_pose.orientation[0] = quat_tf.x();
            lcm_pose.orientation[1] = quat_tf.y();
            lcm_pose.orientation[2] = quat_tf.z();
            lcm_pose.orientation[3] = quat_tf.w();

            lcm.publish("lcm_pose", &lcm_pose);
       }
       catch (tf::TransformException &ex)
       {
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
       }

        rate.sleep();
    }
    
    

    return 0;
}

