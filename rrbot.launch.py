import launch
from launch.substitutions import Command, LaunchConfiguration
import launch_ros
import os
import xacro
def generate_launch_description():
    pkgPath = launch_ros.substitutions.FindPackageShare(package='example1').find('example1')
    urdfModelPath = os.path.join(pkgPath, 'model/robot.xacro')
    robot_desc = xacro.process_file(urdfModelPath).toxml()
    
    params = {'robot_description' : robot_desc}
    
    robot_state_publisher_node = launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output = 'screen',
        parameters=[params])
    
    rviz_node = launch_ros.actions.Node(
        package='rviz2',    
        executable='rviz2',
        name='rviz2',
        output='screen'
    )

    return launch.LaunchDescription([
        launch.actions.DeclareLaunchArgument(name='gui', default_value='True',
                                             description='This is a flag for joint_state_publisher_gui'),
        launch.actions.DeclareLaunchArgument(name='model',default_value=urdfModelPath,
                                             description='Path to the urdf model file'),
        robot_state_publisher_node,
        rviz_node
    ])