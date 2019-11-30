struct StRotateDegrees2 : smacc::SmaccState<StRotateDegrees2, MsDanceBotRunMode>
{
  using SmaccState::SmaccState;

  typedef mpl::list<
      // Expected event
      smacc::transition<EvActionSucceeded<smacc::SmaccMoveBaseActionClient>, StNavigateToWaypointsX>,

      // Keyboard events
      smacc::transition<EvKeyPressP<SbKeyboard>, StNavigateForward1>,
      smacc::transition<EvKeyPressN<SbKeyboard>, StNavigateToWaypointsX>,

      // Error events
      //smacc::transition<smacc::EvTopicMessageTimeout<SbLidarSensor>, StAcquireSensors>,
      smacc::transition<EvActionAborted<smacc::SmaccMoveBaseActionClient>, StNavigateToWaypointsX>>
      reactions;

  static void onDefinition()
  {
    static_configure<NavigationOrthogonal, SbRotate>(/*30*/ -90);
    static_configure<ToolOrthogonal, SbToolStop>();
    static_configure<KeyboardOrthogonal, SbKeyboard>();
    static_configure<ObstaclePerceptionOrthogonal, SbLidarSensor>();
  }
  
  void onInitialize()
  {
    
  }
};