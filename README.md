# SMACC
SMACC is a ROS/C++ library to implement in easy and systematic way [UML State Charts](http://sce.uhcl.edu/helm/rationalunifiedprocess/process/modguide/md_stadm.htm) (AKA state machines). SMACC is inspired by the [SMACH ROS package](http://wiki.ros.org/smach) and it built on top of [Boost StateChart library](https://www.boost.org/doc/libs/1_53_0/libs/statechart/doc/index.html).

Probably the best streght of SMACC is that the statechart you can develop with it are strictly based on the UML Standard. This means that you can access to a clear and thoroughly studied and known approach to describe State Machines. This may be specially important on industrial environments.

The following image shows one example of state machine on the UML standard and shows many of the concepts that can be implemented using SMACC:
<p align="center">
<img src="http://sce.uhcl.edu/helm/rationalunifiedprocess/process/modguide/images/md_state1.gif" width="450" align="center"/>
</p>

## Features
 *  ***Powered by ROS:*** SMACC has been developed specifically to work with ROS. It is a c++ ros package that can be imported from any end-user application package.
 *   ***C++ language:*** ROS lacked the existence of a library to develop task-level state machine in c++. Many libraries in robotics are developed in c++ so that this may help during the integration of different libraries. In industrial development context are sometimes prefered the usage of c++ over Python, so that this tool may be a good choice.
 *  ***Static State Machine Checking:*** SMACC inherits this from the statechart library. This helps the developer to check the consistence of the state machine in compile time (instead of runtime). In other words, it helps you to check if your state machine is well written.
 * ***Dynamically extensible funcionality:*** SMACC uses rosplugins to provide rich action clients that can be dinamically imported at runtime and stored in the local machine. This enables the SMACC application extend or improve the runtime behavior of the system.  You can just replace some plugin components and keeping the original SMACC application without needing to recompile.

## Cannonical SMACC applications
The cannonical SMACC applications are mobile robots (that may optionally have manipulators) that have to navigate around the environment and use some of the onboard tools. One example could be the PR2 Robot working in a factory navigating to some selfs with parcels and fetching them to some delivery point. Other example would be vaccum cleaner or other mobile robot that must perform a sistematic navigation on the ground executing some tool, in order to do that, SMACC provide some navigation planners that can help on this task (see more on section ROS Integration).

## ROS Integration

* ***Intensive use of ROS Action***. SMACC translate Action server events (Result callbacks, Feedback callbacks, etc.) to statechart events. To know more about this check the sections Shared Resources and section SMACC Architecture.
* ***Powerful access to ROS Parameters***. Each SMACC state creates automatically a ros::NodeHandle automatically named according to the SMACC state hierarchy (see more in section Usage Examples - Ros parameters)
* ***ROS Navigation built-in funcionality***. SMACC provides some navigation planners (for the ROS Navigation Stack) that navigate only using pure spinning motions and stright motions. These can be very useful in some industrial applications where the knowledge or certainty on the environment is higher (ros planners are focused on cluttered and dynamic environments).

## Repository Packages

This repository contains several ROS packages:

 * **smacc**: The core smacc library. It works as a template-based c++  header library.

 * **smacc_navigation_plugin**: it implements a smacc-actionclient-plugin to handle the move_base node remotely.

 * **smacc_reelrbtx_plugin**: it implements a smacc-actionclien-plugin to handle the reel robotics reel device.

 * **radial_motion_example**: shows a complete sample application developed with SMACC that can be reused as a canonical example of a mobile robot moving around and interacting with a custom onboard tool.

 * **smacc_tool_plugin_template**: template project that shows how an on/off tool onboard the mobile robot could be used following the SMACC methodology.

## Shared Resources and Shared variables

* ***Shared Action Client Resources*** Action servers in SMACC play an important role because all low-level funcionality must be located on them. In order to interact with these Action Servers SMACC provide an easy way to create shared ActionClients that can be accessed from any State. SMACC is in charge of eficently handle all the requests and send the resulting events from action servers (result messages, feedback messages, etc) to the "subscribed states" in form of statechart events. (See more about his in section internal architecture)

* ***Shared Variables*** UML statecharts basically define the high level behavior of a system. However, in practice the real state of the system may be much more complex (mesurement, environment numerical information, etc.). States usually have to share information (or comunicate to each other). In order to do that, SMACC implements a  simple but effective dictionary-based mechanism to share information (structs, objects, simple variables or pointers). (See below in tutorials: shared variable)
<p align="center">
<img src="https://github.com//brettpac/SMACC/blob/master/doc/shared_resources.png?raw=true"  width="450" align="center"/>
</p>

## Development methodology
SMACC also defines a development methodology where State Machine nodes only contains the task-level logic, that is, the high level behavior of the robot system in some specific application.

SMACC applications have low level coupling with other software components of the robot system. SMACC code is recomended to interact with the rest of components the robot system via ROS Action Servers and **Smacc Action Plugins**.

The proposed methdology split the states into 2 or more statechart orthogonal lines that comunicate to each other via events. The orthogonal line 0 is tipically for the mobile robot navigation. The second orthogonal line and ahead are used for tools (manipulators, grippers or other custom tools).

<p align="center">
<img src="http://smacc.ninja/wp-content/uploads/2018/09/SMACC-Containers-2.jpg"  width="450" align="center"/>
</p>

## Internal Architecture
SMACC State Machines are boost::statechart AsynchronousStateMachines that can work in a multi-threaded application. In SMACC State Machines are two main components that work concurrently in two different threads:

* ***Signal Detector***. It is able to handle the action client resources communication with action servers and translate them to statechart events
* ***State Machine***. It is the end-user code of the state machine itself.

<p align="center">
<img src="http://smacc.ninja/wp-content/uploads/2018/09/SMACC-Node-Map-2-2-1.jpg"  width="450" align="center"/>
</p>

## Executing the Radial Motion Example
Requires: reelrbtx_control and reelrbtx_navigation packages and ridgeback packages

```
export RIDGEBACK_URDF_EXTRAS=$(rospack find reelrbtx_description)/urdf/reelrbtx.urdf.xacro

roslaunch radial_motion_example radial_motion.launch
```
## Usage examples
SMACC states inherits from boost::statechart:State so that you can learn the full potential of SMACC states also diving in the statechart documentation. However, the following examples briefly show how you create define SMACC states and how you would usually use them.

### Code a minimal SMACC StateMachine
SMACC StateMachines and SmaccStates are based on the c++ [Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern) so that the syntax may be strange for some developers but you will notice that it is very easy to follow. The advantage of using this kind of c++ pattern is that the definition of the state machine is correctly written.

The following chunk of code shows the minimal SMACC State machine you can create:
```cpp
#include <smacc/smacc_state_machine_base.h>

struct SimpleStateMachine
    : public SmaccStateMachineBase<SimpleStateMachine,ToolSimpleState> 
{
  SimpleStateMachine(my_context ctx, SignalDetector *signalDetector)
      : SmaccStateMachineBase<SimpleStateMachine,ToolSimpleState>(ctx, signalDetector) 
      {
      }
};
```

Every SMACC State Machine must inherit from SmaccStateMachineBase. The first template parameter is the derived class type (SimpleStateMachine itself) according to the curiosuly recurring template pattern, and the second template parameter (ToolSimpleState) would be the class name of the initial Smacc State

### Code a simple SMACC State

For the previous state machine, this would be the initial SMACC State. It also follows the Curiously recurrent template pattern. However, for Smacc states, the second template parameters is the so called "Context", for this simple case, the context is the StateMachine type itself. However, that could also be other State (in a nexted-substate case) or an orthogonal line.

```cpp
// ------------------ SIMPLE STATE --------------
struct ToolSubstate
    : SmaccState<ToolSubstate, SimpleStateMachine> {
  
public:

  // This is the substate constructor. This code will be executed when the
  // workflow enters in this substate (that is according to statechart the moment when this object is created)
  ToolSubstate(my_context ctx) 
    : SmaccState<ToolSubstate, SimpleStateMachine>(ctx) // call the SmaccState base constructor
  {
    ROS_INFO("Entering ToolSubstate");
  }
};
```
According to the UML statchart standard, things happens essencially when the system enters in the state, when the system exits the state and when some event is triggered. The two first ones are shown in this example. The c++ Constructor code is the place you have to write your "entry code", the destructor is the place you have to write your "exit code". The constructor parameter (my_context) is a reference to the context object (in this case the state machine). This kind of constructor may be verebosy, but is required to implement the rest of SMACC tasks and always follows the same pattern.

### Accessing to action client resources (AKA Action Client Plugins)

```cpp
struct ToolSubstate
    : statechart::simple_state<SimpleStateMachine> 
{
public:

  // This is the substate constructor. This code will be executed when the
  // workflow enters in this substate (that is according to statechart the moment when this object is created)
  ToolSubstate()
  {
    ROS_INFO("Entering ToolSubstate");

    toolActionClient_ =
        context<SimpleStateMachine>().requiresActionClient<smacc::SmaccToolActionClient>("tool_action_server");

    smacc::SmaccToolActionClient::Goal goal;
    goal.command = smacc::SmaccToolActionClient::Goal::CMD_START;
    toolActionClient_->sendGoal(goal);
  }

  smacc::SmaccToolActionClient* toolActionClient_;
};
```

### Adding ROS Parameters to Smacc States
The SMACC states can be configured from the ros parameter server based on their hierarchy
and their class name. It is responsability of the user not to have two different state names at the same level (even if the namespace is distinct since the namespace is trimmed for parameters)

For example, imagine a StateMachine to move the mobile robot initially to some initial position, and then moving it to some other position relative to the initial position. You could put the navigation parameters in a ros configuration yaml file like this (and avoid hardcoding):

```yaml
MyStateMachine:
    State1: #Go to some initial position
        NavigationOrthogonalLine:
            Navigate:
                start_position_x: 3
                start_position_y: 0
    State2: #Go to some initial position
        NavigationOrthogonalLine:
            Navigate:
                initial_orientation_index: 0 # the initial index of the linear motion (factor of angle_increment_degrees)
                angle_increment_degree: 90    # the increment of angle between to linear motions
                linear_trajectories_count: 2  # the number of linear trajectories of the radial 
```

Then, the c++ code for the State MyStateMachine/State1/NavigationOrthogonalLine/Navigate could contain the following parameter reading funcionality:

```cpp
struct Navigate : SmaccState<Navigate, NavigationOrthogonalLine> 
{
public:
  // This is the substate constructor. This code will be executed when the
  // workflow enters in this substate (that is according to statechart the moment when this object is created)
  Navigate(my_context ctx)
    : SmaccState<Navigate, NavigationOrthogonalLine>(ctx) // call the SmaccState base constructor 
  {
      geometry_msgs::Point p;
      param("start_position_x", p.x, 0);
      param("start_position_y", p.y, 0);
  }
}
```
The param template method reads from the parameters server delegating to the method defined ros::NodeHandle handle does but already located at the exact point in the parameter name hierarchy associated to this state. SMACC is also able have methods getParam and setParam that are delegated to ros::NodeHandle in the same way.
